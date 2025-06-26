#pragma once

/**
 * @brief Global deterministic seed for reproducible operations.
 */
extern unsigned int g_deterministic_seed;

/**
 * @brief Get the current deterministic seed value.
 * @return The deterministic seed as an unsigned int.
 */
unsigned int getDeterministicSeed();

/**
 * @brief Get the current time in seconds (high-resolution).
 * @return The current time as a double (seconds).
 */
double getTime();