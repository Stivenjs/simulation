cmake_minimum_required(VERSION 3.20)
project(DiscreteUniverseSimulation VERSION 1.0.0 LANGUAGES CXX)

# Configuración de C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# FetchContent para descargar dependencias automáticamente
include(FetchContent)

# Flags de compilación estrictos
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

# Exportar compile_commands.json para Clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)

# OpenGL
find_package(OpenGL REQUIRED)

# GLFW
pkg_check_modules(GLFW REQUIRED glfw3)

# GLEW
pkg_check_modules(GLEW REQUIRED glew)

# Assimp - Carga de modelos 3D
# Instalar con:
#   MSYS2:  pacman -S mingw-w64-ucrt-x86_64-assimp
#   Ubuntu: sudo apt install libassimp-dev
#   macOS:  brew install assimp
pkg_check_modules(ASSIMP REQUIRED assimp)

# Dear ImGui - Interfaz de usuario
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imgui)

# stb - Carga de imágenes (single-header)
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(stb)

set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${OPENGL_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIRS}
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${stb_SOURCE_DIR}
)

link_directories(
    ${GLFW_LIBRARY_DIRS}
    ${GLEW_LIBRARY_DIRS}
    ${ASSIMP_LIBRARY_DIRS}
)

# Recopilar automáticamente todos los archivos .cpp
file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

# Agregar archivos de ImGui
list(APPEND SOURCES ${IMGUI_SOURCES})

add_executable(simulation ${SOURCES})

target_link_libraries(simulation
    ${GLFW_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${ASSIMP_LIBRARIES}
)

target_compile_options(simulation PRIVATE
    ${GLFW_CFLAGS_OTHER}
    ${GLEW_CFLAGS_OTHER}
    ${ASSIMP_CFLAGS_OTHER}
)

if(WIN32)
    target_link_libraries(simulation
        opengl32
        gdi32
    )
elseif(UNIX AND NOT APPLE)
    target_link_libraries(simulation
        X11
        pthread
        dl
    )
elseif(APPLE)
    target_link_libraries(simulation
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()
