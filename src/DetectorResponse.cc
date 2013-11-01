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

bool DetectorResponse::IsOverThreshold(double e) {
	return e > fThreshold;
}

void DetectorResponse::SetPulseProperties(double rt, double dt, double s,
		double th, int po) {
	fRiseTime = rt;
	fDecayTime = dt;
	fSigma = s;
	fThreshold = th;
	fPolarity = po;
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

TPulseIsland* DetectorResponse::GetGaussianResponse(double e, double t,
		const char* n) {
	std::string name(n);
	std::vector<int> samples;
	std::vector<double> energies;
	int iSample;
	int adc;
	int sampleOver = -1;
	double loct = 0.;
	for (iSample = 0; iSample < fNSamples; iSample++) {
		energies.push_back(
				fCalibration * e
						* std::exp(
								-std::pow(loct - 3. * fSigma, 2)
										/ (2 * std::pow(fSigma, 2))));
		loct += fTickLength;
		if (sampleOver < 0 && IsOverThreshold(energies.at(iSample)))
			sampleOver = iSample;
	}
	int nPreSamples = fNPreSamples - sampleOver;
	for (iSample = 0; iSample < nPreSamples; iSample++)
		samples.push_back(fPedestal);
	for (; iSample < fNSamples; iSample++) {
		adc = fPedestal
				+ fPolarity
						* std::floor(
								((double) fMaxADC) / fMaxEnergy
										* energies[iSample - nPreSamples]);
		if (adc > fMaxADC)
			adc = fMaxADC;
		else if (adc < 0)
			adc = 0;
		samples.push_back(adc);
	}
	Noise(samples);
	return new TPulseIsland(t / fTickLength - nPreSamples, samples, fTickLength,
			name);
}

TPulseIsland* DetectorResponse::GetExponentialResponse(double e, double t,
		const char* n) {
	std::string name(n);
	std::vector<int> samples;
	std::vector<double> energies;
	int iSample;
	int adc;
	int sampleOver = -1;
	double loct = 0.;
	for (iSample = 0; iSample < fNSamples; iSample++) {
		energies.push_back(
				fCalibration * e
						* (std::exp(-loct / fDecayTime)
								- std::exp(-loct / fRiseTime)));
		loct += fTickLength;
		if (sampleOver < 0 && IsOverThreshold(energies.at(iSample)))
			sampleOver = iSample;
	}
	int nPreSamples = fNPreSamples - sampleOver;
	for (iSample = 0; iSample < nPreSamples; iSample++)
		samples.push_back(fPedestal);
	for (; iSample < fNSamples; iSample++) {
		adc = fPedestal
				+ fPolarity
						* std::floor(
								((double) fMaxADC) / fMaxEnergy
										* energies[iSample - nPreSamples]);
		if (adc > fMaxADC)
			adc = fMaxADC;
		else if (adc < 0)
			adc = 0;
		samples.push_back(adc);
	}
	Noise(samples);
	return new TPulseIsland(t / fTickLength - nPreSamples, samples, fTickLength,
			name);
}
