#include "vk_instance.h"

#include "platform.h"
#include "types.h"
#include "logger.h"

VkInstance InstanceFactory::create_instance(const char* engineName, uint32_t api_version) {
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.apiVersion = api_version;
    appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.pEngineName = engineName;

    const std::vector<const char*> extensions = Platform::get_vk_extensions();

    uint32_t systemLayerCount;
    vkEnumerateInstanceLayerProperties(&systemLayerCount, nullptr);
    std::vector<VkLayerProperties> availableLayers(systemLayerCount);
    vkEnumerateInstanceLayerProperties(&systemLayerCount, availableLayers.data());

    std::vector<const char*> targetLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    std::vector<const char*> enabledLayers;
    for (const char* targetName : targetLayers) {
        bool found = false;
        for (const auto& layerProperties : availableLayers) {
            if (strcmp(targetName, layerProperties.layerName) == 0) {
                found = true;
                break;
            }
        }
        if (found) {
            enabledLayers.push_back(targetName);
        } else {
            // Optional: Log a warning using your logger if a target layer is missing
            // ME_LOG(Warning, "Requested layer not found: %s", targetName);
        }
    }

    VkInstanceCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    info.pApplicationInfo = &appInfo;

    info.ppEnabledExtensionNames = extensions.data();
    info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());

    info.ppEnabledLayerNames = enabledLayers.data();
    info.enabledLayerCount = static_cast<uint32_t>(enabledLayers.size());

    VkInstance instance = VK_NULL_HANDLE;
    vkCreateInstance(&info, nullptr, &instance);
    return instance;
}