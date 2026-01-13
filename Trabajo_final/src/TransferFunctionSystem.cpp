/**
 * @file TransferFunctionSystem.cpp
 * @brief Implementación de TransferFunctionSystem
 */

#include "DiscreteSystems/TransferFunctionSystem.h"
#include "DiscreteSystems/Exceptions.h"

#include <algorithm>

namespace DiscreteSystems {

TransferFunctionSystem::TransferFunctionSystem(const std::vector<double>& b,
											   const std::vector<double>& a,
											   double Ts,
											   size_t bufferSize)
	: DiscreteSystem(Ts, bufferSize), b_(), a_(), uHist_(), yHist_()
{
	// Validación de coeficientes básicos
	if (a.empty()) {
		throw InvalidCoefficients("TransferFunctionSystem: el denominador 'a' no debe estar vacío");
	}
	if (b.empty()) {
		throw InvalidCoefficients("TransferFunctionSystem: el numerador 'b' no debe estar vacío");
	}
	if (a[0] == 0.0) {
		throw InvalidCoefficients("TransferFunctionSystem: a[0] debe ser distinto de 0 para permitir la normalización");
	}

	// Normalización para que a[0] == 1
	const double a0 = a[0];
	a_.resize(a.size());
	for (size_t i = 0; i < a.size(); ++i) {
		a_[i] = a[i] / a0;
	}
	b_.resize(b.size());
	for (size_t i = 0; i < b.size(); ++i) {
		b_[i] = b[i] / a0;
	}

	// Inicializar historiales
	uHist_.assign(b_.size(), 0.0);              // [u(k), u(k-1), ..., u(k-m)]
	yHist_.assign(a_.size() > 0 ? a_.size() - 1 : 0, 0.0); // [y(k-1), ..., y(k-n)]
}

double TransferFunctionSystem::compute(double uk)
{
	// Desplazar historial de entradas: uHist_[0] será u(k)
	if (!uHist_.empty()) {
		for (size_t i = uHist_.size() - 1; i > 0; --i) {
			uHist_[i] = uHist_[i - 1];
		}
		uHist_[0] = uk;
	}

	// Parte del numerador: sum b[i] * u(k-i)
	double y_num = 0.0;
	for (size_t i = 0; i < b_.size(); ++i) {
		y_num += b_[i] * uHist_[i];
	}

	// Parte del denominador: sum a[j] * y(k-j) para j=1..n (a[0] == 1)
	double y_den = 0.0;
	for (size_t j = 1; j < a_.size(); ++j) {
		y_den += a_[j] * yHist_[j - 1];
	}

	const double yk = y_num - y_den;

	// Desplazar historial de salidas e insertar y(k)
	if (!yHist_.empty()) {
		for (size_t i = yHist_.size() - 1; i > 0; --i) {
			yHist_[i] = yHist_[i - 1];
		}
		yHist_[0] = yk;
	}

	return yk;
}

void TransferFunctionSystem::resetState()
{
	std::fill(uHist_.begin(), uHist_.end(), 0.0);
	std::fill(yHist_.begin(), yHist_.end(), 0.0);
}

std::ostream& operator<<(std::ostream& os, const TransferFunctionSystem& sys)
{
	const auto& b = sys.getNumerator();
	const auto& a = sys.getDenominator();

	os << "TransferFunctionSystem("
	   << "m=" << (b.size() ? b.size() - 1 : 0)
	   << ", n=" << (a.size() ? a.size() - 1 : 0)
	   << ", Ts=" << sys.getSamplingTime() << ")\n";

	os << "b = [";
	for (size_t i = 0; i < b.size(); ++i) {
		os << b[i];
		if (i + 1 < b.size()) os << ", ";
	}
	os << "]\n";

	os << "a = [";
	for (size_t i = 0; i < a.size(); ++i) {
		os << a[i];
		if (i + 1 < a.size()) os << ", ";
	}
	os << "]\n";

	return os;
}

} // namespace DiscreteSystems

