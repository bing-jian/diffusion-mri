#include <fstream>
#include <iostream>
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_vector.h>

#include "vnl_utils.h"

using namespace std;

int main(int argc, char* argv[]) {
  if (argc < 3) {
    cerr<< "Usage: vnl_nnls_test A b x" << endl;
    return -1;
  }
  vnl_matrix<float> A;
  ifstream infile_A(argv[1]);
  A.read_ascii(infile_A);
  vnl_vector<float> b;
  ifstream infile_b(argv[2]);
  b.read_ascii(infile_b);
  vnl_vector<float> x;
  x.set_size(A.cols());
  vnl_nnls(A, b, x);
  ofstream outfile(argv[3]);
  outfile << x;
}
