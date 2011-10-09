#ifndef SPECIAL_FUNCTIONS_H_
#define SPECIAL_FUNCTIONS_H_

#define PI 3.141592653589f

#ifdef __cplusplus
extern "C" {
#endif

/*
   associated Legendre polynomial
   P_l^m(cos(t))
   =(-1)^{l + m} /(2^l l!) sin^m(t)(d/d cos(t))^{l + m} sin^{2l}(t)
*/
double pLegendre(int l, int m, double t);

double logGamma(double xx);

double factorial(int n);

/* TODO:
double erf(double x);
double erfc(double x);
*/

#ifdef __cplusplus
}
#endif

#endif  // SPECIAL_FUNCTIONS_H_

