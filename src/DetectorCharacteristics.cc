#include <fstream>
#include <map>
#include <iostream>
#include "DetectorCharacteristics.hh"

DetectorCharacteristics::DetectorCharacteristics() {
	fBankName.clear();
	fFastPulse.clear();
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
	// Target
	fBankName.push_back("MC00");
	fFastPulse.push_back(true);
	fRiseTime.push_back();
	fDecayTime.push_back();
	fPulseWidth.push_back();
	fEnergyThreshold.push_back();
	fPolarity.push_back();
	fFrequency.push_back();
	fMaxEnergy.push_back();
	fNBits.push_back();
	fPedestal.push_back();
	fNSamples.push_back();
	fNPreSamples.push_back();
	// Slow Target
	fBankName.push_back("MCF1");
	fFastPulse.push_back(true);
	fRiseTime.push_back();
	fDecayTime.push_back();
	fPulseWidth.push_back();
	fEnergyThreshold.push_back();
	fPolarity.push_back();
	fFrequency.push_back();
	fMaxEnergy.push_back();
	fNBits.push_back();
	fPedestal.push_back();
	fNSamples.push_back();
	fNPreSamples.push_back();
	// Fast Target
	fBankName.push_back("MCS1");
	fFastPulse.push_back(false);
	fRiseTime.push_back();
	fDecayTime.push_back();
	fPulseWidth.push_back();
	fEnergyThreshold.push_back();
	fPolarity.push_back();
	fFrequency.push_back();
	fMaxEnergy.push_back();
	fNBits.push_back();
	fPedestal.push_back();
	fNSamples.push_back();
	fNPreSamples.push_back();
	// Beam Veto
	fBankName.push_back("MC02");
	fFastPulse.push_back(true);
	fRiseTime.push_back();
	fDecayTime.push_back();
	fPulseWidth.push_back();
	fEnergyThreshold.push_back();
	fPolarity.push_back();
	fFrequency.push_back();
	fMaxEnergy.push_back();
	fNBits.push_back();
	fPedestal.push_back();
	fNSamples.push_back();
	fNPreSamples.push_back();
	// Slow Beam Right Thin
	fBankName.push_back("MCF3");
	fFastPulse.push_back(true);
	fRiseTime.push_back();
	fDecayTime.push_back();
	fPulseWidth.push_back();
	fEnergyThreshold.push_back();
	fPolarity.push_back();
	fFrequency.push_back();
	fMaxEnergy.push_back();
	fNBits.push_back();
	fPedestal.push_back();
	fNSamples.push_back();
	fNPreSamples.push_back();
	// Fast Beam Right Thin
	fBankName.push_back("MCS3");
	fFastPulse.push_back(false);
	fRiseTime.push_back();
	fDecayTime.push_back();
	fPulseWidth.push_back();
	fEnergyThreshold.push_back();
	fPolarity.push_back();
	fFrequency.push_back();
	fMaxEnergy.push_back();
	fNBits.push_back();
	fPedestal.push_back();
	fNSamples.push_back();
	fNPreSamples.push_back();
	// Slow Beam Right Thick
	fBankName.push_back("MCF4");
	fFastPulse.push_back(true);
	fRiseTime.push_back();
	fDecayTime.push_back();
	fPulseWidth.push_back();
	fEnergyThreshold.push_back();
	fPolarity.push_back();
	fFrequency.push_back();
	fMaxEnergy.push_back();
	fNBits.push_back();
	fPedestal.push_back();
	fNSamples.push_back();
	fNPreSamples.push_back();
	// Fast Beam Right Thick
	fBankName.push_back("MCS4");
	fFastPulse.push_back(false);
	fRiseTime.push_back();
	fDecayTime.push_back();
	fPulseWidth.push_back();
	fEnergyThreshold.push_back();
	fPolarity.push_back();
	fFrequency.push_back();
	fMaxEnergy.push_back();
	fNBits.push_back();
	fPedestal.push_back();
	fNSamples.push_back();
	fNPreSamples.push_back();
	// Beam Right Veto
	fBankName.push_back("MC05");
	fFastPulse.push_back(true);
	fRiseTime.push_back();
	fDecayTime.push_back();
	fPulseWidth.push_back();
	fEnergyThreshold.push_back();
	fPolarity.push_back();
	fFrequency.push_back();
	fMaxEnergy.push_back();
	fNBits.push_back();
	fPedestal.push_back();
	fNSamples.push_back();
	fNPreSamples.push_back();
	// Slow Beam Left Thin
	fBankName.push_back("MCF6");
	fFastPulse.push_back(true);
	fRiseTime.push_back();
	fDecayTime.push_back();
	fPulseWidth.push_back();
	fEnergyThreshold.push_back();
	fPolarity.push_back();
	fFrequency.push_back();
	fMaxEnergy.push_back();
	fNBits.push_back();
	fPedestal.push_back();
	fNSamples.push_back();
	fNPreSamples.push_back();
	// Fast Beam Left Thin
	fBankName.push_back("MCS6");
	fFastPulse.push_back(false);
	fRiseTime.push_back();
	fDecayTime.push_back();
	fPulseWidth.push_back();
	fEnergyThreshold.push_back();
	fPolarity.push_back();
	fFrequency.push_back();
	fMaxEnergy.push_back();
	fNBits.push_back();
	fPedestal.push_back();
	fNSamples.push_back();
	fNPreSamples.push_back();
	// Slow Beam Left Thick
	fBankName.push_back("MCF7");
	fFastPulse.push_back(true);
	fRiseTime.push_back();
	fDecayTime.push_back();
	fPulseWidth.push_back();
	fEnergyThreshold.push_back();
	fPolarity.push_back();
	fFrequency.push_back();
	fMaxEnergy.push_back();
	fNBits.push_back();
	fPedestal.push_back();
	fNSamples.push_back();
	fNPreSamples.push_back();
	// Fast Beam Left Thick
	fBankName.push_back("MCS7");
	fFastPulse.push_back(false);
	fRiseTime.push_back();
	fDecayTime.push_back();
	fPulseWidth.push_back();
	fEnergyThreshold.push_back();
	fPolarity.push_back();
	fFrequency.push_back();
	fMaxEnergy.push_back();
	fNBits.push_back();
	fPedestal.push_back();
	fNSamples.push_back();
	fNPreSamples.push_back();
	// Beam Left Veto
	fBankName.push_back("MC08");
	fFastPulse.push_back(true);
	fRiseTime.push_back();
	fDecayTime.push_back();
	fPulseWidth.push_back();
	fEnergyThreshold.push_back();
	fPolarity.push_back();
	fFrequency.push_back();
	fMaxEnergy.push_back();
	fNBits.push_back();
	fPedestal.push_back();
	fNSamples.push_back();
	fNPreSamples.push_back();
	// MuSCA
	fBankName.push_back("MC11");
	fFastPulse.push_back(true);
	fRiseTime.push_back();
	fDecayTime.push_back();
	fPulseWidth.push_back();
	fEnergyThreshold.push_back();
	fPolarity.push_back();
	fFrequency.push_back();
	fMaxEnergy.push_back();
	fNBits.push_back();
	fPedestal.push_back();
	fNSamples.push_back();
	fNPreSamples.push_back();

}

void ParameterGetter::LoadFile(std::string fname) {
	std::string param, units;
	double val;
	int opt;
	bool readopts = false;
	char trash[256];

	while (true) {
		fFile >> param;

		// Return if end of file
		if (fFile.eof())
			break;

		// Ignore comments
		if (param.at(0) == fCommentChar) {
			fFile.getline(trash, 256);
			if (!fFile.good()) {
				std::cout << "Error reading input parameter file!" << std::endl;
				std::cout << "Comment line too long or at end of file?"
						<< std::endl;
				return;
			}
			continue;
		}

		// Read Options or Parameters
		if (!param.compare(fParameterIndicator)) {
			readopts = false;
			continue;
		} else if (!param.compare(fOptionIndicator)) {
			readopts = true;
			continue;
		}

		if (readopts) {
			fFile >> opt;
			if (!SetOption(param, opt))
				std::cout << "Could not set option for " << param << "..."
						<< std::endl;
		} else {
			fFile >> val;
			fFile >> units;
			if (!SetValue(param, val, units))
				std::cout << "Could not set value for " << param << "..."
						<< std::endl;
		}
	}

	fFile.close();
	return;
}
