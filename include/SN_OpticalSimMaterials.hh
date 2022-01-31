/// Scintillator.hh
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> 
/// Copyright: 2010 (C) NEMO3/SuperNEMO - University of Texas at Austin

// This class is used to create a plastic scintillator.

#ifndef SN_OpticalSimMaterials_h
#define SN_OpticalSimMaterials_h


#include "G4MaterialPropertiesTable.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "CLHEP/Units/SystemOfUnits.h"

class G4Material;

class SN_OpticalSimMaterials
{
public:

  SN_OpticalSimMaterials(G4String prop_buildfile);
  ~SN_OpticalSimMaterials();
  void Construct();

public:
  G4Material *GetMaterial(G4String);
  

private:

  static const G4String path;

  G4Element* elementH;
  G4Element* elementF;
  G4Element* elementC;
  G4Element* elementO;
  G4Element* elementTi;
  G4Element* elementSi;
  G4Element* elementB;
  G4Element* elementNa;
  G4Element* elementAl;
  G4Element* elementN;
  G4Element* elementFe;

  G4Material *SiO2; 
  G4Material *B2O3;
  G4Material *Na2O;
  G4Material *Al2O3;
  G4Material *Air; 
  G4Material *Fer; 

  G4MaterialPropertiesTable *bs_glassMPT;
  G4MaterialPropertiesTable *vacMPT;
  G4MaterialPropertiesTable *scintMPT;
  G4MaterialPropertiesTable *wlsMPT;
  G4MaterialPropertiesTable *plasticMPT;
  G4MaterialPropertiesTable *greaseMPT;
  G4MaterialPropertiesTable *cargilleMPT;

  G4Material *bs_glass;
  G4Material *polysty;
  G4Material *Ti02;
  G4Material *coating;
  G4Material *scintillator;
  G4Material *Vacuum;
  G4Material *VacuumWorld;
  G4Material *plastic;
  G4Material *mylar;  
  G4Material *teflon;  
  G4Material *grease;
  G4Material *cargille;
  G4Material *RTV;
  G4Material *PMMA;
  G4Material *Alu;

  G4double scintAbsorbconst;
  G4double scintIndexconst;
  G4double scalingfactor_low;
  G4double scalingfactor_high;
  G4double pop_scalingfactor_low;
  G4double pop_scalingfactor_high;
  G4double lightyield;
  G4double paint_ref_coeff;
  G4double mylar_ref_coeff;
  G4double pWavelength;


};
#endif
