#include "TEvent.hh"
#include <iostream>
#include <math.h>
#include <iomanip>

ClassImp(TEvent);

TEvent::TEvent() 
{
	fEvtId = 0;	
	fHit.resize(0);
	fEdep.resize(0);
	fDRParticle.resize(0);
	fDRDetector.resize(0);
	fDREnergy.resize(0);
	fDRTime.resize(0);
	fDRNum = 0;
	fPriStoppedFlag = false;
	fMuTriggerFlag = false;
	fMuVetoFlag = false;
	fMuSCAFlag = false;
}

void TEvent::Reset() 
{
	fHit.resize(0);
	fEdep.resize(0);
	fDRParticle.resize(0);
	fDRDetector.resize(0);
	fDREnergy.resize(0);
	fDRTime.resize(0);
	fDRNum = 0;
	fPriStoppedFlag = false;
	fMuTriggerFlag = false;
	fMuVetoFlag = false;
	fMuSCAFlag = false;
}

TEvent::TEvent(int i) 
{
	fEvtId = i;	
	fHit.resize(0);
	fEdep.resize(0);
	fDRParticle.resize(0);
	fDRDetector.resize(0);
	fDREnergy.resize(0);
	fDRTime.resize(0);
	fDRNum = 0;
	fPriStoppedFlag = false;
	fMuTriggerFlag = false;
	fMuVetoFlag = false;
	fMuSCAFlag = false;
}

TEvent::~TEvent() 
{; }

void TEvent::AddDetectorResponse(int part,int det,double energy,double time)
{
  fDRNum++;
  fDRParticle.push_back(part);
  fDRDetector.push_back(det);
  fDREnergy.push_back(energy);
  fDRTime.push_back(time);
}

int TEvent::GetDetectorResponse(std::vector<int>** part,std::vector<int>** det,std::vector<double>** energy,std::vector<double>** time)
{
  *part = &fDRParticle;
  *det = &fDRDetector;
  *energy = &fDREnergy;
  *time = &fDRTime;
  return fDRNum;
}

