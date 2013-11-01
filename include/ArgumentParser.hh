#ifndef ARGUMENTPARSER_HH
#define ARGUMENTPARSER_HH

#include <string>
#include <vector>

class ArgumentParser {

private:
	bool fParsed;
	bool fValid;

	int fNArgs;

	static const std::string fN;
	static const std::string fS;
	static const std::string fL;

	std::vector<std::string> fShort;
	std::vector<std::string> fLong;
	std::vector<std::string> fDefault;
	std::vector<std::string> fDescription;
	std::vector<bool> fValued;
	std::vector<bool> fMandatory;

	std::vector<bool> fEnabled;
	std::vector<std::string> fValue;

public:
	ArgumentParser();
	~ArgumentParser();

public:
	// Short, Long, Default, Description, Has Associated Value, Mandatory
	bool Register(std::string, std::string, std::string, std::string, bool, bool);

	void Parse(int argc, char* argv[]);

	bool Valid();
	bool Parsed();

	bool Enabled(std::string&);
	std::string Get(std::string&);
	bool Enabled(const char*);
	std::string Get(const char*);

	void Print();

private:
	// Short, Long
	bool CheckArguments(std::string&,std::string&);
	void Validate();
	static bool IsShort(std::string&);
	static bool IsLong(std::string&);
	static void Strip(std::string&);
};

#endif
