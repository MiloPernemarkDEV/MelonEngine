#ifndef MELON_VK_RENDERER_H
#define MELON_VK_RENDERER_H

#include "melon_gui.h"
#include <vulkan/vulkan.h>

#define MELON_VULKAN_API_VERSION VK_MAKE_API_VERSION(0, 1, 3, 0)

struct VkContext {
	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
};

class VkRenderer {
public:
	VkRenderer() = default;
	bool init();
	void draw();
	void terminate();
private:
	VkDebugUtilsMessengerEXT debugMessenger;
	GuiInitInfo guiInitInfo;
	VkContext vkContext;
};

#endif