#ifndef MELONENGINE_RENDERDEFINES_H
#define MELONENGINE_RENDERDEFINES_H

#define VK_CHECK(x)                                                 \
do                                                              \
{                                                               \
VkResult err = x;                                           \
if (err)                                                    \
{                                                           \
std::cout <<"Detected Vulkan error: " << err << std::endl; \
abort();                                                \
}                                                           \
} while (0)

#endif //MELONENGINE_RENDERDEFINES_H
