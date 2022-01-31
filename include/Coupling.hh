/// PMT.hh
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> 
/// Copyright: 2010 (C) NEMO3/SuperNEMO - University of Texas at Austin

/*This class is used to create a special liquid gap.
GetModule() returns the logical volume containing all the components
GetLogicStrip() returns the scintillator for use as an active detector
*/

#ifndef Coupling_h
#define Coupling_h

#include "G4LogicalVolume.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "CLHEP/Units/SystemOfUnits.h"

class SN_OpticalSimMaterials;
class Scintillator;

class Coupling
{
public:


  Coupling();
  ~Coupling();
  void Construct();
  
public:

  G4LogicalVolume *GetSNMW_SphericalCoupling();
  G4LogicalVolume *GetSNMW_8InchesSphericalCoupling();
  G4LogicalVolume *GetSphericalCouplingGlass();
  G4LogicalVolume *GetN3EC_CouplingLightGuide();
  G4LogicalVolume *GetN3EC_SphericalCoupling();
  G4LogicalVolume *GetN3EC_WrapGluedLightGuide();
  G4LogicalVolume *GetN3EC_WrapGluedInterfaceLightGuide();
  G4LogicalVolume *GetSNXW_CouplingLightGuide();
  G4LogicalVolume *GetSNXW_SphericalCoupling();
  G4LogicalVolume *GetSNXW_WrapGluedLightGuide();
  G4LogicalVolume *GetN3DC_8InchesSphericalCoupling();
  G4LogicalVolume *GetAluminiumPlateBordeaux();

private:
  SN_OpticalSimMaterials* scintProp;
  Scintillator *theScint;
  static const G4String path_bin;
  G4Material* Cargille;
  G4Material* Alu;
  G4Material* grease;

  G4LogicalVolume *LogicalSNMW_CouplingSphere;
  G4LogicalVolume *LogicalSNMW_8InchesCouplingSphere;
  G4LogicalVolume *LogicalN3EC_CouplingScLightGuide;
  G4LogicalVolume *LogicalN3EC_CouplingSphere;
  G4LogicalVolume *LogicalN3EC_WrapGluedLightGuide;
  G4LogicalVolume *LogicalN3EC_WrapGluedInterfaceLightGuide;
  G4LogicalVolume *logical_spherical_Coupling_glass;
  G4LogicalVolume *LogicalSNXW_CouplingScLightGuide;
  G4LogicalVolume *LogicalSNXW_CouplingSphere;
  G4LogicalVolume *LogicalSNXW_WrapGluedLightGuide;
  G4LogicalVolume *LogicalN3DC_8InchesCouplingSphere;
  G4LogicalVolume *LogicalAluminiumPlateBordeaux;

  G4double scint_radius;
  G4double coupling_radius;
  G4double paintthickness;
  G4double stripwidth;
  G4double scint_thickness;
  G4double spherical_pmt_radius;
  G4double light_guide_forPMT_spherical_radius;
  G4double light_guide_forPMT_height;

  G4double SNMW_CouplingSphereRadius;
  G4double SNMW_CouplingSphereDepth;
  G4double N3EC_LightGuideCouplingRadius;
  G4double N3EC_LightGuideCouplingDepth;
  G4double N3EC_LightGuideRadius;
  G4double N3EC_LightGuideHeight;
  G4double AirGapMylar;
  G4double GlueThickness;
  G4double PMTGlassRadius;
  G4double MylarThickness;
  G4double N3EC_LightGuideCouplingHeight;
  G4double N3EC_CouplingSphereRadius;
  G4double N3EC_CouplingSphereDepth;
  G4double N3EC_LightGuideStepHeight;
  G4double N3EC_InterfaceLightGuideHeight;
  G4double SNXW_LightGuideRadius;
  G4double SNXW_LightGuideHeight;
  G4double SNXW_LightGuideCouplingHeight;
  G4double SNXW_CouplingSphereRadius;
  G4double SNXW_CouplingSphereDepth;


  G4double scintAbsorbconst;
  G4double scintIndexconst;


};
#endif
