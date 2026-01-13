# Sistema de Control en Lazo Cerrado - Tiempo Real

Simulación de un sistema de control digital en lazo cerrado implementado con herramientas de tiempo real (hilos, semáforos, mutex, temporizadores) en C++.

## Descripción del Proyecto

Este proyecto implementa un sistema de control realimentado con los siguientes componentes:

```
       r(k)     e(k)      u(k)       u(t)      y(t)
Ref ---->(+)---> Regulador ---> DAC ---> Sistema ----+
         (-)                                          |
          ^                                           |
          |           s(k)                            |
          +---------- ADC <-------------------------+
```

### Componentes

- **Generador de Señales de Referencia** (`ref`): Escalón, rampa, senoidal
- **Regulador Discreto**: Controladores P, PI, PID en tiempo discreto
- **DAC**: Conversor digital-analógico
- **Sistema/Planta**: Modelo del sistema a controlar
- **ADC**: Conversor analógico-digital

### Características de Tiempo Real

- Cada bloque es un hilo independiente
- Sincronización con temporizadores, semáforos y mutex
- Proceso principal: sistema de control
- Proceso secundario: UI para visualización y control (mediante IPC)

## Estructura del Proyecto

```
Trabajo_final/
├── include/
│   ├── ref.h              # Generador de señales de referencia
│   └── test_ref.h         # (Pruebas)
├── src/
│   ├── ref.cpp            # Implementación de señales
│   └── test_ref.cpp       # Programa de prueba
├── CMakeLists.txt
├── README.md
└── .gitignore
```

## Compilación

### Requisitos
- CMake >= 3.10
- Compilador C++ que soporte C++11 (gcc, clang)
- Linux/Unix

### Pasos

```bash
cd Trabajo_final
mkdir build && cd build
cmake ..
make
```

### Ejecutar pruebas

```bash
./bin/test_ref
```

## Módulo: Generador de Señales (ref)

Proporciona tres tipos de señales temporizadas discretas:

### Escalón (StepSignal)
```cpp
RefSignal::StepSignal step(Ts=0.1, amplitude=5.0, step_time=0.3);
```
Genera: 0 hasta t=0.3s, luego amplitud 5.0

### Rampa (RampSignal)
```cpp
RefSignal::RampSignal ramp(Ts=0.1, slope=2.0, start_time=0.2);
```
Genera: 0 hasta t=0.2s, luego crece linealmente con pendiente 2.0

### Senoidal (SineSignal)
```cpp
RefSignal::SineSignal sine(Ts=0.1, amplitude=3.0, freq=1.0);
```
Genera: oscilación senoidal de amplitud 3.0 a 1 Hz

**Interfaz común:**
- `compute()`: Calcula valor en tiempo actual sin avanzar
- `next()`: Calcula, almacena en buffer y avanza tiempo
- `reset()`: Reinicia la señal

## Documentación

La documentación del código se genera con Doxygen. Todos los archivos incluyen comentarios compatible con Doxygen para generar documentación HTML/LaTeX.

## Próximos Pasos

- Regulador discreto (P, PI, PID)
- Sistema/Planta
- DAC y ADC
- Hilos y sincronización
- Comunicación IPC con UI

## Autor

Manuel

## Licencia

(Por definir)
