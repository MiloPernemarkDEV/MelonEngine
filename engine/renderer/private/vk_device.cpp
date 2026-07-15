#include "vk_device.h"
#include "logger.h"

namespace DeviceManager {
    VkPhysicalDevice get_physical_device(VkInstance instance, VkQueueFlags flags) {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            return VK_NULL_HANDLE;
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        for (const auto& device : devices) {
            uint32_t queueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

            std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

            VkQueueFlags supportedFlags = 0;
            for (const auto& queueFamily : queueFamilies) {
                supportedFlags |= queueFamily.queueFlags;
            }

            if ((supportedFlags & flags) == flags) {
                return device;
            }
        }

        ME_LOG(Error, "Failed to find a suitable device!");
        return VK_NULL_HANDLE;
    }

     VkDevice create_device(VkPhysicalDevice physicalDevice) {
        float queuePriority = 1.0f;

        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = 0;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        const std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        VkPhysicalDeviceVulkan13Features features13 = {};
        features13.dynamicRendering = VK_TRUE;
        features13.synchronization2 = VK_TRUE;

        VkPhysicalDeviceVulkan12Features features12 = {};
        features12.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
        features12.bufferDeviceAddress = VK_TRUE;
        features12.pNext = &features13;

        VkPhysicalDeviceFeatures2 features2 = {};
        features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
        features2.features.samplerAnisotropy = VK_TRUE;
        features2.pNext = &features12;

        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pNext = &features2;
        createInfo.queueCreateInfoCount = 1;
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();
        createInfo.pEnabledFeatures = nullptr; // handled by pMext chain

        VkDevice device = VK_NULL_HANDLE;
        VkResult result = vkCreateDevice(physicalDevice, &createInfo, nullptr, &device);

        if (result != VK_SUCCESS) {
            ME_LOG(Error, "Failed to create logical device!");
            return VK_NULL_HANDLE;
        }

        return device;
    }

    void destroy(VkDevice device) {
        vkDestroyDevice(device, nullptr);
    }

}
