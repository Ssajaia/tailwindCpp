cmake_minimum_required(VERSION 3.15)
project(TailwindCPP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options
option(TW_DEBUG "Enable debug logging for unknown utilities" OFF)

if(TW_DEBUG)
    add_compile_definitions(TW_DEBUG)
endif()

# Demo executable
add_executable(tailwind_demo main.cpp)

# For header-only usage in other projects
add_library(tailwind_cpp INTERFACE)
target_include_directories(tailwind_cpp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Installation (optional)
install(FILES tailwind.h DESTINATION include)
install(TARGETS tailwind_cpp EXPORT TailwindCPPConfig DESTINATION lib/cmake/TailwindCPP)