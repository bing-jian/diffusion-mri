#include "fltfile_utils.h"

#include <stdio.h>
#include <iostream>
#include <fstream>

#include "bits_utils.h"

using namespace std;

FltFile::FltFile()
    : read_mode_(0), write_mode_('b'),
      swap_(false), opened_(false),
      data_type_(4), num_samples_(1) {
  local_endian_ = is_this_machine_little_endian()?'l':'b';
}

FltFile::FltFile(const char* filename, const char mode)
    : read_mode_(mode), write_mode_('b'),
      swap_(false), opened_(false) {
  // Initialize pointers.
  local_endian_ = is_this_machine_little_endian()?'l':'b';
  num_samples_ = 1;
  Read(filename);
}

int FltFile::Read(const char* filename) {
  // TODO: Handle complex data type.
  ifstream ifs(filename, ios::in | ios::binary);
  if (ifs.is_open()) {
    if (ParseHeader(ifs)) {
      // Continue to read data according to different data type.
      switch (data_type_) {
        case DTTYPE_FLOAT:
          cout << " Data type is float.\n";
          return ReadFloat(ifs);
          break;
        default:
          printf(" ERROR: data type %d is not supported yet.\n", data_type_);
          return -1;
          break;
      }
      return 1;
    } else {
      return -1;
    }
  } else {
    cerr << "Unable to open file " << filename << endl;
    return -1;
  }
}

// readheader
int FltFile::ParseHeader(ifstream& ifs) {
  unsigned dim;
  ifs.read((char *)&dim, sizeof(unsigned)); //4 == sizeof(unsigned)
  swap_ = (dim > IMPOSSIBLY_LARGE_DIM);
  if (swap_) {
    swap_bytes((unsigned char*)&dim, 4);
  }
  dimension_ = dim;
  if (dimension_ != 4 && dimension_ != 3) {
    cerr << " ERROR: Expect 4D or 3D data set!\n";
    return -1;
  }
  unsigned dim_size, num_elements = 1;
  volume_.push_back(num_elements);
  for (unsigned i = 0; i < dimension_; ++i) {
    ifs.read((char*)&dim_size, 4);
    if (swap_)
      swap_bytes((unsigned char*)&dim_size, 4);
    num_elements *= dim_size;
    size_.push_back(dim_size);
    volume_.push_back(num_elements);
  }
  if (dimension_ == 4) {
    //For 3D data, sample total is set to 1 in the constructor
    num_samples_ = size_[3];
  }
  ifs.read((char*) &data_type_, 4);
  ifs.read((char*) &num_elements_, 4);
  if (swap_) {
    swap_bytes((unsigned char*)&num_elements_, 4);
    swap_bytes((unsigned char*)&data_type_, 4);
  }
  return 1;
}

int FltFile::ReadFloat(ifstream& ifs) {
  if (data_type_ != DTTYPE_FLOAT) {
    cerr << " ERROR: Data type is not 32bit float.\n";
    return -1;
  }
  for (unsigned i = 0; i < num_samples_; ++i) {
    //TODO: check possible bug here.
    float* buffer = new float[volume_[3]];
    ifs.read((char*)buffer, 4*volume_[3]);
    if (read_mode_ == 0 && swap_) {
      //default option is to assume the byteorder in data section
      //is the same as in the header
      for (unsigned j = 0; j < volume_[3]; ++j) {
        swap_bytes((unsigned char*)(buffer + j), 4);
      }
    }
    if (read_mode_>0 && read_mode_ != local_endian_) {
      //if the read mode is explicitly specified, check if it is
      //consistent with the local endian
      for (unsigned j = 0; j < volume_[3]; ++j) {
        swap_bytes((unsigned char*)(buffer + j), 4);
      }
    }
    data_.push_back(buffer);
  }
  opened_ = true;
  return 1;
}

//readsignal // position starts from 0
int FltFile::ReadSignal(vector<float>& signal, unsigned position) {
  signal.clear();
  if (position >= 0 && position < volume_[3]) {
    for (unsigned i = 0; i < num_samples_; ++i) {
      signal.push_back(*((data_[i]) + position));
    }
  }
  else {
    cerr << "position out of range\n";
    return -1;
  }
  return 1;
}

//readsignal;  //coordinate starts from [0,0,0]
int FltFile::ReadSignal(vector<float>& signal, unsigned* coordinate) {
  unsigned position = 0;
  for (unsigned i = 0; i < 3; ++i) {
    //assume the dimension of volume is always 3
    if (coordinate[i] >= 0 && coordinate[i] < size_[i])
      position += coordinate[i]*volume_[i];
    else {
      cerr << "coordinate out of range\n";
      return -1;
    }
  }
  return ReadSignal(signal, position);
}

// Destructor.
FltFile::~FltFile() {
  Reset();
}

// Free allocated memory.
void FltFile::Reset() {
  for (vector<float*>::iterator it = data_.begin();
      it != data_.end(); ++it) {
    delete[] *it;
  }
  data_.clear();
  size_.clear();
  volume_.clear();
}

// Write flt data to a file.
int FltFile::WriteFile(const char* filename,
    vector<float*>& data, vector < unsigned>& size,
    const char mode, bool b2DSlice) {
  ofstream ofs(filename, ios::out | ios::binary);
  if (ofs.is_open()) {
    Reset();
    num_samples_ = 1;
    //this->m_Data = data_;
    size_ = size;
    dimension_ = size_.size();
    //check integrity and other numeric values!!
    unsigned num = 1;
    volume_.push_back(num);

    for (unsigned i = 0; i < dimension_; ++i) {
      num *= size_[i];
      volume_.push_back(num);
    }

    cout << "m_nVolume: [";
    for (unsigned k=0; k<volume_.size(); ++k) {
      cout << volume_[k] << " ";
    }
    cout << "]"<<endl;

    num_elements_ = num;
    if (dimension_ == 4) {
      num_samples_ = size_[3]; //default is 1 for 3D.
    }
    if (dimension_ == 3 && b2DSlice ) {
      num_samples_ = size_[2]; // now assume 2D slice and 3rd dim is nSampTotal
    }

    cout << "num_elements_ "<< num_elements_<<endl;
    cout << "num_samples_ "<< num_samples_<<endl;

    write_mode_ = mode;
    opened_ = true;
    // ! debug only: PrintHeader();

    // Write the header.
    if (WriteHeader(ofs)) {
      // Continue to write data according to different data type.
      switch (data_type_) {
        case DTTYPE_FLOAT:
          WriteFloat(ofs, data);
          break;
          /* Implement later
             case DTTYPE_COMPLEX:
             WriteComplex(outputFile);
             break;
             */
        default:
          cerr<<" ERROR: Won't read data type " << data_type_ << endl;
          return -1;
          break;
      }
    } else {
      cerr<<" Errors occured when trying to write the header\n ";
      return -1;
    }
    return 1;
  } else {
    cerr << "Unable to open file " << filename << endl;
    return -1;
  }
}

//write header
int FltFile::WriteHeader(ofstream& ofs) {
  // Write the total number of dimensions from file
  // Only handle 4D data now
  if (size_.size() != 4 && size_.size() != 3) {
    cerr << " ERROR: Can't write non-4D or non-3D data set.\n";
    return -1;
  }
  unsigned temp = dimension_;
  if (write_mode_ != local_endian_)
    swap_bytes((unsigned char*)&temp, 4);
  ofs.write((char*)&temp, 4);

  for (unsigned i = 0; i < dimension_; ++i) {
    temp = size_[i];
    if (write_mode_ != local_endian_)
      swap_bytes((unsigned char*)&temp, 4);
    ofs.write((char*)&temp, 4);
  }
  temp = data_type_;
  if (write_mode_ != local_endian_) {
    swap_bytes((unsigned char*)&temp, 4);
  }
  ofs.write((char*)&temp, 4);
  temp = num_elements_;
  if (write_mode_ != local_endian_) {
    swap_bytes((unsigned char*)&temp, 4);
  }
  ofs.write((char*)&temp, 4);
  return 1;
}

//write float
void FltFile::WriteFloat(ofstream& ofs) {
  WriteFloat(ofs, data_);
}

void FltFile::WriteFloat(ofstream& ofs, vector<float*>& data) {
  for (unsigned i = 0; i < num_samples_; ++i) {
    float* buffer = data[i];
    for (unsigned j = 0; j < volume_[dimension_]/num_samples_; ++j) {
      float temp = buffer[j];
      if (write_mode_ != local_endian_)
        swap_bytes((unsigned char*)&temp, 4);
      ofs.write((char*)&temp, 4);
    }
  }
}

#if 0
//print header
void FltFile::PrintHeader() {
  if (opened_) {
    cout << "=====================================================\n";
    cout << " Total number of dimensions : " << dimension_ << endl;
    unsigned num = 1;
    for (unsigned i = 0; i < size_.size(); ++i) {
      cout << " Size in dim[" << i << "]: " <<  size_[i] << endl;
      num *= size_[i];
    }
    cout << " Calculated total number of data elements is " << num << endl;
    cout << " Data type is " << data_type_ << endl;
    cout << " Total number of data elements is " << num_elements_ << endl;
    cout << " # of samples is " << num_samples_ << endl;
    for (unsigned j = 0; j < volume_.size(); ++j) {
      cout << " Number in volume_[" << j << "]: " <<  volume_[j] << endl;
    }
    cout << "=====================================================\n";
  } else {
    cerr << "Empty Object!\n";
  }
}
#endif

#if 0
void FltFile::CheckData() {
  if (is_open()) {
    vector<float> signal;
    unsigned coord[3] = {1,0,0};
    ReadSignal(signal, coord);
    cout<<"The signal at second voxel [1,0,0]:" <<endl;
    for (unsigned int k=0;k<signal.size();++k) {
      cout<<signal[k]<<" ";
    }
    cout<<endl;
    unsigned position = volume()/2;
    ReadSignal(signal, position);
    cout<<"The signal at position ("<< position<<"):\n";
    for (unsigned int k=0;k<signal.size();++k) {
      cout<<signal[k]<<" ";
    }
    cout<<endl;
    position = volume()-1;
    ReadSignal(signal, position);
    cout<<"The signal at last voxel ("<< position<<"):\n";
    for (unsigned int k=0;k<signal.size();++k) {
      cout<<signal[k]<<" ";
    }
    cout<<endl;
  }
}
#endif
