#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_SWIZZLE_XYZW
#include <glm/glm.hpp>

#include <string.h>

class InputHandler {
public:
    InputHandler();

    void initInputHandler(GLFWwindow* window);

    inline bool isKeyboardKeyPressed(int key) const { return input_keyboard_key_state_[key]; }
    inline bool isMouseButtonPressed(int button) const { return input_mouse_button_state_[button]; }
    inline glm::vec2 pullCursorDelta() {
        glm::vec2 tmp = delta_cursor_pos_;
        delta_cursor_pos_ = glm::vec2(0.0f);
        return tmp;
    }

    void inputKeyboardKey(int key, int scancode, int action, int mods);
    void inputMouseButton(int button, int action, int mods);
    void inputCursorPos(glm::vec2 cursor_pos);

private:
    bool input_keyboard_key_state_[GLFW_KEY_LAST + 1];
    bool input_mouse_button_state_[GLFW_MOUSE_BUTTON_LAST + 1];

    glm::vec2 cursor_pos_;
    glm::vec2 delta_cursor_pos_;
};
