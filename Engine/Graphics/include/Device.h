#ifndef MELONENGINE_DEVICE_H
#define MELONENGINE_DEVICE_H

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>

struct FrameData {
    VkCommandPool _commandPool;
    VkCommandBuffer _commandBuffer;
};

constexpr unsigned int FRAMES_IN_FLIGHT = 2;

class Device {
public:
    Device(GLFWwindow* window);
    void Init();
    void Cleanup();
    FrameData& GetCurrentFrame() { return _frameData[_frameNumber % FRAMES_IN_FLIGHT]; }
    void InitCommands();
private:
    GLFWwindow* _window;
    VkInstance _instance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT _debugMessenger = VK_NULL_HANDLE;
    VkPhysicalDevice _GPU = VK_NULL_HANDLE;
    VkDevice _device = VK_NULL_HANDLE;
    VkSurfaceKHR _surface = VK_NULL_HANDLE;

    VkSwapchainKHR _swapchain = VK_NULL_HANDLE;
    VkFormat _swapchainImageFormat = VK_FORMAT_UNDEFINED;
    std::vector<VkImage> _swapchainImages;
    std::vector<VkImageView> _swapchainImageViews;

    FrameData _frameData[FRAMES_IN_FLIGHT];
    VkQueue _graphicsQueue = VK_NULL_HANDLE;
    uint32_t _graphicsQueueFamily = 0;
    unsigned int _frameNumber = 0;

    void CreateSwapchain();
    void CleanupSwapchain();
    void CleanupCommandPool();
};

#endif //MELONENGINE_DEVICE_H
