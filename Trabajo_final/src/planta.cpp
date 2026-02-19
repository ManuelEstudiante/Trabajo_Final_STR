/**
 * @file planta.cpp
 * @brief Implementación de la planta/sistema
 */

#include "planta.h"

namespace Planta {

Sistema::Sistema(double Tp, size_t bufferSize)
    : TransferFunctionSystem(
        {0.0099, 0.0099},    // numerador: b = [0.0099, 0.0099]
        {1.0, -0.9802},      // denominador: a = [1.0, -0.9802]
        Tp,                   // período de muestreo
        bufferSize
    )
{}

} // namespace Planta
