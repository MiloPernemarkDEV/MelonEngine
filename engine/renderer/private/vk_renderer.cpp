#include "vk_renderer.h"

#include <vulkan/vulkan.h>
#include "vk_instance.h"

bool VkRenderer::init() {

    vkContext.instance = InstanceBuilder::create_instance("MelonEngine", VK_MAKE_API_VERSION(0, 1, 3, 0));
    guiInitInfo.Instance = vkContext.instance;

    return true;
}

void VkRenderer::draw() {

}

void VkRenderer::terminate() {

}