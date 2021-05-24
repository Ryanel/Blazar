mkdir build
cd build

REM conan install .. -g visual_studio_multi -s build_type=Debug --build missing -s compiler.runtime=MDd
REM conan install .. -g visual_studio_multi -s build_type=Release --build missing -s compiler.runtime=MD

cd ..
premake5 vs2019