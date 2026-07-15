#include "vk_renderer.h"

#include <vulkan/vulkan.h>
#include "vk_instance.h"
#include "vk_device.h"

bool VkRenderer::init() {

    vkContext.instance = InstanceManager::create("MelonEngine", MELON_VULKAN_API_VERSION);
    InstanceManager::create_validation_layers(vkContext.instance, debugMessenger);
    guiInitInfo.Instance = vkContext.instance;
    guiInitInfo.ApiVersion = MELON_VULKAN_API_VERSION;

    vkContext.physicalDevice = DeviceManager::get_physical_device(vkContext.instance, VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT);
    guiInitInfo.PhysicalDevice = vkContext.physicalDevice;

    vkContext.device = DeviceManager::create_device(vkContext.physicalDevice);
    guiInitInfo.Device = vkContext.device;

    return true;
}

void VkRenderer::draw() {

}

void VkRenderer::terminate() {

    DeviceManager::destroy(vkContext.device);

    InstanceManager::destroy_validation_layers(vkContext.instance, debugMessenger);
    InstanceManager::destroy(vkContext.instance);
}