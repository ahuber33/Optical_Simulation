/// SN_OpticalSimSteppingAction.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> 
/// Copyright: 2010 (C) NEMO3/SuperNEMO - University of Texas at Austin

#ifndef SN_OpticalSimSteppingAction_h
#define SN_OpticalSimSteppingAction_h

#include "G4UserSteppingAction.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalVolume.hh"
#include "TH2F.h"
#include "TFile.h"

class G4Step;
class G4SteppingManager;
class PMT;

class SN_OpticalSimSteppingAction : public G4UserSteppingAction
{
public:
  SN_OpticalSimSteppingAction();
  ~SN_OpticalSimSteppingAction();
public:
  void UserSteppingAction(const G4Step*);
int Boundary;
  void Count(){Boundary++;}

  void SetTheta (double e) { NRJ1 = e;};
  double GetTheta() {return NRJ1;}

 
 

private:
  static const G4String path;
  int PreviousTrack;
  double NRJ1;
  PMT *R6594;
  G4LogicalVolume *Logical8InchesPMT;
  G4OpticalSurface *OpticalPMT;
  G4LogicalSkinSurface *SSPhotocathode;
  G4MaterialPropertiesTable *PMT_MPT;
  double Length_Track;
  double Total_Length_Track;
  double Time_Transportation_Sc;
  double Time_Emission_Sc;
  double Time_Emission_Sc_PTP;
  double Time_Emission_Sc_POPOP;
  double Time_Transit_PMT;


};
#endif
