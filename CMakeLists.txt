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

# Ejecutable principal
add_executable(simulation main.cpp)
