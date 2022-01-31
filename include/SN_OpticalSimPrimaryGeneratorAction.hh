/// SN_OpticalSimPrimaryGeneratorAction.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> 
/// Copyright: 2010 (C) NEMO3/SuperNEMO - University of Texas at Austin

#ifndef SN_OpticalSimPrimaryGeneratorAction_h
#define SN_OpticalSimPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"

class G4Event;

class SN_OpticalSimPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    SN_OpticalSimPrimaryGeneratorAction();
  ~SN_OpticalSimPrimaryGeneratorAction();

  public:
  void GeneratePrimaries(G4Event* anEvent);
  void SetEnergy(G4double en){particleGun->SetParticleEnergy(en);};
  
  G4double GetEnergy(){return particleGun->GetParticleEnergy();};

  private:
  G4ParticleGun* particleGun;
  G4GeneralParticleSource *particleSource;
  G4double IncidentE;
};

#endif


