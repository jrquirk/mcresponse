// ****************************************************************************
// *     Filename:  TTrackerHit.cxx
// *  Description:  TTrackerHit implementation
// *      License:	GNU LESSER GENERAL PUBLIC LICENSE Version 2.1
// *       Author:  Tran Hoai Nam (nam), nam@kuno-g.phys.sci.osaka-u.ac.jp
// * Organization:  Osaka University
// *      Created:  2012-09-17 18:01:44
// ****************************************************************************
#include "TTrackerHit.hh"
#include <iostream>
#include <math.h>
#include <iomanip>
#include <limits>
#include <string.h>

ClassImp(TTrackerHit);

TTrackerHit::TTrackerHit() 
{
  fHitType = InvalidHit;
  strcpy(fPName,"");
  fParticleGenesis = 16;
  SetPos(0,0,0);
}

TTrackerHit::TTrackerHit(unsigned char i) 
{
  fHitType = InvalidHit;
  fDetID = i;
  strcpy(fPName,"");
  fParticleGenesis = 16; // Invalid detector
  SetPos(0,0,0);
}

void TTrackerHit::SetPos(double x, double y, double z) 
{
	fPos[0] = x;
	fPos[1] = y;
	fPos[2] = z;
}

void TTrackerHit::GetPos(double *Pos) 
{
  for (int i = 0; i < 3; i++) {
    Pos[i] = fPos[i];
  }
}

void TTrackerHit::SetGlobalPos(double x, double y, double z)
{
  fGlobalPos[0] = x;
  fGlobalPos[1] = y;
  fGlobalPos[2] = z;
}

void TTrackerHit::GetGlobalPos(double *Pos)
{
  for (int i = 0; i < 3; i++) {
    Pos[i] = fGlobalPos[i];
  }
}

void TTrackerHit::SetHitType(HitType type) 
{
	if((type<First) || (type>Last))
	{
		printf("Invalid type!\n");
		fHitType = InvalidHit;
		return;
	}
	else
		fHitType = type;
}

void TTrackerHit::ShowHit() 
{
	switch(fHitType)
	{
		case PrimaryStopped:
			printf("Primary stopped in target (DetID %d)",fDetID);
			printf(", PCode %d",fPCode);
			printf(",PName %s",fPName);
			printf(", Pos (%.1f, %.1f, %.1f) um",fPos[0],fPos[1],fPos[2]);
			printf(", time: %.3f ns\n",fTime);
			break;
		case ParticleEntered:
			printf("Particle entered det %d, at ",fDetID);
			printf("(%.1f, %.1f, %.1f) um",fPos[0],fPos[1],fPos[2]);
			printf(", time: %.3f ns",fTime);
			printf(", track %d, PCode %d, PName %s\n",fTrackID,fPCode,fPName);
			break;
		case ParticleExited:
			printf("Particle exited det %d, at ",fDetID);
			printf("(%.1f, %.1f, %.1f) um",fPos[0],fPos[1],fPos[2]);
			printf(", time: %.3f ns",fTime);
			printf(", track %d, PCode %d, PName %s\n",fTrackID,fPCode,fPName);
			break;
		case NewParticle:
			printf("New particle in target (det %d), at ",fDetID);
			printf("(%.1f, %.1f, %.1f) um",fPos[0],fPos[1],fPos[2]);
			printf(", time: %.3f ns",fTime);
			printf(", track %d, PCode %d, PName %s\n",fTrackID,fPCode,fPName);
			break;
		default:
			break;
	}
}
