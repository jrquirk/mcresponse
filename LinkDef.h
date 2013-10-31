#ifdef __CINT__

#include <vector>
#include <string>
#include <map>

#include "datastruct/TGlobalData.h"
#include "datastruct/TPulseIsland.h"

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class TGlobalData+;
#pragma link C++ class TPulseIsland+;
#pragma link C++ class std::vector<TPulseIsland*>+;
#pragma link C++ class std::vector<TPulseIsland*>::iterator;
#pragma link C++ function operator!= (std::vector<TPulseIsland*>::iterator, std::vector<TPulseIsland*>::iterator);
#pragma link C++ class std::map<std::string, std::vector<TPulseIsland*> >+;
#pragma link C++ class std::map<std::string, std::vector<TPulseIsland*> >::iterator;
#pragma link C++ class std::pair<std::string,std::vector<TPulseIsland*> >+;
#endif
