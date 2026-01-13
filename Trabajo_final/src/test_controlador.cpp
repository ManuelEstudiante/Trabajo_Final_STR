/**
 * @file test_controlador.cpp
 * @brief Programa de prueba para los componentes de control
 * 
 * Prueba:
 * - PIDController: Respuesta a escalón y sintonización on-line
 * - ADConverter: Verifica el retardo de una muestra
 * - DAConverter: Verifica el paso directo
 */

#include "controlador.h"
#include "convertidores.h"
#include <iostream>
#include <iomanip>

using namespace Controlador;
using namespace Convertidores;
using namespace std;

int main() {
    cout << "\n";
    cout << "╔══════════════════════════════════════════════════════╗\n";
    cout << "║   PRUEBA DE COMPONENTES DE CONTROL                   ║\n";
    cout << "╚══════════════════════════════════════════════════════╝\n\n";

    const double Ts = 0.1;  // 100 ms
    const int num_samples = 15;

    // ========== PRUEBA 1: PID CONTROLLER ==========
    cout << "========================================\n";
    cout << "  CONTROLADOR PID\n";
    cout << "========================================\n";
    cout << "  Kp=1.0, Ki=0.5, Kd=0.1, Ts=" << Ts << "s\n";
    cout << "  Entrada: error escalón e[k]=1.0\n";
    cout << "----------------------------------------\n";
    cout << setw(6) << "k" << " | " 
         << setw(10) << "e[k]" << " | " 
         << setw(12) << "u[k]" << "\n";
    cout << "----------------------------------------\n";
    
    PIDController pid(1.0, 0.5, 0.1, Ts);
    
    for (int k = 0; k < num_samples; ++k) {
        double error = 1.0;  // Error constante (escalón)
        double uk = pid.next(error);
        
        cout << setw(6) << k << " | " 
             << setw(10) << fixed << setprecision(6) << error << " | " 
             << setw(12) << setprecision(6) << uk << "\n";
    }
    cout << "========================================\n\n";

    // ========== PRUEBA 2: ADC CONVERTER ==========
    cout << "========================================\n";
    cout << "  CONVERSOR ADC (Retardo z^-1)\n";
    cout << "========================================\n";
    cout << "  Ts=" << Ts << "s\n";
    cout << "  Entrada: secuencia [1,2,3,4,5,...]\n";
    cout << "----------------------------------------\n";
    cout << setw(6) << "k" << " | " 
         << setw(10) << "in[k]" << " | " 
         << setw(12) << "out[k]" << "\n";
    cout << "----------------------------------------\n";
    
    ADConverter adc(Ts);
    
    for (int k = 0; k < 10; ++k) {
        double input = static_cast<double>(k + 1);
        double output = adc.next(input);
        
        cout << setw(6) << k << " | " 
             << setw(10) << fixed << setprecision(1) << input << " | " 
             << setw(12) << setprecision(1) << output << "\n";
    }
    cout << "  (Nota: out[k] = in[k-1], primer valor = 0)\n";
    cout << "========================================\n\n";

    // ========== PRUEBA 3: DAC CONVERTER ==========
    cout << "========================================\n";
    cout << "  CONVERSOR DAC (Paso directo)\n";
    cout << "========================================\n";
    cout << "  Ts=" << Ts << "s\n";
    cout << "  Entrada: secuencia [10,20,30,40,50]\n";
    cout << "----------------------------------------\n";
    cout << setw(6) << "k" << " | " 
         << setw(10) << "in[k]" << " | " 
         << setw(12) << "out[k]" << "\n";
    cout << "----------------------------------------\n";
    
    DAConverter dac(Ts);
    
    for (int k = 0; k < 5; ++k) {
        double input = static_cast<double>((k + 1) * 10);
        double output = dac.next(input);
        
        cout << setw(6) << k << " | " 
             << setw(10) << fixed << setprecision(1) << input << " | " 
             << setw(12) << setprecision(1) << output << "\n";
    }
    cout << "  (Nota: out[k] = in[k])\n";
    cout << "========================================\n\n";

    // ========== PRUEBA 4: PID CON SINTONIZACIÓN ON-LINE ==========
    cout << "========================================\n";
    cout << "  PID CON CAMBIO DE GANANCIAS\n";
    cout << "========================================\n";
    cout << "  Inicial: Kp=1.0, Ki=0.5, Kd=0.1\n";
    cout << "  Cambio en k=5: Kp=2.0, Ki=1.0, Kd=0.2\n";
    cout << "----------------------------------------\n";
    cout << setw(6) << "k" << " | " 
         << setw(10) << "e[k]" << " | " 
         << setw(12) << "u[k]" << "\n";
    cout << "----------------------------------------\n";
    
    PIDController pid2(1.0, 0.5, 0.1, Ts);
    
    for (int k = 0; k < 10; ++k) {
        if (k == 5) {
            pid2.setGains(2.0, 1.0, 0.2);
            cout << "  >>> Cambio de ganancias <<<\n";
        }
        
        double error = 1.0;
        double uk = pid2.next(error);
        
        cout << setw(6) << k << " | " 
             << setw(10) << fixed << setprecision(6) << error << " | " 
             << setw(12) << setprecision(6) << uk << "\n";
    }
    cout << "========================================\n\n";

    cout << "Pruebas completadas exitosamente.\n\n";

    return 0;
}
