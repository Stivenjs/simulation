cmake_minimum_required(VERSION 3.20)
project(DiscreteUniverseSimulation VERSION 1.0.0 LANGUAGES CXX)

# Configuración de C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Flags de compilación estrictos
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

# Exportar compile_commands.json para Clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Usar pkg-config para encontrar librerías (funciona en MSYS2, Linux, macOS)
find_package(PkgConfig REQUIRED)

# Buscar OpenGL
find_package(OpenGL REQUIRED)

# Buscar GLFW usando pkg-config
pkg_check_modules(GLFW REQUIRED glfw3)

# Buscar GLEW usando pkg-config
pkg_check_modules(GLEW REQUIRED glew)

# GLM es solo headers, no necesita linking
set(GLM_INCLUDE_DIR "")

# Incluir directorios
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${OPENGL_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
)

# Directorios de librerías
link_directories(
    ${GLFW_LIBRARY_DIRS}
    ${GLEW_LIBRARY_DIRS}
)

# Archivos fuente
set(SOURCES
    src/main.cpp
    src/engine/Window.cpp
)

# Ejecutable principal
add_executable(simulation ${SOURCES})

# Linkear librerías
target_link_libraries(simulation
    ${OPENGL_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${GLEW_LIBRARIES}
)

# Flags de compilación específicos
target_compile_options(simulation PRIVATE
    ${GLFW_CFLAGS_OTHER}
    ${GLEW_CFLAGS_OTHER}
)

# Configuración específica por plataforma
if(WIN32)
    # Windows: Agregar librerías del sistema
    target_link_libraries(simulation
        opengl32
        gdi32
    )
elseif(UNIX AND NOT APPLE)
    # Linux: Linkear X11 y otras dependencias
    target_link_libraries(simulation
        X11
        pthread
        dl
    )
elseif(APPLE)
    # macOS: Frameworks necesarios
    target_link_libraries(simulation
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()
