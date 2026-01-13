/**
 * @file convertidores.h
 * @brief Convertidores ADC y DAC para el sistema de control
 * @author Manuel
 * @date 2026-01-13
 */

#ifndef CONVERTIDORES_H
#define CONVERTIDORES_H

#include <DiscreteSystems/DiscreteSystem.h>

/**
 * @defgroup Convertidores Convertidores Digitales
 * @brief Módulo de conversores analógico-digital y digital-analógico
 * 
 * Incluye:
 * - ADConverter: Conversor analógico-digital con retardo
 * - DAConverter: Conversor digital-analógico
 * 
 * @{
 */

namespace Convertidores {

/**
 * @class ADConverter
 * @brief Conversor Analógico-Digital con retardo de una muestra
 * 
 * Modela el proceso de muestreo y conversión A/D. Introduce un retardo
 * de una muestra para representar el tiempo de conversión:
 * 
 * \f$ y_d[k] = y(kT_s - T_s) = y((k-1)T_s) \f$
 * 
 * En términos de función de transferencia discreta:
 * \f$ H(z) = z^{-1} \f$
 * 
 * Este retardo evita dependencias algebraicas directas en el lazo
 * de control y representa el comportamiento real de los conversores.
 */
class ADConverter : public DiscreteSystems::DiscreteSystem {
private:
    double y_k1_;      ///< Valor de entrada retardado (y[k-1])

protected:
    /**
     * @brief Retarda la entrada una muestra
     * @param yk Señal analógica de entrada
     * @return Señal digital retardada y[k-1]
     */
    double compute(double yk) override;
    
    /**
     * @brief Reinicia el estado del conversor
     */
    void resetState() override;

public:
    /**
     * @brief Constructor del ADC
     * @param Ts Período de muestreo [s]
     * @param bufferSize Tamaño del buffer (default: 1024)
     */
    ADConverter(double Ts, size_t bufferSize = 1024);
};

/**
 * @class DAConverter
 * @brief Conversor Digital-Analógico (pasivo)
 * 
 * Convierte la señal digital a analógica. En este modelo simplificado,
 * actúa como un paso directo:
 * 
 * \f$ u(t) = u[k], \quad kT_s \le t < (k+1)T_s \f$
 * 
 * En términos de función de transferencia discreta:
 * \f$ H(z) = 1 \f$
 * 
 * El comportamiento ZOH (Zero-Order Hold) se gestiona externamente
 * mediante el método nextAt() cuando se requiere simulación multi-tasa.
 * 
 * @note Esta clase es principalmente un placeholder para mantener
 * consistencia arquitectónica. En simulaciones avanzadas, podría
 * incluir modelos de cuantización o dinámicas del conversor.
 */
class DAConverter : public DiscreteSystems::DiscreteSystem {
protected:
    /**
     * @brief Paso directo de la señal
     * @param uk Señal digital de entrada
     * @return Misma señal uk
     */
    double compute(double uk) override;
    
    /**
     * @brief Reinicia el estado (vacío para DAC)
     */
    void resetState() override;

public:
    /**
     * @brief Constructor del DAC
     * @param Ts Período de muestreo [s]
     * @param bufferSize Tamaño del buffer (default: 1024)
     */
    DAConverter(double Ts, size_t bufferSize = 1024);
};

} // namespace Convertidores

/** @} */ // fin del grupo Convertidores

#endif // CONVERTIDORES_H
