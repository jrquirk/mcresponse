#ifndef PARAMETERGETTER_HH
#define PARAMETERGETTER_HH

#include "DetectorResponse.hh"

#include <fstream>
#include <vector>

class DetectorCharacteristics {

public:
  DetectorCharacteristics();
  ~DetectorCharacteristics();

private:
  static char fCommentChar;
  bool fOK;
  std::string fFilename;
  std::ifstream fFile;

  static int fNDetectors;

  // The info
  std::vector<std::string> fBankName;
  std::vector<DetectorResponse::PulseType> fPulseType;
  std::vector<double> fRiseTime;
  std::vector<double> fDecayTime;
  std::vector<double> fPulseWidth;
  std::vector<double> fEnergyThreshold;
  std::vector<bool> fPolarity;
  std::vector<double> fFrequency;
  std::vector<double> fMaxEnergy;
  std::vector<int> fNBits;
  std::vector<int> fPedestal;
  std::vector<int> fNSamples;
  std::vector<int> fNPreSamples;
  std::vector<double> fCalibration;

public:
  void Print();
  void LoadFile(std::string);
  DetectorResponse GetDetectorResponse(std::string);
};


#endif /* Include Guard */
