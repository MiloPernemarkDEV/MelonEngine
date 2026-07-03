use std::ffi::c_void;
use std::sync::Once;
use tracing::{info, warn};

static INIT_LOGGING: Once = Once::new();

#[unsafe(no_mangle)]
pub unsafe extern "C" fn renderer_init(width: u32, height: u32) -> *mut c_void {
    INIT_LOGGING.call_once(|| {
        tracing_subscriber::fmt()
            .with_max_level(tracing::Level::TRACE) 
            .init();
    });

    info!("Initializing Vulkan renderer with viewport size: {}x{}", width, height);

    1 as *mut c_void
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn renderer_draw(context: *mut c_void){
    if context.is_null() {
        warn!("renderer_draw called with a null pointer");
        return;
    }
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn renderer_terminate(context: *mut c_void) {
    if context.is_null() { return; }

    info!("Terminating renderer")

}