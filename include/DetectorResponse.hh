#ifndef DETECTORRESPONSE_HH
#define DETECTORRESPONSE_HH

#include "TRandom3.h"

#include <cmath>

class TPulseIsland;


class DetectorResponse {

public:
	DetectorResponse();
	~DetectorResponse();

private:
	TRandom3 fRandom;
	// Pulse Information
	double fRiseTime; // ns
	double fDecayTime; // ns
	double fSigma; // ns
	double fThreshold; // keV
	int fPolarity;
	// Resolution and Noise
	double fWhiteNoise; // keV
	double fCalibration;
	// Digitization
	double fFrequency; // GHz
	double fTickLength; // ns
	double fMaxEnergy; // keV
	int fNBits;
	int fMaxADC;
	int fPedestal;
	int fNSamples;
	int fNPreSamples;

public:
	// Pulse *etters
	void SetRiseTime(double rt) {
		fRiseTime = rt;
	}
	double GetRiseTime() {
		return fRiseTime;
	}
	void SetDecayTime(double dt) {
		fDecayTime = dt;
	}
	double GetDecayTime() {
		return fDecayTime;
	}
	void SetSigma(double s) {
		fSigma = s;
	}
	double GetSigma() {
		return fSigma;
	}
	void SetThreshold(double th) {
		fThreshold = th;
	}
	double GetThreshold() {
		return fThreshold;
	}
	void SetPolarity(int po) {
		if (po < 0)
			fPolarity = -1;
		else
			fPolarity = 1;
	}
	int GetPolarity() {
		return fPolarity;
	}
	// Resolution and Noise *etters
	void SetWhiteNoise(double n) {
		fWhiteNoise = n;
	}
	double GetWhiteNoise() {
		return fWhiteNoise;
	}
	void SetCalibration(double ca) {
		fCalibration = ca;
	}
	double GetCalibration() {
		return fCalibration;
	}
	// Digitization *etters
	void SetFrequency(double fr) {
		fFrequency = fr;
		fTickLength = 1. / fr;
	}
	double GetFrequency() {
		return fFrequency;
	}
	double GetTickLength() {
		return fTickLength;
	}
	void SetMaxEnergy(double en) {
		fMaxEnergy = en;
	}
	double GetMaxEnergy() {
		return fMaxEnergy;
	}
	void SetNBits(int n) {
		fNBits = n;
		fMaxADC = (int) std::pow(2, n) - 1;
	}
	int GetNBits() {
		return fNBits;
	}
	int GetMaxADC() {
		return fMaxADC;
	}
	void SetPedestal(int pe) {
		fPedestal = pe;
	}
	int GetPedestal() {
		return fPedestal;
	}
	void SetNSamples(int n) {
		fNSamples = n;
	}
	int GetNSamples() {
		return fNSamples;
	}
	void SetNPreSamples(int n) {
		fNPreSamples = n;
	}
	int GetNPreSamples() {
		return fNPreSamples;
	}

	//Global Setters
	void SetPulseProperties(double rt, double dt, double s, double th, int po);
	void SetDigitizationProperties(double fr, double maxen, int nbits, int ped,
			int nsamps, int npresamps);

private:
	void Noise(std::vector<int>& s);

public:
	bool IsOverThreshold(double e);
	TPulseIsland* GetGaussianResponse(double e, double t, const char* n);
	TPulseIsland* GetExponentialResponse(double e, double t, const char* n);
};

#endif
