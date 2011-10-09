#ifndef QBI_RECONSTRUCTOR_H_
#define QBI_RECONSTRUCTOR_H_

#include "reconstructor.h"

class QBIReconstructor: public Reconstructor {
 public:
  QBIReconstructor(const char* f_config) :
      Reconstructor(f_config) {
  }
  void DoReconstruction();
 private:
  vnl_matrix<float> SPH_basis_S, inv_SPH_basis_S;
  void ComputeP(vnl_vector<float>& S, vnl_vector<float>& P);
  void AssembleSPHBasisFromGradients();
  void ComputeODF(unsigned i, const vnl_vector<float>& S);
  void ComputeODF();
};

#endif //QBI_RECONSTRUCTOR_H_
