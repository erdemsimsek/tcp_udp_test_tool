cmake_minimum_required(VERSION 3.3.0)

project(TCP_UDP_Test_Tool VERSION 1.0.0 LANGUAGES CXX)

SET (CMAKE_CXX_COMPILER "/usr/bin/clang++" CACHE STRING "C++ compiler" FORCE)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Building for Linux")
endif ()

set(IMGUI_SRC_DIR "ImGUI")
set(IMGUI_BACKEND_SRC_DIR "ImGUI/backends")

include_directories("GUI")
include_directories("ImGUI")
include_directories("ImGUI/backends")

include_directories("GUI/header")

add_executable(${CMAKE_PROJECT_NAME} 
    main.cpp
    ImGUI/imgui.cpp
    ImGUI/imgui_tables.cpp
    ImGUI/imgui_widgets.cpp
    ImGUI/imgui_draw.cpp
    ImGUI/imgui_demo.cpp
    ImGUI/backends/imgui_impl_glfw.cpp
    ImGUI/backends/imgui_impl_opengl3.cpp
    GUI/src/mainMenuBar.cpp)


find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(${CMAKE_PROJECT_NAME} glfw OpenGL::GL ${CMAKE_DL_LIBS})