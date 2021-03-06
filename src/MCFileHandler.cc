#include "MCFileHandler.hh"

#include "TChain.h"

#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>

MCFileHandler::MCFileHandler() :
		fLoaded(false), fPath("output/"), fPreName("run"), fNRuns(0), fRuns(), fBadRuns(), fEvents(
				"t") {
}

MCFileHandler::~MCFileHandler() {
}

bool MCFileHandler::Register(unsigned int run) {
	if (run > fMaxRun || run < fMinRun)
		return false;
	fRuns.push_back(run);
	return true;
}

bool MCFileHandler::Register(unsigned int lRun, unsigned int hRun) {
	if (lRun >= hRun || lRun < fMinRun || hRun > fMaxRun)
		return false;
	for (unsigned int iRun = lRun; iRun <= hRun; iRun++)
		fRuns.push_back(iRun);
	return true;
}

bool MCFileHandler::Register(std::string runs) {
	unsigned int rangepos;
	unsigned int listpos;
	unsigned int run, lRun, hRun;

	while (runs.length() != 0) {
		rangepos = runs.find(fRangeSep);
		listpos = runs.find(fListSep);
		if (listpos < rangepos) {
			run = (unsigned int) atoi(runs.c_str());
			if (!Register(run))
				return false;
			runs.erase(0, listpos + 1);
		} else if (rangepos < listpos) {
			lRun = atoi(runs.c_str());
			runs.erase(0, rangepos + 1);
			hRun = atoi(runs.c_str());
			runs.erase(0, runs.find(fListSep) + 1);
			if (!Register(lRun, hRun))
				return false;
		} else {
			run = (unsigned int) atoi(runs.c_str());
			if (!Register(run))
				return false;
			runs.erase(0);
		}
	}
	return true;
}

bool MCFileHandler::RegisterFile(std::string) {
	return false;
}

void MCFileHandler::Load() {
	if (fLoaded)
		return;

	std::string rfname;
	for (unsigned int iRun = 0; iRun < fRuns.size(); iRun++) {
		rfname = GetRunFileName(fRuns[iRun]);
		std::cout << "Attempting to load " << rfname << "...";
		if (RunFileExists(rfname)) {
			fEvents.Add(rfname.c_str());
			fNRuns++;
			std::cout << "loaded." << std::endl;
		} else {
			fBadRuns.push_back(iRun);
			std::cout << "NOT FOUND!" << std::endl;
		}
	}

	fLoaded = true;
}

std::string MCFileHandler::GetRunFileName(unsigned int run) {
	char runname[6];
	sprintf(runname, "%.5u", run);
	return (fPath + fPreName + std::string(runname) + fExtension);
}

void MCFileHandler::SetPath(std::string& path) {
	fPath = path;
}

std::string MCFileHandler::GetPath() {
	return fPath;
}

bool MCFileHandler::RunFileExists(std::string& fname) {
	std::ifstream f(fname.c_str());
	if (!f.is_open())
		return false;
	f.close();
	return true;
}

TChain* MCFileHandler::GetEventChain() {
	return &fEvents;
}

const std::string MCFileHandler::fExtension = ".root";
