/// SN_OpticalSimTrackInformation.hh
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> 
/// Copyright: 2010 (C) NEMO3/SuperNEMO - University of Texas at Austin

//This class stores the statistics for each photon

#ifndef SN_OpticalSimTrackInformation_h
#define SN_OpticalSimTrackInformation_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4Allocator.hh"
#include "G4Track.hh"
#include "G4VUserTrackInformation.hh"
#include "CLHEP/Units/SystemOfUnits.h"

class SN_OpticalSimTrackInformation : public G4VUserTrackInformation
{
 public:
  SN_OpticalSimTrackInformation();
  SN_OpticalSimTrackInformation(const G4Track *aTrack);
  ~SN_OpticalSimTrackInformation();
  
  inline void *operator new(size_t);
  inline void operator delete(void *aTrackInfo);
  
 private:
  G4ThreeVector BirthPosition;
  G4double BirthLambda;
  G4int Reflections;
  G4int ReflectBeforeWLS;
  G4int ReflectAfterWLS;
  G4double DistBeforeWLS;
  G4double DistAfterWLS;
  G4double TotalTrackLength;
  G4int WLSCount;
  G4int TotalInternalReflections;
  G4double TimeBeforeWLS;
  G4double my_time;
  G4double my_time2;
  
 public:

  G4ThreeVector GetBirthPosition() const {return BirthPosition;}
  G4double GetBirthLambda() const {return BirthLambda;}
  G4double GetDistBeforeWLS() const {return DistBeforeWLS;}
  G4double GetDistAfterWLS() const {return DistAfterWLS;}
  G4double GetTotalTrackLength() const {return TotalTrackLength;}
  G4double GetTimeBeforeWLS() const {return TimeBeforeWLS;}

  G4int GetReflections() {return Reflections;}
  G4int GetReflectBeforeWLS() const {return ReflectBeforeWLS;}
  G4int GetReflectAfterWLS() const {return ReflectAfterWLS;}
  G4double GetTrackLength() const {return DistBeforeWLS;}
  G4int GetWLSCount() const {return WLSCount;}
  G4int GetTotalInternalReflections() const {return TotalInternalReflections;}

  void CountWLS(){WLSCount++;}

  void CountReflections();
  void CountTotalInternalReflections();


  void SetDistBeforeWLS(G4double dist){DistBeforeWLS = dist;}
  void Print() const {;}
};

extern G4Allocator<SN_OpticalSimTrackInformation> TrackInformationAllocator;

inline void* SN_OpticalSimTrackInformation::operator new(size_t)
{ void* aTrackInfo;
  aTrackInfo = (void*)TrackInformationAllocator.MallocSingle();
  return aTrackInfo;
}

inline void SN_OpticalSimTrackInformation::operator delete(void *aTrackInfo)
{ TrackInformationAllocator.FreeSingle((SN_OpticalSimTrackInformation*)aTrackInfo);}

#endif
