/// SN_OpticalSimVisManager.hh
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> 
/// Copyright: 2010 (C) NEMO3/SuperNEMO - University of Texas at Austin

#ifndef SN_OpticalSimVisManager_h
#define SN_OpticalSimVisManager_h 1

#include "G4VisManager.hh"


class SN_OpticalSimVisManager: public G4VisManager {

public:

  SN_OpticalSimVisManager ();

private:

  void RegisterGraphicsSystems ();

};

#endif
