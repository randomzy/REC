cmake_minimum_required(VERSION 3.10)

project(RedEyeCorrection)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(utils)
find_package (Threads REQUIRED)

add_executable(RedEyeCorrection main.cpp Solution.cpp)
target_link_libraries(RedEyeCorrection utils Threads::Threads)
target_compile_options(RedEyeCorrection PUBLIC -Wall -Wextra)
