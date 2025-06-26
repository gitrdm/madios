/**
 * @file logging.cpp
 * @brief Implementation of global logging level for madios.
 *
 * This file defines the global log level variable used throughout the project.
 */

#include "logging.h"

namespace madios {

/**
 * @brief The current global log level for madios logging.
 *
 * Controls the verbosity of log output. Set to LOG_WARN by default.
 * Can be changed at runtime to adjust logging detail.
 */
LogLevel CURRENT_LOG_LEVEL = LOG_WARN;

} // namespace madios
