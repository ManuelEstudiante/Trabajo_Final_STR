cmake_minimum_required(VERSION 3.10)

project(ControlSystem VERSION 1.0 LANGUAGES CXX)

# Configuración del estándar C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Directorio de salida para binarios
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Directorio de includes
include_directories(${PROJECT_SOURCE_DIR}/include)

# Opciones de compilación
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# ============================================
# Biblioteca DiscreteSystems
# ============================================
add_library(discretesystems STATIC
    src/DiscreteSystem.cpp
    src/TransferFunctionSystem.cpp
    src/StateSpaceSystem.cpp
)

target_include_directories(discretesystems PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# ============================================
# Biblioteca RefSignal
# ============================================
add_library(refsignal STATIC
    src/ref.cpp
)

target_include_directories(refsignal PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# ============================================
# Biblioteca Controlador
# ============================================
add_library(controlador STATIC
    src/controlador.cpp
)

target_include_directories(controlador PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(controlador
    discretesystems
)

# ============================================
# Biblioteca Convertidores
# ============================================
add_library(convertidores STATIC
    src/convertidores.cpp
)

target_include_directories(convertidores PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(convertidores
    discretesystems
)

# ============================================
# Ejecutable de prueba: test_ref
# ============================================
add_executable(test_ref
    src/test_ref.cpp
)

target_link_libraries(test_ref
    refsignal
)

# ============================================
# Ejecutable de prueba: test_controlador
# ============================================
add_executable(test_controlador
    src/test_controlador.cpp
)

target_link_libraries(test_controlador
    controlador
    convertidores
    discretesystems
)

# ============================================
# Información de compilación
# ============================================
message(STATUS "Proyecto: ${PROJECT_NAME}")
message(STATUS "Directorio de binarios: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
