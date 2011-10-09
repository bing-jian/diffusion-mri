#include "nnls.h"

#include <iostream>
using namespace std;

int main() {
  double a[] = {
    // Note that the matrix is in column major.
    1, 4, 5, 10,
    5, 1, 4, 1,
    9, 10, 12, 20
  };
  /*
    i.e. nnls will give NNLS solution for:
   [ 1 10 4 10]    [1]
   [ 4 5  1 12]x - [3]
   [ 5 1  9 20]    [4]
  */
  int    mda = 3;
  int    m = 3, n = 4;
  double b[] = {4, 7, 4, 1};
  double x[] = {1, 1, 1, 1, 1};
  double rnorm;
  double w[5];
  double zz[5];
  int indx[6];
  int mode;
  int i;

  nnls(a, mda, m, n, b, x, &rnorm, w, zz, indx, &mode);
  for (i = 0; i < 5; ++i) {
    cout << x[i] << endl;
  }
  return 0;
}
