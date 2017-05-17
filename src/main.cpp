#include "application.h"

int main() {
    Application* app = new Application();
    app->initApp();
    app->run();
    app->terminateApp();
    delete app;
}
