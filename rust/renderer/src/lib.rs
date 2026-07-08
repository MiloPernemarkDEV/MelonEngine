use std::ffi::{c_void, CStr};
use std::sync::Once;
use ash::{vk, Entry, Instance, Device};
use ash::khr::{surface, win32_surface};
use tracing::{info, error};
pub mod vk_helper;

static INIT_LOGGING: Once = Once::new();

pub struct RendererContext {
    _entry: Entry,
    instance: Instance,
    surface_loader: surface::Instance,
    surface: vk::SurfaceKHR,
    device: Device,
    graphics_queue: vk::Queue,
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn renderer_init(
    width: u32,
    height: u32,
    hwnd: *mut c_void,
    hinstance: *mut c_void
) -> *mut c_void {
    INIT_LOGGING.call_once(|| {
        tracing_subscriber::fmt()
            .with_max_level(tracing::Level::TRACE)
            .init();
    });

    info!("Initializing Vulkan renderer with viewport size: {}x{}", width, height);

    let entry = match unsafe { Entry::load() } {
        Ok(e) => e,
        Err(err) => {
            error!("Failed to load Vulkan graphics driver entry: {:?}", err);
            return std::ptr::null_mut();
        }
    };

    let instance = match create_vulkan_instance(&entry) {
        Ok(i) => i,
        Err(_) => return std::ptr::null_mut(),
    };

    let surface_loader = surface::Instance::new(&entry, &instance);
    let win32_loader = win32_surface::Instance::new(&entry, &instance);

    let create_info = vk::Win32SurfaceCreateInfoKHR::default()
        .hinstance(hinstance as isize)
        .hwnd(hwnd as isize);

    let vk_surface = match unsafe { win32_loader.create_win32_surface(&create_info, None) } {
        Ok(surf) => surf,
        Err(err) => {
            error!("Failed to create Win32 Vulkan surface: {:?}", err);
            return std::ptr::null_mut();
        }
    };

    let (device, graphics_queue) = match create_logical_device(&instance) {
        Ok(res) => res,
        Err(_) => return std::ptr::null_mut(),
    };

    info!("Vulkan Context and Window Surface initialized successfully!");

    let context = Box::new(RendererContext {
        _entry: entry,
        instance,
        surface_loader,
        surface: vk_surface,
        device,
        graphics_queue,
    });

    Box::into_raw(context) as *mut c_void
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn renderer_draw(context: *mut c_void) {
    if context.is_null() { return; }
    let _ctx = unsafe { &*(context as *mut RendererContext) };
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn renderer_terminate(context: *mut c_void) {
    if context.is_null() { return; }

    info!("Terminating Vulkan renderer and destroying device contexts...");

    unsafe {
        let ctx = Box::from_raw(context as *mut RendererContext);

        ctx.device.device_wait_idle().unwrap();
        ctx.device.destroy_device(None);
        ctx.surface_loader.destroy_surface(ctx.surface, None);
        ctx.instance.destroy_instance(None);
    }

    info!("Renderer resources cleaned up successfully.");
}

fn create_vulkan_instance(entry: &Entry) -> Result<Instance, ()> {
    let app_name = CStr::from_bytes_with_nul(b"Melon Engine\0").unwrap();
    let engine_name = CStr::from_bytes_with_nul(b"Melon Core\0").unwrap();

    let app_info = vk::ApplicationInfo::default()
        .application_name(app_name)
        .application_version(vk::make_api_version(0, 1, 0, 0))
        .engine_name(engine_name)
        .engine_version(vk::make_api_version(0, 1, 0, 0))
        .api_version(vk::API_VERSION_1_3);

    let extension_names = [
        surface::NAME.as_ptr(),
        win32_surface::NAME.as_ptr(),
    ];

    let create_info = vk::InstanceCreateInfo::default()
        .application_info(&app_info)
        .enabled_extension_names(&extension_names);

    unsafe {
        match entry.create_instance(&create_info, None) {
            Ok(instance) => {
                info!("Vulkan 1.3 Instance with Surface extensions created successfully.");
                Ok(instance)
            }
            Err(err) => {
                error!("Failed to create Vulkan Instance: {:?}", err);
                Err(())
            }
        }
    }
}

fn create_logical_device(instance: &Instance) -> Result<(Device, vk::Queue), ()> {
    unsafe {
        let pdevices = instance.enumerate_physical_devices().map_err(|e| {
            error!("Failed to enumerate GPUs: {:?}", e);
        })?;

        let pdevice = pdevices.first().ok_or_else(|| {
            error!("No discrete or integrated GPUs found matching Vulkan driver specifications.");
        })?;

        vk_helper::print_device_info(instance, *pdevice);

        let queue_props = instance.get_physical_device_queue_family_properties(*pdevice);
        let mut graphics_queue_index = None;
        for (index, prop) in queue_props.iter().enumerate() {
            if prop.queue_flags.contains(vk::QueueFlags::GRAPHICS) {
                graphics_queue_index = Some(index as u32);
                break;
            }
        }

        let queue_family_index = graphics_queue_index.ok_or_else(|| {
            error!("Failed to locate a standard graphics queue layout on selected GPU.");
        })?;

        let priorities = [1.0f32];
        let queue_create_info = vk::DeviceQueueCreateInfo::default()
            .queue_family_index(queue_family_index)
            .queue_priorities(&priorities);

        let mut features13 = vk::PhysicalDeviceVulkan13Features::default()
            .dynamic_rendering(true)
            .synchronization2(true);

        let queue_create_infos = [queue_create_info];
        let device_create_info = vk::DeviceCreateInfo::default()
            .queue_create_infos(&queue_create_infos)
            .push_next(&mut features13);

        let device = instance.create_device(*pdevice, &device_create_info, None).map_err(|e| {
            error!("Failed to initialize logical hardware context: {:?}", e);
        })?;

        let graphics_queue = device.get_device_queue(queue_family_index, 0);

        Ok((device, graphics_queue))
    }
}