#include "vnl_utils.h"

#include <fstream>
#include <iostream>
#include <vnl/algo/vnl_matrix_inverse.h>
#include <vnl/vnl_trace.h>

#include "nnls.h"

#define BUFSIZE 4096

template <class T>
vnl_matrix_fixed<T, 3, 3> projection(vnl_vector_fixed<T, 3> v) {
  // vnl_vector_fixed<float, 3> v (x);
  v  = v.normalize();
  vnl_matrix_fixed<T, 3, 3> P;
  P.set_identity();
  P = P - outer_product(v, v);
  return P;
}

int vnl_nnls(const vnl_matrix<float>& A,
    const vnl_vector<float>& b, vnl_vector<float>& sol) {
  int m = A.rows();
  int n = A.cols();
  // Using column major for results....
  double *a = new double[m*n];
  for (int i = 0; i < n; ++i) {
    for(int j = 0; j < m; ++j) {
      a[i * m + j] = A(j, i);
    }
  }

  double *rhs = new double[m];
  for (int j = 0; j < m; ++j) {
    rhs[j] = b[j];
  }
  double rnorm;
  double *w = new double[n];
  double *zz = new double[m];
  int *indx = new int[3*n];
  int mode;

  double *x = new double[n];
  nnls(a, m, m, n, rhs, x, &rnorm, w, zz, indx, &mode);
  for (int i = 0; i < n; ++i) sol[i] = x[i];
  delete[] a;
  delete[] rhs;
  delete[] x;
  delete[] w;
  delete[] zz;
  delete[] indx;
  return mode;
}
