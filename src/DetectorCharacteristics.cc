#include "DetectorCharacteristics.hh"
#include "DetectorResponse.hh"

#include <fstream>
#include <vector>
#include <iostream>

DetectorCharacteristics::DetectorCharacteristics() {
	fBankName.clear();
	fPulseType.clear();
	fRiseTime.clear();
	fDecayTime.clear();
	fPulseWidth.clear();
	fEnergyThreshold.clear();
	fPolarity.clear();
	fFrequency.clear();
	fMaxEnergy.clear();
	fNBits.clear();
	fPedestal.clear();
	fNSamples.clear();
	fNPreSamples.clear();
	// Beam Counter
	fBankName.push_back("MC00");
	fPulseType.push_back(DetectorResponse::FAST);
	fRiseTime.push_back(5.);
	fDecayTime.push_back(10.);
	fPulseWidth.push_back(0.);
	fEnergyThreshold.push_back(100.);
	fPolarity.push_back(false);
	fFrequency.push_back(0.170);
	fMaxEnergy.push_back(10000.);
	fNBits.push_back(12);
	fPedestal.push_back(4095);
	fNSamples.push_back(100);
	fNPreSamples.push_back(20);
	fCalibration.push_back(10.);
	// Slow Target
	fBankName.push_back("MCF1");
	fPulseType.push_back(DetectorResponse::FAST);
	fRiseTime.push_back(25.);
	fDecayTime.push_back(50.);
	fPulseWidth.push_back(0.);
	fEnergyThreshold.push_back(100.);
	fPolarity.push_back(true);
	fFrequency.push_back(0.170);
	fMaxEnergy.push_back(10000.);
	fNBits.push_back(12);
	fPedestal.push_back(0);
	fNSamples.push_back(100);
	fNPreSamples.push_back(20);
	fCalibration.push_back(1.);
	// Fast Target
	fBankName.push_back("MCS1");
	fPulseType.push_back(DetectorResponse::SLOW);
	fRiseTime.push_back(0.);
	fDecayTime.push_back(0.);
	fPulseWidth.push_back(1000.);
	fEnergyThreshold.push_back(100.);
	fPolarity.push_back(true);
	fFrequency.push_back(0.017);
	fMaxEnergy.push_back(10000.);
	fNBits.push_back(12);
	fPedestal.push_back(0);
	fNSamples.push_back(100);
	fNPreSamples.push_back(20);
	fCalibration.push_back(1.);
	// Beam Veto
	fBankName.push_back("MC02");
	fPulseType.push_back(DetectorResponse::FAST);
	fRiseTime.push_back(5.);
	fDecayTime.push_back(10.);
	fPulseWidth.push_back(0.);
	fEnergyThreshold.push_back(100.);
	fPolarity.push_back(false);
	fFrequency.push_back(0.170);
	fMaxEnergy.push_back(10000.);
	fNBits.push_back(12);
	fPedestal.push_back(4095);
	fNSamples.push_back(100);
	fNPreSamples.push_back(20);
	fCalibration.push_back(10.);
	// Fast Beam Right Thin
	fBankName.push_back("MCF3");
	fPulseType.push_back(DetectorResponse::FAST);
	fRiseTime.push_back(25.);
	fDecayTime.push_back(50.);
	fPulseWidth.push_back(0.);
	fEnergyThreshold.push_back(100.);
	fPolarity.push_back(true);
	fFrequency.push_back(0.170);
	fMaxEnergy.push_back(10000.);
	fNBits.push_back(12);
	fPedestal.push_back(0);
	fNSamples.push_back(100);
	fNPreSamples.push_back(20);
	fCalibration.push_back(10.);
	// Slow Beam Right Thin
	fBankName.push_back("MCS3");
	fPulseType.push_back(DetectorResponse::SLOW);
	fRiseTime.push_back(0.);
	fDecayTime.push_back(0.);
	fPulseWidth.push_back(1000.);
	fEnergyThreshold.push_back(100.);
	fPolarity.push_back(true);
	fFrequency.push_back(0.017);
	fMaxEnergy.push_back(10000.);
	fNBits.push_back(12);
	fPedestal.push_back(0);
	fNSamples.push_back(100);
	fNPreSamples.push_back(20);
	fCalibration.push_back(10.);
	// Fast Beam Right Thick
	fBankName.push_back("MCF4");
	fPulseType.push_back(DetectorResponse::FAST);
	fRiseTime.push_back(25.);
	fDecayTime.push_back(50.);
	fPulseWidth.push_back(0.);
	fEnergyThreshold.push_back(100.);
	fPolarity.push_back(true);
	fFrequency.push_back(0.170);
	fMaxEnergy.push_back(10000.);
	fNBits.push_back(12);
	fPedestal.push_back(0);
	fNSamples.push_back(100);
	fNPreSamples.push_back(20);
	fCalibration.push_back(2.);
	// Slow Beam Right Thick
	fBankName.push_back("MCS4");
	fPulseType.push_back(DetectorResponse::SLOW);
	fRiseTime.push_back(0.);
	fDecayTime.push_back(0.);
	fPulseWidth.push_back(1000.);
	fEnergyThreshold.push_back(100.);
	fPolarity.push_back(true);
	fFrequency.push_back(0.017);
	fMaxEnergy.push_back(10000.);
	fNBits.push_back(12);
	fPedestal.push_back(0);
	fNSamples.push_back(100);
	fNPreSamples.push_back(20);
	fCalibration.push_back(2.);
	// Beam Right Veto
	fBankName.push_back("MC05");
	fPulseType.push_back(DetectorResponse::FAST);
	fRiseTime.push_back(5.);
	fDecayTime.push_back(10.);
	fPulseWidth.push_back(0.);
	fEnergyThreshold.push_back(100.);
	fPolarity.push_back(false);
	fFrequency.push_back(0.170);
	fMaxEnergy.push_back(10000.);
	fNBits.push_back(12);
	fPedestal.push_back(0);
	fNSamples.push_back(100);
	fNPreSamples.push_back(20);
	fCalibration.push_back(10.);
	// Fast Beam Left Thin
	fBankName.push_back("MCF6");
	fPulseType.push_back(DetectorResponse::FAST);
	fRiseTime.push_back(25.);
	fDecayTime.push_back(50.);
	fPulseWidth.push_back(0.);
	fEnergyThreshold.push_back(100.);
	fPolarity.push_back(true);
	fFrequency.push_back(0.170);
	fMaxEnergy.push_back(10000.);
	fNBits.push_back(12);
	fPedestal.push_back(0);
	fNSamples.push_back(100);
	fNPreSamples.push_back(20);
	fCalibration.push_back(10.);
	// Slow Beam Left Thin
	fBankName.push_back("MCS6");
	fPulseType.push_back(DetectorResponse::SLOW);
	fRiseTime.push_back(0.);
	fDecayTime.push_back(0.);
	fPulseWidth.push_back(1000.);
	fEnergyThreshold.push_back(100.);
	fPolarity.push_back(true);
	fFrequency.push_back(0.017);
	fMaxEnergy.push_back(10000.);
	fNBits.push_back(12);
	fPedestal.push_back(0);
	fNSamples.push_back(100);
	fNPreSamples.push_back(20);
	fCalibration.push_back(10.);
	// Fast Beam Left Thick
	fBankName.push_back("MCF7");
	fPulseType.push_back(DetectorResponse::FAST);
	fRiseTime.push_back(25.);
	fDecayTime.push_back(50.);
	fPulseWidth.push_back(0.);
	fEnergyThreshold.push_back(100.);
	fPolarity.push_back(true);
	fFrequency.push_back(0.170);
	fMaxEnergy.push_back(10000.);
	fNBits.push_back(12);
	fPedestal.push_back(0);
	fNSamples.push_back(100);
	fNPreSamples.push_back(20);
	fCalibration.push_back(2.);
	// Slow Beam Left Thick
	fBankName.push_back("MCS7");
	fPulseType.push_back(DetectorResponse::SLOW);
	fRiseTime.push_back(0.);
	fDecayTime.push_back(0.);
	fPulseWidth.push_back(1000.);
	fEnergyThreshold.push_back(100.);
	fPolarity.push_back(true);
	fFrequency.push_back(0.017);
	fMaxEnergy.push_back(10000.);
	fNBits.push_back(12);
	fPedestal.push_back(0);
	fNSamples.push_back(100);
	fNPreSamples.push_back(20);
	fCalibration.push_back(2.);
	// Beam Left Veto
	fBankName.push_back("MC08");
	fPulseType.push_back(DetectorResponse::FAST);
	fRiseTime.push_back(5.);
	fDecayTime.push_back(10.);
	fPulseWidth.push_back(0.);
	fEnergyThreshold.push_back(100.);
	fPolarity.push_back(false);
	fFrequency.push_back(0.170);
	fMaxEnergy.push_back(10000.);
	fNBits.push_back(12);
	fPedestal.push_back(0);
	fNSamples.push_back(100);
	fNPreSamples.push_back(20);
	fCalibration.push_back(10.);
	// MuSCA
	fBankName.push_back("MC11");
	fPulseType.push_back(DetectorResponse::FAST);
	fRiseTime.push_back(5.);
	fDecayTime.push_back(10.);
	fPulseWidth.push_back(0.);
	fEnergyThreshold.push_back(100.);
	fPolarity.push_back(false);
	fFrequency.push_back(0.170);
	fMaxEnergy.push_back(10000.);
	fNBits.push_back(12);
	fPedestal.push_back(4095);
	fNSamples.push_back(100);
	fNPreSamples.push_back(20);
	fCalibration.push_back(10.);
}

DetectorCharacteristics::~DetectorCharacteristics() {
}

void DetectorCharacteristics::Print() {
	std::cout << "Bank" << "\t" << "Type" << "\t" << "Rise" << "\t" << "Fall"
			<< "\t" << "Width" << "\t" << "Threshold" << "\t" << "Polarity"
			<< "\t" << "Frequency" << "\t" << "E Ceil" << "\t" << "NBits"
			<< "\t" << "Pedestal" << "\t" << "NSamps" << "\t" << "NPreSamps"
			<< "\t" << "Calibration" << std::endl;
	for (int iDet = 0; iDet < fNDetectors; iDet++)
		std::cout << fBankName[iDet] << "\t" << fPulseType[iDet] << "\t"
				<< fRiseTime[iDet] << "\t" << fDecayTime[iDet] << "\t"
				<< fPulseWidth[iDet] << "\t" << fEnergyThreshold[iDet] << "\t\t"
				<< fPolarity[iDet] << "\t\t" << fFrequency[iDet] << "\t\t"
				<< fMaxEnergy[iDet] << "\t" << fNBits[iDet] << "\t"
				<< fPedestal[iDet] << "\t\t" << fNSamples[iDet] << "\t"
				<< fNPreSamples[iDet] << "\t\t" << fCalibration[iDet]
				<< std::endl;
}

void DetectorCharacteristics::LoadFile(std::string fname) {
	char trash[256];

	fFilename = fname;
	fFile.open(fFilename.c_str());
	if (!fFile.is_open())
		return;

	std::string bankname;
	bool fastpulse, polarity;
	int nbits, pedestal, nsamps, npresamps;
	double risetime, decaytime, pulsewidth, threshold, frequency, maxenergy,
			calibration;

	while (true) {
		fFile >> bankname;
		if (fFile.eof())
			break;

		// Ignore comments
		if (bankname.at(0) == fCommentChar) {
			fFile.getline(trash, 256);
			if (!fFile.good()) {
				std::cout << "Error reading detector description file!"
						<< std::endl;
				std::cout << "Comment line too long or at end of file?"
						<< std::endl;
				std::cout << "Last read: \"" << trash << "\"" << std::endl;
				return;
			}
			continue;
		}

		fFile >> fastpulse >> risetime >> decaytime >> pulsewidth >> threshold
				>> polarity >> frequency >> maxenergy >> nbits >> pedestal
				>> nsamps >> npresamps >> calibration;
		for (int iDetector = 0; iDetector <= fNDetectors; iDetector++) {
			if (iDetector == fNDetectors) {
				std::cout << "Unknown detector! (" << bankname << ")"
						<< std::endl;
				break;
			}
			if (fBankName[iDetector] == bankname) {
				fPulseType[iDetector] =
						(fastpulse ?
								DetectorResponse::FAST : DetectorResponse::SLOW);
				fRiseTime[iDetector] = risetime;
				fDecayTime[iDetector] = decaytime;
				fPulseWidth[iDetector] = pulsewidth;
				fEnergyThreshold[iDetector] = threshold;
				fPolarity[iDetector] = polarity;
				fFrequency[iDetector] = frequency;
				fMaxEnergy[iDetector] = maxenergy;
				fNBits[iDetector] = nbits;
				fPedestal[iDetector] = pedestal;
				fNSamples[iDetector] = nsamps;
				fNPreSamples[iDetector] = npresamps;
				fCalibration[iDetector] = calibration;
				break;
			}
		}
	}

	fFile.close();
	return;
}

DetectorResponse DetectorCharacteristics::GetDetectorResponse(
		std::string bankname) {
	DetectorResponse dr;
	for (int iDet = 0; iDet < fNDetectors; iDet++) {
		if (fBankName[iDet] == bankname) {
			if (fPolarity[iDet])
				dr.SetPulseProperties(fRiseTime[iDet], fDecayTime[iDet],
						fPulseWidth[iDet], fEnergyThreshold[iDet], 1,
						fPulseType[iDet]);
			else
				dr.SetPulseProperties(fRiseTime[iDet], fDecayTime[iDet],
						fPulseWidth[iDet], fEnergyThreshold[iDet], -1,
						fPulseType[iDet]);
			dr.SetDigitizationProperties(fFrequency[iDet], fMaxEnergy[iDet],
					fNBits[iDet], fPedestal[iDet], fNSamples[iDet],
					fNPreSamples[iDet]);
			dr.SetCalibration(fCalibration[iDet]);
			return dr;
		}
	}
	std::cout << "Unknown detector! (" << bankname << ")" << std::endl;
	return DetectorResponse();
}

int DetectorCharacteristics::fNDetectors = 15;
char DetectorCharacteristics::fCommentChar = '#';
