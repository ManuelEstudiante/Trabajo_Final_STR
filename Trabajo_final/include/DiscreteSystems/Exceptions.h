/**
 * @file Exceptions.h
 * @brief Definición de excepciones específicas para la librería DiscreteSystems
 * @author Profesor + GitHub Copilot
 * @date 2025-10-26
 */

#ifndef DISCRETESYSTEMS_EXCEPTIONS_H
#define DISCRETESYSTEMS_EXCEPTIONS_H

#include <stdexcept>
#include <string>

namespace DiscreteSystems {

/**
 * @class InvalidSamplingTime
 * @brief Excepción lanzada cuando el período de muestreo es inválido (Ts <= 0)
 */
class InvalidSamplingTime : public std::invalid_argument {
public:
    /**
     * @brief Constructor
     * @param message Mensaje descriptivo del error
     */
    explicit InvalidSamplingTime(const std::string& message)
        : std::invalid_argument(message) {}
};

/**
 * @class InvalidCoefficients
 * @brief Excepción lanzada cuando los coeficientes de una función de transferencia son inválidos
 * 
 * Ejemplos: denominador con a[0] == 0, vectores vacíos, etc.
 */
class InvalidCoefficients : public std::invalid_argument {
public:
    /**
     * @brief Constructor
     * @param message Mensaje descriptivo del error
     */
    explicit InvalidCoefficients(const std::string& message)
        : std::invalid_argument(message) {}
};

/**
 * @class InvalidDimensions
 * @brief Excepción lanzada cuando las dimensiones de matrices en espacio de estados son inconsistentes
 * 
 * Ejemplos: matriz A no cuadrada, tamaños de B o C incompatibles con A, etc.
 */
class InvalidDimensions : public std::invalid_argument {
public:
    /**
     * @brief Constructor
     * @param message Mensaje descriptivo del error
     */
    explicit InvalidDimensions(const std::string& message)
        : std::invalid_argument(message) {}
};

} // namespace DiscreteSystems

#endif // DISCRETESYSTEMS_EXCEPTIONS_H
