#ifndef MELONENGINE_INPUT_H
#define MELONENGINE_INPUT_H

#include "Keycode.h"
#include "GLFW/glfw3.h"

class Input {
public:
    static bool is_pressed(GLFWwindow* window, Keycode key, Keycode alternativeKey = NONE);
};

#endif // MELONENGINE_INPUT_H