#include "DetectorResponse.hh"
#include "TRandom3.h"
#include "TPulseIsland.hh"

#include <cmath>
#include <vector>
#include <string>
#include <iostream>

DetectorResponse::DetectorResponse() :
		fRandom(TRandom3()), fRiseTime(5.), fDecayTime(10.), fSigma(1000.), fThreshold(
				100.), fPolarity(1), fWhiteNoise(0.), fCalibration(1.), fFrequency(
				0.17), fTickLength(1 / 0.17), fMaxEnergy(10000.), fNBits(12), fMaxADC(
				4095), fPedestal(0), fNSamples(100), fNPreSamples(20) {
}

DetectorResponse::~DetectorResponse() {
}

void DetectorResponse::SetPulseProperties(double rt, double dt, double s,
		double th, int po, PulseType pt) {
	fRiseTime = rt;
	fDecayTime = dt;
	fSigma = s;
	fThreshold = th;
	if (po < 0)
		fPolarity = -1;
	else
		fPolarity = 1;
	fType = pt;
}

void DetectorResponse::SetDigitizationProperties(double f, double e, int nb,
		int p, int ns, int nps) {
	fFrequency = f;
	fTickLength = 1. / f;
	fMaxEnergy = e;
	fNBits = nb;
	fMaxADC = (int) std::pow(2., nb) - 1;
	fPedestal = p;
	fNSamples = ns;
	fNPreSamples = nps;
}

void DetectorResponse::Noise(std::vector<int>& s) {
	for (unsigned int i = 0; i < s.size(); i++)
		s[i] += (int) fRandom.Gaus(0., fWhiteNoise);
}

TPulseIsland* DetectorResponse::GetResponse(double e, double t, std::string& name) {
	// Get energy vector
	// Return NULL pointer if never goes over threshold
	std::vector<double> energies;
	int sampleOver;
	if (fType == FAST)
		sampleOver = GetExponentialResponse(e, energies);
	else
		sampleOver = GetGaussianResponse(e, energies);
	if (sampleOver == -1)
		return NULL;

	// Turn into ADC samples
	double conv = (double) fMaxADC / fMaxEnergy;
	int iSample, adc;
	/* If the pulse starts and goes over threshold in fewer than the
	 * amount of pretrigger samples we take, we fill the space before
	 * the pulse starts with the pedestal. If the pulse starts well
	 * before fNPreSamples before the trigger threshold,
	 * we lose some of the pulse. 
	 */
	std::vector<int> samples;
	int nPreSamples = fNPreSamples - sampleOver;
	if (nPreSamples > 0) {
		samples = std::vector<int>(nPreSamples, fPedestal);
		iSample = nPreSamples;
	} else {
		iSample = 0;
	}

	for (; iSample < fNSamples; iSample++) {
		adc = (int) (conv * energies[iSample - nPreSamples]);
		adc = fPedestal + fPolarity * adc;
		if (adc > fMaxADC)
			adc = fMaxADC;
		else if (adc < 0)
			adc = 0;
		samples.push_back(adc);
	}
	Noise(samples);
	return new TPulseIsland((int) (t / fTickLength), samples,
			fTickLength, name);
}

int DetectorResponse::GetGaussianResponse(double e,
		std::vector<double>& energies) {
	energies.clear();
	int sampleOver = -1;
	double energy;
	double t = 0.; 
	double sigsq2 = 2 * std::pow(fSigma, 2);
	for (int iSample = 0; iSample < fNSamples; iSample++) {
		energy = std::exp(-std::pow(t - 3. * fSigma, 2) / sigsq2);
		energy *= fCalibration * e;
		energies.push_back(energy);
		t += fTickLength;
		if (sampleOver < 0 && energy > fThreshold)
			sampleOver = iSample;
	}
	return sampleOver;
}

int DetectorResponse::GetExponentialResponse(double e,
		std::vector<double>& energies) {
	energies.clear();
	int sampleOver = -1;
	double energy;
	double t = 0.;
	for (int iSample = 0; iSample < fNSamples; iSample++) {
		energy = std::exp(-t/fDecayTime) - std::exp(-t/fRiseTime);
		energy *= fCalibration * e;
		energies.push_back(energy);
		t += fTickLength;
		if (sampleOver < 0 && energy > fThreshold)
			sampleOver = iSample;
	}
	return sampleOver;
}
