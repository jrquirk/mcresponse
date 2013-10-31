#ifndef MCFILEHANDLER_HH
#define MCFILEHANDLER_HH

#include "TChain.h"

#include <string>
#include <vector>

class MCFileHandler {
	
private:
	static const unsigned int fMaxRun = 99999;
	static const unsigned int fMinRun = 0;
	static const std::string fExtension;
	static const char fListSep = ',';
	static const char fRangeSep = '-';
	
	bool fLoaded;
	
	std::string fPath;
	std::string fPreName;
	
	int fNRuns;
	std::vector<unsigned int> fRuns;
	std::vector<unsigned int> fBadRuns;
	TChain fEvents;
	
public:
	MCFileHandler();
	~MCFileHandler();
	
public:
	bool Register(unsigned int);
	bool Register(unsigned int, unsigned int);
	bool Register(std::string);
	bool RegisterFile(std::string);

	void Load();

public:
	void SetPath(std::string&);
	std::string GetPath();
	
	TChain* GetEventChain();
	
private:
	static bool RunFileExists(std::string& fname);
	std::string GetRunFileName(unsigned int);
};

#endif
