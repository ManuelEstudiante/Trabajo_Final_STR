/**
 * @file test_ref.cpp
 * @brief Programa de prueba para el generador de señales de referencia.
 *
 * Genera muestras de cada tipo de señal (escalón, rampa, senoidal) y
 * muestra los valores para verificación manual.
 */

#include "ref.h"
#include <iostream>
#include <iomanip>
#include <memory>

using namespace RefSignal;
using namespace std;

/**
 * @brief Función auxiliar para imprimir muestras de una señal.
 * @param signal Señal a muestrear.
 * @param name Nombre descriptivo de la señal.
 * @param num_samples Número de muestras a generar.
 */
void printSignal(Signal& signal, const string& name, int num_samples) {
    cout << "\n========================================\n";
    cout << "  " << name << "\n";
    cout << "========================================\n";
    cout << "  Ts = " << signal.T() << " s\n";
    cout << "----------------------------------------\n";
    cout << setw(6) << "k" << " | " 
         << setw(10) << "t(s)" << " | " 
         << setw(12) << "valor" << "\n";
    cout << "----------------------------------------\n";
    
    for (int k = 0; k < num_samples; ++k) {
        double value = signal.next();
        cout << setw(6) << k << " | " 
             << setw(10) << fixed << setprecision(3) << signal.t() - signal.T() << " | " 
             << setw(12) << setprecision(6) << value << "\n";
    }
    cout << "========================================\n\n";
}

int main() {
    cout << "\n";
    cout << "╔══════════════════════════════════════════════════════╗\n";
    cout << "║   PRUEBA DE GENERADOR DE SEÑALES DE REFERENCIA      ║\n";
    cout << "╚══════════════════════════════════════════════════════╝\n";

    const double Ts = 0.1;  // Período de muestreo: 100 ms
    const int num_samples = 10;

    // ========== SEÑAL ESCALÓN ==========
    // Escalón de amplitud 5.0 en t=0.3s
    auto step = StepSignal(Ts, 5.0, 0.3);
    printSignal(step, "SEÑAL ESCALÓN (amplitude=5.0, step_time=0.3s)", num_samples);

    // ========== SEÑAL RAMPA ==========
    // Rampa con pendiente 2.0 que empieza en t=0.2s
    auto ramp = RampSignal(Ts, 2.0, 0.2);
    printSignal(ramp, "SEÑAL RAMPA (slope=2.0, start_time=0.2s)", num_samples);

    // ========== SEÑAL SINUSOIDAL ==========
    // Seno de amplitud 3.0 y frecuencia 1 Hz
    auto sine = SineSignal(Ts, 3.0, 1.0);
    printSignal(sine, "SEÑAL SINUSOIDAL (amplitude=3.0, freq=1.0Hz)", num_samples);

    // ========== SEÑAL SINUSOIDAL CON OFFSET Y FASE ==========
    // Seno con offset=2.0 y fase=π/4
    auto sine_offset = SineSignal(Ts, 1.5, 0.5, 0.785398, 2.0);
    printSignal(sine_offset, "SEÑAL SINUSOIDAL (amplitude=1.5, freq=0.5Hz, phase=π/4, offset=2.0)", num_samples);

    cout << "Prueba completada exitosamente.\n\n";

    return 0;
}
