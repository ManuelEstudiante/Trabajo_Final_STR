/**
 * @file StateSpaceSystem.cpp
 * @brief Implementación de StateSpaceSystem
 */

#include "DiscreteSystems/StateSpaceSystem.h"
#include "DiscreteSystems/Exceptions.h"

#include <algorithm>

namespace DiscreteSystems {

StateSpaceSystem::StateSpaceSystem(const std::vector<std::vector<double>>& A,
                                   const std::vector<double>& B,
                                   const std::vector<double>& C,
                                   double D,
                                   double Ts,
                                   size_t bufferSize)
    : DiscreteSystem(Ts, bufferSize), A_(A), B_(B), C_(C), D_(D), x_(), n_(A.size())
{
    // Validaciones de dimensiones
    if (n_ == 0) {
        throw InvalidDimensions("StateSpaceSystem: A no debe estar vacía y debe ser cuadrada (n x n)");
    }

    for (size_t i = 0; i < n_; ++i) {
        if (A_[i].size() != n_) {
            throw InvalidDimensions("StateSpaceSystem: A debe ser cuadrada (cada fila debe tener n columnas)");
        }
    }

    if (B_.size() != n_) {
        throw InvalidDimensions("StateSpaceSystem: el tamaño de B debe coincidir con A (n)");
    }

    if (C_.size() != n_) {
        throw InvalidDimensions("StateSpaceSystem: el tamaño de C debe coincidir con A (n)");
    }

    // Estado inicial a cero
    x_.assign(n_, 0.0);
}

double StateSpaceSystem::compute(double uk)
{
    // y(k) = C * x(k) + D * u(k)
    double yk = 0.0;
    for (size_t i = 0; i < n_; ++i) {
        yk += C_[i] * x_[i];
    }
    yk += D_ * uk;

    // x(k+1) = A * x(k) + B * u(k)
    std::vector<double> x_next(n_, 0.0);
    for (size_t i = 0; i < n_; ++i) {
        double sum = 0.0;
        const std::vector<double>& Ai = A_[i];
        for (size_t j = 0; j < n_; ++j) {
            sum += Ai[j] * x_[j];
        }
        sum += B_[i] * uk;
        x_next[i] = sum;
    }

    x_.swap(x_next);
    return yk;
}

void StateSpaceSystem::resetState()
{
    std::fill(x_.begin(), x_.end(), 0.0);
}

std::ostream& operator<<(std::ostream& os, const StateSpaceSystem& sys)
{
    const auto& A = sys.getA();
    const auto& B = sys.getB();
    const auto& C = sys.getC();
    const auto& x = sys.getState();
    size_t n = A.size();

    os << "StateSpaceSystem(n=" << n << ", D=" << sys.getD()
       << ", Ts=" << sys.getSamplingTime() << ")\n";

    os << "A = [";
    for (size_t i = 0; i < n; ++i) {
        if (i) os << "      ";
        os << "[";
        for (size_t j = 0; j < n; ++j) {
            os << A[i][j];
            if (j + 1 < n) os << ", ";
        }
        os << "]";
        if (i + 1 < n) os << ",\n";
    }
    os << "]\n";

    os << "B = [";
    for (size_t i = 0; i < B.size(); ++i) {
        os << B[i];
        if (i + 1 < B.size()) os << ", ";
    }
    os << "]\n";

    os << "C = [";
    for (size_t i = 0; i < C.size(); ++i) {
        os << C[i];
        if (i + 1 < C.size()) os << ", ";
    }
    os << "]\n";

    os << "x = [";
    for (size_t i = 0; i < x.size(); ++i) {
        os << x[i];
        if (i + 1 < x.size()) os << ", ";
    }
    os << "]\n";

    return os;
}

} // namespace DiscreteSystems
