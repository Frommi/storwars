#include "InputHandler.h"

static InputHandler* s_inputHandler;

static void keyboardKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    s_inputHandler->inputKeyboardKey(key, scancode, action, mods);
}

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    s_inputHandler->inputMouseButton(button, action, mods);
}

static void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos) {
    s_inputHandler->inputCursorPos(xPos, yPos);
}

void InputHandler::initInputHandler(GLFWwindow* window) {
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    s_inputHandler = this;
}

void InputHandler::inputKeyboardKey(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_UNKNOWN) return;
    if (action == GLFW_REPEAT) return;

    bool newState = (action == GLFW_PRESS);
    inputKeyboardKeyState[key] = newState;
}

void InputHandler::inputMouseButton(int button, int action, int mods) {
    bool newState = (action == GLFW_PRESS);
    inputMouseButtonState[button] = newState;
}

void InputHandler::inputCursorPos(double xPos, double yPos) {
    deltaCursorX += xPos - cursorX;
    deltaCursorY += yPos - cursorY;
    cursorX = xPos;
    cursorY = yPos;
}
