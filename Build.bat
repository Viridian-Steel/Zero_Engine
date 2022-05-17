if not exist Build mkdir Build
cd Build
cmake -S ../ -B . -G "MinGW Makefiles"
mingw32-make.exe && mingw32-make.exe Shaders

cd ..