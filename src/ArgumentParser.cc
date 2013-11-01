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
	fValue.push_back(def);
	fDescription.push_back(desc);
	fValued.push_back(val);
	fMandatory.push_back(mand);
	fEnabled.push_back(false);

	fNArgs++;
	return true;
}

void ArgumentParser::Parse(int argc, char* argv[]) {

	if (fNArgs == 0)
		return;

	fParsed = true;

	std::string cCLA;
	for (int iCLA = 1; iCLA < argc; iCLA++) {
		cCLA = argv[iCLA];
		if (IsShort(cCLA)) {
			Strip(cCLA);
			for (int iArg = 0; iArg < fNArgs; iArg++) {
				if (cCLA == fShort[iArg]) {
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
		} else if (IsLong(cCLA)) {
			Strip(cCLA);
			for (int iArg = 0; iArg < fNArgs; iArg++) {
				if (cCLA == fLong[iArg]) {
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
	}

	// Load defaults

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

	if (arg.length() == 1) {
		for (int i = 0; i < fNArgs; i++)
			if (fShort[i] == arg)
				return fEnabled[i];
	} else {
		for (int i = 0; i < fNArgs; i++)
			if (fLong[i] == arg)
				return fEnabled[i];
	}

	return false;
}

std::string ArgumentParser::Get(std::string& arg) {

	if (arg.length() == 1) {
		for (int i = 0; i < fNArgs; i++)
			if (fShort[i] == arg)
				return fValue[i];
	} else {
		for (int i = 0; i < fNArgs; i++)
			if (fLong[i] == arg)
				return fValue[i];
	}
	return fN;
}

bool ArgumentParser::Enabled(const char* arg_c) {
	std::string arg_s(arg_c);
	return Enabled(arg_s);
}

std::string ArgumentParser::Get(const char* arg_c) {
	std::string arg_s(arg_c);
	return Get(arg_s);
}

void ArgumentParser::Print() {

	std::cout << "Options:" << std::endl;
	std::cout << "Short argument are preceded by a single dash (-) and long arguments by a double dash (--)." << std::endl;
	std::cout << "------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "Short" << "\t\t" << "Long" << "\t\t" << "Description" << std::endl;
	for (int i = 0; i < fNArgs; i++)
		std::cout << fShort[i] << "\t\t" << fLong[i] << "\t\t" << fDescription[i]
				<< std::endl;
}

bool ArgumentParser::CheckArguments(std::string& sarg, std::string& larg) {
	return !(sarg.length() > 1) && !(larg.length() == 1);
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

bool ArgumentParser::IsShort(std::string& arg) {
	return (arg.length() == 2 && arg.substr(0, 1) == fS
			&& arg.substr(1, 1) != fS);
}

bool ArgumentParser::IsLong(std::string& arg) {
	return (arg.length() > 2 && arg.substr(0, 2) == fL);
}

void ArgumentParser::Strip(std::string& arg) {
	if (IsShort(arg))
		arg = arg.substr(1);
	else if (IsLong(arg))
		arg = arg.substr(2);
}

const std::string ArgumentParser::fN = "";
const std::string ArgumentParser::fS = "-";
const std::string ArgumentParser::fL = "--";
