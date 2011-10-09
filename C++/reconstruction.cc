#include <cstring>
#include <iostream>
#include <memory>

using namespace std;

#include "mow_reconstructor.h"
#include "dot_reconstructor.h"
#include "qbi_reconstructor.h"

typedef auto_ptr<MoWReconstructor> MoWReconstructorPtr;
typedef auto_ptr<DOTReconstructor> DOTReconstructorPtr;
typedef auto_ptr<QBIReconstructor> QBIReconstructorPtr;

const int MAX_PATH_LENGTH = 512;
bool get_config_fullpath(const char* input_config, char* f_config) {
#ifdef WIN32
  char* lpPart[MAX_PATH_LENGTH] = {NULL};
  int retval = GetFullPathName(input_config,
      MAX_PATH_LENGTH, f_config,
      lpPart);
  if (retval == 0) {
    // If GetFullPathName function fails.
    printf("GetFullPathName failed (%d)\n", GetLastError());
    return false;
  } else {
    printf("The full path name is: %s\n", f_config);
  }
#else
  strcpy(f_config, input_config);
#endif
  return true;
}

int main(int argc, char* argv[]) {
  if (argc<3) {
    cerr << "Usage: " << argv[0] << " config_file method." << endl;
    return -1;
  }
  char f_config[MAX_PATH_LENGTH];
  if (!get_config_fullpath(argv[1], f_config)) {
    return -1;
  }

  if (!strcmp(argv[2], "mow")) {
    MoWReconstructorPtr reconstructor(new MoWReconstructor(f_config));
    reconstructor->DoReconstruction();
  } else if (!strcmp(argv[2], "dot")) {
    DOTReconstructorPtr reconstructor(new DOTReconstructor(f_config));
    reconstructor->DoReconstruction();
  } else if (!strcmp(argv[2], "qbi")) {
    QBIReconstructorPtr reconstructor(new QBIReconstructor(f_config));
    reconstructor->DoReconstruction();
  } else {
    cerr << "Currently only three methods are supported. Try 'mow','dot' or 'qbi'. " << endl;
    return -1;
  }
  return 0;
}
