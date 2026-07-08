use std::ffi::CStr;
use std::os::raw::c_char;
use ash::{vk, Instance};
use tracing::info;

pub fn c_str_to_rust<'a>(c_ptr: *const c_char) -> &'a str {
    if c_ptr.is_null() {
        return "";
    }

    unsafe {
        CStr::from_ptr(c_ptr)
            .to_str()
            .unwrap_or("")
    }
}

pub fn print_device_info(instance: &Instance, pdevice: vk::PhysicalDevice) {
    unsafe {
        let props = instance.get_physical_device_properties(pdevice);
        let device_name = CStr::from_ptr(props.device_name.as_ptr()).to_string_lossy();

        let api_version = props.api_version;
        let major = vk::api_version_major(api_version);
        let minor = vk::api_version_minor(api_version);
        let patch = vk::api_version_patch(api_version);

        let device_type = match props.device_type {
            vk::PhysicalDeviceType::DISCRETE_GPU => "Discrete GPU",
            vk::PhysicalDeviceType::INTEGRATED_GPU => "Integrated GPU",
            vk::PhysicalDeviceType::VIRTUAL_GPU => "Virtual GPU",
            vk::PhysicalDeviceType::CPU => "CPU Software Rasterizer",
            _ => "Unknown Hardware Type",
        };

        info!("[Device Hardware Info]");
        info!("  Name:          {}", device_name);
        info!("  Type:          {}", device_type);
        info!("  API Version:   {}.{}.{}", major, minor, patch);
        info!("  Driver Version: {}", props.driver_version);
        info!("  Max Texture 2D Size: {}x{}", props.limits.max_image_dimension2_d, props.limits.max_image_dimension2_d);
        info!("  Max Bound Descriptor Sets: {}", props.limits.max_bound_descriptor_sets);
    }
}