// File: maths/special.h
// Purpose: Declares special mathematical functions used by the ADIOS algorithm.
// Provides random number generation, special functions (gamma, digamma, factorial), and cubic equation solver.
// All functions are robust to edge cases and documented for developer clarity.
// Part of the ADIOS grammar induction project. See README for usage and structure.

#ifndef SPECIAL_H
#define SPECIAL_H

#include <cmath>
#include <vector>
#include <limits>

/**
 * @brief Smallest positive normalized double value.
 */
extern const double realmin;
/**
 * @brief Largest finite double value.
 */
extern const double realmax;
/**
 * @brief Largest finite integer value.
 */
extern const int intmax;

/**
 * @brief Returns a random double in [0,1).
 * @return Random double in [0,1).
 */
double uniform_rand();
/**
 * @brief Returns a random double in [l,u).
 * @param l Lower bound (inclusive).
 * @param u Upper bound (exclusive).
 * @return Random double in [l,u).
 */
double uniform_rand(double l, double u);
/**
 * @brief Returns a normally distributed random number (mean 0, stddev 1).
 * @return Random double from standard normal distribution.
 */
double normal_rand();
/**
 * @brief Returns a normally distributed random number with given mean and stddev.
 * @param mu Mean of the distribution.
 * @param stddev Standard deviation.
 * @return Random double from normal distribution.
 */
double normal_rand(double mu, double stddev);

/**
 * @brief Returns cotangent of x.
 * @param x Input value.
 * @return Cotangent of x.
 */
double cot(double x);

/**
 * @brief Returns the natural logarithm of the gamma function (Lanczos approximation).
 * @param x Input value.
 * @return ln(gamma(x)).
 */
double gammaln(double x);
/**
 * @brief Returns the digamma function (derivative of log gamma).
 * @param x Input value.
 * @return digamma(x).
 */
double digamma(double x);
/**
 * @brief Returns the natural logarithm of factorial(n).
 * @param n Input integer.
 * @return ln(n!).
 */
double factln(unsigned int n);
/**
 * @brief Returns the binomial probability.
 * @param k Number of successes.
 * @param n Number of trials.
 * @param p Probability of success.
 * @return Binomial probability.
 */
double binom(unsigned int k, unsigned int n, double p);

/**
 * @brief Solves a cubic equation a*x^3 + b*x^2 + c*x + d = 0.
 * @param a Coefficient of x^3.
 * @param b Coefficient of x^2.
 * @param c Coefficient of x.
 * @param d Constant term.
 * @param result0 Reference to store first real root.
 * @param result1 Reference to store second real root.
 * @param result2 Reference to store third real root.
 * @return Number of real roots found.
 */
unsigned int solve_cubic(double a, double b, double c, double d, double &result0, double &result1, double &result2);

/**
 * @brief Clamps value v to [min, max].
 * @tparam T Type of the value.
 * @param min Minimum value.
 * @param max Maximum value.
 * @param v Value to clamp (modified in place).
 */
template <class T>
inline void clamp(const T& min, const T& max, T& v)
{
    if (v < min) v = min;
    if (v > max) v = max;
}

#endif
