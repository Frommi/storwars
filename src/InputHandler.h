#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class InputHandler {
public:
    InputHandler() {}

    void initInputHandler(GLFWwindow* window);

    void inputKeyboardKey(int key, int scancode, int action, int mods);
    void inputMouseButton(int button, int action, int mods);
    void inputCursorPos(double xPos, double yPos);

    inline bool isKeyboardKeyPressed(int key) const { return inputKeyboardKeyState[key]; }
    inline bool isMouseButtonPressed(int button) const { return inputMouseButtonState[button]; }

    inline double pullCursorDeltaX() {
        double tmpX = deltaCursorX;
        deltaCursorX = 0;
        return tmpX;
    }

    inline double pullCursorDeltaY() {
        double tmpY = deltaCursorY;
        deltaCursorY = 0;
        return tmpY;
    }

private:
    bool inputKeyboardKeyState[GLFW_KEY_LAST + 1];
    bool inputMouseButtonState[GLFW_MOUSE_BUTTON_LAST + 1];
    double cursorX, cursorY;
    double deltaCursorX, deltaCursorY;
};
