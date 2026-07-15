#ifndef MELON_VK_INSTANCE
#define MELON_VK_INSTANCE

#include <vulkan/vulkan.h>

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& info);

namespace InstanceManager {
	VkInstance create(const char* engineName, uint32_t api_version);
	void destroy(VkInstance instance);

	void create_validation_layers(VkInstance instance, VkDebugUtilsMessengerEXT& debugMessenger);
	void destroy_validation_layers(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger);
}

#endif // MELON_VK_INSTANCE