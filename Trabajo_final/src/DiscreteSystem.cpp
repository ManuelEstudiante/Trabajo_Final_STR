/**
 * @file DiscreteSystem.cpp
 * @brief Implementación de la clase base DiscreteSystem
 */

#include "DiscreteSystems/DiscreteSystem.h"
#include "DiscreteSystems/Exceptions.h"

#include <ostream>

namespace DiscreteSystems {

DiscreteSystem::DiscreteSystem(double Ts, size_t bufferSize)
    : Ts_(Ts), k_(0), bufferSize_(bufferSize), writeIndex_(0), count_(0), buffer_()
{
    if (Ts_ <= 0.0) {
        throw InvalidSamplingTime("DiscreteSystem: el período de muestreo Ts debe ser > 0");
    }
    if (bufferSize_ == 0) {
        // Permitimos bufferSize_ == 0? Preferible lanzar excepción para evitar edge raro.
        throw InvalidDimensions("DiscreteSystem: el tamaño del buffer debe ser > 0");
    }
    buffer_.resize(bufferSize_);
}

double DiscreteSystem::next(double uk)
{
    double yk = compute(uk);      // Hook virtual implementado por derivadas
    storeSample(uk, yk);          // Gestiona el buffer circular
    ++k_;                         // Avanza el tiempo discreto
    return yk;
}

void DiscreteSystem::reset()
{
    k_ = 0;
    writeIndex_ = 0;
    count_ = 0;
    // No es necesario reinicializar todo el buffer, basta con ignorar muestras previas
    // pero lo dejamos con valores por defecto por claridad
    for (auto & s : buffer_) {
        s = Sample{0.0, 0.0, 0};
    }
    resetState(); // Hook para clases derivadas
}

void DiscreteSystem::bufferDump(std::ostream& os, ExportFormat format) const
{
    // Exportar en orden temporal: las count_ muestras más recientes
    // El buffer es circular, así que debemos reconstruir orden desde la más antigua.
    if (count_ == 0) {
        os << "# Empty buffer" << '\n';
        return;
    }

    // Determinar índice de la muestra más antigua
    size_t oldestIndex = (count_ == bufferSize_) ? (writeIndex_) : 0;

    if (format == ExportFormat::TSV) {
        os << "# k\tu(k)\ty(k)" << '\n';
        for (size_t i = 0; i < count_; ++i) {
            size_t idx = (oldestIndex + i) % bufferSize_;
            const Sample& s = buffer_[idx];
            os << s.k << '\t' << s.in << '\t' << s.out << '\n';
        }
    } else if (format == ExportFormat::MATLAB) {
        os << "% Export format: MATLAB compatible" << '\n';
        os << "% Columns: k u y" << '\n';
        os << "data = [";
        for (size_t i = 0; i < count_; ++i) {
            size_t idx = (oldestIndex + i) % bufferSize_;
            const Sample& s = buffer_[idx];
            os << s.k << ' ' << s.in << ' ' << s.out;
            if (i + 1 < count_) os << ';';
        }
        os << "];" << '\n';
        os << "% Usage in MATLAB/Octave: load('file'); k = data(:,1); u = data(:,2); y = data(:,3);" << '\n';
    }
}

void DiscreteSystem::storeSample(double uk, double yk)
{
    // Guardar muestra en posición writeIndex_
    buffer_[writeIndex_] = Sample{uk, yk, k_};

    if (count_ < bufferSize_) {
        ++count_;
    }
    writeIndex_ = (writeIndex_ + 1) % bufferSize_;
}

} // namespace DiscreteSystems
