#ifndef MELONENGINE_DEVICE_H
#define MELONENGINE_DEVICE_H

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>


class Device {
public:
    Device(GLFWwindow* window);
    void Init();
    void Cleanup();
private:
    GLFWwindow* _window;
    VkInstance _instance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT _debugMessenger = VK_NULL_HANDLE;
    VkPhysicalDevice _GPU = VK_NULL_HANDLE;
    VkDevice _device = VK_NULL_HANDLE;
    VkSurfaceKHR _surface = VK_NULL_HANDLE;

    VkSwapchainKHR _swapchain = VK_NULL_HANDLE;
    VkFormat _swapchainImageFormat;
    std::vector<VkImage> _swapchainImages;
    std::vector<VkImageView> _swapchainImageViews;

    void CreateSwapchain();
    void CleanupSwapchain();
};

#endif //MELONENGINE_DEVICE_H
