#pragma once

#ifdef BLAZAR_PLATFORM_WINDOWS
namespace Blazar {
extern Application* CreateApplication();
}

int main(int argc, char** argv) {
    using namespace Blazar;
    Log::Init();

    auto app = CreateApplication();
    app->Run();
    delete app;

    return 0;
}

#endif
