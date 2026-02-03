# Motor de Simulación 3D con Renderizado

Motor híbrido en C++20 que combina simulación de universo discreto con renderizado 3D en tiempo real usando OpenGL. Modela un universo simplificado basado en reglas locales deterministas y lo visualiza en 3D.

## Inicio Rápido

```bash
# 1. Clonar el repositorio
git clone https://github.com/Stivenjs/simulation.git
cd simulation

# 2. Compilar (Windows con MinGW)
cmake -B build -S . -G "MinGW Makefiles"
cmake --build build

# 2. Compilar (Linux/macOS)
cmake -B build -S .
cmake --build build

# 3. Ejecutar
./build/simulation.exe  # Windows
./build/simulation      # Linux/macOS
```

## Características

- **Simulación discreta 3D** con estados configurables por celda
- **Renderizado 3D en tiempo real** con OpenGL 3.3+
- **Evolución determinista** basada en reglas locales (autómatas celulares)
- **Arquitectura modular** con separación entre simulación y renderizado
- **C++20 moderno** con verificación estricta de lints
- **Multiplataforma** (Windows, Linux, macOS)

## Requisitos

### Herramientas Necesarias

#### Windows

- **CMake** 3.20+ - [Descargar](https://cmake.org/download/)
- **MinGW-w64** (GCC 11+) via MSYS2 - [Descargar](https://www.msys2.org/)
  ```bash
  # Después de instalar MSYS2, ejecutar en terminal MSYS2 UCRT64:
  pacman -S mingw-w64-ucrt-x86_64-gcc
  pacman -S mingw-w64-ucrt-x86_64-gdb
  pacman -S mingw-w64-ucrt-x86_64-make
  pacman -S mingw-w64-ucrt-x86_64-glfw
  pacman -S mingw-w64-ucrt-x86_64-glew
  pacman -S mingw-w64-ucrt-x86_64-glm
  ```
  - Agregar `C:\msys64\ucrt64\bin` al PATH del sistema

#### Linux

- **CMake** 3.20+
  ```bash
  sudo apt install cmake  # Debian/Ubuntu
  sudo dnf install cmake  # Fedora
  ```
- **GCC** 11+ o **Clang** 12+
  ```bash
  sudo apt install build-essential  # Debian/Ubuntu
  sudo dnf install gcc-c++          # Fedora
  ```
- **Librerías OpenGL/GLFW**

  ```bash
  # Debian/Ubuntu
  sudo apt install libglfw3-dev libglew-dev libglm-dev

  # Fedora
  sudo dnf install glfw-devel glew-devel glm-devel
  ```

#### macOS

- **CMake** 3.20+
  ```bash
  brew install cmake
  ```
- **Clang** (viene con Xcode Command Line Tools)
  ```bash
  xcode-select --install
  ```
- **Librerías OpenGL/GLFW**
  ```bash
  brew install glfw glew glm
  ```

### Extensiones para Cursor/VSCode (Recomendadas)

- **Clangd** (`llvm-vs-code-extensions.vscode-clangd`) - IntelliSense y análisis de código
- **CMake Tools** (`ms-vscode.cmake-tools`) - Integración con CMake
- **CMake Language** (`twxs.cmake`) - Sintaxis de CMakeLists.txt

## Estado del Proyecto

**Fase actual**: Desarrollo incremental en progreso

### Completado

- Estructura base del proyecto
- Configuración completa del entorno de desarrollo
- Sistema de ventana con GLFW
- Inicialización de OpenGL 3.3+

### En desarrollo

- Renderizado básico (triángulo, cubo)
- Core: Cell y Grid 2D
- Visualización de Grid 2D en 3D
- Extensión a Grid 3D
- Cámara orbital
- Reglas de simulación

## Compilación

### Configuración Inicial

#### Windows (MinGW)

```bash
cmake -B build -S . -G "MinGW Makefiles" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build
```

#### Linux/macOS

```bash
cmake -B build -S . -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build
```

### Ejecución

#### Windows

```bash
./build/simulation.exe
```

#### Linux/macOS

```bash
./build/simulation
```

### Compilación con Flags Estrictos

El proyecto está configurado con flags estrictos: `-Wall -Wextra -Wpedantic`

Para compilar con `-Werror` (tratar warnings como errores):

```bash
cmake -B build -S . -DCMAKE_CXX_FLAGS="-Wall -Wextra -Wpedantic -Werror"
cmake --build build
```

### Limpiar y Recompilar

```bash
rm -rf build
cmake -B build -S .
cmake --build build
```

## Licencia

Este proyecto está licenciado bajo la Licencia MIT. Consulta el archivo [LICENSE](LICENSE) para más detalles.
