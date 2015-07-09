#include "Application.h"

static Application* s_app;

int main() {
    s_app = new Application();
    s_app->initApp();
    s_app->run();
    s_app->terminateApp();
    delete s_app;
}
