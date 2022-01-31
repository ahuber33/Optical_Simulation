/// SN_OpticalSimTrackingAction.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> 
/// Copyright: 2010 (C) NEMO3/SuperNEMO - University of Texas at Austin

#include "globals.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4Event.hh"
#include "SN_OpticalSimTrackingAction.hh"
#include "G4VProcess.hh"
#include "G4TrackVector.hh"
#include "SN_OpticalSimTrackInformation.hh"
#include "G4TrackingManager.hh"
#include "G4EventManager.hh"

SN_OpticalSimTrackingAction::SN_OpticalSimTrackingAction(){}
SN_OpticalSimTrackingAction::~SN_OpticalSimTrackingAction(){}
void SN_OpticalSimTrackingAction::PreUserTrackingAction(const G4Track* aTrack){

  //attach a new TrackInformation to photons at scintillation
  G4String partname = aTrack->GetDefinition()->GetParticleName();
  if( partname == "opticalphoton" && aTrack->GetUserInformation() == 0 ){
      if(aTrack->GetCreatorProcess()->GetProcessName()=="Scintillation"){
	  G4Track* theTrack=(G4Track*)aTrack;
	  theTrack->SetUserInformation(new SN_OpticalSimTrackInformation(aTrack));
      }
    if(aTrack->GetCreatorProcess()->GetProcessName()=="Cerenkov" && aTrack->GetUserInformation() == 0){
	  G4Track* theTrack=(G4Track*)aTrack;
	  theTrack->SetUserInformation(new SN_OpticalSimTrackInformation(aTrack));
      }
  }

  /*
  double electronMomentum;

  if( partname == "e-" && aTrack->GetUserInformation() == 0 ){
	  Hep3Vector MomDir=aStep->GetTrack()->GetMomentumDirection();
	  electronMomentum = MomDir[2];

    G4cout << "we got an electron " << G4endl;
    G4cout << "the momentum is " << electronMomentum <<  G4endl;

  }
  */

}



void SN_OpticalSimTrackingAction::PostUserTrackingAction(const G4Track* aTrack){

  //attach a new TrackInformation to WLS photons, add parent info...only for wavelength shifting!!
  //otherwise this should work fine.
  //             G4cout << "we got to tracking action also" << G4endl;
  
  G4String partname = aTrack->GetDefinition()->GetParticleName();
  if(partname == "opticalphoton"){
      G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
      if(secondaries){
	  size_t nSeco = secondaries->size();
	  if(nSeco > 0){
	      for(size_t i = 0; i < nSeco; i++){
		  G4String cproc = (*secondaries)[i]->GetCreatorProcess()->GetProcessName();
		  if(cproc == "OpWLS") {	     
		      (*secondaries)[i]->SetUserInformation(new SN_OpticalSimTrackInformation(aTrack));
		  }
	      }
	  }
      }   
  }
  


  //  G4cout << "Finished post user tracking action " << G4endl;

 
}  


// end PostUserTrackingAction()
