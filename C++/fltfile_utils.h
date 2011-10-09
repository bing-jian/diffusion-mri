// Based on code written by Tim McGraw and Zhizhou Wang

#ifndef FLTFILE_H_
#define FLTFILE_H_

#include <iostream>
#include <fstream>
#include <vector>

#define _LITTLE_ENDIAN
#define LITTLE_ENDIAN_MODE      0
#define BIG_ENDIAN_MODE         1

const unsigned IMPOSSIBLY_LARGE_DIM = 100;
typedef unsigned char BYTE;
typedef unsigned int  WORD32;

enum {DTTYPE_UNDEFINED, DTTYPE_BYTE, DTTYPE_INTEGER, DTTYPE_LWORD,
  DTTYPE_FLOAT, DTTYPE_DOUBLE, DTTYPE_COMPLEX};

class FltFile {
 public:
  FltFile();
  FltFile(const char*, const char = 0);
  int WriteFile(const char*, std::vector<float*>&, std::vector<unsigned>&, const char = 'b',bool b2DSlice=0);
  virtual ~FltFile();

  inline int samples() const {
    return num_samples_;
  }
  inline std::vector<float*> & data() {
    return data_;
  }
  inline std::vector<unsigned> & size() {
    return size_;
  }
  inline int num_samples() const {
    return num_samples_;
  }
  inline int volume() const {
    if (opened_)
      return volume_[3];
    else
      return -1;
  }
  // void CheckData();
  int ReadSignal(std::vector<float>&, unsigned);
  int ReadSignal(std::vector<float>&, unsigned*);
  //void PrintHeader();

 protected:
  // for file header
  unsigned dimension_;
  unsigned data_type_;
  unsigned num_elements_;
  unsigned num_samples_;
  std::vector<float *> data_;
  std::vector<unsigned> size_;
  std::vector<unsigned> volume_;

 private:
  int Read(const char*);
  int ParseHeader(std::ifstream&);
  int ReadFloat(std::ifstream&);
  int WriteHeader(std::ofstream&);
  void WriteFloat(std::ofstream&);
  void Reset();
  void WriteFloat(std::ofstream&, std::vector<float*>&);

  mutable char local_endian_, read_mode_, write_mode_;
  mutable bool swap_, opened_;

};

#endif  // FLTFILE_H_
