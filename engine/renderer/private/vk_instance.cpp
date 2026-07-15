#include "vk_instance.h"
#include "platform.h"
#include "types.h"
#include "logger.h"
#include <iostream>

#ifdef NDEBUG
inline bool enableValidationLayers = false;
#else
inline bool enableValidationLayers = true;
#endif

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        std::cerr << "[Vulkan Validation]: " << pCallbackData->pMessage << std::endl;
    }

    return VK_FALSE;
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& info) {
    info = {};
    info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    info.pfnUserCallback = debugCallback;
}

VkInstance InstanceManager::create(const char* engineName, uint32_t api_version) {
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
    if (enableValidationLayers) {
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
            }
        }
    }

    VkInstanceCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    info.pApplicationInfo = &appInfo;

    info.ppEnabledExtensionNames = extensions.data();
    info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());

    info.ppEnabledLayerNames = enabledLayers.data();
    info.enabledLayerCount = static_cast<uint32_t>(enabledLayers.size());

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
    if (enableValidationLayers) {
        populateDebugMessengerCreateInfo(debugCreateInfo);
        info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    } else {
        info.pNext = nullptr;
    }

    VkInstance instance = VK_NULL_HANDLE;
    if (vkCreateInstance(&info, nullptr, &instance) != VK_SUCCESS) {
        return VK_NULL_HANDLE;
    }

    return instance;
}

void InstanceManager::destroy(VkInstance instance) {
    vkDestroyInstance(instance, nullptr);
}

void InstanceManager::create_validation_layers(VkInstance instance, VkDebugUtilsMessengerEXT& debugMessenger) {
    if (!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT info;
    populateDebugMessengerCreateInfo(info);

    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, &info, nullptr, &debugMessenger);
    }
}

void InstanceManager::destroy_validation_layers(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger) {
    if (!enableValidationLayers || debugMessenger == VK_NULL_HANDLE) return;

    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, nullptr);
    }
}
