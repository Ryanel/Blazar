#pragma once

#ifdef BLAZAR_PLATFORM_WINDOWS
namespace Blazar {
extern Application* CreateApplication();
}

#ifdef BLAZAR_CREATE_APPLICATION


#ifdef BLAZAR_CONSOLE_WINDOW
int main(int argc, char** argv) {
    using namespace Blazar;
    Log::Init();

    auto app = CreateApplication();
    app->Run();
    delete app;

    return 0;
}
#else

#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
    using namespace Blazar;
    Log::Init();

    auto app = CreateApplication();
    app->Run();
    delete app;

    return 0;
}

#endif
#endif

#endif
