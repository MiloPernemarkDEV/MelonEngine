#ifndef MELON_VK_INSTANCE
#define MELON_VK_INSTANCE

#include <vulkan/vulkan.h>

class VkInstanceBuilder {
public:
	VkInstanceBuilder(const char* engineName, uint32_t api_version);
};

#endif // MELON_VK_INSTANCE