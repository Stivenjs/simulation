# Motor de Simulación de Universo Discreto

Motor de simulación discreta en C++20 que modela un universo simplificado basado en reglas locales deterministas.

## Características

- **Grilla 2D discreta** con estados configurables por celda
- **Evolución determinista** basada en reglas locales (autómatas celulares)
- **Arquitectura modular** preparada para extensión a 3D
- **C++20 moderno** con verificación estricta de lints

## Estado del Proyecto

**Fase actual**: Desarrollo incremental en progreso

### Completado
- Estructura base del proyecto

### En desarrollo
- Core: Cell y Grid con vecindario

## Compilación

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Verificación de Lints

El proyecto compila con flags estrictos: `-Wall -Wextra -Wpedantic -Werror`

```bash
cd build
cmake -DCMAKE_CXX_FLAGS="-Wall -Wextra -Wpedantic -Werror" ..
cmake --build .
```

## Licencia

MIT
