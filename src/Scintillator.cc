// Scintillator.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu>
/// Copyright: 2010 (C) NEMO3/SuperNEMO - University of Texas at Austin

#include "SN_OpticalSimMaterials.hh"
#include "Scintillator.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4Transform3D.hh"
#include "G4UnionSolid.hh"
#include <fstream>
#include <iostream>
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Polyhedra.hh"
#include <G4Polycone.hh>
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4SubtractionSolid.hh"
#include "G4PVPlacement.hh"
#include "G4UnitsTable.hh"
#include <math.h>

using namespace CLHEP;

// ***********************
// Constructor
// ***********************
const G4String Scintillator::path_bin = "/home/local1/Optical_Simulation_Arnaud/bin/";


Scintillator::Scintillator(G4String buildfile){


 
  // Read keys and values from file buildfile defined in x_blockGeometry.cc 
  // Some of these variables may not be needed.  
  std::ifstream config;
  config.open(buildfile);
  if ( !config.is_open() )
    G4cout << "Error opening file " << buildfile << G4endl;
  else{
      while( config.is_open() ){
	  G4String variable;
	  G4String unit;
	  G4double value;
	  
	  config >> variable;
	  if(!config.good()) break;
//####################### COMMON variables ###########################
          if(variable == "TeflonThickness"){
	    config >> value >> unit;
	    TeflonThickness = value*G4UnitDefinition::GetValueOf(unit);
	  }
	  else if(variable == "AirGapTeflon"){
	    config >> value >> unit;
	    AirGapTeflon = value*G4UnitDefinition::GetValueOf(unit);
	  }
	  else if(variable == "MylarThickness"){
	    config >> value >> unit;
	    MylarThickness = value*G4UnitDefinition::GetValueOf(unit);
	  }
	  else if(variable == "AirGapMylar"){
	    config >> value >> unit;
	    AirGapMylar = value*G4UnitDefinition::GetValueOf(unit);
	  }  
	  else if(variable == "GlueThickness"){
	    config >> value >> unit;
	    GlueThickness = value*G4UnitDefinition::GetValueOf(unit);
	  }  
	  // PMT 5 inches Dimensions
	  else if(variable == "PMTGlassRadius"){
	    config >> value >> unit;
	    PMTGlassRadius = value*G4UnitDefinition::GetValueOf(unit);
	  }  
	  else if(variable == "PMTGlassThickness"){
	    config >> value >> unit;
	    PMTGlassThickness = value*G4UnitDefinition::GetValueOf(unit);
	  }  
	  else if(variable == "PMTRearGlassRadius"){
	    config >> value >> unit;
	    PMTRearGlassRadius = value*G4UnitDefinition::GetValueOf(unit);
	  }  
	  else if(variable == "PMTPhotocathodeThickness"){
	    config >> value >> unit;
	    PMTPhotocathodeThickness = value*G4UnitDefinition::GetValueOf(unit);
	  }  
	  else if(variable == "PMTGlassTubeRadius"){
	    config >> value >> unit;
	    PMTGlassTubeRadius = value*G4UnitDefinition::GetValueOf(unit);
	  } 
	  else if(variable == "PMTGlassTubeHeight"){
	    config >> value >> unit;
	    PMTGlassTubeHeight = value*G4UnitDefinition::GetValueOf(unit);
	  }
//####################### SuperNEMO Main Wall variables ###########################
	  // Scint Block Dimensions
	  else if (variable == "SNMW_ScintStepWidth"){
	    config >> value >> unit;
	    SNMW_ScintStepWidth = value*G4UnitDefinition::GetValueOf(unit);
	  }
	  else if(variable == "SNMW_ScintStepHeight"){
	    config >> value >> unit;
	    SNMW_ScintStepHeight = value*G4UnitDefinition::GetValueOf(unit);
	  }
	  else if(variable == "SNMW_ScintBodyWidth"){
	    config >> value >> unit;
	    SNMW_ScintBodyWidth = value*G4UnitDefinition::GetValueOf(unit);
	  }
	  else if(variable == "SNMW_ScintBodyHeight"){
	    config >> value >> unit;
	    SNMW_ScintBodyHeight = value*G4UnitDefinition::GetValueOf(unit);
	  }
	  else if(variable == "SNMW_CouplingSphereRadius"){
	    config >> value >> unit;
	    SNMW_CouplingSphereRadius = value*G4UnitDefinition::GetValueOf(unit);
	  }
	  else if(variable == "SNMW_CouplingSphereDepth"){
	    config >> value >> unit;
	    SNMW_CouplingSphereDepth = value*G4UnitDefinition::GetValueOf(unit);
	  }
	 
//####################### NEMO 3 EC  variables ###########################
	  // Scintillator Dimensions
	  else if(variable == "N3EC_ScintWidth"){
	    config >> value >> unit;
	    N3EC_ScintWidth = value*G4UnitDefinition::GetValueOf(unit);
	  }  
	  else if(variable == "N3EC_ScintHeight"){
	    config >> value >> unit;
	    N3EC_ScintHeight = value*G4UnitDefinition::GetValueOf(unit);
	  } 
	  // Light Guide Dimensions
	  else if(variable == "N3EC_LightGuideRadius"){
	    config >> value >> unit;
	    N3EC_LightGuideRadius = value*G4UnitDefinition::GetValueOf(unit);
	  }  
	  else if(variable == "N3EC_LightGuideHeight"){
	    config >> value >> unit;
	    N3EC_LightGuideHeight = value*G4UnitDefinition::GetValueOf(unit);
	  }  
	  else if(variable == "N3EC_LightGuideCouplingRadius"){
	    config >> value >> unit;
	    N3EC_LightGuideCouplingRadius = value*G4UnitDefinition::GetValueOf(unit);
	  }  
	  else if(variable == "N3EC_LightGuideCouplingHeight"){
	    config >> value >> unit;
	    N3EC_LightGuideCouplingHeight = value*G4UnitDefinition::GetValueOf(unit);
	  }  
	  else if(variable == "N3EC_CouplingSphereDepth"){
	    config >> value >> unit;
	    N3EC_CouplingSphereDepth = value*G4UnitDefinition::GetValueOf(unit);
	  }    
	  else if(variable == "N3EC_InterfaceLightGuideHeight"){
	    config >> value >> unit;
	    N3EC_InterfaceLightGuideHeight = value*G4UnitDefinition::GetValueOf(unit);
	  }    
	  else if(variable == "N3EC_LightGuideStepHeight"){
	    config >> value >> unit;
	    N3EC_LightGuideStepHeight = value*G4UnitDefinition::GetValueOf(unit);
	  }    
	  else if(variable == "N3EC_CouplingSphereRadius"){
	    config >> value >> unit;
	    N3EC_CouplingSphereRadius = value*G4UnitDefinition::GetValueOf(unit);
	  }
//####################### SuperNEMO XWALL  variables ###########################
	  // Scintillator Dimensions
	  else if(variable == "SNXW_ScintWidth"){
	    config >> value >> unit;
	    SNXW_ScintWidth = value*G4UnitDefinition::GetValueOf(unit);
	  }  
	  else if(variable == "SNXW_ScintHeight"){
	    config >> value >> unit;
	    SNXW_ScintHeight = value*G4UnitDefinition::GetValueOf(unit);
	  } 
	  // Light Guide Dimensions
	  else if(variable == "SNXW_LightGuideRadius"){
	    config >> value >> unit;
	    SNXW_LightGuideRadius = value*G4UnitDefinition::GetValueOf(unit);
	  }  
	  else if(variable == "SNXW_LightGuideHeight"){
	    config >> value >> unit;
	    SNXW_LightGuideHeight = value*G4UnitDefinition::GetValueOf(unit);
	  }  
	  else if(variable == "SNXW_LightGuideCouplingHeight"){
	    config >> value >> unit;
	    SNXW_LightGuideCouplingHeight = value*G4UnitDefinition::GetValueOf(unit);
	  }  
	  else if(variable == "SNXW_CouplingSphereDepth"){
	    config >> value >> unit;
	    SNXW_CouplingSphereDepth = value*G4UnitDefinition::GetValueOf(unit);
	  }     
	  else if(variable == "SNXW_CouplingSphereRadius"){
	    config >> value >> unit;
	    SNXW_CouplingSphereRadius = value*G4UnitDefinition::GetValueOf(unit);
	  }
//########################### N3MO3 L4  variables ###########################
	  // Scintillator Dimensions
	  else if(variable == "N3L4_ScintLength"){
	    config >> value >> unit;
	    N3L4_ScintLength = value*G4UnitDefinition::GetValueOf(unit);
	  }  
	  else if(variable == "N3L4_ScintWidth"){
	    config >> value >> unit;
	    N3L4_ScintWidth = value*G4UnitDefinition::GetValueOf(unit);
	  }  
	  else if(variable == "N3L4_ScintHeight"){
	    config >> value >> unit;
	    N3L4_ScintHeight = value*G4UnitDefinition::GetValueOf(unit);
	  } 
      }
  }
  config.close();
/*
  G4cout << "The Variables that we read in are: " 
	 << "\n SNMW_ScintStepWidth = " << SNMW_ScintStepWidth 
	 << "\n SNMW_ScintStepHeight = " << SNMW_ScintStepHeight
	 << "\n SNMW_cintBodyWidth = " << SNMW_ScintBodyWidth 
	 << "\n SNMW_ScintBodyHeight = " << SNMW_ScintBodyHeight 
	 << "\n SNMW_CouplingSphereRadius = " << SNMW_CouplingSphereRadius
	 << "\n SNMW_CouplingSphereDepth = " << SNMW_CouplingSphereDepth
	 << "\n TeflonThickness = " << TeflonThickness
	 << "\n AirGapTeflon = " << AirGapTeflon
	 << "\n MylarThickness = " << MylarThickness
	 << "\n AirGapMylar = " << AirGapMylar
	 << "\n GlueThickness = " << GlueThickness
	 << "\n PMTGlassRadius = " << PMTGlassRadius
	 << "\n PMTRearGlassRadius = " << PMTRearGlassRadius
	 << "\n PMTGlassThickness = " << PMTGlassThickness
	 << "\n PMTPhotocathodeThickness = " << PMTPhotocathodeThickness
	 << "\n PMTGlassTubeRadius = " << PMTGlassTubeRadius
	 << "\n PMTGlassTubeHeight = " << PMTGlassTubeHeight
	 << "\n N3EC_ScintWidth = " << N3EC_ScintWidth
	 << "\n N3EC_ScintHeight = " << N3EC_ScintHeight
	 << "\n N3EC_LightGuideRadius = " << N3EC_LightGuideRadius
	 << "\n N3EC_LightGuideHeight = " << N3EC_LightGuideHeight
	 << "\n N3EC_LightGuideCouplingRadius = " << N3EC_LightGuideCouplingRadius
	 << "\n N3EC_LightGuideCouplingHeight = " << N3EC_LightGuideCouplingHeight
	 << "\n N3EC_CouplingSphereDepth = " << N3EC_CouplingSphereDepth
	 << "\n N3EC_InterfaceLightGuideHeight = " << N3EC_InterfaceLightGuideHeight
	 << "\n N3EC_LightGuideStepHeight = " << N3EC_LightGuideStepHeight
	 << "\n N3EC_CouplingSphereRadius = " << N3EC_CouplingSphereRadius
	 << "\n SNXW_ScintWidth = " << SNXW_ScintWidth
	 << "\n SNXW_ScintHeight = " << SNXW_ScintHeight
	 << "\n SNXW_LightGuideRadius = " << SNXW_LightGuideRadius
	 << "\n SNXW_LightGuideHeight = " << SNXW_LightGuideHeight
	 << "\n SNXW_LightGuideCouplingHeight = " << SNXW_LightGuideCouplingHeight
	 << "\n SNXW_CouplingSphereDepth = " << SNXW_CouplingSphereDepth
	 << "\n SNXW_CouplingSphereRadius = " << SNXW_CouplingSphereRadius
	 << "\n N3L4_ScintLength = " << N3L4_ScintLength
	 << "\n N3L4_ScintWidth = " << N3L4_ScintWidth
	 << "\n N3L4_ScintHeight = " << N3L4_ScintHeight << G4endl;
*/
}
// ***********************
// Destructor
// ***********************
Scintillator::~Scintillator(){
  if(scintProp) { delete scintProp; scintProp = 0; }
  if(Polystyrene) { delete Polystyrene; Polystyrene = 0; }
  if(coating) { delete coating; coating = 0; }
  if(Vacuum) { delete Vacuum; Vacuum = 0; }
  if(clear) { delete clear; clear = 0; }
  //  if(l_scint) { delete l_scint; l_scint = 0; }
}

//************************************************************************************
//************************************************************************************
//************************** SUPERNEMO MAIN WALL (SNMW) ******************************
//************************************************************************************
//************************************************************************************

// ***********************
// Builds Scintitllator Block
// ***********************
G4LogicalVolume *Scintillator::GetSNMW_Scint(){
  // Material Properties for scint
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");  
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Plastic = scintProp->GetMaterial("plastic");
  Polystyrene = scintProp->GetMaterial("polysty");
  scintillator = scintProp->GetMaterial("scintillator");

  // Define some translations used for joining or subtracting
   Step_BodyUnion = SNMW_ScintStepHeight/2 + SNMW_ScintBodyHeight/2;
   Block_SphereSubtraction = SNMW_ScintStepHeight/2 + SNMW_ScintBodyHeight + SNMW_CouplingSphereRadius - SNMW_CouplingSphereDepth; // Center of union will be center of step
   G4cout << "Step + body union translation = " << Step_BodyUnion
          << "\nBlock - Sphere subtraction translation = " << Block_SphereSubtraction << G4endl;

  // Define some rotations
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0*deg);
  G4RotationMatrix Rotate;
  Rotate.rotateX(180*deg);
  
  // ####################### SCINT BLOCK #######################
  // Scintillator Step
  G4Box *Step = new G4Box("Step",          
			  SNMW_ScintStepWidth/2, SNMW_ScintStepWidth/2, 
			  SNMW_ScintStepHeight/2);  

  // Scintillator Main Body
  G4Box *MainBody = new G4Box("MainBody",         
			      SNMW_ScintBodyWidth/2, SNMW_ScintBodyWidth/2,
			      SNMW_ScintBodyHeight/2); 
  
  // Union of two volumes: Center will be at the center of first volume defined in union -> Step center
  // Last argument diplaces the body center relative to step center before joining
  G4UnionSolid* SolidScint = new G4UnionSolid("Step+MainBody",Step,MainBody,G4Transform3D(DontRotate,G4ThreeVector(0,0,Step_BodyUnion)));
  
  // ####################### COUPLING SPHERE #######################
  G4Sphere *CouplingSphere = new G4Sphere("CouplingSphere",
					  0., SNMW_CouplingSphereRadius, // Coupling Sphere Radius = PMT Radius for now
					  0., 2*180*deg , // From 0-360 degrees in theta for full sphere
					  0., 0.45*180*deg) ; // From 0-90 degrees in phi for full sphere

  // ####################### FINAL BLOCK #######################
  // Subtract 2nd volume from the 1st 
  // SphereBlockTranslation = dist. from step center to center of sphere position where subtraction will occur
  G4SubtractionSolid* FinalScint = new G4SubtractionSolid("FinalScint", 
							  SolidScint,
							  CouplingSphere,
							  G4Transform3D(Rotate,G4ThreeVector(0,0,Block_SphereSubtraction)));
  
  // Logical volume for FinalScint: material = scintillator

  LogicalSNMW_FinalScint = new G4LogicalVolume(FinalScint, scintillator, "LogicalFinalScint",0,0,0);
  
  //G4VisAttributes * clear;
  //clear = new G4VisAttributes( G4Colour(255/255. ,255/255. ,255/255. ));

  return LogicalSNMW_FinalScint;  
}

// ***********************
// Builds Teflon wrapping
// ***********************
G4LogicalVolume *Scintillator::GetSNMW_Teflon(){
  // Define materials
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Teflon = scintProp->GetMaterial("teflon"); // Why is this plastic? Need to add Teflon?
  
  // Define some translations used for joining or subtracting
  TeflonStep_BodyUnion = SNMW_ScintStepHeight/2 + SNMW_ScintBodyHeight/2;
  Teflon_ScintSubtraction = (SNMW_ScintStepHeight + AirGapTeflon + TeflonThickness)/2 - SNMW_ScintStepHeight/2;
   G4cout << "Teflon step + body union translation = " << TeflonStep_BodyUnion 
          << "\nTeflon - scint subtraction translation = " << Teflon_ScintSubtraction << G4endl;

  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0*deg);
  G4RotationMatrix Rotate;
  Rotate.rotateX(180*deg);
  
  // ####################### TEFLON SOLID #######################
  // Teflon step
  G4Box *TeflonStep = new G4Box("TeflonStep",
				(TeflonThickness+AirGapTeflon+SNMW_ScintStepWidth+AirGapTeflon+TeflonThickness)/2, // Add air gap and teflon thickness to EACH side (x2)
				(TeflonThickness+AirGapTeflon+SNMW_ScintStepWidth+AirGapTeflon+TeflonThickness)/2, // Length = Width
				(SNMW_ScintStepHeight+AirGapTeflon+TeflonThickness)/2);  // Teflon doesn't cover top and bottom BUT needs to cover top of step
  
  // Teflon main body
  G4Box *TeflonMainBody = new G4Box("TeflonMainBody",         
				    (TeflonThickness+AirGapTeflon+SNMW_ScintBodyWidth+AirGapTeflon+TeflonThickness)/2, 
				    (TeflonThickness+AirGapTeflon+SNMW_ScintBodyWidth+AirGapTeflon+TeflonThickness)/2,
				    (SNMW_ScintBodyHeight-AirGapTeflon-TeflonThickness)/2); //Overall height must equal Scint block to leave no teflon above or below
  
  // Union of two volumes: Just as with scintillator
  G4UnionSolid* SolidTeflon = new G4UnionSolid("TeflonStep+TeflonMainBody",TeflonStep,TeflonMainBody,G4Transform3D(DontRotate,G4ThreeVector(0,0,TeflonStep_BodyUnion)));
  
  // ####################### SCINT SOLID #######################
  // Easy way now is to subtract scint block (and any air gaps) to leave only wrap... be we need to remake it
  // Scintillator Step
  G4Box *Step = new G4Box("Step",          
			  (AirGapTeflon+SNMW_ScintStepWidth+AirGapTeflon)/2, 
			  (AirGapTeflon+SNMW_ScintStepWidth+AirGapTeflon)/2, 
			  SNMW_ScintStepHeight/2+AirGapTeflon);  
  
  // Scintillator Main Body
  G4Box *MainBody = new G4Box("MainBody",         
			      (AirGapTeflon+SNMW_ScintBodyWidth+AirGapTeflon)/2, 
			      (AirGapTeflon+SNMW_ScintBodyWidth+AirGapTeflon)/2, 
			      SNMW_ScintBodyHeight/2+AirGapTeflon); 
  
  // Union of two volumes: Center will be at the center of first volume defined in union -> Step center
  G4UnionSolid* SolidScint = new G4UnionSolid("Step+MainBody",Step,MainBody,G4Transform3D(DontRotate,G4ThreeVector(0,0,TeflonStep_BodyUnion)));
  
  // ####################### TEFLON WRAP #######################
  // Now we just need to subract the scintillator to leave the Teflon wrap on just the sides
  // Displace solid scint so top and bottom edges match up
  G4SubtractionSolid* TeflonWrap = new G4SubtractionSolid("TeflonWrap", 
							  SolidTeflon,
							  SolidScint,
							  G4Transform3D(DontRotate,G4ThreeVector(0,0,-Teflon_ScintSubtraction)));

  // Logical volume for Teflon: material = plastic?

  LogicalSNMW_TeflonWrap = new G4LogicalVolume(TeflonWrap, Teflon, "LogicalTeflonWrap",0,0,0);
  
  G4VisAttributes *white;
  white = new G4VisAttributes(G4Color(1,1,1));
  //  LogicalTeflonWrap->SetVisAttributes(scinti);
  
  return LogicalSNMW_TeflonWrap;
}

// ***********************
// Builds Mylar wrapping
// ***********************
G4LogicalVolume *Scintillator::GetSNMW_Mylar(){
  // Define materials
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Mylar = scintProp->GetMaterial("mylar"); 
  
  // Define some translations used for joining or subtracting
  MylarStep_BodyUnion = (MylarThickness+AirGapMylar+SNMW_ScintStepHeight+AirGapTeflon+TeflonThickness+AirGapMylar+MylarThickness)/2 + (MylarThickness+AirGapMylar+SNMW_ScintBodyHeight-AirGapTeflon-TeflonThickness-AirGapMylar-MylarThickness)/2 ;
  TeflonStep_BodyUnion = (AirGapMylar+SNMW_ScintStepHeight+AirGapTeflon+TeflonThickness+AirGapMylar)/2 + (AirGapMylar+SNMW_ScintBodyHeight-AirGapTeflon-TeflonThickness-AirGapMylar)/2 ;
  //  G4double Mylar_TeflonSubtraction = (SNMW_ScintStepHeight+AirGapMylar+TeflonThickness+AirGapTeflon)/2 - SNMW_ScintStepHeight/2;
  Mylar_SphereSubtraction = MylarThickness+AirGapMylar+SNMW_ScintStepHeight+SNMW_ScintBodyHeight+SNMW_CouplingSphereRadius-SNMW_CouplingSphereDepth-(MylarThickness+AirGapMylar+SNMW_ScintStepHeight+AirGapTeflon+TeflonThickness+AirGapMylar+MylarThickness)/2;
   G4cout << "Mylar step + body union translation = " << MylarStep_BodyUnion 
          << "\nTeflon step + body union translation = " << TeflonStep_BodyUnion 
     //          << "\nMylar - teflon subtraction translation = " << Mylar_TeflonSubtraction    
          << "\nMylar - sphere subtraction translation = " << Mylar_SphereSubtraction << G4endl;  

  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0*deg);
  G4RotationMatrix Rotate;
  Rotate.rotateX(180*deg);
  
  // ####################### MYLAR SOLID #######################
  // Mylar step
  G4Box *MylarStep = new G4Box("MylarStep",
			       (MylarThickness+AirGapMylar+TeflonThickness+AirGapTeflon+SNMW_ScintStepWidth+AirGapTeflon+TeflonThickness+AirGapMylar+MylarThickness)/2, // 2 air gaps & Teflon & Mlyar thickness EACH side
			       (MylarThickness+AirGapMylar+TeflonThickness+AirGapTeflon+SNMW_ScintStepWidth+AirGapTeflon+TeflonThickness+AirGapMylar+MylarThickness)/2, 
			       (MylarThickness+AirGapMylar+SNMW_ScintStepHeight+AirGapTeflon+TeflonThickness+AirGapMylar+MylarThickness)/2);  // Bottom to top layers
  
  // Mylar Main Body
  G4Box *MylarMainBody = new G4Box("MylarMainBody",         
				   (MylarThickness+AirGapMylar+TeflonThickness+AirGapTeflon+SNMW_ScintBodyWidth+AirGapTeflon+TeflonThickness+AirGapMylar+MylarThickness)/2, 
				   (MylarThickness+AirGapMylar+TeflonThickness+AirGapTeflon+SNMW_ScintBodyWidth+AirGapTeflon+TeflonThickness+AirGapMylar+MylarThickness)/2,
				   (MylarThickness+AirGapMylar+SNMW_ScintBodyHeight-AirGapTeflon-TeflonThickness-AirGapMylar-MylarThickness)/2); //Now accounts for BOTH Teflon and Mylar and all air gaps: top to bottom
  
  // Union of two volumes: Just as with Teflon above
  G4UnionSolid* SolidMylar = new G4UnionSolid("MylarStep+MylarMainBody",MylarStep,MylarMainBody,G4Transform3D(DontRotate,G4ThreeVector(0,0,MylarStep_BodyUnion)));
  
  // Easy way again is to subtract a Teflon solid (and air gaps)... but we need to remake it
  // ####################### TEFLON SOLID #######################
  // Teflon step
  G4Box *TeflonStep = new G4Box("TeflonStep",
				(AirGapMylar+TeflonThickness+AirGapTeflon+SNMW_ScintStepWidth+AirGapTeflon+TeflonThickness+AirGapMylar)/2, 
				(AirGapMylar+TeflonThickness+AirGapTeflon+SNMW_ScintStepWidth+AirGapTeflon+TeflonThickness+AirGapMylar)/2, 
				(AirGapMylar+SNMW_ScintStepHeight+AirGapTeflon+TeflonThickness+AirGapMylar)/2);  // Teflon doesn't cover top and bottom BUT needs to cover top of step
  
  // Teflon Main Body
  G4Box *TeflonMainBody = new G4Box("TeflonMainBody",         
				    (AirGapMylar+TeflonThickness+AirGapTeflon+SNMW_ScintBodyWidth+AirGapTeflon+TeflonThickness+AirGapMylar)/2, 
				    (AirGapMylar+TeflonThickness+AirGapTeflon+SNMW_ScintBodyWidth+AirGapTeflon+TeflonThickness+AirGapMylar)/2, 
				    (AirGapMylar+SNMW_ScintBodyHeight-AirGapTeflon-TeflonThickness-AirGapMylar)/2);  
  
  // Union of two Teflon volumes
  G4UnionSolid* SolidTeflon = new G4UnionSolid("TeflonStep+TeflonMainBody",TeflonStep,TeflonMainBody,G4Transform3D(DontRotate,G4ThreeVector(0,0,TeflonStep_BodyUnion)));
  
  // ####################### MYLAR WRAP #######################
  // Now we just need to subract the Teflon to leave the Mylar wrap on all sides
  G4SubtractionSolid* MylarWrap = new G4SubtractionSolid("MylarWrap", 
							 SolidMylar,
							 SolidTeflon,
							 //							 G4Transform3D(Rotate,G4ThreeVector(0,0,Mylar_TeflonSubtraction)));
							 G4Transform3D(DontRotate,G4ThreeVector(0,0,0)));

  // Lastly, since there is Mylar on top we need to subtract the coupling sphere to leave an opening for the PMT
  G4Sphere *CouplingSphere = new G4Sphere("CouplingSphere",
					  0., SNMW_CouplingSphereRadius, 
					  0., 2*180*deg , 
					  0., 0.45*180*deg) ; 

  // Subtracting the coupling sphere
  G4SubtractionSolid* FinalMylarWrap = new G4SubtractionSolid("FinalMylarWrap", MylarWrap,CouplingSphere,G4Transform3D(Rotate,G4ThreeVector(0,0,Mylar_SphereSubtraction)));
  
  // Logical volume for Mylar: material = coating?
  
  LogicalSNMW_MylarWrap = new G4LogicalVolume(FinalMylarWrap, Mylar, "LogicalMylarWrap",0,0,0);
  
  
  return LogicalSNMW_MylarWrap;
}







//************************************************************************************
//************************************************************************************
//********************** SUPERNEMO MAIN WALL 8 INCHES(SNMW) **************************
//************************************************************************************
//************************************************************************************

// ***********************
// Builds Scintitllator Block
// ***********************
G4LogicalVolume *Scintillator::GetSNMW_8InchesScint(){
  // Material Properties for scint
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");  
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Plastic = scintProp->GetMaterial("plastic");
  Polystyrene = scintProp->GetMaterial("polysty");
  scintillator = scintProp->GetMaterial("scintillator");

  // Define some translations used for joining or subtracting
   Step_BodyUnion = SNMW_ScintStepHeight/2 + SNMW_ScintBodyHeight/2;
   Block_SphereSubtraction = SNMW_ScintStepHeight/2 + SNMW_ScintBodyHeight + SNMW_CouplingSphereRadius - SNMW_CouplingSphereDepth; // Center of union will be center of step


  // Define some rotations
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0*deg);
  G4RotationMatrix Rotate;
  Rotate.rotateX(180*deg);
  
  // ####################### SCINT BLOCK #######################
  // Scintillator Step
  G4Box *Step8Inches = new G4Box("Step8Inches",          
			  SNMW_ScintStepWidth/2, SNMW_ScintStepWidth/2, 
			  SNMW_ScintStepHeight/2);  

  // Scintillator Main Body
  G4Box *MainBody8Inches = new G4Box("MainBody8Inches",         
			      SNMW_ScintBodyWidth/2, SNMW_ScintBodyWidth/2,
			      SNMW_ScintBodyHeight/2); 
  
  // Union of two volumes: Center will be at the center of first volume defined in union -> Step center
  // Last argument diplaces the body center relative to step center before joining
  G4UnionSolid* SolidScint8Inches = new G4UnionSolid("Step+MainBody",Step8Inches,MainBody8Inches,G4Transform3D(DontRotate,G4ThreeVector(0,0,Step_BodyUnion)));
  
  // ####################### COUPLING SPHERE #######################
G4int N_z_plane_glue_extrusion = 36;
G4double Phi_start = 0.*deg;
G4double Phi_end = 2*180*deg;

const G4double z_plane_glue_extrusion[] = {-110.*mm, -109.829*mm, -108.647*mm, -108.65*mm, -107.416*mm, -106.481*mm, -103.084*mm, -98.6401*mm, -43.6458*mm, -33.8076*mm, -27.3606*mm, -22.9623*mm, -18.0893*mm, -12.2671*mm, -5.65007*mm, 2.09728*mm, 10.6724*mm, 20.0733*mm, 29.9204*mm, 39.9182*mm, 49.8607*mm, 59.4345*mm, 68.3814*mm, 76.581*mm, 83.5154*mm, 89.9187*mm, 95.2918*mm, 99.6867*mm, 103.405*mm, 106.303*mm, 107.669*mm, 108.556*mm, 108.658*mm, 109.334*mm, 109.825*mm, 110.*mm};

const G4double r_inner_glue_extrusion[]= {0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm};


const G4double r_outer_glue_extrusion[]= {2.*mm, 34.0648*mm, 37.6705*mm, 37.6705*mm, 39.4533*mm, 40.4624*mm, 42.5955*mm, 43.5084*mm, 43.9043*mm, 47.2687*mm, 56.2783*mm, 65.2852*mm, 73.4126*mm, 81.2565*mm, 88.3265*mm, 94.3066*mm, 98.803*mm, 101.5055*mm, 102.905*mm, 102.846*mm, 101.3918*mm, 97.9735*mm, 92.7321*mm, 86.4727*mm, 78.9557*mm, 70.9368*mm, 62.0639*mm, 52.8971*mm, 43.1194*mm, 32.596*mm, 27.291*mm, 22.8952*mm, 22.3897*mm, 15.4348*mm, 10.53213*mm, 2.*mm};

  G4Polycone *GlueExtrusion = new G4Polycone("Glue extrusion",
			Phi_start,
			Phi_end,
			N_z_plane_glue_extrusion,
			z_plane_glue_extrusion,
			r_inner_glue_extrusion,
			r_outer_glue_extrusion);

   
  // ####################### FINAL BLOCK #######################
  // Subtract 2nd volume from the 1st 
  // SphereBlockTranslation = dist. from step center to center of sphere position where subtraction will occur

  G4SubtractionSolid* FinalScint8Inches = new G4SubtractionSolid("FinalScint_8inches",SolidScint8Inches,GlueExtrusion,G4Transform3D(Rotate,G4ThreeVector(0,0,+220.*mm)));



  
  // Logical volume for FinalScint: material = scintillator

  LogicalSNMW_8InchesFinalScint = new G4LogicalVolume(FinalScint8Inches, scintillator, "LogicalFinalScint8Inches",0,0,0);
  
  //G4VisAttributes * clear;
  //clear = new G4VisAttributes( G4Colour(255/255. ,255/255. ,255/255. ));

  return LogicalSNMW_8InchesFinalScint;  
}



// ***********************
// Builds Scintitllator Block
// ***********************
G4LogicalVolume *Scintillator::GetSNMW_8InchesScint_coinc(){
  // Material Properties for scint
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");  
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Plastic = scintProp->GetMaterial("plastic");
  Polystyrene = scintProp->GetMaterial("polysty");
  scintillator = scintProp->GetMaterial("scintillator");

  // Define some translations used for joining or subtracting
   Step_BodyUnion = SNMW_ScintStepHeight/2 + SNMW_ScintBodyHeight/2;
   Block_SphereSubtraction = SNMW_ScintStepHeight/2 + SNMW_ScintBodyHeight + SNMW_CouplingSphereRadius - SNMW_CouplingSphereDepth; // Center of union will be center of step


  // Define some rotations
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0*deg);
  G4RotationMatrix Rotate;
  Rotate.rotateX(180*deg);
  
  // ####################### SCINT BLOCK #######################
  // Scintillator Step
  G4Box *Step8Inches = new G4Box("Step8Inches",          
			  SNMW_ScintStepWidth/2, SNMW_ScintStepWidth/2, 
			  SNMW_ScintStepHeight/2);  

  // Scintillator Main Body
  G4Box *MainBody8Inches = new G4Box("MainBody8Inches",         
			      SNMW_ScintBodyWidth/2, SNMW_ScintBodyWidth/2,
			      SNMW_ScintBodyHeight/2); 
  
  // Union of two volumes: Center will be at the center of first volume defined in union -> Step center
  // Last argument diplaces the body center relative to step center before joining
  G4UnionSolid* SolidScint8Inches = new G4UnionSolid("Step+MainBody",Step8Inches,MainBody8Inches,G4Transform3D(DontRotate,G4ThreeVector(0,0,Step_BodyUnion)));
  
  // ####################### COUPLING SPHERE #######################
G4int N_z_plane_glue_extrusion = 36;
G4double Phi_start = 0.*deg;
G4double Phi_end = 2*180*deg;

const G4double z_plane_glue_extrusion[] = {-110.*mm, -109.829*mm, -108.647*mm, -108.65*mm, -107.416*mm, -106.481*mm, -103.084*mm, -98.6401*mm, -43.6458*mm, -33.8076*mm, -27.3606*mm, -22.9623*mm, -18.0893*mm, -12.2671*mm, -5.65007*mm, 2.09728*mm, 10.6724*mm, 20.0733*mm, 29.9204*mm, 39.9182*mm, 49.8607*mm, 59.4345*mm, 68.3814*mm, 76.581*mm, 83.5154*mm, 89.9187*mm, 95.2918*mm, 99.6867*mm, 103.405*mm, 106.303*mm, 107.669*mm, 108.556*mm, 108.658*mm, 109.334*mm, 109.825*mm, 110.*mm};

const G4double r_inner_glue_extrusion[]= {0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm};


const G4double r_outer_glue_extrusion[]= {2.*mm, 34.0648*mm, 37.6705*mm, 37.6705*mm, 39.4533*mm, 40.4624*mm, 42.5955*mm, 43.5084*mm, 43.9043*mm, 47.2687*mm, 56.2783*mm, 65.2852*mm, 73.4126*mm, 81.2565*mm, 88.3265*mm, 94.3066*mm, 98.803*mm, 101.5055*mm, 102.905*mm, 102.846*mm, 101.3918*mm, 97.9735*mm, 92.7321*mm, 86.4727*mm, 78.9557*mm, 70.9368*mm, 62.0639*mm, 52.8971*mm, 43.1194*mm, 32.596*mm, 27.291*mm, 22.8952*mm, 22.3897*mm, 15.4348*mm, 10.53213*mm, 2.*mm};

  G4Polycone *GlueExtrusion = new G4Polycone("Glue extrusion",
			Phi_start,
			Phi_end,
			N_z_plane_glue_extrusion,
			z_plane_glue_extrusion,
			r_inner_glue_extrusion,
			r_outer_glue_extrusion);

   
  // ####################### FINAL BLOCK #######################
  // Subtract 2nd volume from the 1st 
  // SphereBlockTranslation = dist. from step center to center of sphere position where subtraction will occur

  G4SubtractionSolid* FinalScint8Inches = new G4SubtractionSolid("FinalScint_8inches",SolidScint8Inches,GlueExtrusion,G4Transform3D(Rotate,G4ThreeVector(0,0,+220.*mm)));



  
  // Logical volume for FinalScint: material = scintillator

  LogicalSNMW_8InchesFinalScint_coinc = new G4LogicalVolume(FinalScint8Inches, Plastic, "LogicalFinalScint8Inches_coinc",0,0,0);
  
  //G4VisAttributes * clear;
  //clear = new G4VisAttributes( G4Colour(255/255. ,255/255. ,255/255. ));

  return LogicalSNMW_8InchesFinalScint_coinc;  
}










// ***********************
// Builds Mylar wrapping
// ***********************
G4LogicalVolume *Scintillator::GetSNMW_8InchesMylar(){
  // Define materials
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Mylar = scintProp->GetMaterial("mylar"); 
  
  // Define some translations used for joining or subtracting
  MylarStep_BodyUnion = (MylarThickness+AirGapMylar+SNMW_ScintStepHeight+AirGapTeflon+TeflonThickness+AirGapMylar+MylarThickness)/2 + (MylarThickness+AirGapMylar+SNMW_ScintBodyHeight-AirGapTeflon-TeflonThickness-AirGapMylar-MylarThickness)/2 ;
  TeflonStep_BodyUnion = (AirGapMylar+SNMW_ScintStepHeight+AirGapTeflon+TeflonThickness+AirGapMylar)/2 + (AirGapMylar+SNMW_ScintBodyHeight-AirGapTeflon-TeflonThickness-AirGapMylar)/2 ;
  //  G4double Mylar_TeflonSubtraction = (SNMW_ScintStepHeight+AirGapMylar+TeflonThickness+AirGapTeflon)/2 - SNMW_ScintStepHeight/2;
  Mylar_SphereSubtraction = MylarThickness+AirGapMylar+SNMW_ScintStepHeight+SNMW_ScintBodyHeight+SNMW_CouplingSphereRadius-SNMW_CouplingSphereDepth-(MylarThickness+AirGapMylar+SNMW_ScintStepHeight+AirGapTeflon+TeflonThickness+AirGapMylar+MylarThickness)/2;
   G4cout << "Mylar step + body union translation = " << MylarStep_BodyUnion 
          << "\nTeflon step + body union translation = " << TeflonStep_BodyUnion 
     //          << "\nMylar - teflon subtraction translation = " << Mylar_TeflonSubtraction    
          << "\nMylar - sphere subtraction translation = " << Mylar_SphereSubtraction << G4endl;  

  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0*deg);
  G4RotationMatrix Rotate;
  Rotate.rotateX(180*deg);
  
  // ####################### MYLAR SOLID #######################
  // Mylar step
  G4Box *MylarStep8Inches = new G4Box("MylarStep8Inches",
			       (MylarThickness+AirGapMylar+TeflonThickness+AirGapTeflon+SNMW_ScintStepWidth+AirGapTeflon+TeflonThickness+AirGapMylar+MylarThickness)/2, // 2 air gaps & Teflon & Mlyar thickness EACH side
			       (MylarThickness+AirGapMylar+TeflonThickness+AirGapTeflon+SNMW_ScintStepWidth+AirGapTeflon+TeflonThickness+AirGapMylar+MylarThickness)/2, 
			       (MylarThickness+AirGapMylar+SNMW_ScintStepHeight+AirGapTeflon+TeflonThickness+AirGapMylar+MylarThickness)/2);  // Bottom to top layers
  
  // Mylar Main Body
  G4Box *MylarMainBody8Inches = new G4Box("MylarMainBody8Inches",         
				   (MylarThickness+AirGapMylar+TeflonThickness+AirGapTeflon+SNMW_ScintBodyWidth+AirGapTeflon+TeflonThickness+AirGapMylar+MylarThickness)/2, 
				   (MylarThickness+AirGapMylar+TeflonThickness+AirGapTeflon+SNMW_ScintBodyWidth+AirGapTeflon+TeflonThickness+AirGapMylar+MylarThickness)/2,
				   (MylarThickness+AirGapMylar+SNMW_ScintBodyHeight-AirGapTeflon-TeflonThickness-AirGapMylar-MylarThickness)/2); //Now accounts for BOTH Teflon and Mylar and all air gaps: top to bottom
  
  // Union of two volumes: Just as with Teflon above
  G4UnionSolid* SolidMylar8Inches = new G4UnionSolid("MylarStep+MylarMainBody",MylarStep8Inches,MylarMainBody8Inches,G4Transform3D(DontRotate,G4ThreeVector(0,0,MylarStep_BodyUnion)));
  
  // Easy way again is to subtract a Teflon solid (and air gaps)... but we need to remake it
  // ####################### TEFLON SOLID #######################
  // Teflon step
  G4Box *TeflonStep8Inches = new G4Box("TeflonStep8Inches",
				(AirGapMylar+TeflonThickness+AirGapTeflon+SNMW_ScintStepWidth+AirGapTeflon+TeflonThickness+AirGapMylar)/2, 
				(AirGapMylar+TeflonThickness+AirGapTeflon+SNMW_ScintStepWidth+AirGapTeflon+TeflonThickness+AirGapMylar)/2, 
				(AirGapMylar+SNMW_ScintStepHeight+AirGapTeflon+TeflonThickness+AirGapMylar)/2);  // Teflon doesn't cover top and bottom BUT needs to cover top of step
  
  // Teflon Main Body
  G4Box *TeflonMainBody8Inches = new G4Box("TeflonMainBody8Inches",         
				    (AirGapMylar+TeflonThickness+AirGapTeflon+SNMW_ScintBodyWidth+AirGapTeflon+TeflonThickness+AirGapMylar)/2, 
				    (AirGapMylar+TeflonThickness+AirGapTeflon+SNMW_ScintBodyWidth+AirGapTeflon+TeflonThickness+AirGapMylar)/2, 
				    (AirGapMylar+SNMW_ScintBodyHeight-AirGapTeflon-TeflonThickness-AirGapMylar)/2);  
  
  // Union of two Teflon volumes
  G4UnionSolid* SolidTeflon8Inches = new G4UnionSolid("TeflonStep+TeflonMainBody",TeflonStep8Inches,TeflonMainBody8Inches,G4Transform3D(DontRotate,G4ThreeVector(0,0,TeflonStep_BodyUnion)));
  
  // ####################### MYLAR WRAP #######################
  // Now we just need to subract the Teflon to leave the Mylar wrap on all sides
  G4SubtractionSolid* MylarWrap8Inches = new G4SubtractionSolid("MylarWrap8Inches", 
							 SolidMylar8Inches,
							 SolidTeflon8Inches,
							 //							 G4Transform3D(Rotate,G4ThreeVector(0,0,Mylar_TeflonSubtraction)));
							 G4Transform3D(DontRotate,G4ThreeVector(0,0,0)));

  // Lastly, since there is Mylar on top we need to subtract the coupling sphere to leave an opening for the PMT
G4int N_z_plane_glue_extrusion = 36;
G4double Phi_start = 0.*deg;
G4double Phi_end = 2*180*deg;

const G4double z_plane_glue_extrusion[] = {-110.*mm, -109.829*mm, -108.647*mm, -108.65*mm, -107.416*mm, -106.481*mm, -103.084*mm, -98.6401*mm, -43.6458*mm, -33.8076*mm, -27.3606*mm, -22.9623*mm, -18.0893*mm, -12.2671*mm, -5.65007*mm, 2.09728*mm, 10.6724*mm, 20.0733*mm, 29.9204*mm, 39.9182*mm, 49.8607*mm, 59.4345*mm, 68.3814*mm, 76.581*mm, 83.5154*mm, 89.9187*mm, 95.2918*mm, 99.6867*mm, 103.405*mm, 106.303*mm, 107.669*mm, 108.556*mm, 108.658*mm, 109.334*mm, 109.825*mm, 110.*mm};

const G4double r_inner_glue_extrusion[]= {0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm};


const G4double r_outer_glue_extrusion[]= {2.*mm, 34.0648*mm, 37.6705*mm, 37.6705*mm, 39.4533*mm, 40.4624*mm, 42.5955*mm, 43.5084*mm, 43.9043*mm, 47.2687*mm, 56.2783*mm, 65.2852*mm, 73.4126*mm, 81.2565*mm, 88.3265*mm, 94.3066*mm, 98.803*mm, 101.5055*mm, 102.905*mm, 102.846*mm, 101.3918*mm, 97.9735*mm, 92.7321*mm, 86.4727*mm, 78.9557*mm, 70.9368*mm, 62.0639*mm, 52.8971*mm, 43.1194*mm, 32.596*mm, 27.291*mm, 22.8952*mm, 22.3897*mm, 15.4348*mm, 10.53213*mm, 2.*mm};

  G4Polycone *GlueExtrusion = new G4Polycone("Glue extrusion",
			Phi_start,
			Phi_end,
			N_z_plane_glue_extrusion,
			z_plane_glue_extrusion,
			r_inner_glue_extrusion,
			r_outer_glue_extrusion);

  // Subtracting the coupling sphere
  G4SubtractionSolid* FinalMylarWrap8Inches = new G4SubtractionSolid("FinalMylarWrap8Inches", MylarWrap8Inches,GlueExtrusion,G4Transform3D(Rotate,G4ThreeVector(0,0,+219.706*mm)));
  
  // Logical volume for Mylar: material = coating?
  
  LogicalSNMW_8InchesMylarWrap = new G4LogicalVolume(FinalMylarWrap8Inches, Mylar, "LogicalMylarWrap8inches",0,0,0);
  
  
  return LogicalSNMW_8InchesMylarWrap;
}







//************************************************************************************
//************************************************************************************
//********************************* NEMO3 EC (N3EC) **********************************
//************************************************************************************
//************************************************************************************



//************************************************************************************
//************************************************************************************
//************************* SCINTILLATOR NEMO3 ***************************************
//************************************************************************************
//************************************************************************************

G4LogicalVolume *Scintillator::GetN3EC_Scintillator(){

//Define the materials
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Plastic = scintProp->GetMaterial("plastic");
  Polystyrene = scintProp->GetMaterial("polysty");
  scintillator = scintProp->GetMaterial("scintillator");


//**********************************************************************************
//Variables declaration and link between them **************************************
//**********************************************************************************

   // N3EC_ScintWidth = theScint->GetN3EC_ScintWidth();
   // N3EC_ScintHeight = theScint->GetN3EC_ScintHeight();


    G4RotationMatrix DontRotate;
    DontRotate.rotateX(0*deg);

    G4RotationMatrix Rotate;
    Rotate.rotateX(180*deg);


//**********************************************************************************
//Definition of volume**************************************************************
//**********************************************************************************


  G4Box *Scintillator_NEMO= new G4Box("Scintillator_NEMO",          
                  N3EC_ScintWidth/2, N3EC_ScintWidth/2,        
                   N3EC_ScintHeight/2);
                 




    LogicalN3EC_Scintillator = new G4LogicalVolume(Scintillator_NEMO, scintillator, "LogicalN3EC_Scintillator",0,0,0);


    return LogicalN3EC_Scintillator;


}





//************************************************************************************
//************************************************************************************
//****************************** TEFLON NEMO3 ****************************************
//************************************************************************************
//************************************************************************************

G4LogicalVolume *Scintillator::GetN3EC_Teflon(){

//Define the materials
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Teflon = scintProp->GetMaterial("teflon");
  Polystyrene = scintProp->GetMaterial("polysty");
  scintillator = scintProp->GetMaterial("scintillator");


//**********************************************************************************
//Variables declaration and link between them **************************************
//**********************************************************************************

    //width_block_NEMO = thestrip->GetWidthBlockNEMO();
    //height_block_NEMO = thestrip->GetHeightBlockNEMO();
    //thickness_Teflon = thestrip->GetThicknessTeflon();
    //gap_Teflon = thestrip->GetGapTeflon();


    G4RotationMatrix DontRotate;
    DontRotate.rotateX(0*deg);

    G4RotationMatrix Rotate;
    Rotate.rotateX(180*deg);


//**********************************************************************************
//Definition of volume**************************************************************
//**********************************************************************************

//###################### Creation Teflon's Volume ###############################
  G4Box *Teflon_Full= new G4Box("Teflon_Full",          
                  N3EC_ScintWidth/2 + AirGapTeflon + TeflonThickness,
		  N3EC_ScintWidth/2 + AirGapTeflon + TeflonThickness,        
                  N3EC_ScintHeight/2);


//#################### Creation Scintillator's Volume for the subtraction ##################
  G4Box *Scintillator_Subtraction= new G4Box("Scintillator_Subtraction",          
                  N3EC_ScintWidth/2 + AirGapTeflon, // with the gap because we want to obtain just the Teflon with an air gap
		  N3EC_ScintWidth/2 + AirGapTeflon,        
                  N3EC_ScintHeight/2);
                 

//################### Subtraction to obtain layer of Teflon ############################
 G4SubtractionSolid* Teflon_NEMO = new G4SubtractionSolid("Teflon_NEMO", Teflon_Full,Scintillator_Subtraction,G4Transform3D(DontRotate,G4ThreeVector(0,0,0)));


    LogicalN3EC_TeflonWrap= new G4LogicalVolume(Teflon_NEMO, Teflon, "LogicalN3EC_TeflonWrap",0,0,0);


    return LogicalN3EC_TeflonWrap;


}



//************************************************************************************
//************************************************************************************
//******************************* MYLAR NEMO3 *****************************************
//************************************************************************************
//************************************************************************************

G4LogicalVolume *Scintillator::GetN3EC_Mylar(){

//Define the materials
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Mylar = scintProp->GetMaterial("mylar");
  Polystyrene = scintProp->GetMaterial("polysty");
  scintillator = scintProp->GetMaterial("scintillator");


//**********************************************************************************
//Variables declaration and link between them **************************************
//**********************************************************************************

    //width_block_NEMO = thestrip->GetWidthBlockNEMO();
    //height_block_NEMO = thestrip->GetHeightBlockNEMO();
    //thickness_Teflon = thestrip->GetThicknessTeflon();
    //gap_Teflon = thestrip->GetGapTeflon();
    //thickness_Mylar = thestrip->GetThicknessMylar();
    //gap_Mylar = thestrip->GetGapMylar();

    //radius_coupling = thestrip->GetRadiusCoupling();
    //height_coupling = thestrip->GetHeightCoupling();



    G4RotationMatrix DontRotate;
    DontRotate.rotateX(0*deg);

    G4RotationMatrix Rotate;
    Rotate.rotateX(180*deg);


//**********************************************************************************
//Definition of volume**************************************************************
//**********************************************************************************

//###################### Creation Mylar's Volume ###############################
  G4Box *Mylar_Full= new G4Box("Mylar_Full",          
                  N3EC_ScintWidth/2 + AirGapTeflon + TeflonThickness + AirGapMylar + MylarThickness,
		  N3EC_ScintWidth/2 + AirGapTeflon + TeflonThickness + AirGapMylar + MylarThickness,        
                  N3EC_ScintHeight/2 + AirGapMylar + MylarThickness);
                 
//#################### Creation Teflon's Volume for the subtraction ##################
  G4Box *Teflon_Subtraction= new G4Box("Teflon_Subtraction",          
                  N3EC_ScintWidth/2 + AirGapTeflon + TeflonThickness + AirGapMylar,  // with the gap because we want to obtain just the Teflon with an air gap
		  N3EC_ScintWidth/2 + AirGapTeflon + TeflonThickness + AirGapMylar,        
                  N3EC_ScintHeight/2 + AirGapMylar);

//################### Subtraction to obtain layer of Mylar ############################
 G4SubtractionSolid* Mylar_NEMO_Full = new G4SubtractionSolid("Mylar_NEMO_Full", Mylar_Full, Teflon_Subtraction, G4Transform3D(DontRotate,G4ThreeVector(0,0,0)));


//###################### Creation Coupling's Volume for the subtraction###############################
G4Tubs *Coupling;
Coupling = new G4Tubs("Coupling",
             0.*mm,   //G4double  pRMin
             N3EC_LightGuideCouplingRadius,    //G4double  pRMax
             N3EC_LightGuideCouplingHeight/2,    //G4double  pDz
             0.,    //G4double  pSPhi
             2*pi*rad);   //G4double  pDPhi


//################### Subtraction to obtain layer of Mylar without a part on the top for the coupling with the light guide ############################
 G4SubtractionSolid* Mylar_NEMO = new G4SubtractionSolid("Mylar_NEMO", Mylar_NEMO_Full, Coupling, G4Transform3D(DontRotate,G4ThreeVector(0,0,N3EC_ScintHeight/2 + N3EC_LightGuideCouplingHeight/2)));



    LogicalN3EC_MylarWrap = new G4LogicalVolume(Mylar_NEMO, Mylar, "LogicalN3EC_MylarWrap",0,0,0);


    return LogicalN3EC_MylarWrap;


}




//************************************************************************************
//************************************************************************************
//***************************LIGHT GUIDE NEMO3 ***************************************
//************************************************************************************
//************************************************************************************

G4LogicalVolume *Scintillator::GetN3EC_LightGuide(){


//Define the materials
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  PMMA = scintProp->GetMaterial("PMMA");

//**********************************************************************************
//Variables declaration and link between them **************************************
//**********************************************************************************
  /*radius_light_guide_min = thestrip->GetRadiusLightGuideMin();
  radius_light_guide_max = thestrip->GetRadiusLightGuideMax();
  height_light_guide = thestrip->GetHeightLightGuide();
  height_step_mechanical_support = thestrip->GetHeightStepMechanicalSupport();
  radius_glue_NEMO = thestrip->GetRadiusGlueNEMO();
*/

  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);
  
  G4RotationMatrix Flip;
  Flip.rotateX(180.0*deg);


//**********************************************************************************
//Definition of volume**************************************************************
//**********************************************************************************

G4Tubs *Light_Guide_Full;
Light_Guide_Full = new G4Tubs("Light_Guide_Full",
             0.*mm,   //G4double  pRMin
             N3EC_LightGuideRadius,    //G4double  pRMax
             N3EC_LightGuideHeight/2,    //G4double  pDz
             0.,    //G4double  pSPhi
             2*pi*rad);   //G4double  pDPhi


G4Tubs *Light_Guide_Step;                             // For the part subtract for the mecanical support
Light_Guide_Step = new G4Tubs("Light_Guide_Step",
             0.,   //G4double  pRMin
             N3EC_LightGuideCouplingRadius,    //G4double  pRMax
             N3EC_LightGuideStepHeight/2,    //G4double  pDz
             0.,    //G4double  pSPhi
             2*pi*rad);   //G4double  pDPhi



  G4UnionSolid* Light_Guide_NEMO = new G4UnionSolid("Light_Guide_NEMO", Light_Guide_Full, Light_Guide_Step,G4Transform3D(DontRotate,G4ThreeVector(0, 0,-(N3EC_LightGuideHeight + N3EC_LightGuideStepHeight)/2)));      

 

  LogicalN3EC_LightGuide = new G4LogicalVolume(Light_Guide_NEMO,PMMA,"LogicalN3EC_LightGuide",0,0,0);
  

    
  return LogicalN3EC_LightGuide;

  
}







//************************************************************************************
//************************************************************************************
//***************************WRAP LIGHT GUIDE NEMO3 **********************************
//************************************************************************************
//************************************************************************************

G4LogicalVolume *Scintillator::GetN3EC_WrapLightGuide(){


  theScint = new Scintillator(path_bin+"OpticalModule.cfg");
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Mylar = scintProp->GetMaterial("mylar");

//**********************************************************************************
//Variables declaration and link between them **************************************
//**********************************************************************************
/*  radius_light_guide_max = thestrip->GetRadiusLightGuideMax();
  radius_light_guide_min = thestrip->GetRadiusLightGuideMin();
  height_light_guide = thestrip->GetHeightLightGuide();
  height_step_mechanical_support = thestrip->GetHeightStepMechanicalSupport();
  height_interface_light_guide = thestrip->GetHeightInterfaceLightGuide();
  thickness_Mylar = thestrip->GetThicknessMylar();
  gap_Mylar = thestrip->GetGapMylar();
*/

  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);
  
  G4RotationMatrix Flip;
  Flip.rotateX(180.0*deg);


//**********************************************************************************
//Definition of volume**************************************************************
//**********************************************************************************

//Creation of light guide volume

G4Tubs *Light_Guide_Full;
Light_Guide_Full = new G4Tubs("Light_Guide_Full",
             0.*mm,   //G4double  pRMin
             N3EC_LightGuideRadius + AirGapMylar,    //G4double  pRMax
             N3EC_LightGuideHeight/2,    //G4double  pDz
             0.,    //G4double  pSPhi
             2*pi*rad);   //G4double  pDPhi


G4Tubs *Light_Guide_Step;                             // For the part subtract for the mecanical support
Light_Guide_Step = new G4Tubs("Light_Guide_Step",
             0.,   //G4double  pRMin
             N3EC_LightGuideCouplingRadius + AirGapMylar,    //G4double  pRMax
             N3EC_LightGuideStepHeight/2,    //G4double  pDz
             0.,    //G4double  pSPhi
             2*pi*rad);   //G4double  pDPhi



  G4UnionSolid* Light_Guide_NEMO = new G4UnionSolid("Light_Guide_NEMO", Light_Guide_Full, Light_Guide_Step,G4Transform3D(DontRotate,G4ThreeVector(0, 0,-(N3EC_LightGuideHeight + N3EC_LightGuideStepHeight)/2)));  



//Creation of Wrap light guide volume

G4Tubs *Wrap_Light_Guide_Full;
Wrap_Light_Guide_Full = new G4Tubs("Wrap_Light_Guide_Full",
             0.*mm,  
             N3EC_LightGuideRadius + AirGapMylar + MylarThickness,    
             (N3EC_LightGuideHeight + AirGapMylar + MylarThickness)/2,   
             0.,    
             twopi*rad); 


G4Tubs *Wrap_Light_Guide_Step;
Wrap_Light_Guide_Step = new G4Tubs("Wrap_Light_Guide_Step",
             0.,  
             N3EC_LightGuideCouplingRadius + AirGapMylar + MylarThickness,    
             (N3EC_LightGuideStepHeight - AirGapMylar - MylarThickness)/2,   
             0.,    
             twopi*rad); 


  G4UnionSolid* Wrap_Full_Light_Guide_NEMO = new G4UnionSolid("Wrap_Full_Light_Guide_NEMO", Wrap_Light_Guide_Full, Wrap_Light_Guide_Step,G4Transform3D(DontRotate,G4ThreeVector(0, 0,-(N3EC_LightGuideHeight + N3EC_LightGuideStepHeight)/2)));      


//Subtraction to obtain just the wrap around the light guide

  G4SubtractionSolid* Wrap_Subtraction_Light_Guide_NEMO = new G4SubtractionSolid("Wrap_Subtraction_Light_Guide_NEMO", Wrap_Full_Light_Guide_NEMO, Light_Guide_NEMO,G4Transform3D(DontRotate,G4ThreeVector(0, 0, 0)));  



//Subtraction to obtain wrap around the light guide without the part for the interface light guide    


G4Tubs *Interface_Light_Guide_Subtraction;
Interface_Light_Guide_Subtraction = new G4Tubs("Interface_Light_Guide_Subtraction",
             0.*mm,  
             N3EC_LightGuideCouplingRadius + AirGapMylar + MylarThickness,    
             N3EC_InterfaceLightGuideHeight/2,   
             0.,    
             twopi*rad); 

  G4SubtractionSolid* Wrap_Light_Guide_NEMO = new G4SubtractionSolid("Wrap_Light_Guide_NEMO", Wrap_Subtraction_Light_Guide_NEMO, Interface_Light_Guide_Subtraction,G4Transform3D(DontRotate,G4ThreeVector(0, 0, (N3EC_LightGuideHeight + N3EC_InterfaceLightGuideHeight)/2)));  
 
   
  LogicalN3EC_WrapLightGuide = new G4LogicalVolume(Wrap_Light_Guide_NEMO,Mylar,"LogicalN3EC_WrapLightGuide",0,0,0);
  

    
  return LogicalN3EC_WrapLightGuide;

  
}




//************************************************************************************
//************************************************************************************
//********************** INTERFACE LIGHT GUIDE NEMO3 *********************************
//************************************************************************************
//************************************************************************************

G4LogicalVolume *Scintillator::GetN3EC_InterfaceLightGuide(){


  theScint = new Scintillator(path_bin+"OpticalModule.cfg");
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  PMMA = scintProp->GetMaterial("PMMA");

//**********************************************************************************
//Variables declaration and link between them **************************************
//**********************************************************************************
/*  radius_light_guide_min = thestrip->GetRadiusLightGuideMin();
  radius_light_guide_max = thestrip->GetRadiusLightGuideMax();
  height_interface_light_guide = thestrip->GetHeightInterfaceLightGuide();
  radius_glue_NEMO = thestrip->GetRadiusGlueNEMO();
  depth_bowl_NEMO = thestrip->GetDepthBowlNEMO();
*/

  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);
  
  G4RotationMatrix Flip;
  Flip.rotateX(180.0*deg);


//**********************************************************************************
//Definition of volume**************************************************************
//**********************************************************************************

G4Tubs *Interface_Light_Guide_Full;
Interface_Light_Guide_Full = new G4Tubs("Interface_Light_Guide_Full",
             0.*mm,   //G4double  pRMin
             N3EC_LightGuideCouplingRadius,    //G4double  pRMax
             N3EC_InterfaceLightGuideHeight/2,    //G4double  pDz
             0.,    //G4double  pSPhi
             2*pi*rad);   //G4double  pDPhi


G4Sphere *Sphere_subtraction;
  Sphere_subtraction = new G4Sphere("Sphere_subtraction",
               0.,
               N3EC_CouplingSphereRadius,
               0.,
               twopi*rad ,
               0.,
               0.35*pi*rad) ;



  G4SubtractionSolid* Interface_Light_Guide_NEMO = new G4SubtractionSolid("Interface_Light_Guide_NEMO", Interface_Light_Guide_Full, Sphere_subtraction,G4Transform3D(Flip,G4ThreeVector(0, 0,N3EC_InterfaceLightGuideHeight/2 + N3EC_CouplingSphereRadius - N3EC_CouplingSphereDepth)));      

 

  LogicalN3EC_InterfaceLightGuide = new G4LogicalVolume(Interface_Light_Guide_NEMO,PMMA,"LogicalN3EC_InterfaceLightGuide",0,0,0);
  

    
  return LogicalN3EC_InterfaceLightGuide;

  
}





//************************************************************************************
//************************************************************************************
//********************** WRAP INTERFACE LIGHT GUIDE NEMO3 ****************************
//************************************************************************************
//************************************************************************************

G4LogicalVolume *Scintillator::GetN3EC_WrapInterfaceLightGuide(){


  theScint = new Scintillator(path_bin+"OpticalModule.cfg");
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Mylar = scintProp->GetMaterial("mylar");

//**********************************************************************************
//Variables declaration and link between them **************************************
//**********************************************************************************
  /*radius_light_guide_min = thestrip->GetRadiusLightGuideMin();
  radius_light_guide_max = thestrip->GetRadiusLightGuideMax();
  height_interface_light_guide = thestrip->GetHeightInterfaceLightGuide();
  radius_glue_NEMO = thestrip->GetRadiusGlueNEMO();
  depth_bowl_NEMO = thestrip->GetDepthBowlNEMO();
  thickness_Mylar = thestrip->GetThicknessMylar();
  gap_Mylar = thestrip->GetGapMylar();
*/

  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);
  
  G4RotationMatrix Flip;
  Flip.rotateX(180.0*deg);


//**********************************************************************************
//Definition of volume**************************************************************
//**********************************************************************************

G4Tubs *Interface_Light_Guide_Full;
Interface_Light_Guide_Full = new G4Tubs("Interface_Light_Guide_Full",
             0.*mm,   //G4double  pRMin
             N3EC_LightGuideCouplingRadius + AirGapMylar,    //G4double  pRMax
             N3EC_InterfaceLightGuideHeight/2,    //G4double  pDz
             0.,    //G4double  pSPhi
             2*pi*rad);   //G4double  pDPhi



G4Tubs *Interface_Light_Guide_Wrap_Full;
Interface_Light_Guide_Wrap_Full = new G4Tubs("Interface_Light_Guide_Wrap_Full",
             0.*mm,   //G4double  pRMin
             N3EC_LightGuideCouplingRadius + AirGapMylar + MylarThickness,    //G4double  pRMax
             N3EC_InterfaceLightGuideHeight/2,    //G4double  pDz
             0.,    //G4double  pSPhi
             2*pi*rad);   //G4double  pDPhi


  G4SubtractionSolid* Interface_Light_Guide_NEMO_Full = new G4SubtractionSolid("Interface_Light_Guide_NEMO_Full", Interface_Light_Guide_Wrap_Full, Interface_Light_Guide_Full,G4Transform3D(Flip,G4ThreeVector(0, 0, 0)));    



G4Sphere *Sphere_subtraction;
  Sphere_subtraction = new G4Sphere("Sphere_subtraction",
               0.,
               N3EC_CouplingSphereRadius,
               0.,
               twopi*rad ,
               0.,
               0.35*pi*rad) ;



  G4SubtractionSolid* Interface_Light_Guide_NEMO = new G4SubtractionSolid("Interface_Light_Guide_NEMO", Interface_Light_Guide_NEMO_Full, Sphere_subtraction,G4Transform3D(Flip,G4ThreeVector(0, 0, N3EC_InterfaceLightGuideHeight/2 + N3EC_CouplingSphereRadius - N3EC_CouplingSphereDepth)));      

 
   
  LogicalN3EC_WrapInterfaceLightGuide = new G4LogicalVolume(Interface_Light_Guide_NEMO,Mylar,"LogicalN3EC_WrapInterfaceLightGuide",0,0,0);
  

    
  return LogicalN3EC_WrapInterfaceLightGuide;

  
}




//************************************************************************************
//************************************************************************************
//***************************** SUPERNEMO XWALL (SNXW) *******************************
//************************************************************************************
//************************************************************************************




//************************************************************************************
//************************************************************************************
//*****************************SCINTILLATOR XWALL*************************************
//************************************************************************************
//************************************************************************************

G4LogicalVolume *Scintillator::GetSNXW_Scintillator(){

//Define the materials
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Plastic = scintProp->GetMaterial("plastic");
  Polystyrene = scintProp->GetMaterial("polysty");
  scintillator = scintProp->GetMaterial("scintillator");


//**********************************************************************************
//Variables declaration and link between them **************************************
//**********************************************************************************


    G4RotationMatrix DontRotate;
    DontRotate.rotateX(0*deg);

    G4RotationMatrix Rotate;
    Rotate.rotateX(180*deg);


//**********************************************************************************
//Definition of volume**************************************************************
//**********************************************************************************


  G4Box *Scintillator_XWALL= new G4Box("Scintillator_XWALL",          
                  SNXW_ScintWidth/2, SNXW_ScintWidth/2,        
                   SNXW_ScintHeight/2);
                 




    LogicalSNXW_Scintillator = new G4LogicalVolume(Scintillator_XWALL, scintillator, "LogicalSNXW_Scintillator",0,0,0);


    return LogicalSNXW_Scintillator;


}





//************************************************************************************
//************************************************************************************
//********************************** TEFLON XWALL ************************************
//************************************************************************************
//************************************************************************************

G4LogicalVolume *Scintillator::GetSNXW_Teflon(){

//Define the materials
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Teflon = scintProp->GetMaterial("teflon");
  Polystyrene = scintProp->GetMaterial("polysty");
  scintillator = scintProp->GetMaterial("scintillator");


//**********************************************************************************
//Variables declaration and link between them **************************************
//**********************************************************************************

    G4RotationMatrix DontRotate;
    DontRotate.rotateX(0*deg);

    G4RotationMatrix Rotate;
    Rotate.rotateX(180*deg);


//**********************************************************************************
//Definition of volume**************************************************************
//**********************************************************************************

//###################### Creation Teflon's Volume ###############################
  G4Box *Teflon_Full= new G4Box("Teflon_Full",          
                  SNXW_ScintWidth/2 + AirGapTeflon + TeflonThickness,
		  SNXW_ScintWidth/2 + AirGapTeflon + TeflonThickness,        
                  SNXW_ScintHeight/2);


//#################### Creation Scintillator's Volume for the subtraction ##################
  G4Box *Scintillator_Subtraction= new G4Box("Scintillator_Subtraction",          
                  SNXW_ScintWidth/2 + AirGapTeflon, // with the gap because we want to obtain just the Teflon with an air gap
		  SNXW_ScintWidth/2 + AirGapTeflon,        
                  SNXW_ScintHeight/2);
                 

//################### Subtraction to obtain layer of Teflon ############################
 G4SubtractionSolid* Teflon_XWALL = new G4SubtractionSolid("Teflon_XWALL", Teflon_Full,Scintillator_Subtraction,G4Transform3D(DontRotate,G4ThreeVector(0,0,0)));


    LogicalSNXW_TeflonWrap= new G4LogicalVolume(Teflon_XWALL, Teflon, "LogicalSNXW_TeflonWrap",0,0,0);


    return LogicalSNXW_TeflonWrap;


}



//************************************************************************************
//************************************************************************************
//*********************************** MYLAR XWALL ************************************
//************************************************************************************
//************************************************************************************

G4LogicalVolume *Scintillator::GetSNXW_Mylar(){

//Define the materials
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Mylar = scintProp->GetMaterial("mylar");
  Polystyrene = scintProp->GetMaterial("polysty");
  scintillator = scintProp->GetMaterial("scintillator");


//**********************************************************************************
//Variables declaration and link between them **************************************
//**********************************************************************************

    G4RotationMatrix DontRotate;
    DontRotate.rotateX(0*deg);

    G4RotationMatrix Rotate;
    Rotate.rotateX(180*deg);


//**********************************************************************************
//Definition of volume**************************************************************
//**********************************************************************************

//###################### Creation Mylar's Volume ###############################
  G4Box *Mylar_Full= new G4Box("Mylar_Full",          
                  SNXW_ScintWidth/2 + AirGapTeflon + TeflonThickness + AirGapMylar + MylarThickness,
		  SNXW_ScintWidth/2 + AirGapTeflon + TeflonThickness + AirGapMylar + MylarThickness,        
                  SNXW_ScintHeight/2 + AirGapMylar + MylarThickness);
                 
//#################### Creation Teflon's Volume for the subtraction ##################
  G4Box *Teflon_Subtraction= new G4Box("Teflon_Subtraction",          
                  SNXW_ScintWidth/2 + AirGapTeflon + TeflonThickness + AirGapMylar,  // with the gap because we want to obtain just the Teflon with an air gap
		  SNXW_ScintWidth/2 + AirGapTeflon + TeflonThickness + AirGapMylar,        
                  SNXW_ScintHeight/2 + AirGapMylar);

//################### Subtraction to obtain layer of Mylar ############################
 G4SubtractionSolid* Mylar_XWALL_Full = new G4SubtractionSolid("Mylar_XWALL_Full", Mylar_Full, Teflon_Subtraction, G4Transform3D(DontRotate,G4ThreeVector(0,0,0)));


//###################### Creation Coupling's Volume for the subtraction###############################
G4Tubs *Coupling;
Coupling = new G4Tubs("Coupling",
             0.*mm,   //G4double  pRMin
             SNXW_LightGuideRadius,    //G4double  pRMax
             SNXW_LightGuideCouplingHeight/2,    //G4double  pDz
             0.,    //G4double  pSPhi
             2*pi*rad);   //G4double  pDPhi


//################### Subtraction to obtain layer of Mylar without a part on the top for the coupling with the light guide ############################
 G4SubtractionSolid* Mylar_XWALL = new G4SubtractionSolid("Mylar_XWALL", Mylar_XWALL_Full, Coupling, G4Transform3D(DontRotate,G4ThreeVector(0,0,SNXW_ScintHeight/2 + SNXW_LightGuideCouplingHeight/2)));



    LogicalSNXW_MylarWrap = new G4LogicalVolume(Mylar_XWALL, Mylar, "LogicalSNXW_MylarWrap",0,0,0);


    return LogicalSNXW_MylarWrap;


}






//************************************************************************************
//************************************************************************************
//***************************** LIGHT GUIDE XWALL ************************************
//************************************************************************************
//************************************************************************************

G4LogicalVolume *Scintillator::GetSNXW_LightGuide(){


  theScint = new Scintillator(path_bin+"OpticalModule.cfg");
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  PMMA = scintProp->GetMaterial("PMMA");

//**********************************************************************************
//Variables declaration and link between them **************************************
//**********************************************************************************

  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);
  
  G4RotationMatrix Flip;
  Flip.rotateX(180.0*deg);


//**********************************************************************************
//Definition of volume**************************************************************
//**********************************************************************************

G4Tubs *Light_Guide_Full;
Light_Guide_Full = new G4Tubs("Light_Guide_Full",
             0.*mm,   //G4double  pRMin
             SNXW_LightGuideRadius,    //G4double  pRMax
             SNXW_LightGuideHeight/2,    //G4double  pDz
             0.,    //G4double  pSPhi
             2*pi*rad);   //G4double  pDPhi


G4Sphere *Sphere_subtraction;
  Sphere_subtraction = new G4Sphere("Sphere_subtraction",
               0.,
               SNXW_CouplingSphereRadius,
               0.,
               twopi*rad ,
               0.,
               0.35*pi*rad) ;



  G4SubtractionSolid* Light_Guide_XWALL = new G4SubtractionSolid("Light_Guide_XWALL", Light_Guide_Full, Sphere_subtraction,G4Transform3D(Flip,G4ThreeVector(0, 0,SNXW_LightGuideHeight/2 + SNXW_CouplingSphereRadius - SNXW_CouplingSphereDepth)));      

 

  LogicalSNXW_LightGuide = new G4LogicalVolume(Light_Guide_XWALL,PMMA,"LogicalSNXW_LightGuide",0,0,0);
  

    
  return LogicalSNXW_LightGuide;

  
}





//************************************************************************************
//************************************************************************************
//****************************** WRAP LIGHT GUIDE XWALL ******************************
//************************************************************************************
//************************************************************************************

G4LogicalVolume *Scintillator::GetSNXW_WrapLightGuide(){


  theScint = new Scintillator(path_bin+"OpticalModule.cfg");
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Mylar = scintProp->GetMaterial("mylar");

//**********************************************************************************
//Variables declaration and link between them **************************************
//**********************************************************************************

  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);
  
  G4RotationMatrix Flip;
  Flip.rotateX(180.0*deg);


//**********************************************************************************
//Definition of volume**************************************************************
//**********************************************************************************

G4Tubs *Light_Guide_Full;
Light_Guide_Full = new G4Tubs("Light_Guide_Full",
             0.*mm,   //G4double  pRMin
             SNXW_LightGuideRadius + AirGapMylar,    //G4double  pRMax
             SNXW_LightGuideHeight/2,    //G4double  pDz
             0.,    //G4double  pSPhi
             2*pi*rad);   //G4double  pDPhi



G4Tubs *Light_Guide_Wrap_Full;
Light_Guide_Wrap_Full = new G4Tubs("Light_Guide_Wrap_Full",
             0.*mm,   //G4double  pRMin
             SNXW_LightGuideRadius + AirGapMylar + MylarThickness,    //G4double  pRMax
             SNXW_LightGuideHeight/2,    //G4double  pDz
             0.,    //G4double  pSPhi
             2*pi*rad);   //G4double  pDPhi


  G4SubtractionSolid* Wrap_Light_Guide_XWALL_Full = new G4SubtractionSolid("Wrap_Light_Guide_XWALL_Full", Light_Guide_Wrap_Full, Light_Guide_Full,G4Transform3D(Flip,G4ThreeVector(0, 0, 0)));    



G4Sphere *Sphere_subtraction;
  Sphere_subtraction = new G4Sphere("Sphere_subtraction",
               0.,
               SNXW_CouplingSphereRadius,
               0.,
               twopi*rad ,
               0.,
               0.35*pi*rad) ;



  G4SubtractionSolid* Wrap_Light_Guide_XWALL = new G4SubtractionSolid("Wrap_Light_Guide_XWALL", Wrap_Light_Guide_XWALL_Full, Sphere_subtraction,G4Transform3D(Flip,G4ThreeVector(0, 0, SNXW_LightGuideHeight/2 + SNXW_CouplingSphereRadius - SNXW_CouplingSphereDepth)));      

 
   
  LogicalSNXW_WrapLightGuide = new G4LogicalVolume(Wrap_Light_Guide_XWALL,Mylar,"LogicalSNXW_WrapLightGuide",0,0,0);
  

    
  return LogicalSNXW_WrapLightGuide;

  
}


//************************************************************************************
//************************************************************************************
//************************ NEMO3 DIRECT COUPLING (N3DC) ******************************
//************************************************************************************
//************************************************************************************



//************************************************************************************
//************************************************************************************
//***************** SCINTILLATOR NEMO3 DIRECT COUPLING (N3DC) ************************
//************************************************************************************
//************************************************************************************

G4LogicalVolume *Scintillator::GetN3DC_Scintillator(){

//Define the materials
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Plastic = scintProp->GetMaterial("plastic");
  Polystyrene = scintProp->GetMaterial("polysty");
  scintillator = scintProp->GetMaterial("scintillator");


//**********************************************************************************
//Variables declaration and link between them **************************************
//**********************************************************************************


    G4RotationMatrix DontRotate;
    DontRotate.rotateX(0*deg);

    G4RotationMatrix Rotate;
    Rotate.rotateX(180*deg);


//**********************************************************************************
//Definition of volume**************************************************************
//**********************************************************************************

  // ####################### SCINT BLOCK #######################
  G4Box *Scintillator_N3DC= new G4Box("Scintillator_N3DC",          
                  N3EC_ScintWidth/2, N3EC_ScintWidth/2,        // We can take the variables from N3EC beacause its a NEMO3 EC block !!!!
                   N3EC_ScintHeight/2);
                 


  // ####################### COUPLING SPHERE #######################
  G4Sphere *CouplingSphere = new G4Sphere("CouplingSphere",
					  0., N3EC_CouplingSphereRadius, // We can take the variables from N3EC beacause its a NEMO3 EC block !!!!
					  0., 2*180*deg , // From 0-360 degrees in theta for full sphere
					  0., 0.45*180*deg) ; // From 0-90 degrees in phi for full sphere

  // ####################### FINAL BLOCK #######################
  // Subtract 2nd volume from the 1st 
  // SphereBlockTranslation = dist. from step center to center of sphere position where subtraction will occur
  G4SubtractionSolid* FinalScint_N3DC = new G4SubtractionSolid("FinalScint_N3DC", 
							  Scintillator_N3DC,
							  CouplingSphere,
							  G4Transform3D(Rotate,G4ThreeVector(0,0,N3EC_ScintHeight/2 - N3EC_CouplingSphereDepth + N3EC_CouplingSphereRadius )));

    LogicalN3DC_Scintillator = new G4LogicalVolume(FinalScint_N3DC, scintillator, "LogicalN3DC_Scintillator",0,0,0);


    return LogicalN3DC_Scintillator;


}






G4LogicalVolume *Scintillator::GetN3DC_8inch_Scintillator(){

//Define the materials
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Plastic = scintProp->GetMaterial("plastic");
  Polystyrene = scintProp->GetMaterial("polysty");
  scintillator = scintProp->GetMaterial("scintillator");


//**********************************************************************************
//Variables declaration and link between them **************************************
//**********************************************************************************


    G4RotationMatrix DontRotate;
    DontRotate.rotateX(0*deg);

    G4RotationMatrix Rotate;
    Rotate.rotateX(180*deg);


//**********************************************************************************
//Definition of volume**************************************************************
//**********************************************************************************

  // ####################### SCINT BLOCK #######################
  G4Box *Scintillator_N3DC= new G4Box("Scintillator_N3DC",          
                  N3EC_ScintWidth/2, N3EC_ScintWidth/2,        // We can take the variables from N3EC beacause its a NEMO3 EC block !!!!
                   N3EC_ScintHeight/2);
                 


  // ####################### COUPLING SPHERE #######################
G4int N_z_plane_glue = 17;
G4double Phi_start = 0.*deg;
G4double Phi_end = 2*180*deg;

const G4double z_plane_glue[] = {39.9182*mm, 49.8607*mm, 59.4345*mm, 68.3814*mm, 76.581*mm, 83.5154*mm, 89.9187*mm, 95.2918*mm, 99.6867*mm, 103.405*mm, 106.303*mm, 107.669*mm, 108.556*mm, 108.658*mm, 109.334*mm, 109.825*mm, 110.*mm};

const G4double r_inner_glue[]= {0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm};


const G4double r_outer_glue[]= {102.846*mm, 101.3918*mm, 97.9735*mm, 92.7321*mm, 86.4727*mm, 78.9557*mm, 70.9368*mm, 62.0639*mm, 52.8971*mm, 43.1194*mm, 32.596*mm, 27.291*mm, 22.8952*mm, 22.3897*mm, 15.4348*mm, 10.53213*mm, 2.*mm};

  G4Polycone *CouplingSphere = new G4Polycone("CouplingSphere",
			Phi_start,
			Phi_end,
			N_z_plane_glue,
			z_plane_glue,
			r_inner_glue,
			r_outer_glue);

  // ####################### FINAL BLOCK #######################
  // Subtract 2nd volume from the 1st 
  // SphereBlockTranslation = dist. from step center to center of sphere position where subtraction will occur
  G4SubtractionSolid* FinalScint_N3DC_8inch = new G4SubtractionSolid("FinalScint_N3DC", 
							  Scintillator_N3DC,
							  CouplingSphere,
							  G4Transform3D(Rotate,G4ThreeVector(0,0,112*mm )));

    LogicalN3DC_8inch_Scintillator = new G4LogicalVolume(FinalScint_N3DC_8inch, scintillator, "LogicalN3DC_8inch_Scintillator",0,0,0);


    return LogicalN3DC_8inch_Scintillator;


}




//************************************************************************************
//************************************************************************************
//*********************** TEFLON NEMO3 DIRECT COUPLING (N3DC) ************************
//************************************************************************************
//************************************************************************************

G4LogicalVolume *Scintillator::GetN3DC_Teflon(){

//Define the materials
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Teflon = scintProp->GetMaterial("teflon");
  Polystyrene = scintProp->GetMaterial("polysty");
  scintillator = scintProp->GetMaterial("scintillator");


//**********************************************************************************
//Variables declaration and link between them **************************************
//**********************************************************************************


    G4RotationMatrix DontRotate;
    DontRotate.rotateX(0*deg);

    G4RotationMatrix Rotate;
    Rotate.rotateX(180*deg);


//**********************************************************************************
//Definition of volume**************************************************************
//**********************************************************************************

//###################### Creation Teflon's Volume ###############################
  G4Box *Teflon_Full= new G4Box("Teflon_Full",          
                  N3EC_ScintWidth/2 + AirGapTeflon + TeflonThickness,
		  N3EC_ScintWidth/2 + AirGapTeflon + TeflonThickness,        
                  N3EC_ScintHeight/2);


//#################### Creation Scintillator's Volume for the subtraction ##################
  G4Box *Scintillator_Subtraction= new G4Box("Scintillator_Subtraction",          
                  N3EC_ScintWidth/2 + AirGapTeflon, // with the gap because we want to obtain just the Teflon with an air gap
		  N3EC_ScintWidth/2 + AirGapTeflon,        
                  N3EC_ScintHeight/2);
                 

//################### Subtraction to obtain layer of Teflon ############################
 G4SubtractionSolid* Teflon_N3DC = new G4SubtractionSolid("Teflon_N3DC", Teflon_Full,Scintillator_Subtraction,G4Transform3D(DontRotate,G4ThreeVector(0,0,0)));





    LogicalN3DC_TeflonWrap = new G4LogicalVolume(Teflon_N3DC, Teflon, "LogicalN3DC_TeflonWrap",0,0,0);


    return LogicalN3DC_TeflonWrap;


}





//************************************************************************************
//************************************************************************************
//*********************** MYLAR NEMO3 DIRECT COUPLING (N3DC) *************************
//************************************************************************************
//************************************************************************************

G4LogicalVolume *Scintillator::GetN3DC_Mylar(){

//Define the materials
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Mylar = scintProp->GetMaterial("mylar");
  Polystyrene = scintProp->GetMaterial("polysty");
  scintillator = scintProp->GetMaterial("scintillator");


//**********************************************************************************
//Variables declaration and link between them **************************************
//**********************************************************************************

    G4RotationMatrix DontRotate;
    DontRotate.rotateX(0*deg);

    G4RotationMatrix Rotate;
    Rotate.rotateX(180*deg);


//**********************************************************************************
//Definition of volume**************************************************************
//**********************************************************************************

//###################### Creation Mylar's Volume ###############################
  G4Box *Mylar_Full= new G4Box("Mylar_Full",          
                  N3EC_ScintWidth/2 + AirGapTeflon + TeflonThickness + AirGapMylar + MylarThickness,
		  N3EC_ScintWidth/2 + AirGapTeflon + TeflonThickness + AirGapMylar + MylarThickness,        
                  N3EC_ScintHeight/2 + AirGapMylar + MylarThickness);
                 
//#################### Creation Teflon's Volume for the subtraction ##################
  G4Box *Teflon_Subtraction= new G4Box("Teflon_Subtraction",          
                  N3EC_ScintWidth/2 + AirGapTeflon + TeflonThickness + AirGapMylar,  // with the gap because we want to obtain just the Teflon with an air gap
		  N3EC_ScintWidth/2 + AirGapTeflon + TeflonThickness + AirGapMylar,        
                  N3EC_ScintHeight/2 + AirGapMylar);

//################### Subtraction to obtain layer of Mylar ############################
 G4SubtractionSolid* Mylar_N3DC_Full = new G4SubtractionSolid("Mylar_N3DC_Full", Mylar_Full, Teflon_Subtraction, G4Transform3D(DontRotate,G4ThreeVector(0,0,0)));


  // ####################### COUPLING SPHERE #######################
  G4Sphere *CouplingSphere = new G4Sphere("CouplingSphere",
					  0., N3EC_CouplingSphereRadius, // We can take the variables from N3EC beacause its a NEMO3 EC block !!!!
					  0., 2*180*deg , // From 0-360 degrees in theta for full sphere
					  0., 0.45*180*deg) ; // From 0-90 degrees in phi for full sphere


//################### Subtraction to obtain layer of Mylar without a part on the top for the coupling with the light guide ############################
 G4SubtractionSolid* Mylar_N3DC = new G4SubtractionSolid("Mylar_N3DC", Mylar_N3DC_Full, CouplingSphere, G4Transform3D(DontRotate,G4ThreeVector(0,0,N3EC_ScintHeight/2 - N3EC_CouplingSphereDepth + N3EC_CouplingSphereRadius)));



    LogicalN3DC_MylarWrap = new G4LogicalVolume(Mylar_N3DC, Mylar, "LogicalN3DC_MylarWrap",0,0,0);


    return LogicalN3DC_MylarWrap;


}






G4LogicalVolume *Scintillator::GetN3DC_8inch_Mylar(){

//Define the materials
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Mylar = scintProp->GetMaterial("mylar");
  Polystyrene = scintProp->GetMaterial("polysty");
  scintillator = scintProp->GetMaterial("scintillator");


//**********************************************************************************
//Variables declaration and link between them **************************************
//**********************************************************************************

    G4RotationMatrix DontRotate;
    DontRotate.rotateX(0*deg);

    G4RotationMatrix Rotate;
    Rotate.rotateX(180*deg);


//**********************************************************************************
//Definition of volume**************************************************************
//**********************************************************************************

//###################### Creation Mylar's Volume ###############################
  G4Box *Mylar_Full= new G4Box("Mylar_Full",          
                  N3EC_ScintWidth/2 + AirGapTeflon + TeflonThickness + AirGapMylar + MylarThickness,
		  N3EC_ScintWidth/2 + AirGapTeflon + TeflonThickness + AirGapMylar + MylarThickness,        
                  N3EC_ScintHeight/2 + AirGapMylar + MylarThickness);
                 
//#################### Creation Teflon's Volume for the subtraction ##################
  G4Box *Teflon_Subtraction= new G4Box("Teflon_Subtraction",          
                  N3EC_ScintWidth/2 + AirGapTeflon + TeflonThickness + AirGapMylar,  // with the gap because we want to obtain just the Teflon with an air gap
		  N3EC_ScintWidth/2 + AirGapTeflon + TeflonThickness + AirGapMylar,        
                  N3EC_ScintHeight/2 + AirGapMylar);

//################### Subtraction to obtain layer of Mylar ############################
 G4SubtractionSolid* Mylar_N3DC_Full = new G4SubtractionSolid("Mylar_N3DC_Full", Mylar_Full, Teflon_Subtraction, G4Transform3D(DontRotate,G4ThreeVector(0,0,0)));


 // ####################### COUPLING SPHERE #######################
G4int N_z_plane_glue = 17;
G4double Phi_start = 0.*deg;
G4double Phi_end = 2*180*deg;

const G4double z_plane_glue[] = {39.9182*mm, 49.8607*mm, 59.4345*mm, 68.3814*mm, 76.581*mm, 83.5154*mm, 89.9187*mm, 95.2918*mm, 99.6867*mm, 103.405*mm, 106.303*mm, 107.669*mm, 108.556*mm, 108.658*mm, 109.334*mm, 109.825*mm, 110.*mm};

const G4double r_inner_glue[]= {0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm};


const G4double r_outer_glue[]= {102.846*mm, 101.3918*mm, 97.9735*mm, 92.7321*mm, 86.4727*mm, 78.9557*mm, 70.9368*mm, 62.0639*mm, 52.8971*mm, 43.1194*mm, 32.596*mm, 27.291*mm, 22.8952*mm, 22.3897*mm, 15.4348*mm, 10.53213*mm, 2.*mm};

  G4Polycone *CouplingSphere = new G4Polycone("CouplingSphere",
			Phi_start,
			Phi_end,
			N_z_plane_glue,
			z_plane_glue,
			r_inner_glue,
			r_outer_glue);


//################### Subtraction to obtain layer of Mylar without a part on the top for the coupling with the light guide ############################
 G4SubtractionSolid* Mylar_N3DC_8inch = new G4SubtractionSolid("Mylar_N3DC_8inch", Mylar_N3DC_Full, CouplingSphere, G4Transform3D(DontRotate,G4ThreeVector(0,0,112*mm)));



    LogicalN3DC_8inch_MylarWrap = new G4LogicalVolume(Mylar_N3DC_8inch, Mylar, "LogicalN3DC_8inch_MylarWrap",0,0,0);


    return LogicalN3DC_8inch_MylarWrap;


}








//************************************************************************************
//************************************************************************************
//******************************* NEMO3 L4 (N3L4) ************************************
//************************************************************************************
//************************************************************************************



//************************************************************************************
//************************************************************************************
//************************** SCINTILLATOR NEMO3 L4 (N3L4) ****************************
//************************************************************************************
//************************************************************************************

G4LogicalVolume *Scintillator::GetN3L4_Scintillator(){

//Define the materials
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Plastic = scintProp->GetMaterial("plastic");
  Polystyrene = scintProp->GetMaterial("polysty");
  scintillator = scintProp->GetMaterial("scintillator");


//**********************************************************************************
//Variables declaration and link between them **************************************
//**********************************************************************************


    G4RotationMatrix DontRotate;
    DontRotate.rotateX(0*deg);

    G4RotationMatrix Rotate;
    Rotate.rotateX(180*deg);


//**********************************************************************************
//Definition of volume**************************************************************
//**********************************************************************************

  // ####################### SCINT BLOCK #######################

G4Trd *Scintillator_Full_N3L4 = new G4Trd("Scintillator_Full_N3L4",
             	(N3L4_ScintLength/2), //Half-length along x at the surface positioned at -dz
             	(N3L4_ScintWidth/2), //Half-length along x at the surface positioned at +dz
              	(N3L4_ScintLength/2), //Half-length along y at the surface positioned at -dz
             	(N3L4_ScintWidth/2), //Half-length along y at the surface positioned at +dz
             	(N3L4_ScintHeight/2)); //Half-length along z axis 


    LogicalN3L4_Scintillator = new G4LogicalVolume(Scintillator_Full_N3L4, scintillator, "LogicalN3L4_Scintillator",0,0,0);


    return LogicalN3L4_Scintillator;


}




//************************************************************************************
//************************************************************************************
//******************************** TEFLON NEMO3 L4 (N3L4) ****************************
//************************************************************************************
//************************************************************************************

G4LogicalVolume *Scintillator::GetN3L4_Teflon(){

//Define the materials
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Teflon = scintProp->GetMaterial("teflon");
  Polystyrene = scintProp->GetMaterial("polysty");
  scintillator = scintProp->GetMaterial("scintillator");


//**********************************************************************************
//Variables declaration and link between them **************************************
//**********************************************************************************


    G4RotationMatrix DontRotate;
    DontRotate.rotateX(0*deg);

    G4RotationMatrix Rotate;
    Rotate.rotateX(180*deg);


//**********************************************************************************
//Definition of volume**************************************************************
//**********************************************************************************

  // ####################### Teflon's volume #######################

G4Trd *Teflon_Full_N3L4 = new G4Trd("Teflon_Full_N3L4",
             	(N3L4_ScintLength/2 + AirGapTeflon + TeflonThickness), //Half-length along x at the surface positioned at -dz
             	(N3L4_ScintWidth/2 + AirGapTeflon + TeflonThickness), //Half-length along x at the surface positioned at +dz
              	(N3L4_ScintLength/2 + AirGapTeflon + TeflonThickness), //Half-length along y at the surface positioned at -dz
             	(N3L4_ScintWidth/2 + AirGapTeflon + TeflonThickness), //Half-length along y at the surface positioned at +dz
             	(N3L4_ScintHeight/2)); //Half-length along z axis 



  // ####################### Creation of Scintillator's volume for subtraction #######################

G4Trd *Subtraction_Scint = new G4Trd("Subtraction_Scint",
             	(N3L4_ScintLength/2 + AirGapTeflon), //Half-length along x at the surface positioned at -dz
             	(N3L4_ScintWidth/2 + AirGapTeflon), //Half-length along x at the surface positioned at +dz
              	(N3L4_ScintLength/2 + AirGapTeflon), //Half-length along y at the surface positioned at -dz
             	(N3L4_ScintWidth/2 + AirGapTeflon), //Half-length along y at the surface positioned at +dz
             	(N3L4_ScintHeight/2)); //Half-length along z axis 


//################### Subtraction to obtain layer of Teflon ############################
 G4SubtractionSolid* Teflon_N3L4 = new G4SubtractionSolid("Teflon_N3L4", Teflon_Full_N3L4, Subtraction_Scint, G4Transform3D(DontRotate,G4ThreeVector(0,0,0)));


    LogicalN3L4_TeflonWrap = new G4LogicalVolume(Teflon_N3L4, Teflon, "LogicalN3L4_TeflonWrap",0,0,0);


    return LogicalN3L4_TeflonWrap;


}




//************************************************************************************
//************************************************************************************
//********************************* MYLAR NEMO3 L4 (N3L4) ****************************
//************************************************************************************
//************************************************************************************

G4LogicalVolume *Scintillator::GetN3L4_Mylar(){

//Define the materials
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Mylar = scintProp->GetMaterial("mylar");
  Polystyrene = scintProp->GetMaterial("polysty");
  scintillator = scintProp->GetMaterial("scintillator");


//**********************************************************************************
//Variables declaration and link between them **************************************
//**********************************************************************************


    G4RotationMatrix DontRotate;
    DontRotate.rotateX(0*deg);

    G4RotationMatrix Rotate;
    Rotate.rotateX(180*deg);


//**********************************************************************************
//Definition of volume**************************************************************
//**********************************************************************************

  // ####################### Mylar's volume #######################

G4Trd *Mylar_Full_N3L4 = new G4Trd("Mylar_Full_N3L4",
             	(N3L4_ScintLength/2 + AirGapTeflon + TeflonThickness + AirGapMylar + MylarThickness), //Half-length along x at the surface positioned at -dz
             	(N3L4_ScintWidth/2 + AirGapTeflon + TeflonThickness + AirGapMylar + MylarThickness), //Half-length along x at the surface positioned at +dz
              	(N3L4_ScintLength/2 + AirGapTeflon + TeflonThickness + AirGapMylar + MylarThickness), //Half-length along y at the surface positioned at -dz
             	(N3L4_ScintWidth/2 + AirGapTeflon + TeflonThickness + AirGapMylar + MylarThickness), //Half-length along y at the surface positioned at +dz
             	(N3L4_ScintHeight/2 + AirGapMylar + MylarThickness)); //Half-length along z axis 



  // ####################### Creation of Teflon's volume for subtraction #######################

G4Trd *Subtraction_Teflon = new G4Trd("Subtraction_Teflon",
             	(N3L4_ScintLength/2 + AirGapTeflon + TeflonThickness + AirGapMylar), //Half-length along x at the surface positioned at -dz
             	(N3L4_ScintWidth/2 + AirGapTeflon + TeflonThickness + AirGapMylar), //Half-length along x at the surface positioned at +dz
              	(N3L4_ScintLength/2 + AirGapTeflon + TeflonThickness + AirGapMylar), //Half-length along y at the surface positioned at -dz
             	(N3L4_ScintWidth/2 + AirGapTeflon + TeflonThickness + AirGapMylar), //Half-length along y at the surface positioned at +dz
             	(N3L4_ScintHeight/2 + AirGapMylar)); //Half-length along z axis 


//################### Subtraction to obtain layer of Mylar ############################
 G4SubtractionSolid* Mylar_Complete_N3L4 = new G4SubtractionSolid("Mylar_Complete_N3L4", Mylar_Full_N3L4, Subtraction_Teflon, G4Transform3D(DontRotate,G4ThreeVector(0,0,0)));


//###################### Creation Coupling's Volume for the subtraction (We use the same light from SNXW configuration !!!)###############################
G4Tubs *Coupling;
Coupling = new G4Tubs("Coupling",
             0.*mm,   //G4double  pRMin
             SNXW_LightGuideRadius,    //G4double  pRMax
             SNXW_LightGuideCouplingHeight/2,    //G4double  pDz
             0.,    //G4double  pSPhi
             2*pi*rad);   //G4double  pDPhi


//################### Subtraction to obtain layer of Mylar without a part on the top for the coupling with the light guide ############################
 G4SubtractionSolid* Mylar_N3L4 = new G4SubtractionSolid("Mylar_N3L4", Mylar_Complete_N3L4, Coupling, G4Transform3D(DontRotate,G4ThreeVector(0,0,N3L4_ScintHeight/2 + SNXW_LightGuideCouplingHeight/2)));


    LogicalN3L4_MylarWrap = new G4LogicalVolume(Mylar_N3L4, Mylar, "LogicalN3L4_MylarWrap",0,0,0);


    return LogicalN3L4_MylarWrap;


}




G4LogicalVolume *Scintillator::GetSource(){
  // Material Properties for scint
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");  
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Plastic = scintProp->GetMaterial("plastic");
  Polystyrene = scintProp->GetMaterial("polysty");
  scintillator = scintProp->GetMaterial("scintillator");


  // Define some rotations
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0*deg);
  G4RotationMatrix Rotate;
  Rotate.rotateX(180*deg);
  
  // ####################### SCINT BLOCK #######################
  // Scintillator Step
  G4Box *Step = new G4Box("Step",          
			  26*mm/2, 20*mm/2, 
			  10*mm/2);  


  

  


  LogicalSource = new G4LogicalVolume(Step, Plastic, "LogicalSource",0,0,0);
  
  //G4VisAttributes * clear;
  //clear = new G4VisAttributes( G4Colour(255/255. ,255/255. ,255/255. ));

  return LogicalSource;  
}





G4LogicalVolume *Scintillator::GetBlindage(){
  // Material Properties for scint
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");  
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Fer = scintProp->GetMaterial("Fer");



  // Define some rotations
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0*deg);
  G4RotationMatrix Rotate;
  Rotate.rotateX(180*deg);
  
  // ####################### SCINT BLOCK #######################
  // Scintillator Step
  G4Box *Full_Blindage = new G4Box("Full_Blindage",          
			  249*mm/2, 249*mm/2, 
			  420*mm/2);  

  G4Box *Up_Extraction = new G4Box("Up_Extraction",          
			  243*mm/2, 243*mm/2, 
			  257*mm/2);  


  G4ThreeVector Tr1(0, 0, -81.5*mm);

  G4SubtractionSolid* Blindage_Up_Extraction = new G4SubtractionSolid("Blindage_Up_Extraction", Full_Blindage, Up_Extraction, G4Transform3D(DontRotate, Tr1));



  G4Box *Down_Extraction = new G4Box("Down_Extraction",          
			  243*mm/2, 243*mm/2, 
			  160*mm/2);  

  G4ThreeVector Tr2(0, 0, 130*mm);

  G4SubtractionSolid* Blindage_Up_Down_Extraction = new G4SubtractionSolid("Blindage_Up_Down_Extraction", Blindage_Up_Extraction, Down_Extraction, G4Transform3D(DontRotate, Tr2));


  G4Tubs* PM_Extraction = new G4Tubs("PM_Extraction",
				     0.,
				     105.,
				     200.,
				     0.,
				     2*180*deg);

  G4ThreeVector Tr3(0, 0, 0);

  G4SubtractionSolid* Blindage = new G4SubtractionSolid("Blindage", Blindage_Up_Down_Extraction, PM_Extraction, G4Transform3D(DontRotate, Tr3));
  

  


  LogicalBlindage = new G4LogicalVolume(Blindage, Plastic, "LogicalBlindage",0,0,0);
  
  //G4VisAttributes * clear;
  //clear = new G4VisAttributes( G4Colour(255/255. ,255/255. ,255/255. ));

  return LogicalBlindage;  
}


