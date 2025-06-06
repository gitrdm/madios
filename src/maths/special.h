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

// Externally visible constants for numeric limits
extern const double realmin;
extern const double realmax;
extern const int intmax;

// Returns a random double in [0,1)
double uniform_rand();
// Returns a random double in [l,u)
double uniform_rand(double l, double u);
// Returns a normally distributed random number (mean 0, stddev 1)
double normal_rand();
// Returns a normally distributed random number with given mean and stddev
double normal_rand(double mu, double stddev);

// Returns cotangent of x
double cot(double x);

// Returns the natural logarithm of the gamma function (Lanczos approximation)
double gammaln(double x);
// Returns the digamma function (derivative of log gamma)
double digamma(double x);
// Returns the natural logarithm of factorial(n)
double factln(unsigned int n);
// Returns the binomial probability
//   k: number of successes, n: number of trials, p: probability of success
double binom(unsigned int k, unsigned int n, double p);

// Solves a cubic equation a*x^3 + b*x^2 + c*x + d = 0
// Returns the number of real roots and stores them in result0, result1, result2
unsigned int solve_cubic(double a, double b, double c, double d, double &result0, double &result1, double &result2);

// Clamps value v to [min, max]
template <class T>
inline void clamp(const T& min, const T& max, T& v)
{
    if (v < min) v = min;
    if (v > max) v = max;
}

#endif
