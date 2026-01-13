/**
 * @file convertidores.cpp
 * @brief Implementación de los convertidores ADC y DAC
 */

#include "convertidores.h"

namespace Convertidores {

/*========================================================================*/
/*                          ADC CONVERTER                                 */
/*========================================================================*/

ADConverter::ADConverter(double Ts, size_t bufferSize)
    : DiscreteSystem(Ts, bufferSize),
      y_k1_(0.0)
{}

double ADConverter::compute(double yk) {
    // Retardo de una muestra: y_d[k] = y[k-1]
    double output = y_k1_;
    y_k1_ = yk;
    return output;
}

void ADConverter::resetState() {
    y_k1_ = 0.0;
}

/*========================================================================*/
/*                          DAC CONVERTER                                 */
/*========================================================================*/

DAConverter::DAConverter(double Ts, size_t bufferSize)
    : DiscreteSystem(Ts, bufferSize)
{}

double DAConverter::compute(double uk) {
    // Paso directo: y[k] = u[k]
    return uk;
}

void DAConverter::resetState() {
    // No hay estado interno
}

} // namespace Convertidores
