cmake_minimum_required(VERSION 3.12)
project(sphere)

# set variables
set(CMAKE_CXX_STANDARD 17)
set(TARGET "sphere" )

set(INCLUDE_FOLDER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/")
set(INCLUDE_ODE_FOLDER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/ode/")

set(SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/" )

set(LIB_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/lib")

set(CMAKE_CXX_FLAGS "-static-libgcc -static-libstdc++ -Xlinker --verbose -I${INCLUDE_FOLDER_PATH} -I${INCLUDE_ODE_FOLDER_PATH} -L ${LIB_FOLDER}")


#scan repo for files and headers (glob is not recommended by the cmake devs but idc)
file( GLOB_RECURSE HHS "${SOURCE_PATH}/*.h" )
file( GLOB_RECURSE HPPS "${SOURCE_PATH}/*.hpp" )
file( GLOB_RECURSE CPPS "${SOURCE_PATH}/*.cpp" )

# add_executable( ${TARGET} ${CPPS} ${CPPS_2} )
add_executable(${TARGET} ${CPPS} ${HHS} ${HPPS})


# Add ODE and raylib libraries (-lpthread is needed along with the static linking to avoid dll dependency, also, include the libwinpthread-1.dll with the .exe file, they need to be in the same folder for the program to work)
target_link_libraries(${TARGET} -lode -lraylib -lopengl32 -lgdi32 -lwinmm -lpthread)