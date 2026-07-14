#ifndef MELON_VK_RENDERER_H
#define MELON_VK_RENDERER_H

#include "melon_gui.h"

struct VkContext {
	VkInstance instance;
};

class VkRenderer {
public:
	VkRenderer() = default;
	bool init();
	void draw();
	void terminate();
private:
	GuiInitInfo guiInitInfo;
	VkContext vkContext;
};

#endif