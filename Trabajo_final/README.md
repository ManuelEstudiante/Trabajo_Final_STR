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
│   ├── DiscreteSystems/          # Librería base de sistemas discretos
│   ├── DiscreteSystems.h
│   ├── ref.h                      # Generador de señales de referencia
│   ├── controlador.h              # Controlador PID discreto
│   └── convertidores.h            # Convertidores ADC/DAC
├── src/
│   ├── DiscreteSystem.cpp
│   ├── TransferFunctionSystem.cpp
│   ├── StateSpaceSystem.cpp
│   ├── ref.cpp                    # Implementación de señales
│   ├── controlador.cpp            # Implementación del PID
│   ├── convertidores.cpp          # Implementación de convertidores
│   ├── test_ref.cpp               # Pruebas del generador de señales
│   └── test_controlador.cpp       # Pruebas del controlador
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
./bin/test_ref          # Pruebas del generador de señales
./bin/test_controlador  # Pruebas del controlador PID, ADC y DAC
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

## Módulo: Controlador PID Discreto (controlador)

Implementa regulador PID en forma incremental (velocity form):

$$\Delta u[k] = a_0 e[k] + a_1 e[k-1] + a_2 e[k-2]$$

$$u[k] = u[k-1] + \Delta u[k]$$

Donde los coeficientes se calculan a partir de las ganancias:
- $a_0 = K_p + K_i T_s + K_d/T_s$
- $a_1 = -K_p - 2K_d/T_s$
- $a_2 = K_d/T_s$

### Ventajas de la forma incremental
- **Evita windup del integrador**: No acumula infinitamente
- **Mejor comportamiento numérico**: Menos propenso a errores de redondeo
- **Recomendado para sistemas embebidos**: Especialmente importante en tiempo real

### Características
- Constructor: `PIDController(Kp, Ki, Kd, Ts, bufferSize)`
- Sintonización on-line: `setKp()`, `setKi()`, `setKd()`, `setGains()`
- Buffers circulares para historial de muestras
- Método `next(error)` calcula la acción de control

## Módulo: Convertidores (convertidores)

### ADConverter (Analógico-Digital)
Introduce un retardo de una muestra para representar el tiempo de conversión real:

$$y_d[k] = y[k-1] \quad \Rightarrow \quad H(z) = z^{-1}$$

**Uso:**
```cpp
Convertidores::ADConverter adc(Ts);
double y_digital = adc.next(y_analogico);
```

### DAConverter (Digital-Analógico)
Paso directo de la señal digital:

$$u(t) = u[k], \quad kT_s \le t < (k+1)T_s \quad \Rightarrow \quad H(z) = 1$$

El comportamiento Zero-Order Hold (ZOH) se gestiona externamente mediante `nextAt()` en simulaciones multi-tasa.

**Uso:**
```cpp
Convertidores::DAConverter dac(Ts);
double u_analogico = dac.next(u_digital);
```

## Documentación

La documentación del código se genera con Doxygen. Todos los archivos incluyen comentarios compatible con Doxygen para generar documentación HTML/LaTeX.

## Próximos Pasos

- Sistema/Planta (discreto o discretizado)
- Simulación completa del lazo cerrado
- Hilos y sincronización (temporizadores, semáforos, mutex)
- Comunicación IPC con UI

## Autor

Manuel Gutiérrez


