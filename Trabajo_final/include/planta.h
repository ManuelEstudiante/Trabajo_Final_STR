/**
 * @file planta.h
 * @brief Módulo de la planta/sistema del control
 * @author Manuel Gutiérrez
 * @date 2026-02-19
 */

#ifndef PLANTA_H
#define PLANTA_H

#include <DiscreteSystems/TransferFunctionSystem.h>

/**
 * @defgroup Planta Sistema/Planta de Control
 * @brief Módulo que modela el sistema físico a controlar
 * 
 * Implementa un modelo discreto de primer orden típico de un motor:
 * G(s) = 1 / (0.5s + 1)
 * 
 * Discretizado con Tustin a Tp = Ts/N = 0.01s:
 * G(z) = (0.0099 + 0.0099*z^-1) / (1 - 0.9802*z^-1)
 * 
 * @{
 */

namespace Planta {

/**
 * @class Sistema
 * @brief Modelo discreto del sistema/planta de control
 * 
 * Encapsula una planta de primer orden discretizada.
 * Típicamente se actualiza a tasa rápida (Tp = Ts/N).
 * 
 * Función de transferencia continua original:
 * \f$ G(s) = \frac{1}{0.5s + 1} \f$
 * 
 * Discretizada con método Tustin a Tp=0.01s:
 * \f$ G(z) = \frac{0.0099 + 0.0099 z^{-1}}{1 - 0.9802 z^{-1}} \f$
 */
class Sistema : public DiscreteSystems::TransferFunctionSystem {
public:
    /**
     * @brief Constructor de la planta
     * @param Tp Período de muestreo de la planta [s] (default: 0.01)
     * @param bufferSize Tamaño del buffer (default: 1024)
     */
    Sistema(double Tp = 0.01, size_t bufferSize = 1024);
};

} // namespace Planta

/** @} */ // fin del grupo Planta

#endif // PLANTA_H
