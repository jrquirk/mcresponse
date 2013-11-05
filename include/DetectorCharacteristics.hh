#ifndef PARAMETERGETTER_HH
#define PARAMETERGETTER_HH

#include <fstream>
#include <map>

class DetectorCharacteristics {

public:
  DetectorCharacteristics();
  ~DetectorCharacteristics();

private:
  void LoadFile(std::string);


private:
  bool fOK;
  std::string fFilename;
  char fCommentChar;
  std::ifstream fFile;

  // The info
  std::vector<std::string> fBankName;
  std::vector<bool> fFastPulse;
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

public:
  bool IsPulseFast(std::string);
  double GetRiseTime(std::string);
  double GetDecayTime(std::string);
  double GetPulseWidth(std::string);
  double GetEnergyThreshold(std::string);
  bool IsPulsePositive(std::string);
  double GetPulseFrequency(std::string);
  double GetMaxEnergy(std::string);
  int GetNBits(std::string);
  int GetPedestal(std::string);
  int GetNSamples(std::string);
  int GetNPreSamples(std::string);
};


#endif /* Include Guard */
