/**
 * @file TimeFuncs.cpp
 * @brief Implementation of time and seed-related utility functions for madios.
 *
 * This file provides functions for deterministic seeding and (optionally) time utilities.
 * The deterministic seed is used to ensure reproducibility in tests and runs.
 */

#include <chrono>

/**
 * @brief Global deterministic seed for reproducibility.
 *
 * This variable is used to ensure that random number generation is repeatable
 * across runs and matches the test/integration setup.
 */
unsigned int g_deterministic_seed = 42;

/**
 * @brief Get the deterministic seed used for this run.
 *
 * This function returns the global deterministic seed, which should match the
 * test/integration setup for reproducibility.
 *
 * @return The deterministic seed as an unsigned int.
 */
unsigned int getDeterministicSeed() {
    // Return the deterministic seed used for this run (should match test/integration setup)
    extern unsigned int g_deterministic_seed;
    return g_deterministic_seed;
}

// Note: The getTime() function was removed from this file to avoid duplication.
// The implementation now resides only in utils/TimeFuncs.cpp.