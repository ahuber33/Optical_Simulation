/// PMT.hh
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> 
/// Copyright: 2010 (C) NEMO3/SuperNEMO - University of Texas at Austin

/*This class is used to create a plastic scintillator.
GetModule() returns the logical volume containing all the components
GetLogicStrip() returns the scintillator for use as an active detector
*/

#ifndef PMT_h
#define PMT_h

#include "G4LogicalVolume.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"

class SN_OpticalSimMaterials;
class Scintillator;
class PMT
{
public:

  PMT();
  ~PMT();
  void Construct();
  
public:

  G4LogicalVolume *GetSphericalPMTGlass();
  G4LogicalVolume *GetPMTTubeGlass();
  G4LogicalVolume *GetPMTGlass();
  G4LogicalVolume *GetTubePMT();
  G4LogicalVolume *GetPhotocathode();
  G4LogicalVolume *Get8InchesPhotocathode();
  G4LogicalVolume *Get8InchesPMTGlass();
  G4LogicalVolume *Get5InchesPhotocathode();
  G4LogicalVolume *Get5InchesPMTGlass();

private:
  SN_OpticalSimMaterials* scintProp;
  Scintillator *theScint;
  static const G4String path_bin;

  G4Material *Vacuum;
  G4Material *Borosilicate_Glass;
  G4Material *Cargille;
  G4Material *Plastic;

  G4double PMTGlassRadius;
  G4double PMTGlassThickness;
  G4double PMTPhotocathodeRadius;
  G4double PMTPhotocathodeThickness;
  G4double PMTRearGlassRadius;
  G4double PMTGlassTubeRadius;
  G4double PMTGlassTubeHeight;
  G4double CouplingSphereRadius;

  G4LogicalVolume *LogicalSphericalPMTGlass;
  G4LogicalVolume *LogicalPMTGlassTube;
  G4LogicalVolume *LogicalPhotocathode;
  G4LogicalVolume *Logical8InchesPhotocathode;
  G4LogicalVolume *LogicalTubePMT;
  G4LogicalVolume *LogicalTubePMTGlass;
  G4LogicalVolume *Logical8InchesPMTGlass;
  G4LogicalVolume *Logical5InchesPMTGlass;
  G4LogicalVolume *Logical5InchesPhotocathode;

};
#endif
