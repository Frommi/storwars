#include "Application.h"

static Application* app;

int main() {
    app = new Application();
    app->initApp();
    app->run();
    app->terminateApp();
    delete app;
}
