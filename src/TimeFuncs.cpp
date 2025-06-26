#include <chrono>

unsigned int g_deterministic_seed = 42;

unsigned int getDeterministicSeed() {
    // Return the deterministic seed used for this run (should match test/integration setup)
    extern unsigned int g_deterministic_seed;
    return g_deterministic_seed;
}

// Removed duplicate getTime() definition; only use the one in utils/TimeFuncs.cpp