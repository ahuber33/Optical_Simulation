/// SN_OpticalSimTrackInformation.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> 
/// Copyright: 2010 (C) NEMO3/SuperNEMO - University of Texas at Austin

#include "SN_OpticalSimTrackInformation.hh"
#include "G4VProcess.hh"

using namespace CLHEP;

G4Allocator<SN_OpticalSimTrackInformation> TrackInformationAllocator;
SN_OpticalSimTrackInformation::SN_OpticalSimTrackInformation(){}
SN_OpticalSimTrackInformation::SN_OpticalSimTrackInformation(const G4Track *aTrack)
{
  //set new TrackInformation for each photon at scintillation
  if(!aTrack->GetUserInformation()){
    BirthPosition = aTrack->GetVertexPosition();
    BirthLambda = 1240/(aTrack->GetTotalEnergy()/eV);
    DistBeforeWLS = 0;
    DistAfterWLS = 0;
    ReflectBeforeWLS = 0;
    ReflectAfterWLS = 0;
    Reflections = 0;
    WLSCount = 0;
    TotalInternalReflections = 0;
  }

  else{
    SN_OpticalSimTrackInformation *anInfo = (SN_OpticalSimTrackInformation*)(aTrack->GetUserInformation());

    BirthPosition = aTrack->GetVertexPosition();

    if(WLSCount == 0){
      DistBeforeWLS = aTrack->GetTrackLength()/mm;
    }

    if(WLSCount > 0){
      DistBeforeWLS = aTrack->GetTrackLength()/mm;
    }

    ReflectBeforeWLS = anInfo->GetReflectBeforeWLS();
    ReflectAfterWLS = anInfo->GetReflectAfterWLS();
    Reflections = anInfo->GetReflections();
    WLSCount = anInfo->GetWLSCount()+1;
//G4cout<< "WLS count = " << anInfo->GetWLSCount() << G4endl;

  }
}

void SN_OpticalSimTrackInformation::CountReflections(){
  Reflections++;
      
  if(WLSCount==0)
    ReflectBeforeWLS++;
  else
    if(WLSCount > 0){
      ReflectAfterWLS++;
    }
  
}

void SN_OpticalSimTrackInformation::CountTotalInternalReflections(){

    TotalInternalReflections++;
}

SN_OpticalSimTrackInformation::~SN_OpticalSimTrackInformation(){}

