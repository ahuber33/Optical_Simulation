/// SN_OpticalSimPrimaryGeneratorAction.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> 
/// Copyright: 2010 (C) NEMO3/SuperNEMO - University of Texas at Austin

#include "SN_OpticalSimPrimaryGeneratorAction.hh"
#include "SN_OpticalSimEventAction.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4UImanager.hh"
#include "globals.hh"
#include "G4EventManager.hh"
#include "SN_OpticalSimEventAction.hh"


// The commented out commands are to use this module with a G4ParticleGun 
// Note that setgun.mac is used with G4ParticleGun
// Note that setsource.mac is used with G4GeneralParticleSource
// More information about how to use G4GeneralParticleSouce at:
// http://reat.space.qinetiq.com/gps/


G4UImanager* UI = G4UImanager::GetUIpointer();
SN_OpticalSimPrimaryGeneratorAction::SN_OpticalSimPrimaryGeneratorAction(){
  G4int n_particle = 1;

  //particleGun = new G4ParticleGun(n_particle);
  //UI->ApplyCommand("/control/execute setgun.mac");
  //UI->ApplyCommand("/control/execute setgun_script_use.mac");
  
  particleSource = new G4GeneralParticleSource();
  //UI->ApplyCommand("/control/execute setsource.mac");
}

SN_OpticalSimPrimaryGeneratorAction::~SN_OpticalSimPrimaryGeneratorAction(){
  //delete particleGun;
  delete particleSource;
}

void SN_OpticalSimPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){
  
  //particleGun->GeneratePrimaryVertex(anEvent);
  particleSource->GeneratePrimaryVertex(anEvent);
  
  //  always require these two lines
  G4EventManager *evtman = G4EventManager::GetEventManager();
  SN_OpticalSimEventAction *evtac = (SN_OpticalSimEventAction*)evtman->GetUserEventAction();

  //evtac->SetIncidentE(particleGun->GetParticleEnergy());
  evtac->SetIncidentE(particleSource->GetParticleEnergy());
}


