#ifndef DOT_RECONSTRUCTOR_H_
#define DOT_RECONSTRUCTOR_H_

#include "reconstructor.h"

class DOTReconstructor: public Reconstructor {
 public:
  DOTReconstructor(const char* f_config) :
      Reconstructor(f_config) {
  }
  void DoReconstruction();

 private:
  void ComputeP(vnl_vector<float>& S, vnl_vector<float>& P);
  void AssembleM();
  void LoadDOTConstants();

 private:
  vnl_matrix<float> M;
  int degreeDOT;
  vnl_vector<float> wj;
};

#endif  // DOT_RECONSTRUCTOR_H_
