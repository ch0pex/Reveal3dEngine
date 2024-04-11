cmake_minimum_required(VERSION 3.22)
project(Reveal3dEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE ON)


include_directories(PUBLIC .)

#find_program(CLANG_TIDY_EXE NAMES clang-tidy)
#set(CMAKE_CXX_CLANG_TIDY "C:/Program Files/LLVM/bin/clang-tidy.exe" -header-filter=.*)
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(Engine)
add_subdirectory(Samples)
#add_subdirectory(Wrapper)
#add_subdirectory(Test)

