#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string.h>

class InputHandler {
public:
    InputHandler();

    void initInputHandler(GLFWwindow* window);

    void inputKeyboardKey(int key, int scancode, int action, int mods);
    void inputMouseButton(int button, int action, int mods);
    void inputCursorPos(double xPos, double yPos);

    inline bool isKeyboardKeyPressed(int key) const { return input_keyboard_key_state_[key]; }
    inline bool isMouseButtonPressed(int button) const { return input_mouse_button_state_[button]; }

    inline double pullCursorDeltaX() {
        double tmp_x = delta_cursor_x_;
        delta_cursor_x_ = 0;
        return tmp_x;
    }

    inline double pullCursorDeltaY() {
        double tmp_y = delta_cursor_y_;
        delta_cursor_y_ = 0;
        return tmp_y;
    }

private:
    bool input_keyboard_key_state_[GLFW_KEY_LAST + 1];
    bool input_mouse_button_state_[GLFW_MOUSE_BUTTON_LAST + 1];
    double cursor_x_, cursor_y_;
    double delta_cursor_x_, delta_cursor_y_;
};
