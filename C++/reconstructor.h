#ifndef RECONSTRUCTOR_H_
#define RECONSTRUCTOR_H_

#include <cstring>
#include <iostream>
#include <vector>
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_vector.h>

class FltFile;

const int BUFF_MAX = 256;

// Abstract base class for reconstruction task.
// TODO: Disallow copy and assignment.
class Reconstructor {
 public:
  Reconstructor(const char* config) :
      fltFile(0), S0Image(0), b2Dslice(0),
      minSig(0.001f), S0Scale(1), S0default(1), bUseS0(0),
      bUseBMatrix(0), bFixedBValue(1), degree(6) {
    strcpy(f_config, config);
  }

  virtual ~Reconstructor();

  //void SetConfigFile(const char* input_config);
  // Interface for performing reconstruction.
  // Should be implemented by derived classes.
  virtual void DoReconstruction() = 0;

 protected:
  virtual void ComputeP(vnl_vector<float>& S, vnl_vector<float>& P) = 0;
  bool ReadSignal(unsigned i, vnl_vector <float> &S);
  void SignalAttenuation(unsigned i, vnl_vector <float> &S);
  void ComputeADC(vnl_vector <float> &S);
  void SaveProbabilityProfile(unsigned i, vnl_vector <float> &P);
  void Reconstruct();
  void Write();
  void Prepare();
  void LoadOutputDataFiles();
  void LoadInputDataFiles();
  void PrepareInputData();
  void PrepareOutputData();
  void LoadImagingParams();
  void LoadCommonConstants();
  void LoadFltFile(const char*);
  void LoadS0File(const char*);
  void WriteFltFile(const char*);
  void AssembleSPHBasis();

 protected:
  // diffusion time, measured in seconds; typical value: 0.020s.
  float t;
  // radius of the probability surface, measured in mm = 1000um; typical value: 0.015mm
  float r;
  // regularization factor for spherical harmonics coefficients
  float lambda;
  // configuration file
  char f_config[512];
  // int K, T;
  // K*3 matrix containing gradient directions where K is the # of measurements
  vnl_matrix<float> gradients;
  // K*6 matrix containing b-matrices where K is the # of measurements
  vnl_matrix<float> bmatrix;
  // T*6 matrix containing tessellation vertices where T is the # of vertices
  vnl_matrix<float> tessellation;
  // b-values of diffusion gradients
  vnl_vector<float> b;

  FltFile* fltFile;
  int nSamples, nVoxels;
  int bUseBMatrix;

  int degree;
  int nCoeffs, nMoreCoeffs;
  std::vector<float *> m_SPH_coeff;
  //std::vector<float *> m_SPH_coeff_mag;
  std::vector<float *> m_coeff_real;
  std::vector<float *> m_coeff_imag;
  std::vector<float *> m_weights;

  std::vector<float *> m_S_coeff;
  std::vector<float *> m_S_coeff_real;
  std::vector<float *> m_S_coeff_imag;
  std::vector<float *> m_qball_coeff_real;
  std::vector<float *> m_qball_coeff_imag;

 private:
  float minSig;
  std::vector<float*> data, S0;

  vnl_matrix<float> SPH_basis, inv_SPH_basis;
  FltFile*  S0Image;
  float S0default, S0Scale;
  bool b2Dslice;
  int bUseS0;
  int bFixedBValue;
};
#endif  // RECONSTRUCTOR_H_
