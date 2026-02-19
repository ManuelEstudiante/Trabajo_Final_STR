/**
 * @file test_planta.cpp
 * @brief Programa de prueba para la planta/sistema
 * 
 * Verifica la respuesta del sistema a:
 * - Escalón unitario
 * - Rampa
 */

#include "planta.h"
#include <iostream>
#include <iomanip>

using namespace Planta;
using namespace std;

int main() {
    cout << "\n";
    cout << "╔══════════════════════════════════════════════════════╗\n";
    cout << "║   PRUEBA DE LA PLANTA/SISTEMA                        ║\n";
    cout << "╚══════════════════════════════════════════════════════╝\n\n";

    const double Tp = 0.01;   // 10 ms
    const int num_samples = 200;  // 2 segundos de simulación

    // ========== PRUEBA 1: RESPUESTA A ESCALÓN UNITARIO ==========
    cout << "========================================\n";
    cout << "  RESPUESTA A ESCALÓN UNITARIO\n";
    cout << "========================================\n";
    cout << "  Planta: G(s) = 1 / (0.5s + 1)\n";
    cout << "  Discretizada: Tp = " << Tp << "s\n";
    cout << "  Entrada: u[k] = 1.0 (escalón unitario)\n";
    cout << "----------------------------------------\n";
    cout << setw(6) << "k" << " | " 
         << setw(10) << "t(s)" << " | " 
         << setw(10) << "u[k]" << " | " 
         << setw(12) << "y[k]" << "\n";
    cout << "----------------------------------------\n";
    
    Sistema planta1(Tp);
    
    for (int k = 0; k < num_samples; ++k) {
        double uk = 1.0;  // Escalón unitario
        double yk = planta1.next(uk);
        double t = k * Tp;
        
        // Imprimir cada 20 muestras (cada 0.2s)
        if (k % 20 == 0) {
            cout << setw(6) << k << " | " 
                 << setw(10) << fixed << setprecision(3) << t << " | " 
                 << setw(10) << setprecision(6) << uk << " | " 
                 << setw(12) << setprecision(6) << yk << "\n";
        }
    }
    cout << "========================================\n\n";

    // ========== PRUEBA 2: RESPUESTA A RAMPA ==========
    cout << "========================================\n";
    cout << "  RESPUESTA A RAMPA\n";
    cout << "========================================\n";
    cout << "  Entrada: u[k] = 0.1 * k (rampa)\n";
    cout << "----------------------------------------\n";
    cout << setw(6) << "k" << " | " 
         << setw(10) << "t(s)" << " | " 
         << setw(10) << "u[k]" << " | " 
         << setw(12) << "y[k]" << "\n";
    cout << "----------------------------------------\n";
    
    Sistema planta2(Tp);
    
    for (int k = 0; k < 100; ++k) {
        double uk = 0.1 * k;  // Rampa
        double yk = planta2.next(uk);
        double t = k * Tp;
        
        // Imprimir cada 10 muestras (cada 0.1s)
        if (k % 10 == 0) {
            cout << setw(6) << k << " | " 
                 << setw(10) << fixed << setprecision(3) << t << " | " 
                 << setw(10) << setprecision(6) << uk << " | " 
                 << setw(12) << setprecision(6) << yk << "\n";
        }
    }
    cout << "========================================\n\n";

    // ========== PRUEBA 3: RESPUESTA A IMPULSO ==========
    cout << "========================================\n";
    cout << "  RESPUESTA A IMPULSO\n";
    cout << "========================================\n";
    cout << "  Entrada: u[0]=1.0, luego u[k]=0\n";
    cout << "----------------------------------------\n";
    cout << setw(6) << "k" << " | " 
         << setw(10) << "t(s)" << " | " 
         << setw(10) << "u[k]" << " | " 
         << setw(12) << "y[k]" << "\n";
    cout << "----------------------------------------\n";
    
    Sistema planta3(Tp);
    
    for (int k = 0; k < 50; ++k) {
        double uk = (k == 0) ? 1.0 : 0.0;  // Impulso en k=0
        double yk = planta3.next(uk);
        double t = k * Tp;
        
        cout << setw(6) << k << " | " 
             << setw(10) << fixed << setprecision(3) << t << " | " 
             << setw(10) << setprecision(6) << uk << " | " 
             << setw(12) << setprecision(6) << yk << "\n";
    }
    cout << "========================================\n\n";

    cout << "Pruebas de planta completadas exitosamente.\n\n";

    return 0;
}
