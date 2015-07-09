#include "input_handler.h"

static InputHandler* s_inputHandler;

static void keyboardKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    s_inputHandler->inputKeyboardKey(key, scancode, action, mods);
}

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    s_inputHandler->inputMouseButton(button, action, mods);
}

static void cursorPositionCallback(GLFWwindow* window, double x_pos, double y_pos) {
    s_inputHandler->inputCursorPos(x_pos, y_pos);
}

void InputHandler::initInputHandler(GLFWwindow* window) {
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    s_inputHandler = this;
}

void InputHandler::inputKeyboardKey(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_UNKNOWN) return;
    if (action == GLFW_REPEAT) return;

    bool new_state = (action == GLFW_PRESS);
    input_keyboard_key_state_[key] = new_state;
}

void InputHandler::inputMouseButton(int button, int action, int mods) {
    bool new_state = (action == GLFW_PRESS);
    input_mouse_button_state_[button] = new_state;
}

void InputHandler::inputCursorPos(double x_pos, double y_pos) {
    delta_cursor_x_ += x_pos - cursor_x_;
    delta_cursor_y_ += y_pos - cursor_y_;
    cursor_x_ = x_pos;
    cursor_y_ = y_pos;
}
