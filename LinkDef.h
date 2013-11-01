#ifdef __CINT__

#include <vector>
#include <string>
#include <map>

#include "include/TGlobalData.hh"
#include "include/TPulseIsland.hh"
#include "include/TTrackerHit.hh"
#include "include/TEvent.hh"

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class std::vector<int>+;
#pragma link C++ class TGlobalData+;
#pragma link C++ class TPulseIsland+;
#pragma link C++ class TTrackerHit+;
#pragma link C++ class TEvent+;
#pragma link C++ class std::vector<TPulseIsland*>+;
#pragma link C++ class std::vector<TPulseIsland*>::iterator;
#pragma link C++ class std::vector<TTrackerHit>+;
#pragma link C++ function operator!= (std::vector<TPulseIsland*>::iterator, std::vector<TPulseIsland*>::iterator);
#pragma link C++ class std::map<std::string, std::vector<TPulseIsland*> >+;
#pragma link C++ class std::map<std::string, std::vector<TPulseIsland*> >::iterator;
#pragma link C++ class std::pair<std::string,std::vector<TPulseIsland*> >+;
#endif
