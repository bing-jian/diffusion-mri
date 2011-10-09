#ifndef SPHERICAL_HARMONICS_H_
#define SPHERICAL_HARMONICS_H_

#include <vnl/vnl_vector.h>
#include <vnl/vnl_matrix.h>

void convert_coeffs(int degree, const vnl_vector<float>& sym_coeff,
    vnl_vector<float>& coeff_real, vnl_vector<float>& coeff_imag);

void compute_qball_ODF(int degree, vnl_vector<float>& sym_coeff,
    vnl_vector<float>& coeff_real, vnl_vector<float>& coeff_imag);

vnl_matrix <float> construct_basis(int degree,
    const vnl_matrix<float>& sphere_points);

void comp_magnitude(int degree, const vnl_vector<float>& sym_coeff,
    vnl_vector<float>& coeff_mag);

#endif  //SPHERICAL_HARMONICS_H_
