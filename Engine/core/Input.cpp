#include "input.h"

bool Input::is_pressed(GLFWwindow* window, const Keycode key, const Keycode alternativeKey) {
    if (alternativeKey == NONE) {
        return glfwGetKey(window, key) == GLFW_PRESS;
    }
    return glfwGetKey(window, key) == GLFW_PRESS && glfwGetKey(window, alternativeKey) != GLFW_PRESS;
}
