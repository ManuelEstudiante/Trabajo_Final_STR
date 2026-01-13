/**
 * @file DiscreteSystems.h
 * @brief Cabecera principal de la librería DiscreteSystems
 * @author Profesor + GitHub Copilot
 * @date 2025-10-26
 * 
 * Esta cabecera incluye todos los componentes de la librería para
 * facilitar su uso. Incluye:
 * - Excepciones personalizadas
 * - Clase base DiscreteSystem (y estructura Sample)
 * - TransferFunctionSystem (función de transferencia)
 * - StateSpaceSystem (espacio de estados)
 * 
 * @example
 * #include <DiscreteSystems/DiscreteSystems.h>
 * 
 * using namespace DiscreteSystems;
 * 
 * // Crear sistema de función de transferencia
 * std::vector<double> b = {1.0, 0.5};
 * std::vector<double> a = {1.0, -0.8};
 * TransferFunctionSystem sys(b, a, 0.1);
 */

#ifndef DISCRETESYSTEMS_H
#define DISCRETESYSTEMS_H

#include "DiscreteSystems/Exceptions.h"
#include "DiscreteSystems/DiscreteSystem.h"
#include "DiscreteSystems/TransferFunctionSystem.h"
#include "DiscreteSystems/StateSpaceSystem.h"

#endif // DISCRETESYSTEMS_H
