#include "input_handler.h"

static InputHandler* s_inputHandler;

static void keyboardKeyCallback(GLFWwindow*, int key, int scancode, int action, int mods) {
    s_inputHandler->inputKeyboardKey(key, scancode, action, mods);
}

static void mouseButtonCallback(GLFWwindow*, int button, int action, int mods) {
    s_inputHandler->inputMouseButton(button, action, mods);
}

static void cursorPositionCallback(GLFWwindow*, double x_pos, double y_pos) {
    s_inputHandler->inputCursorPos(glm::vec2(static_cast<float>(x_pos), static_cast<float>(y_pos)));
}

InputHandler::InputHandler() {
    memset(input_keyboard_key_state_, 0, sizeof(input_keyboard_key_state_));
    memset(input_mouse_button_state_, 0, sizeof(input_mouse_button_state_));
    delta_cursor_pos_ = glm::vec2(0.0f);
    cursor_pos_ = glm::vec2(0.0f);
}

void InputHandler::initInputHandler(GLFWwindow* window) {
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetKeyCallback(window, keyboardKeyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);

    s_inputHandler = this;
}

void InputHandler::inputKeyboardKey(int key, int, int action, int) {
    if (key == GLFW_KEY_UNKNOWN) return;
    if (action == GLFW_REPEAT) return;

    bool new_state = (action == GLFW_PRESS);
    input_keyboard_key_state_[key] = new_state;
}

void InputHandler::inputMouseButton(int button, int action, int) {
    bool new_state = (action == GLFW_PRESS);
    input_mouse_button_state_[button] = new_state;
}

void InputHandler::inputCursorPos(glm::vec2 cursor_pos) {
    delta_cursor_pos_ += cursor_pos - cursor_pos_;
    cursor_pos_ = cursor_pos;
}
