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
	/* Pulse Information */
	double fRiseTime; // Rise time for exponential response (ns)
	double fDecayTime; // Decay time for exponential response (ns)
	double fSigma; // Width for Gaussian response (ns)
	double fThreshold; // Threshold for triggering (keV)
	int fPolarity; // Polarity of signal
	// Resolution and Noise //
	double fWhiteNoise; // Uncertainty in any measurement (keV)
	// Calibration
	/* There is come conversion factor between energy (which the Geant4 simulation gives)
	 * and the voltage we'll see in a detector. I don't even know where to begin
	 * estimating this, so I just chose one gave a reasonable dynamic range for the pulses
	 * I looked at.
	 */
	double fCalibration; // A number to multiply energy by.
	/* Digitization */
	double fFrequency; // Frequency of digitization (GHz)
	double fTickLength; // Tickmark length (ns)
	double fMaxEnergy; // Maximum energy before WFD hits ceiling/floow (keV)
	int fNBits; // Number of bits of digitization
	int fMaxADC; // Maximum digital count count
	int fPedestal; // DC Offset
	int fNSamples; // Total number of samples in trigger window
	int fNPreSamples; // Number of samples before trigger

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
		fMaxADC = (int) std::pow(2., n) - 1;
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

	// Comprehensive Setters
	/* Properties of the pulse
	 * rt:			Rise time for exponential response
	 * dt:			Decay time for exponential response
	 * s:			Sigma for Gaussian response
	 * th:			Threshold for triggering
	 * po:			Polarity (+/- 1)
	 */
	void SetPulseProperties(double rt, double dt, double s, double th, int po);
	/* Properties of the digitizer
	 * fr:			Frequency of digitization
	 * maxen:		Energy ceiling in keV
	 * nbits:		Number of bits for digitization
	 * ped:			Pedestal in ADC units
	 * nsamps:		Total number of samples in trigger window
	 * npresamps:	Number of samples in trigger window before going over threshold
	 */
	void SetDigitizationProperties(double fr, double maxen, int nbits, int ped,
			int nsamps, int npresamps);

private:
	void Noise(std::vector<int>& s);

public:
	bool IsOverThreshold(double e);
	/* Return TPulseIsland pointer
	 * Gaussian response use fSigma and just return a Gaussian (Slow)
	 * Exponential response returns a sum of exponentials (Fast)
	 * e:			Energy deposited in hit
	 * t:			Time of first hit
	 * n:			Bank name
	 */
	TPulseIsland* GetGaussianResponse(double e, double t, const char* n);
	TPulseIsland* GetExponentialResponse(double e, double t, const char* n);
};

#endif
