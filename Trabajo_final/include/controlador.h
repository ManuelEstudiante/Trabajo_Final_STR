/**
 * @file controlador.h
 * @brief Controlador PID discreto
 * @author Manuel
 * @date 2026-01-13
 */

#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include <DiscreteSystems/DiscreteSystem.h>
#include <vector>

/**
 * @defgroup Controlador Controlador PID Discreto
 * @brief Módulo del regulador PID digital
 * 
 * Implementa el controlador PID en forma incremental para control digital.
 * 
 * @{
 */

namespace Controlador {

/**
 * @class PIDController
 * @brief Controlador PID discreto en forma incremental con anti-windup
 * 
 * Implementa el algoritmo PID usando la forma incremental (velocity form):
 * 
 * \f$ \Delta u[k] = a_0 e[k] + a_1 e[k-1] + a_2 e[k-2] \f$
 * 
 * \f$ u[k] = u[k-1] + \Delta u[k] \f$
 * 
 * Donde los coeficientes son:
 * - \f$ a_0 = K_p + K_i T_s + \frac{K_d}{T_s} \f$
 * - \f$ a_1 = -K_p - 2\frac{K_d}{T_s} \f$
 * - \f$ a_2 = \frac{K_d}{T_s} \f$
 * 
 * **Ventajas de la forma incremental:**
 * - Evita windup del integrador
 * - Mejor comportamiento numérico
 * - Recomendado para sistemas embebidos
 * 
 * @note La señal de entrada debe ser el error: e[k] = r[k] - y[k]
 */
class PIDController : public DiscreteSystems::DiscreteSystem {
private:
    double Kp_;        ///< Ganancia proporcional
    double Ki_;        ///< Ganancia integral
    double Kd_;        ///< Ganancia derivativa
    
    double a0_;        ///< Coeficiente a0 = Kp + Ki*Ts + Kd/Ts
    double a1_;        ///< Coeficiente a1 = -Kp - 2*Kd/Ts
    double a2_;        ///< Coeficiente a2 = Kd/Ts
    
    double e_k1_;      ///< Error en k-1
    double e_k2_;      ///< Error en k-2
    double u_k1_;      ///< Salida en k-1
    
    /**
     * @brief Recalcula los coeficientes a0, a1, a2 a partir de Kp, Ki, Kd y Ts
     */
    void updateCoefficients();

protected:
    /**
     * @brief Calcula la acción de control para el error actual
     * @param ek Error de control e[k] = r[k] - y[k]
     * @return Acción de control u[k]
     */
    double compute(double ek) override;
    
    /**
     * @brief Reinicia el estado interno del controlador
     */
    void resetState() override;

public:
    /**
     * @brief Constructor del controlador PID
     * @param Kp Ganancia proporcional
     * @param Ki Ganancia integral
     * @param Kd Ganancia derivativa
     * @param Ts Período de muestreo [s]
     * @param bufferSize Tamaño del buffer de muestras (default: 1024)
     */
    PIDController(double Kp, double Ki, double Kd, double Ts, size_t bufferSize = 1024);
    
    /** @name Setters para sintonización on-line */
    ///@{
    /**
     * @brief Establece ganancia proporcional y recalcula coeficientes
     * @param Kp Nueva ganancia proporcional
     */
    void setKp(double Kp);
    
    /**
     * @brief Establece ganancia integral y recalcula coeficientes
     * @param Ki Nueva ganancia integral
     */
    void setKi(double Ki);
    
    /**
     * @brief Establece ganancia derivativa y recalcula coeficientes
     * @param Kd Nueva ganancia derivativa
     */
    void setKd(double Kd);
    
    /**
     * @brief Establece todas las ganancias simultáneamente
     * @param Kp Ganancia proporcional
     * @param Ki Ganancia integral
     * @param Kd Ganancia derivativa
     */
    void setGains(double Kp, double Ki, double Kd);
    ///@}
    
    /** @name Getters */
    ///@{
    double getKp() const { return Kp_; }
    double getKi() const { return Ki_; }
    double getKd() const { return Kd_; }
    ///@}
};

} // namespace Controlador

/** @} */ // fin del grupo Controlador

#endif // CONTROLADOR_H
