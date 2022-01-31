/// SN_OpticalSimPhysics.hh
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> 
/// Copyright: 2010 (C) NEMO3/SuperNEMO - University of Texas at Austin

#ifndef  SN_OpticalSimPhysics_h
#define   SN_OpticalSimPhysics_h  1

//#include "G4VUserPhysicsList.hh"
#include "G4VModularPhysicsList.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
class  SN_OpticalSimPhysics:  public G4VModularPhysicsList
{      public:
  SN_OpticalSimPhysics();
  virtual ~SN_OpticalSimPhysics();
  
protected:
  virtual void ConstructParticle();
  virtual void ConstructProcess();
  virtual void ConstructOp();
  virtual void SetCuts();
protected:
  //virtual void ConstructBosons();
  //virtual void ConstructLeptons();
  //virtual void ConstructMesons();
  //virtual void ConstructBaryons();
protected:
  //virtual void ConstructGeneral();
  //virtual void ConstructEM();
  //virtual void ConstructAllShortLiveds();

private:
    G4VPhysicsConstructor*  emPhysicsList;
    G4VPhysicsConstructor*  particleList;
    G4VPhysicsConstructor*  raddecayList;
};
#endif

