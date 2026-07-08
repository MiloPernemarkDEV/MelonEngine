#ifndef MELON_VK_INSTANCE
#define MELON_VK_INSTANCE

#include <vulkan/vulkan.h>

struct VkInstanceHandles {
	VkInstance _instance = VK_NULL_HANDLE;
	VkInstanceCreateInfo _instanceInfo = VK_NULL_HANDLE;
	VkApplicationInfo _appInfo = VK_NULL_HANDLE;

	VkQueue _graphicsQueue = VK_NULL_HANDLE;
	VkQueue _presentQueue = VK_NULL_HANDLE;
	bool enableValidationLayers = true;
};

class VkInstanceBuilder {
public:
	VkInstanceBuilder(const char* engineName, uint32_t api_version)
	VkInstanceHandles _handles;
};

#endif // MELON_VK_INSTANCE