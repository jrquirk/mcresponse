// ****************************************************************************
// *     Filename:  TEvent.h
// *  Description:  ROOT event class
// *      License:	GNU LESSER GENERAL PUBLIC LICENSE Version 2.1
// *       Author:  Tran Hoai Nam (nam), nam@kuno-g.phys.sci.osaka-u.ac.jp
// * Organization:  Osaka University
// *      Created:  2012-09-14 23:52:13
// ****************************************************************************
#ifndef TEVENT_5C88AHBF
#define TEVENT_5C88AHBF

#include "TTrackerHit.hh"
#include "TObject.h"
#include "TH1D.h"
#include "Rtypes.h"
#include <vector>
//This event object stores:
//- event id
//- a vector of deposited energies
//- a vector of tracker-type hits
//- a flag for stopped primary particles

class TEvent : public TObject {
	public:
		TEvent ();
		TEvent (int id);
		~TEvent ();

	private:
		unsigned int fEvtId;
		std::vector<double> fEdep;
		bool fPriStoppedFlag;
		bool fMuVetoFlag;
		bool fMuSCAFlag;
		bool fMuTriggerFlag;
		unsigned int fPriStopDet;
		std::vector<TTrackerHit> fHit;

		// For detector response
		int fDRNum;
		std::vector<int> fDRDetector;
		std::vector<int> fDRParticle;
		std::vector<double> fDREnergy;
		std::vector<double> fDRTime;

	public:
		void Reset();
		void SetEvtId (int value) {fEvtId = value;}
		int GetEvtId() { return fEvtId;}

		unsigned int GetNoOfDets() { return  fEdep.size();}
		void SetEdep (double value, int i) {fEdep[i] = value;} //should use AddEdep 
		double GetEdep(int i) { return fEdep.at(i);}
		void AddEdep (double value) {fEdep.push_back(value);}
		void SetPriStopDet(unsigned int value) {fPriStopDet = value;}
		unsigned int GetPriStopDet() {return fPriStopDet;}

		void SetPriStoppedFlag() {fPriStoppedFlag = true;}
		bool HasPriStopped() {return fPriStoppedFlag;}
		void SetMuTrigger() {fMuTriggerFlag = true;}
		bool HasMuTriggered() {return fMuTriggerFlag;}
		void SetMuVeto() {fMuVetoFlag = true;}
		bool HasMuVeto() {return fMuVetoFlag;}
		void SetMuSCAVeto() {fMuSCAFlag = true;}
		bool HasMuSCA() {return fMuSCAFlag;}
		bool IsValid() {return (fMuTriggerFlag && !(fMuVetoFlag || fMuSCAFlag));}

		unsigned int GetNoOfHits() {return fHit.size();}
		void AddHit(TTrackerHit aHit){fHit.push_back(aHit);}
		TTrackerHit GetHit(unsigned int i){return fHit[i];}

		void AddDetectorResponse(int part,int det,double energy,double time);
		int GetDetectorResponse(std::vector<int>** part,std::vector<int>** det,std::vector<double>** energy,std::vector<double>** time);

		ClassDef(TEvent, 1);
}; // end of class declaration TEvent 

#endif /* end of include guard: TEVENT_5C88AHBF */
