#pragma once


namespace Blazar {
extern Application* CreateApplication();
}

#ifdef BLAZAR_CREATE_APPLICATION
#include "Tracy.hpp"
    #ifdef BLAZAR_PLATFORM_WINDOWS
        #ifdef BLAZAR_CONSOLE_WINDOW
            int main(int argc, char** argv) {
                ZoneScoped;
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
    #endif // Windows
    #ifdef BLAZAR_PLATFORM_LINUX
        #include <filesystem>
        #include <unistd.h>
        #include <iostream>
        
        std::string getexepath()
        {
            char result[ PATH_MAX ];
            ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
            return std::string( result, (count > 0) ? count : 0 );
        }


        // From https://stackoverflow.com/questions/8518743/get-directory-from-file-path-c
        std::string dirnameOf(const std::string& fname)
        {
            size_t pos = fname.find_last_of("\\/");
            return (std::string::npos == pos)
                ? ""
                : fname.substr(0, pos);
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
    #endif // Linux
#endif
