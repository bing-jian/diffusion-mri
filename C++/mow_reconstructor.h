#ifndef _mow_reconstructor_h_
#define _mow_reconstructor_h_

#include "reconstructor.h"

class MoWReconstructor: public Reconstructor {
 public:
  void DoReconstruction();
  MoWReconstructor(const char* f_config) :
      Reconstructor(f_config), p(2) {
    ew[0] = 0.0015f;
    ew[1] = 0.0004f;
    ew[2] = 0.0004f;
  }

 private:
  int N;
  float ew[3]; // eigenvalues;
  vnl_matrix<float> ev; // eigenvectors;
  float cond;
  float p; // p-paremeter of Wishart distribution
  vnl_matrix<float> A, Ainv, R, R_QBI, R_DSI;
  void ComputeP(vnl_vector<float>& S, vnl_vector<float>& P);
  void AssembleA();
  void LoadWishartParameters();
};
#endif //MOW_RECONSTRUCTOR_H_
