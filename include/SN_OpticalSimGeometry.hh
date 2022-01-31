/// SN_OpticalSimGeometry.hh
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> 
/// Copyright: 2010 (C) NEMO3/SuperNEMO - University of Texas at Austin

#ifndef SN_OpticalSimGeometry_h
#define SN_OpticalSimGeometry_h  1

#include "G4MaterialPropertiesTable.hh"

class Scintillator;
//class LogicalHolder;
//class Sealant;
class Coupling;
//class LightGuide;
class PMT;
class SN_OpticalSimMaterials;
class G4VPhysicalVolume;

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "CLHEP/Units/SystemOfUnits.h"

class  SN_OpticalSimGeometry:  public G4VUserDetectorConstruction
   {
   public:
      SN_OpticalSimGeometry();
     ~SN_OpticalSimGeometry();

   public:
     G4VPhysicalVolume* Construct();
     G4LogicalVolume* GetLWorld() {return LogicalWorld;}

   private:
     static const G4String path_bin;
     static const G4String path;
     // Classes for building various components
     //     LogicalHolder *holder;
     SN_OpticalSimMaterials *scintProp;
     Scintillator *theScint;
     Coupling *CouplingGlue;
     Coupling *CouplingMaker;
     //     LightGuide *LightGuideMaker;
     PMT *R6594;
     G4Material *Vacuum;
     G4Material *VacuumWorld;
     G4Material *Air;

     // Colors for visualizations
     G4VisAttributes *invis;
     G4VisAttributes *white;
     G4VisAttributes *gray;
     G4VisAttributes *gray_bis;
     G4VisAttributes *black;
     G4VisAttributes *red;
     G4VisAttributes *orange;
     G4VisAttributes *yellow;
     G4VisAttributes *green;
     G4VisAttributes *cyan;
     G4VisAttributes *blue;
     G4VisAttributes *magenta;

     // Logical Volumes
     G4LogicalVolume *LogicalWorld;
     G4LogicalVolume *LogicalHolder;
     G4LogicalVolume *LogicalSNMW_Scint;
     G4LogicalVolume *LogicalSNMW_8InchesScint;
     G4LogicalVolume *LogicalSNMW_8InchesScint_bis;
     G4LogicalVolume *LogicalSNMW_Teflon;
     G4LogicalVolume *LogicalSNMW_Mylar;
     G4LogicalVolume *LogicalSNMW_8InchesMylar;
     G4LogicalVolume *LogicalSNMW_Glue;
     G4LogicalVolume *LogicalSNMW_8InchesGlue;
     G4LogicalVolume *LogicalN3DC_8InchesGlue;
     G4LogicalVolume *LogicalPMTGlass;
     G4LogicalVolume *Logical8InchesPMTGlass;
     G4LogicalVolume *LogicalPMT;
     G4LogicalVolume *LogicalSource;
     G4LogicalVolume *LogicalBlindage;
     G4LogicalVolume *Logical8InchesPMT;
     G4LogicalVolume *LogicalN3EC_Scintillator;
     G4LogicalVolume *LogicalN3EC_Teflon;
     G4LogicalVolume *LogicalN3EC_Mylar;
     G4LogicalVolume *LogicalN3EC_LightGuide;
     G4LogicalVolume *LogicalN3EC_WrapLightGuide;
     G4LogicalVolume *LogicalN3EC_WrapGluedLightGuide;
     G4LogicalVolume *LogicalN3EC_InterfaceLightGuide;
     G4LogicalVolume *LogicalN3EC_WrapInterfaceLightGuide;
     G4LogicalVolume *LogicalN3EC_WrapGluedInterfaceLightGuide;
     G4LogicalVolume *LogicalN3EC_CouplingSc;
     G4LogicalVolume *LogicalN3EC_CouplingPMMA;
     G4LogicalVolume *LogicalN3EC_Glue;
     G4LogicalVolume *LogicalSNXW_Scintillator;
     G4LogicalVolume *LogicalSNXW_Teflon;
     G4LogicalVolume *LogicalSNXW_Mylar;
     G4LogicalVolume *LogicalSNXW_LightGuide;
     G4LogicalVolume *LogicalSNXW_WrapLightGuide;
     G4LogicalVolume *LogicalSNXW_WrapGluedLightGuide;
     G4LogicalVolume *LogicalSNXW_CouplingSc;
     G4LogicalVolume *LogicalSNXW_Glue;
     G4LogicalVolume *LogicalN3DC_Scintillator;
     G4LogicalVolume *LogicalN3DC_Teflon;
     G4LogicalVolume *LogicalN3DC_Mylar;
     G4LogicalVolume *LogicalN3DC_Glue;
     G4LogicalVolume *LogicalN3L4_Scintillator;
     G4LogicalVolume *LogicalN3L4_Teflon;
     G4LogicalVolume *LogicalN3L4_Mylar;
     G4LogicalVolume *LogicalAluminiumPlate;


     // Physical volumes
     G4VPhysicalVolume *PhysicalWorld;
     G4VPhysicalVolume *PhysicalHolder;
     G4VPhysicalVolume *PhysicalScint;
     G4VPhysicalVolume *PhysicalScint_bis;
     G4VPhysicalVolume *PhysicalTeflon;
     G4VPhysicalVolume *PhysicalMylar;
     G4VPhysicalVolume *PhysicalCouplingSc;
     G4VPhysicalVolume *PhysicalCouplingPMMA;
     G4VPhysicalVolume *PhysicalGlue;
     G4VPhysicalVolume *PhysicalLightGuide;
     G4VPhysicalVolume *PhysicalWrapLightGuide;
     G4VPhysicalVolume *PhysicalWrapGluedLightGuide;
     G4VPhysicalVolume *PhysicalInterfaceLightGuide;
     G4VPhysicalVolume *PhysicalWrapInterfaceLightGuide;
     G4VPhysicalVolume *PhysicalWrapGluedInterfaceLightGuide;
     G4VPhysicalVolume *PhysicalPMTGlass;
     G4VPhysicalVolume *PhysicalPMT;
     G4VPhysicalVolume *PhysicalSource;
     G4VPhysicalVolume *PhysicalBlindage;
     G4VPhysicalVolume *PhysicalAlu;

     // Optical surfaces
     G4OpticalSurface *OpticalTeflon;
     G4OpticalSurface *OpticalMylar;
     G4OpticalSurface *OpticalVikuiti;
     G4OpticalSurface *OpticalPMT;

     // Skin surfaces
     G4LogicalSkinSurface *SSTeflon;
     G4LogicalSkinSurface *SSMylar;
     G4LogicalSkinSurface *SSWrapLightGuide;
     G4LogicalSkinSurface *SSWrapInterfaceLightGuide;
     G4LogicalSkinSurface *SSPhotocathode;

     // Dimension values
     // scint SuperNEMO
     G4double SNMW_ScintStepWidth;
     G4double SNMW_ScintStepHeight;
     G4double SNMW_ScintBodyWidth;
     G4double SNMW_ScintBodyHeight;
     G4double SNMW_CouplingSphereRadius;
     G4double SNMW_CouplingSphereDepth;
     G4double SNMW_FullScintHeight;
     // wrapping
     G4double TeflonGap;
     G4double TeflonThickness;
     G4double MylarGap;
     G4double MylarThickness;  
     // glue
     G4double GlueThickness;  
     // PMT
     G4double PMTGlassRadius;  
     G4double PMTGlassThickness;  
     G4double PMTRearGlassRadius;  
     G4double PMTPhotocathodeThickness;  
     G4double PMTGlassTubeRadius;  
     G4double PMTGlassTubeHeight; 
     G4double PMTPhotocathodeRadius;

     // scint NEMO3 EC
     G4double N3EC_ScintWidth;
     G4double N3EC_ScintHeight; 

     // light guide NEMO3 EC
     G4double N3EC_LightGuideRadius;
     G4double N3EC_LightGuideHeight;
     G4double N3EC_LightGuideCouplingRadius;
     G4double N3EC_LightGuideCouplingHeight;
     G4double N3EC_CouplingSphereDepth;
     G4double N3EC_LightGuideStepHeight;
     G4double N3EC_InterfaceLightGuideHeight;
     G4double N3EC_CouplingSphereRadius;

     // scint SuperNEMO XWALL
     G4double SNXW_ScintWidth;
     G4double SNXW_ScintHeight; 

     // light guide SuperNEMO XWALL
     G4double SNXW_LightGuideRadius;
     G4double SNXW_LightGuideHeight;
     G4double SNXW_LightGuideCouplingHeight;
     G4double SNXW_CouplingSphereDepth;
     G4double SNXW_CouplingSphereRadius;

     // scint NEMO3 L4
     G4double N3L4_ScintLength;
     G4double N3L4_ScintWidth;
     G4double N3L4_ScintHeight; 

     // Material properties tables
     G4MaterialPropertiesTable *TeflonMPT;
     G4MaterialPropertiesTable *MylarMPT;
     G4MaterialPropertiesTable *VikuitiMPT;
     G4MaterialPropertiesTable *PMT_MPT;

  };
#endif

