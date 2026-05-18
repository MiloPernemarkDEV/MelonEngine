#ifndef MELONENGINE_DEVICE_H
#define MELONENGINE_DEVICE_H

#include <functional>
#include <span>
#include <vector>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "RenderTypes.h"
#include "vk_mem_alloc.h"
#include "Descriptors.h"

class MelonImGui;

constexpr unsigned int FRAMES_IN_FLIGHT = 2;

class Device {
public:
    explicit Device(GLFWwindow* window);
    void Init();
    void Cleanup();
    FrameData& GetCurrentFrame() { return _frameData[_frameNumber % FRAMES_IN_FLIGHT]; }
    void InitCommands();

    VkInstance& GetInstance() { return _instance; }
    VkDevice& GetDevice() { return _device; }
    VkSwapchainKHR& GetSwapchain() { return _swapchain; }
    std::vector<VkImage>& GetSwapchainImages() { return _swapchainImages; }
    unsigned int& GetFrameNumber() { return _frameNumber; }
    VkQueue& GetGraphicsQueue() { return _graphicsQueue; }
    uint32_t GetGraphicsQueueFamily() const { return _graphicsQueueFamily; }
    void IncrementFrameNumber() { _frameNumber++; }
    VkFormat& GetFormat() {return _swapchainImageFormat;}
    GPUMeshBuffers UploadMesh(std::span<const uint32_t> indices, std::span<const Vertex> vertices);

    MelonImGui* _melonImgui = nullptr;
    DeleteQueue _mainDeletionQueue;
    VmaAllocator _allocator;
    AllocatedImage& GetDrawImage() { return _drawImage; }
    AllocatedImage _drawImage;
    VkExtent2D _drawExtent;
    VkExtent2D  _swapchainExtent;
    MelonImguiObjects _uiSyncObjects;
    VkInstance _instance = VK_NULL_HANDLE;
    VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
    std::vector<VkImageView> _swapchainImageViews;
    // One per swapchain image — safe to reuse as vkQueuePresentKHR wait semaphores.
    std::vector<VkSemaphore> _renderSemaphores;

    // Todo: Remove primitive later
    GPUMeshBuffers rectangle{};
    void InitMeshData();
    void ImmediateSubmit(const std::function<void(VkCommandBuffer cmd)>& func);

private:
    GLFWwindow* _window;
    VkDebugUtilsMessengerEXT _debugMessenger = VK_NULL_HANDLE;
    VkDevice _device = VK_NULL_HANDLE;
    VkSurfaceKHR _surface = VK_NULL_HANDLE;

    VkSwapchainKHR _swapchain = VK_NULL_HANDLE;
    VkFormat _swapchainImageFormat = VK_FORMAT_UNDEFINED;
    std::vector<VkImage> _swapchainImages;

    FrameData _frameData[FRAMES_IN_FLIGHT];
    VkQueue _graphicsQueue = VK_NULL_HANDLE;
    uint32_t _graphicsQueueFamily = 0;
    unsigned int _frameNumber = 0;

    void InitSwapchain();
    void CleanupSwapchain();
    void CleanupCommandPool();
    void InitSyncObjects();
    void InitImGuiCommands();
};

#endif //MELONENGINE_DEVICE_H
