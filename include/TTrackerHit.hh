// ****************************************************************************
// *     Filename:  TTrackerHit.h
// *  Description:  Interested tracker hit info
// *      License:	GNU LESSER GENERAL PUBLIC LICENSE Version 2.1
// *       Author:  Tran Hoai Nam (nam), nam@kuno-g.phys.sci.osaka-u.ac.jp
// * Organization:  Osaka University
// *      Created:  2012-09-17 17:47:27
// ****************************************************************************
#ifndef TTRACKERHIT_7TXHZO36
#define TTRACKERHIT_7TXHZO36

#include "TString.h"
#include "TObject.h"
#include "Rtypes.h"

class TTrackerHit : public TObject {
	public:
		enum HitType {
			PrimaryStopped = 0,
			ParticleEntered,
			ParticleExited,
			NewParticle,
			First = PrimaryStopped,
			Last = NewParticle,
			InvalidHit
		};

	public:
		TTrackerHit ();
		TTrackerHit (unsigned char i);
		~TTrackerHit (){;}
	private:
		HitType fHitType;
		unsigned char fDetID;
		unsigned char fDetCopyNo;
		unsigned int fTrackID;
		unsigned int fParentTrackID;
		char fCreatorProcessName[100];
		UInt_t fParticleGenesis;
		int fPCode; //Particle PDG code
		char fPName[20];
		double fKE;
		double fTime;
		double fPos[3];
		double fGlobalPos[3];
	public:
		//Remember to check for valid hit type before using getters
		void SetDetID (unsigned char value) {fDetID = value;}
		unsigned char GetDetID() { return fDetID;}
		void SetDetCopyNo(unsigned char value) {fDetCopyNo = value;}
		unsigned char GetDetCopyNo() { return fDetCopyNo;}
		void SetHitType (HitType value);
		HitType GetHitType() { return fHitType;}
		void SetTrackID (unsigned int value) {fTrackID = value;}
		unsigned int GetTrackID() { return fTrackID;}
		void SetParentTrackID(unsigned int value) {fParentTrackID = value;}
		unsigned int GetParentTrackID() { return fParentTrackID;}
		void SetPCode (int value) {fPCode = value;}
		int GetPCode() { return fPCode;}
		void SetPName(std::string value) {strcpy(fPName,value.c_str());}
		char* GetPName() { return fPName;}
		void SetCreatorProcessName(std::string value) {strcpy(fCreatorProcessName,value.c_str());}
		char* GetCreatorProcessName() { return fCreatorProcessName;}
		void SetParticleGenesis(UInt_t value){ fParticleGenesis = value;}
		UInt_t GetParticleGenesis(){ return fParticleGenesis;}
		void SetKE (double value) {fKE = value;}
		double GetKE() { return fKE;}
		void SetTime (double value) {fTime = value;}
		double GetTime() { return fTime;}
		void SetPos (double,double,double);
		void GetPos(double *);
		void SetGlobalPos(double,double,double);
		void GetGlobalPos(double *);
		
		void ShowHit();

		ClassDef(TTrackerHit,1);
}; // end of class declaration TTrackerHit 



#endif /* end of include guard: TTRACKERHIT_7TXHZO36 */
