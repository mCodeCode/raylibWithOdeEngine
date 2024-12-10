
for the ode engine:

(note, when building, link statically to avoid dll dependencies)
(add the static flags for the compiler in the cmakelist.txt)

set(CMAKE_CXX_FLAGS "-static-libgcc -static-libstdc++ -Xlinker --verbose -I${INCLUDE_FOLDER_PATH} -I${INCLUDE_ODE_FOLDER_PATH} -L ${LIB_FOLDER}")

(-lpthread is needed along with the static linking to avoid dll dependency, also, include the libwinpthread-1.dll with the .exe file, they need to be in the same folder for the program to work)


cmake -G "MinGW Makefiles" -B build_MATO -S .    

---- this will create a makefile on the build_MATO dir


then to build use (inside the build_MATO folder, where the Makefile is)

mingw32-make  

(using cmake --build build_MATO after using the minGw flag will cause compilation errors)



// or you can use Ninja

cmake -G Ninja -B build_MATO -S .

cmake --build build_MATO



include this flags in cmake command:

///
cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -G Ninja -B build_MATO -S .












(create the libraylib.a running the make command in the src folder of the raylib source code)

// important! 
to compile the program in the folder, use the same gcc compiler that was used when you build libraylib.a file , it could be w64devit or the common cmd with minwg

this one was built with the "mingw32-make" command

//basic compile command
/*
gcc ./*c -o ../buildDest/testbase.exe -O1 -Wall -std=c99 -Wno-missing-braces -I ../include/ -L ../lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
 */


//NOTE : TO BUILD ODE ENGINE YOU MUST CHANGE THE BUILD OPTIONS IN THE CMAKEFILE.TXT 
TO ENABLE THE STATIC BUILD or other setting you may need




Building
To build the projects you can use the following commands, typically I use Ninja build system as it is much faster than the default make system. Run the following in the root of the project.

cmake -G Ninja -B build_MATO -S .
cmake -G "MinGW Makefiles" -B build_MATO -S . 


(use cmake --help to see the options for the generators)
The -G flag indicates the build system to use, in this case Ninja. The -B flag indicates the build directory to use, in this case build. The -S flag indicates the source directory to use, in this case the current directory.

Then to build the project run the following:

cmake --build build_MATO

build is the directory name, you can change it to cmake --build ./ if you are in root folder
or cmake --build ./myBuilDir


so, to build the physics libraries, download the source code from github
go to the root folder and run 

cmake -G Ninja -B build -S .

then go to the build folder that has been created 
(look the name in the operation result log in the cmd)

and run cmake --build ./myBuilDir

or cmake --build ./ if you are in the root folder

this will create the libs that you can later use to inlcude in your proyects
(dont forget to also paste the headers or any source code needed)










(create the libraylib.a running the make command in the src folder of the raylib source code)

// important! 
to compile the program in the folder, use the same gcc compiler that was used when you build libraylib.a file , it could be w64devit or the common cmd with minwg

this one was built with the "mingw32-make" command

//basic compile command
/*
g++ ./*c -o ../buildDest/testbase.exe -O1 -Wall -std=c++17 -Wno-missing-braces -I ../include/ -L ../lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
 */




