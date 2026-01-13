/**
 * @file controlador.cpp
 * @brief Implementación de los componentes de control discreto
 */

#include "controlador.h"
#include <DiscreteSystems/Exceptions.h>
#include <cmath>

namespace Controlador {

/*========================================================================*/
/*                          PID CONTROLLER                                */
/*========================================================================*/

PIDController::PIDController(double Kp, double Ki, double Kd, double Ts, size_t bufferSize)
    : DiscreteSystem(Ts, bufferSize),
      Kp_(Kp), Ki_(Ki), Kd_(Kd),
      a0_(0.0), a1_(0.0), a2_(0.0),
      e_k1_(0.0), e_k2_(0.0), u_k1_(0.0)
{
    updateCoefficients();
}

void PIDController::updateCoefficients() {
    double Ts = getSamplingTime();
    a0_ = Kp_ + Ki_ * Ts + Kd_ / Ts;
    a1_ = -Kp_ - 2.0 * Kd_ / Ts;
    a2_ = Kd_ / Ts;
}

double PIDController::compute(double ek) {
    // Forma incremental del PID
    // Δu[k] = a0*e[k] + a1*e[k-1] + a2*e[k-2]
    double delta_u = a0_ * ek + a1_ * e_k1_ + a2_ * e_k2_;
    
    // u[k] = u[k-1] + Δu[k]
    double uk = u_k1_ + delta_u;
    
    // Actualizar historiales
    e_k2_ = e_k1_;
    e_k1_ = ek;
    u_k1_ = uk;
    
    return uk;
}

void PIDController::resetState() {
    e_k1_ = 0.0;
    e_k2_ = 0.0;
    u_k1_ = 0.0;
}

void PIDController::setKp(double Kp) {
    Kp_ = Kp;
    updateCoefficients();
}

void PIDController::setKi(double Ki) {
    Ki_ = Ki;
    updateCoefficients();
}

void PIDController::setKd(double Kd) {
    Kd_ = Kd;
    updateCoefficients();
}

void PIDController::setGains(double Kp, double Ki, double Kd) {
    Kp_ = Kp;
    Ki_ = Ki;
    Kd_ = Kd;
    updateCoefficients();
}

} // namespace Controlador
