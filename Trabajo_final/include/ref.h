#pragma once

#include <vector>
#include <deque>
#include <memory>
#include <ostream>
#include <cstddef>
#include <cmath>

/**
 * @defgroup RefSignal Generador de Señal de Referencia
 * @brief Módulo para generar señales de referencia discretas para el sistema de control.
 *
 * Proporciona implementaciones de señales comunes en control:
 * - Escalón (step)
 * - Rampa (ramp)
 * - Senoidal (sine)
 *
 * Diseño:
 * - Clase base abstracta Signal con interfaz común
 * - Subclases concretas para cada tipo de señal
 * - Buffers circulares para almacenar historial de tiempos y valores
 * - Separación entre computación pura (compute) y muestreo (next)
 *
 * @{
 */

namespace RefSignal {

/**
 * @brief Clase base para señales temporizadas discretas de referencia.
 *
 * Proporciona la infraestructura común:
 * - Período de muestreo (Ts)
 * - Tiempo actual
 * - Buffers circulares para historial de tiempos y valores
 * - Interfaz abstracta para cálculo de muestras
 *
 * @note Esta clase es abstracta y define la interfaz que las subclases deben implementar.
 */
class Signal {
protected:
    double Ts_;             /**< Período de muestreo [s] */
    double offset_;         /**< Desplazamiento vertical */
    double t_;              /**< Tiempo actual [s] */
    std::size_t buffer_size_;  /**< Tamaño máximo del buffer circular */

    std::deque<double> time_buffer_;     /**< Buffer circular de tiempos */
    std::deque<double> value_buffer_;    /**< Buffer circular de valores */

    /**
     * @brief Añade un par (time, value) al buffer circular.
     * @param time Tiempo a almacenar.
     * @param value Valor de la señal a almacenar.
     */
    void addToBuffer(double time, double value);

public:
    /**
     * @brief Constructor de la clase Signal.
     * @param Ts Período de muestreo en segundos (debe ser > 0).
     * @param offset Desplazamiento vertical que se suma a la salida (default: 0.0).
     * @param buffer_size Tamaño máximo del buffer circular (default: 1024).
     * @throw std::invalid_argument si Ts <= 0 o buffer_size == 0.
     */
    explicit Signal(double Ts, double offset = 0.0, std::size_t buffer_size = 1024);

    /**
     * @brief Destructor virtual.
     */
    virtual ~Signal() = default;

    /**
     * @brief Calcula el valor de la señal en el tiempo actual sin modificar estado.
     * @return Valor de la señal en tiempo t_.
     */
    double compute() const;

    /**
     * @brief Calcula el valor de la señal en la muestra k sin modificar estado.
     * @param k Índice de muestra (0-based).
     * @return Valor de la señal en time = k * Ts.
     */
    double compute(std::size_t k) const;

    /**
     * @brief Calcula la siguiente muestra, la almacena en buffer y avanza el tiempo.
     *
     * Realiza:
     * 1. Computa el valor en t_ actual
     * 2. Almacena (t_, value) en los buffers
     * 3. Avanza t_ += Ts
     *
     * @return Valor calculado antes de avanzar el tiempo.
     */
    virtual double next();

    /**
     * @brief Reinicia la señal: pone t_ a 0 y limpia los buffers.
     */
    virtual void reset();

    /** @name Getters y Setters */
    ///@{
    double& T();
    const double& T() const;

    double& offset();
    const double& offset() const;

    double& t();
    const double& t() const;

    std::size_t& bufferSize();
    const std::size_t& bufferSize() const;

    const std::deque<double>& timeBuffer() const;
    const std::deque<double>& valueBuffer() const;
    ///@}

    /**
     * @brief Serializa buffers como CSV al stream de salida.
     * @param os Stream de salida.
     * @param s Señal a serializar.
     * @return Referencia al stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const Signal& s);

protected:
    /**
     * @brief Método puro virtual: subclases implementan cálculo en tiempo dado.
     * @param time Tiempo en segundos.
     * @return Valor de la señal en el tiempo especificado.
     */
    virtual double computeAt(double time) const = 0;
};

/**
 * @brief Señal de escalón (step).
 *
 * Genera una señal que es 0 hasta step_time, luego toma el valor amplitude.
 * Fórmula: r(t) = amplitude * H(t - step_time) + offset, donde H es la función escalón.
 */
class StepSignal : public Signal {
    double amplitude_;   /**< Valor del escalón */
    double step_time_;   /**< Tiempo en que ocurre el escalón [s] */

public:
    /**
     * @brief Constructor de señal escalón.
     * @param Ts Período de muestreo [s].
     * @param amplitude Amplitud del escalón.
     * @param step_time Tiempo en que ocurre el escalón [s].
     * @param offset Desplazamiento vertical adicional (default: 0.0).
     * @param buffer_size Tamaño del buffer (default: 1024).
     */
    StepSignal(double Ts, double amplitude, double step_time,
               double offset = 0.0, std::size_t buffer_size = 1024);

    /**
     * @brief Calcula el valor del escalón en un tiempo dado.
     * @param time Tiempo en segundos.
     * @return Valor del escalón: 0 si time < step_time, amplitude en otro caso.
     */
    double computeAt(double time) const override;

    /** @name Getters y Setters */
    ///@{
    double& amplitude();
    const double& amplitude() const;

    double& stepTime();
    const double& stepTime() const;
    ///@}
};

/**
 * @brief Señal de rampa (ramp).
 *
 * Genera una señal que crece linealmente: r(t) = slope * (t - start_time) + offset
 * para t >= start_time, y 0 en otro caso.
 */
class RampSignal : public Signal {
    double slope_;       /**< Pendiente de la rampa */
    double start_time_;  /**< Tiempo de inicio de la rampa [s] */

public:
    /**
     * @brief Constructor de señal rampa.
     * @param Ts Período de muestreo [s].
     * @param slope Pendiente de la rampa (unidades/segundo).
     * @param start_time Tiempo en que comienza la rampa [s].
     * @param offset Desplazamiento vertical inicial (default: 0.0).
     * @param buffer_size Tamaño del buffer (default: 1024).
     */
    RampSignal(double Ts, double slope, double start_time,
               double offset = 0.0, std::size_t buffer_size = 1024);

    /**
     * @brief Calcula el valor de la rampa en un tiempo dado.
     * @param time Tiempo en segundos.
     * @return Valor: 0 si time < start_time, slope * (time - start_time) en otro caso.
     */
    double computeAt(double time) const override;

    /** @name Getters y Setters */
    ///@{
    double& slope();
    const double& slope() const;

    double& startTime();
    const double& startTime() const;
    ///@}
};

/**
 * @brief Señal sinusoidal.
 *
 * Genera una onda senoidal: r(t) = amplitude * sin(2π * freq * t + phase) + offset
 */
class SineSignal : public Signal {
    double amplitude_;   /**< Amplitud de la señal */
    double freq_;        /**< Frecuencia en Hz */
    double phase_;       /**< Fase inicial en radianes */

public:
    /**
     * @brief Constructor de señal sinusoidal.
     * @param Ts Período de muestreo [s].
     * @param amplitude Amplitud de la onda.
     * @param freq Frecuencia en Hz.
     * @param phase Fase inicial en radianes (default: 0.0).
     * @param offset Desplazamiento vertical (default: 0.0).
     * @param buffer_size Tamaño del buffer (default: 1024).
     */
    SineSignal(double Ts, double amplitude, double freq,
               double phase = 0.0, double offset = 0.0,
               std::size_t buffer_size = 1024);

    /**
     * @brief Calcula el valor sinusoidal en un tiempo dado.
     * @param time Tiempo en segundos.
     * @return Valor: amplitude * sin(2π * freq * time + phase).
     */
    double computeAt(double time) const override;

    /** @name Getters y Setters */
    ///@{
    double& amplitude();
    const double& amplitude() const;

    double& frequency();
    const double& frequency() const;

    double& phase();
    const double& phase() const;
    ///@}
};

} // namespace RefSignal

/** @} */ // fin del grupo RefSignal
