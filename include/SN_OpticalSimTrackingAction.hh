/// SN_OpticalSimTrackingAction.hh
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> 
/// Copyright: 2010 (C) NEMO3/SuperNEMO - University of Texas at Austin

#ifndef SN_OpticalSimTrackingAction_h
#define SN_OpticalSimTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "G4Track.hh"

class SN_OpticalSimTrackingAction : public G4UserTrackingAction
{
public:
  SN_OpticalSimTrackingAction();
  ~SN_OpticalSimTrackingAction();

  void PreUserTrackingAction(const G4Track*);
  void PostUserTrackingAction(const G4Track*);
};
#endif
