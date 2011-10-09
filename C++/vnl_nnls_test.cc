#include <fstream>
#include <iostream>
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_matrix.h>

#include "vnl_utils.h"

using namespace std;

int main(int argc, char* argv[]) {
  if (argc < 3) {
    cerr<< "Usage: vnl_nnls_test A b x" << endl;
    return -1;
  }
  vnl_matrix<double> A;
  A.read_ascii(ifstream(argv[1], ios_base::in));
  vnl_vector<double> b;
  b.read_ascii(ifstream(argv[2], ios_base::in));
  vnl_vector<double> x;
  x.set_size(A.cols());
  vnl_nnls(A, b, x);
  ofstream(argv[3], ios_base::out)<<x;
}
