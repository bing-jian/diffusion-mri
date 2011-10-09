#ifndef VNL_UTILS_H_
#define VNL_UTILS_H_

#include <vnl/vnl_vector.h>
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_double_3x3.h>
#include <vnl/vnl_double_3.h>

template <class T> vnl_matrix_fixed<T, 3, 3> projection(vnl_vector_fixed<T, 3> v);
int vnl_nnls(const vnl_matrix<float>& A, const vnl_vector<float>& b, vnl_vector<float>& x);

#endif // VNL_UTILS_H_
