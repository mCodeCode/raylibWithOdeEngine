
for the ode engine:

(note, when building, link statically to avoid dll dependencies)
(add the static flags for the compiler in the cmakelist.txt)

set(CMAKE_CXX_FLAGS "-static-libgcc -static-libstdc++ -Xlinker --verbose -I${INCLUDE_FOLDER_PATH} -I${INCLUDE_ODE_FOLDER_PATH} -L ${LIB_FOLDER}")

(-lpthread is needed along with the static linking to avoid dll dependency, also, include the libwinpthread-1.dll with the .exe file, they need to be in the same folder for the program to work)

cmake -G "MinGW Makefiles" -B build_FOLDER -S .    

---- this will create a makefile on the build_FOLDER dir

then to build use (inside the build_FOLDER folder, where the Makefile is)

mingw32-make  

(using cmake --build build_FOLDER after using the minGw _G flag will cause compilation errors)



// or you can use Ninja

do

cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -G Ninja -B build_FOLDER -S .

then do

cmake --build build_FOLDER




when the exe is created, to run it, you need to put the libwinpthread-1.dll into the same folder as the exe file

you can avoid this issue probably recompiling correctly the whole repo but im lazy and this is enough for me


