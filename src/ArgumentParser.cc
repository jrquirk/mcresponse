#include "ArgumentParser.hh"

#include <vector>
#include <string>
#include <iostream>

ArgumentParser::ArgumentParser() {
	fParsed = false;
	fValid = false;
	fNArgs = 0;
}

ArgumentParser::~ArgumentParser() {
}

bool ArgumentParser::Register(std::string sarg, std::string larg = "",
		std::string def = "", std::string desc = "", bool val = false,
		bool mand = false) {

	if (fParsed)
		return false;

	if (!CheckArguments(sarg, larg))
		return false;

	fShort.push_back(sarg);
	fLong.push_back(larg);
	fDefault.push_back(def);
	fDescription.push_back(desc);
	fValued.push_back(val);
	fMandatory.push_back(mand);

	fNArgs++;
	return true;
}

void ArgumentParser::Parse(int argc, char* argv[]) {

	if (fNArgs == 0)
		return;

	fParsed = true;

	fValue = std::vector<std::string>(fNArgs, fN);
	fEnabled = std::vector<bool>(fNArgs, false);

	std::string cCLA;
	for (int iCLA = 1; iCLA < argc; iCLA++) {
		cCLA = argv[iCLA];
		for (int iArg = 0; iArg < fNArgs; iArg++) {
			if (cCLA == fShort[iArg] || cCLA == fLong[iArg]) {
				fEnabled[iArg] = true;
				if (fValued[iArg]) {
					if (++iCLA == argc)
						break;
					cCLA = argv[iCLA];
					if (IsShort(cCLA) || IsLong(cCLA))
						break;
					fValue[iArg] = cCLA;
				}
			}
		}
	}

	Validate();
}

bool ArgumentParser::Valid() {
	return fValid;
}

bool ArgumentParser::Parsed() {
	return fParsed;
}

bool ArgumentParser::Enabled(std::string& arg) {

	if (!fParsed)
		return false;

	if (IsShort(arg)) {
		for (int i = 0; i < fNArgs; i++)
			if (fShort[i] == arg)
				return fEnabled[i];
	} else if (IsLong(arg)) {
		for (int i = 0; i < fNArgs; i++)
			if (fLong[i] == arg)
				return fEnabled[i];
	}

	return false;
}

std::string ArgumentParser::Get(std::string& arg) {

	if (IsShort(arg)) {
		for (int i = 0; i < fNArgs; i++)
			if (fShort[i] == arg)
				return fValue[i];
	} else if (IsLong(arg)) {
		for (int i = 0; i < fNArgs; i++)
			if (fLong[i] == arg)
				return fValue[i];
	}

	return fN;
}

void ArgumentParser::Print() {

	if (!fParsed)
		return;

	std::cout << "Options:" << std::endl;
	for (int i = 0; i < fNArgs; i++)
		std::cout << fShort[i] << "\t" << fLong[i] << "\t" << fDescription[i]
				<< std::endl;
}

bool ArgumentParser::CheckArguments(std::string& sarg, std::string& larg) {
	return !(sarg.length() > 1) && !(larg.length() == 1);
}

bool ArgumentParser::IsShort(std::string& arg) {
	return arg.length() == 1;
}

bool ArgumentParser::IsLong(std::string& arg) {
	return arg.length() > 1;
}

void ArgumentParser::Validate() {

	if (!fParsed)
		return;

	for (int i = 0; i < fNArgs; i++) {
		if (fMandatory[i] && !fEnabled[i])
			return;
		if (fEnabled[i] && fValued[i])
			if (fValue[i] == fN)
				return;
	}

	fValid = true;
}

const std::string ArgumentParser::fN = "";
const std::string ArgumentParser::fS = "-";
const std::string ArgumentParser::fL = "--";
