#pragma once

namespace Blazar {
extern Application* CreateApplication();
}

#ifdef BLAZAR_CREATE_APPLICATION
#include "Tracy.hpp"

#ifdef BLAZAR_PLATFORM_WINDOWS
#include <timeapi.h>
#pragma comment(lib, "Winmm.lib")

#ifdef BLAZAR_CONSOLE_WINDOW
int main(int argc, char** argv) {
    timeBeginPeriod(1); //TODO: May be a terrible idea for laptop users. Should abstract?
    using namespace Blazar;

    Log::Init();
    auto app = CreateApplication();
    app->Run();
    delete app;

    timeEndPeriod(1);
    return 0;
}
#else

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
    timeBeginPeriod(1);
    using namespace Blazar;
    Log::Init();

    auto app = CreateApplication();
    app->Run();
    delete app;

    return 0;
}
#endif

#endif  // Windows

#ifdef BLAZAR_PLATFORM_LINUX
#include <unistd.h>
#include <filesystem>
#include <iostream>

std::string getexepath() {
    char    result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    return std::string(result, (count > 0) ? count : 0);
}

// From https://stackoverflow.com/questions/8518743/get-directory-from-file-path-c
std::string dirnameOf(const std::string& fname) {
    size_t pos = fname.find_last_of("\\/");
    return (std::string::npos == pos) ? "" : fname.substr(0, pos);
}

int main(int argc, char** argv) {
    using namespace Blazar;
    Log::Init();

    // Set current working directory of Game to executable folder
    std::string new_working_directory = dirnameOf(getexepath());
    LOG_CORE_TRACE("Changing working directory to {}", new_working_directory);
    std::filesystem::current_path(new_working_directory);

    auto app = CreateApplication();
    app->Run();
    delete app;

    return 0;
}

#endif  // Linux
#endif  // Create application
