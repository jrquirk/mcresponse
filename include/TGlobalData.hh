#ifndef TGlobalData_h__
#define TGlobalData_h__

#include <vector>
#include <map>
#include <string>

#include <TObject.h>

#include "TPulseIsland.hh"

/** This class holds all of the data created by analysis modules.
 */
class TGlobalData: public TObject {
public:
	// Map of bank names to vectors of islands.
	// Elements are of the type
	//   std::pair<std::string, std::vector<TPulseIsland*> >
	std::map<std::string, std::vector<TPulseIsland*> > fPulseIslandToChannelMap;

	ClassDef(TGlobalData, 1)
};
#endif
