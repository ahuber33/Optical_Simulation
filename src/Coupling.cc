/// Sealant.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu>
/// Copyright: 2010 (C) NEMO3/SuperNEMO - University of Texas at Austin

#include "SN_OpticalSimMaterials.hh"
#include "Coupling.hh"
#include "Scintillator.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4Transform3D.hh"
#include <fstream>
#include <iostream>
#include "G4Tubs.hh"
#include "G4UnitsTable.hh"
#include "G4Sphere.hh"
#include "G4Box.hh"
#include <G4Polycone.hh>
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"

using namespace CLHEP;

const G4String Coupling::path_bin = "/home/local1/Optical_Simulation_Arnaud/bin/";

Coupling::Coupling(){}

Coupling::~Coupling(){
    if(scintProp) { delete scintProp; scintProp = 0; }
    if(Cargille) { delete Cargille; Cargille = 0; }
    //if(logical_spherical_Coupling) { delete logical_spherical_Coupling; logical_spherical_Coupling = 0; }
    if(theScint) { delete theScint; theScint = 0; }
}

G4LogicalVolume *Coupling::GetSNMW_SphericalCoupling(){

  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Cargille = scintProp->GetMaterial("cargille");
  
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");



  //TEST COUPLING RTV615

 // Read in and define dimensions
  SNMW_CouplingSphereRadius = theScint->GetSNMW_CouplingRadius();
  SNMW_CouplingSphereDepth = theScint->GetSNMW_CouplingDepth();
  PMTGlassRadius = theScint->GetPMTGlassRadius();
  GlueThickness = theScint->GetGlueThickness();
  //  G4cout << " Coupling angle = " << acos((CouplingSphereRadius-CouplingDepth)/CouplingSphereRadius) << G4endl;

  // Define translations used for adding or subtracting volumes
  G4double CouplingSphereSubtraction = SNMW_CouplingSphereRadius - PMTGlassRadius - GlueThickness;
				      
  // Define some rotations
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);
  G4RotationMatrix Flip;
  Flip.rotateX(180.0*deg);

  // Create first solid coupling sphere: phi only covers up to edge of bowl though
  G4Sphere *SolidCouplingSphere;
  SolidCouplingSphere = new G4Sphere("CouplingSphere",
				     0, SNMW_CouplingSphereRadius, // Outer radius to match scintillator bowl
				     0, twopi*rad ,
				     0., (acos((SNMW_CouplingSphereRadius-SNMW_CouplingSphereDepth)/SNMW_CouplingSphereRadius))*rad); // So coupling only extends to edges of the scintillator bowl
  
  // create subtraction sphere that will leave only a "shell" of coupling medium  
  /*
  G4Sphere *SolidSubtractionSphere;
  SolidSubtractionSphere = new G4Sphere("SubtractionSphere",
					0., PMTGlassRadius, // Inner radius to math PMT glass
					0., twopi*rad,
					0., pi*rad); // This sphere can be complete

*/


G4int N_z_plane = 30;
G4double Phi_start = 0.*deg;
G4double Phi_end = 2*180*deg;


const G4double z_plane_extru[] = {-89.*mm, -88.75*mm, -87.5*mm, -87.51*mm, -86.*mm, -85.4*mm, -84.6*mm, -84.*mm, -82.5187*mm, -7.54324*mm, -2.54324*mm, 2.45676*mm, 7.19658*mm, 11.0775*mm, 13.9256*mm, 21.252*mm, 30.2224*mm, 40.0277*mm, 50.0224*mm, 59.6324*mm, 67.6679*mm, 74.4474*mm, 77.7374*mm, 80.3403*mm, 82.8689*mm, 84.6888*mm, 86.3574*mm, 87.4988*mm, 88.4534*mm, 89.*mm};

const G4double r_inner_extru[]= {0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm};

const G4double r_outer_extru[]= {0.*mm, 36.5*mm, 37.*mm, 37.*mm, 37.5*mm, 40.8*mm, 41.75*mm, 42.*mm, 42.3589*mm, 42.4425*mm, 42.4425*mm, 43.1688*mm, 46.0797*mm, 51.0241*mm, 54.6315*mm, 59.7845*mm, 63.1125*mm, 64.*mm, 63.473*mm, 59.9087*mm, 53.4484*mm, 45.4762*mm, 40.948*mm, 36.5916*mm, 31.0122*mm, 26.1237*mm, 19.9433*mm, 13.889*mm, 7.70708*mm, 0.*mm};


  G4Polycone *PM5Inches_extru = new G4Polycone("PM5Inches_extru",
			Phi_start,
			Phi_end,
			N_z_plane,
			z_plane_extru,
			r_inner_extru,
			r_outer_extru);

  
  // Subtract smaller sphere from the larger to make shell 
  G4SubtractionSolid* FinalCoupling = new G4SubtractionSolid("CouplingShell", 
							     SolidCouplingSphere,
							     PM5Inches_extru,
							     G4Transform3D(DontRotate,G4ThreeVector(0, 0, -11.5*mm))); // Offset to produce 0.3 mm glue thickness on central axis => CouplingSphereSubtraction
							     
  // Llogical volume: material RTV  
  LogicalSNMW_CouplingSphere = new G4LogicalVolume(FinalCoupling,Cargille,"LogicalSNMW_CouplingSphere",0,0,0);


  
  // Visualization attributes
  G4VisAttributes *invis = new G4VisAttributes( G4Colour(255/255. ,255/255. ,255/255. ));
  invis->SetVisibility(false);
  G4VisAttributes *green = new G4VisAttributes(G4Colour(0,1,0));
  green->SetForceWireframe(true);
  //logical_spherical_Coupling->SetVisAttributes(green);
  
  return LogicalSNMW_CouplingSphere;
}





//************************************************************************************
//************************************************************************************
//************************* COUPLING 8 INCHES SUPERNEMO ******************************
//************************************************************************************
//************************************************************************************





G4LogicalVolume *Coupling::GetSNMW_8InchesSphericalCoupling(){

  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Cargille = scintProp->GetMaterial("cargille");
  
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");



  //TEST COUPLING RTV615

 // Read in and define dimensions
  SNMW_CouplingSphereRadius = theScint->GetSNMW_CouplingRadius();
  SNMW_CouplingSphereDepth = theScint->GetSNMW_CouplingDepth();
  PMTGlassRadius = theScint->GetPMTGlassRadius();
  GlueThickness = theScint->GetGlueThickness();
  //  G4cout << " Coupling angle = " << acos((CouplingSphereRadius-CouplingDepth)/CouplingSphereRadius) << G4endl;

  // Define translations used for adding or subtracting volumes
  G4double CouplingSphereSubtraction = SNMW_CouplingSphereRadius - PMTGlassRadius - GlueThickness;
				      
  // Define some rotations
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);
  G4RotationMatrix Flip;
  Flip.rotateX(180.0*deg);

  
  // ####################### COUPLING SPHERE #######################
G4int N_z_plane_glue = 17;
G4double Phi_start = 0.*deg;
G4double Phi_end = 2*180*deg;

const G4double z_plane_glue[] = {39.9182*mm, 49.8607*mm, 59.4345*mm, 68.3814*mm, 76.581*mm, 83.5154*mm, 89.9187*mm, 95.2918*mm, 99.6867*mm, 103.405*mm, 106.303*mm, 107.669*mm, 108.556*mm, 108.658*mm, 109.334*mm, 109.825*mm, 110.*mm};

const G4double r_inner_glue[]= {0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm};


const G4double r_outer_glue[]= {102.846*mm, 101.3918*mm, 97.9735*mm, 92.7321*mm, 86.4727*mm, 78.9557*mm, 70.9368*mm, 62.0639*mm, 52.8971*mm, 43.1194*mm, 32.596*mm, 27.291*mm, 22.8952*mm, 22.3897*mm, 15.4348*mm, 10.53213*mm, 2.*mm};

  G4Polycone *Glue8Inches = new G4Polycone("Glue9Inches",
			Phi_start,
			Phi_end,
			N_z_plane_glue,
			z_plane_glue,
			r_inner_glue,
			r_outer_glue);




  // ####################### PMT GLASS SOLID SUBTRACTION #######################
G4int N_z_plane = 36;


const G4double z_plane_extrusion[] = {-110.*mm, -109.829*mm, -108.647*mm, -108.65*mm, -107.416*mm, -106.481*mm, -103.084*mm, -98.6401*mm, -43.6458*mm, -33.8076*mm, -27.3606*mm, -22.9623*mm, -18.0893*mm, -12.2671*mm, -5.65007*mm, 2.09728*mm, 10.6724*mm, 20.0733*mm, 29.9204*mm, 39.9182*mm, 49.8607*mm, 59.4345*mm, 68.3814*mm, 76.581*mm, 83.5154*mm, 89.9187*mm, 95.2918*mm, 99.6867*mm, 103.405*mm, 106.303*mm, 107.669*mm, 108.556*mm, 108.658*mm, 109.334*mm, 109.825*mm, 110.*mm};

const G4double r_inner_extrusion[]= {0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm};

const G4double r_outer_extrusion[]= {0.*mm, 32.0648*mm, 35.6705*mm, 35.6705*mm, 37.4533*mm, 38.4624*mm, 40.5955*mm, 41.5084*mm, 41.9043*mm, 45.2687*mm, 54.2783*mm, 63.2852*mm, 71.4126*mm, 79.2565*mm, 86.3265*mm, 92.3066*mm, 96.803*mm, 99.5055*mm, 100.905*mm, 100.846*mm, 99.3918*mm, 95.9735*mm, 90.7321*mm, 84.4727*mm, 76.9557*mm, 68.9368*mm, 60.0639*mm, 50.8971*mm, 41.1194*mm, 30.596*mm, 25.291*mm, 20.8952*mm, 20.3897*mm, 13.4348*mm, 8.53213*mm, 0.*mm};

  G4Polycone *PMTGlass8InchesExtrusion = new G4Polycone("PMTGlass8InchesExtrusion",
			Phi_start,
			Phi_end,
			N_z_plane,
			z_plane_extrusion,
			r_inner_extrusion,
			r_outer_extrusion);  


  // Subtract smaller sphere from the larger to make shell 
  G4SubtractionSolid* FinalCoupling8Inches = new G4SubtractionSolid("FinalCoupling8Inches", 
							     Glue8Inches,
							     PMTGlass8InchesExtrusion,
							     G4Transform3D(DontRotate,G4ThreeVector(0, 0, 0)));  
							     
  // Llogical volume: material RTV  
  LogicalSNMW_8InchesCouplingSphere = new G4LogicalVolume(FinalCoupling8Inches,Cargille,"LogicalSNMW_8InchesCouplingSphere",0,0,0);


  

  
  return LogicalSNMW_8InchesCouplingSphere;
}



//************************************************************************************
//************************************************************************************
//********************************* COUPLING NEMO ************************************
//************************************************************************************
//************************************************************************************

G4LogicalVolume *Coupling::GetN3EC_CouplingLightGuide(){

  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Cargille = scintProp->GetMaterial("cargille");
  grease = scintProp->GetMaterial("grease");
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");


//**********************************************************************************
//Variables declaration and link between them **************************************
//**********************************************************************************
  /*radius_light_guide_min = thestrip->GetRadiusLightGuideMin();
  height_coupling = thestrip->GetHeightCoupling();
  thickness_Mylar = thestrip->GetThicknessMylar();
  gap_Mylar = thestrip->GetGapMylar();*/

  N3EC_LightGuideCouplingRadius = theScint->GetN3EC_LightGuideCouplingRadius();
  N3EC_LightGuideCouplingHeight = theScint->GetN3EC_LightGuideCouplingHeight();
  AirGapMylar = theScint->GetAirGapMylar();
  MylarThickness = theScint->GetMylarThickness();

  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);
  
  G4RotationMatrix Flip;
  Flip.rotateX(180.0*deg);


//**********************************************************************************
//Definition of volume**************************************************************
//**********************************************************************************

G4Tubs *Coupling_Glue;
Coupling_Glue = new G4Tubs("Coupling_Glue",
             0.*mm,   //G4double  pRMin
             N3EC_LightGuideCouplingRadius + AirGapMylar + MylarThickness,    //With air gap and mylar thickness to take into account the wrap around the light guide
             N3EC_LightGuideCouplingHeight/2,    //G4double  pDz
             0.,    //G4double  pSPhi
             2*pi*rad);   //G4double  pDPhi

  

  LogicalN3EC_CouplingScLightGuide = new G4LogicalVolume(Coupling_Glue,grease,"LogicalN3EC_CouplingScLightGuide",0,0,0);




  return LogicalN3EC_CouplingScLightGuide;
}



//************************************************************************************
//************************************************************************************
//********************************** GLUE NEMO ***************************************
//************************************************************************************
//************************************************************************************

G4LogicalVolume *Coupling::GetN3EC_SphericalCoupling(){

  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Cargille = scintProp->GetMaterial("cargille");
  grease = scintProp->GetMaterial("grease");
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");


//**********************************************************************************
//Variables declaration and link between them **************************************
//**********************************************************************************
/*  radius_glue_NEMO = thestrip->GetRadiusGlueNEMO();
  radius_PMT_glass_photocathode = thestrip->GetRadiusPMTGlassPhotocathode();
*/
  N3EC_CouplingSphereRadius = theScint->GetN3EC_CouplingSphereRadius();
  N3EC_CouplingSphereDepth = theScint->GetN3EC_CouplingSphereDepth();
  GlueThickness = theScint->GetGlueThickness();
  PMTGlassRadius = theScint->GetPMTGlassRadius();


  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);
  
  G4RotationMatrix Flip;
  Flip.rotateX(180.0*deg);


//**********************************************************************************
//Definition of volume**************************************************************
//**********************************************************************************

G4Sphere *Glue_Full;
 Glue_Full = new G4Sphere("Glue_Full",
               0,
               N3EC_CouplingSphereRadius,
               0,
               twopi*rad ,
               0.,
               (acos((N3EC_CouplingSphereRadius-N3EC_CouplingSphereDepth)/N3EC_CouplingSphereRadius))*rad) ;


/*
 G4Sphere *Sphere_Photocathode_Subtraction;
 Sphere_Photocathode_Subtraction = new G4Sphere("Sphere_Photocathode_Subtraction",
               0,
               PMTGlassRadius,
               0,
               twopi ,
               0.,
               pi*rad) ;
*/


G4int N_z_plane = 30;
G4double Phi_start = 0.*deg;
G4double Phi_end = 2*180*deg;


const G4double z_plane_extru[] = {-89.*mm, -88.75*mm, -87.5*mm, -87.51*mm, -86.*mm, -85.4*mm, -84.6*mm, -84.*mm, -82.5187*mm, -7.54324*mm, -2.54324*mm, 2.45676*mm, 7.19658*mm, 11.0775*mm, 13.9256*mm, 21.252*mm, 30.2224*mm, 40.0277*mm, 50.0224*mm, 59.6324*mm, 67.6679*mm, 74.4474*mm, 77.7374*mm, 80.3403*mm, 82.8689*mm, 84.6888*mm, 86.3574*mm, 87.4988*mm, 88.4534*mm, 89.*mm};

const G4double r_inner_extru[]= {0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm};

const G4double r_outer_extru[]= {0.*mm, 36.5*mm, 37.*mm, 37.*mm, 37.5*mm, 40.8*mm, 41.75*mm, 42.*mm, 42.3589*mm, 42.4425*mm, 42.4425*mm, 43.1688*mm, 46.0797*mm, 51.0241*mm, 54.6315*mm, 59.7845*mm, 63.1125*mm, 64.*mm, 63.473*mm, 59.9087*mm, 53.4484*mm, 45.4762*mm, 40.948*mm, 36.5916*mm, 31.0122*mm, 26.1237*mm, 19.9433*mm, 13.889*mm, 7.70708*mm, 0.*mm};


  G4Polycone *PM5Inches_extru = new G4Polycone("PM5Inches_extru",
			Phi_start,
			Phi_end,
			N_z_plane,
			z_plane_extru,
			r_inner_extru,
			r_outer_extru);



  G4SubtractionSolid* Glue_NEMO = new G4SubtractionSolid("Glue_NEMO", Glue_Full, PM5Inches_extru,G4Transform3D(DontRotate,G4ThreeVector(0, 0, -11.5*mm)));  

  

  LogicalN3EC_CouplingSphere = new G4LogicalVolume(Glue_NEMO,Cargille,"LogicalN3EC_CouplingSphere",0,0,0);




  return LogicalN3EC_CouplingSphere;
}



//************************************************************************************
//************************************************************************************
//************************* WRAP GLUED LIGHT GUIDE NEMO ******************************
//************************************************************************************
//************************************************************************************

G4LogicalVolume *Coupling::GetN3EC_WrapGluedLightGuide(){


  theScint = new Scintillator(path_bin+"OpticalModule.cfg");
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Cargille = scintProp->GetMaterial("cargille");
  grease = scintProp->GetMaterial("grease");

//**********************************************************************************
//Variables declaration and link between them **************************************
//**********************************************************************************

  N3EC_LightGuideRadius = theScint->GetN3EC_LightGuideRadius();
  N3EC_LightGuideHeight = theScint->GetN3EC_LightGuideHeight();
  N3EC_LightGuideCouplingRadius = theScint->GetN3EC_LightGuideCouplingRadius();
  N3EC_LightGuideStepHeight = theScint->GetN3EC_LightGuideStepHeight();
  AirGapMylar = theScint->GetAirGapMylar();
  MylarThickness = theScint->GetMylarThickness();
  N3EC_InterfaceLightGuideHeight = theScint->GetN3EC_InterfaceLightGuideHeight();


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
             N3EC_LightGuideRadius ,    //G4double  pRMax
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



//Creation of Wrap light guide volume

G4Tubs *Wrap_Light_Guide_Full;
Wrap_Light_Guide_Full = new G4Tubs("Wrap_Light_Guide_Full",
             0.*mm,  
             N3EC_LightGuideRadius + AirGapMylar,    
             (N3EC_LightGuideHeight + AirGapMylar + MylarThickness)/2,   
             0.,    
             twopi*rad); 


G4Tubs *Wrap_Light_Guide_Step;
Wrap_Light_Guide_Step = new G4Tubs("Wrap_Light_Guide_Step",
             0.,  
             N3EC_LightGuideCouplingRadius + AirGapMylar ,    
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
             N3EC_LightGuideCouplingRadius + AirGapMylar,    
             N3EC_InterfaceLightGuideHeight/2,   
             0.,    
             twopi*rad); 

  G4SubtractionSolid* Wrap_Light_Guide_NEMO = new G4SubtractionSolid("Wrap_Light_Guide_NEMO", Wrap_Subtraction_Light_Guide_NEMO, Interface_Light_Guide_Subtraction,G4Transform3D(DontRotate,G4ThreeVector(0, 0, (N3EC_LightGuideHeight + N3EC_InterfaceLightGuideHeight)/2)));  
 
   
  LogicalN3EC_WrapGluedLightGuide = new G4LogicalVolume(Wrap_Light_Guide_NEMO,grease,"LogicalN3EC_WrapGluedLightGuide",0,0,0);
  

    
  return LogicalN3EC_WrapGluedLightGuide;

  
}



//************************************************************************************
//************************************************************************************
//*****************WRAP GLUED INTERFACE LIGHT GUIDE NEMO *****************************
//************************************************************************************
//************************************************************************************

G4LogicalVolume *Coupling::GetN3EC_WrapGluedInterfaceLightGuide(){


  theScint = new Scintillator(path_bin+"OpticalModule.cfg");
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Cargille = scintProp->GetMaterial("cargille");
  grease = scintProp->GetMaterial("grease");

//**********************************************************************************
//Variables declaration and link between them **************************************
//**********************************************************************************

  N3EC_LightGuideRadius = theScint->GetN3EC_LightGuideRadius();
  N3EC_LightGuideHeight = theScint->GetN3EC_LightGuideHeight();
  N3EC_LightGuideCouplingRadius = theScint->GetN3EC_LightGuideCouplingRadius();
  N3EC_LightGuideStepHeight = theScint->GetN3EC_LightGuideStepHeight();
  AirGapMylar = theScint->GetAirGapMylar();
  MylarThickness = theScint->GetMylarThickness();
  N3EC_InterfaceLightGuideHeight = theScint->GetN3EC_InterfaceLightGuideHeight();

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
             N3EC_LightGuideCouplingRadius ,    //G4double  pRMax
             N3EC_InterfaceLightGuideHeight/2,    //G4double  pDz
             0.,    //G4double  pSPhi
             2*pi*rad);   //G4double  pDPhi



G4Tubs *Interface_Light_Guide_Wrap_Full;
Interface_Light_Guide_Wrap_Full = new G4Tubs("Interface_Light_Guide_Wrap_Full",
             0.*mm,   //G4double  pRMin
             N3EC_LightGuideCouplingRadius + AirGapMylar,    //G4double  pRMax
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



  G4SubtractionSolid* Interface_Light_Guide_NEMO = new G4SubtractionSolid("Interface_Light_Guide_NEMO", Interface_Light_Guide_NEMO_Full, Sphere_subtraction,G4Transform3D(Flip,G4ThreeVector(0, 0, N3EC_InterfaceLightGuideHeight/2 + N3EC_CouplingSphereRadius - N3EC_LightGuideCouplingDepth)));      

 
   
  LogicalN3EC_WrapGluedInterfaceLightGuide = new G4LogicalVolume(Interface_Light_Guide_NEMO,grease,"LogicalN3EC_WrapGluedInterfaceLightGuide",0,0,0);
  

    
  return LogicalN3EC_WrapGluedInterfaceLightGuide;

  
}







//************************************************************************************
//************************************************************************************
//********************************* COUPLING XWALL ***********************************
//************************************************************************************
//************************************************************************************

G4LogicalVolume *Coupling::GetSNXW_CouplingLightGuide(){

  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Cargille = scintProp->GetMaterial("cargille");
  grease = scintProp->GetMaterial("grease");
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");


//**********************************************************************************
//Variables declaration and link between them **************************************
//**********************************************************************************

  SNXW_LightGuideRadius = theScint->GetSNXW_LightGuideRadius();
  SNXW_LightGuideCouplingHeight = theScint->GetSNXW_LightGuideCouplingHeight();
  AirGapMylar = theScint->GetAirGapMylar();
  MylarThickness = theScint->GetMylarThickness();

  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);
  
  G4RotationMatrix Flip;
  Flip.rotateX(180.0*deg);


//**********************************************************************************
//Definition of volume**************************************************************
//**********************************************************************************

G4Tubs *Coupling_Glue;
Coupling_Glue = new G4Tubs("Coupling_Glue",
             0.*mm,   //G4double  pRMin
             SNXW_LightGuideRadius + AirGapMylar + MylarThickness,    //With air gap and mylar thickness to take into account the wrap around the light guide
             SNXW_LightGuideCouplingHeight/2,    //G4double  pDz
             0.,    //G4double  pSPhi
             2*pi*rad);   //G4double  pDPhi

  

  LogicalSNXW_CouplingScLightGuide = new G4LogicalVolume(Coupling_Glue,grease,"LogicalSNXW_CouplingScLightGuide",0,0,0);




  return LogicalSNXW_CouplingScLightGuide;
}



//************************************************************************************
//************************************************************************************
//********************************** GLUE XWALL **************************************
//************************************************************************************
//************************************************************************************

G4LogicalVolume *Coupling::GetSNXW_SphericalCoupling(){

  scintProp = new SN_OpticalSimMaterials("Materials.cfg");
  Cargille = scintProp->GetMaterial("cargille");
  grease = scintProp->GetMaterial("grease");
  theScint = new Scintillator("OpticalModule.cfg");


//**********************************************************************************
//Variables declaration and link between them **************************************
//**********************************************************************************

  SNXW_CouplingSphereRadius = theScint->GetSNXW_CouplingSphereRadius();
  SNXW_CouplingSphereDepth = theScint->GetSNXW_CouplingSphereDepth();
  GlueThickness = theScint->GetGlueThickness();
  PMTGlassRadius = theScint->GetPMTGlassRadius();


  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);
  
  G4RotationMatrix Flip;
  Flip.rotateX(180.0*deg);


//**********************************************************************************
//Definition of volume**************************************************************
//**********************************************************************************

G4Sphere *Glue_Full;
 Glue_Full = new G4Sphere("Glue_Full",
               0,
               SNXW_CouplingSphereRadius,
               0,
               twopi*rad ,
               0.,
               (acos((SNXW_CouplingSphereRadius-SNXW_CouplingSphereDepth)/SNXW_CouplingSphereRadius))*rad) ;


/*
 G4Sphere *Sphere_Photocathode_Subtraction;
 Sphere_Photocathode_Subtraction = new G4Sphere("Sphere_Photocathode_Subtraction",
               0,
               PMTGlassRadius,
               0,
               twopi ,
               0.,
               pi*rad) ;
*/


G4int N_z_plane = 30;
G4double Phi_start = 0.*deg;
G4double Phi_end = 2*180*deg;


const G4double z_plane_extru[] = {-89.*mm, -88.75*mm, -87.5*mm, -87.51*mm, -86.*mm, -85.4*mm, -84.6*mm, -84.*mm, -82.5187*mm, -7.54324*mm, -2.54324*mm, 2.45676*mm, 7.19658*mm, 11.0775*mm, 13.9256*mm, 21.252*mm, 30.2224*mm, 40.0277*mm, 50.0224*mm, 59.6324*mm, 67.6679*mm, 74.4474*mm, 77.7374*mm, 80.3403*mm, 82.8689*mm, 84.6888*mm, 86.3574*mm, 87.4988*mm, 88.4534*mm, 89.*mm};

const G4double r_inner_extru[]= {0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm};

const G4double r_outer_extru[]= {0.*mm, 36.5*mm, 37.*mm, 37.*mm, 37.5*mm, 40.8*mm, 41.75*mm, 42.*mm, 42.3589*mm, 42.4425*mm, 42.4425*mm, 43.1688*mm, 46.0797*mm, 51.0241*mm, 54.6315*mm, 59.7845*mm, 63.1125*mm, 64.*mm, 63.473*mm, 59.9087*mm, 53.4484*mm, 45.4762*mm, 40.948*mm, 36.5916*mm, 31.0122*mm, 26.1237*mm, 19.9433*mm, 13.889*mm, 7.70708*mm, 0.*mm};


  G4Polycone *PM5Inches_extru = new G4Polycone("PM5Inches_extru",
			Phi_start,
			Phi_end,
			N_z_plane,
			z_plane_extru,
			r_inner_extru,
			r_outer_extru);



  G4SubtractionSolid* Glue_XWALL = new G4SubtractionSolid("Glue_XWALL", Glue_Full, PM5Inches_extru,G4Transform3D(DontRotate,G4ThreeVector(0, 0, -11.5*mm)));  

  

  LogicalSNXW_CouplingSphere = new G4LogicalVolume(Glue_XWALL,Cargille,"LogicalSNXW_CouplingSphere",0,0,0);




  return LogicalSNXW_CouplingSphere;
}




//************************************************************************************
//************************************************************************************
//************************ WRAP GLUED  LIGHT GUIDE XWALL *****************************
//************************************************************************************
//************************************************************************************

G4LogicalVolume *Coupling::GetSNXW_WrapGluedLightGuide(){


  theScint = new Scintillator(path_bin+"OpticalModule.cfg");
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Cargille = scintProp->GetMaterial("cargille");
  grease = scintProp->GetMaterial("grease");

//**********************************************************************************
//Variables declaration and link between them **************************************
//**********************************************************************************

  SNXW_LightGuideRadius = theScint->GetSNXW_LightGuideRadius();
  SNXW_LightGuideHeight = theScint->GetSNXW_LightGuideHeight();
  SNXW_CouplingSphereRadius = theScint->GetSNXW_CouplingSphereRadius();
  SNXW_CouplingSphereDepth = theScint->GetSNXW_CouplingSphereDepth();
  AirGapMylar = theScint->GetAirGapMylar();
  MylarThickness = theScint->GetMylarThickness();


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
             SNXW_LightGuideRadius ,    //G4double  pRMax
             SNXW_LightGuideHeight/2,    //G4double  pDz
             0.,    //G4double  pSPhi
             2*pi*rad);   //G4double  pDPhi



G4Tubs *Light_Guide_Wrap_Full;
Light_Guide_Wrap_Full = new G4Tubs("Light_Guide_Wrap_Full",
             0.*mm,   //G4double  pRMin
             SNXW_LightGuideRadius + AirGapMylar,    //G4double  pRMax
             SNXW_LightGuideHeight/2,    //G4double  pDz
             0.,    //G4double  pSPhi
             2*pi*rad);   //G4double  pDPhi


  G4SubtractionSolid* Light_Guide_XWALL_Full = new G4SubtractionSolid("Light_Guide_XWALL_Full", Light_Guide_Wrap_Full, Light_Guide_Full,G4Transform3D(Flip,G4ThreeVector(0, 0, 0)));    



G4Sphere *Sphere_subtraction;
  Sphere_subtraction = new G4Sphere("Sphere_subtraction",
               0.,
               SNXW_CouplingSphereRadius,
               0.,
               twopi*rad ,
               0.,
               0.35*pi*rad) ;



  G4SubtractionSolid* Light_Guide_XWALL = new G4SubtractionSolid("Light_Guide_XWALL", Light_Guide_XWALL_Full, Sphere_subtraction,G4Transform3D(Flip,G4ThreeVector(0, 0, SNXW_LightGuideHeight/2 + SNXW_CouplingSphereRadius - SNXW_CouplingSphereDepth)));      

 
   
  LogicalSNXW_WrapGluedLightGuide = new G4LogicalVolume(Light_Guide_XWALL,grease,"LogicalSNXW_WrapGluedLightGuide",0,0,0);
  

    
  return LogicalSNXW_WrapGluedLightGuide;

  
}



G4LogicalVolume *Coupling::GetAluminiumPlateBordeaux(){

  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Alu = scintProp->GetMaterial("Alu");
  

				      
  // Define some rotations
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);
  G4RotationMatrix Flip;
  Flip.rotateX(180.0*deg);

  
 G4Box *AluminiumPlate = new G4Box("AluminiumPlate",
				170*mm/2,
				110*mm/2,
				3*mm/2);   
							     
  // Llogical volume: material RTV  
  LogicalAluminiumPlateBordeaux = new G4LogicalVolume(AluminiumPlate,Alu,"LogicalAluminiumPlateBordeaux",0,0,0);


  

  
  return LogicalAluminiumPlateBordeaux;
}


G4LogicalVolume *Coupling::GetN3DC_8InchesSphericalCoupling(){

  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Cargille = scintProp->GetMaterial("cargille");
  
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");



  //TEST COUPLING RTV615

 // Read in and define dimensions
  SNMW_CouplingSphereRadius = theScint->GetSNMW_CouplingRadius();
  SNMW_CouplingSphereDepth = theScint->GetSNMW_CouplingDepth();
  PMTGlassRadius = theScint->GetPMTGlassRadius();
  GlueThickness = theScint->GetGlueThickness();
  //  G4cout << " Coupling angle = " << acos((CouplingSphereRadius-CouplingDepth)/CouplingSphereRadius) << G4endl;

  // Define translations used for adding or subtracting volumes
  G4double CouplingSphereSubtraction = SNMW_CouplingSphereRadius - PMTGlassRadius - GlueThickness;
				      
  // Define some rotations
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);
  G4RotationMatrix Flip;
  Flip.rotateX(180.0*deg);

  
  // ####################### COUPLING SPHERE #######################
G4int N_z_plane_glue = 14;
G4double Phi_start = 0.*deg;
G4double Phi_end = 2*180*deg;

const G4double z_plane_glue[] = {68.3814*mm, 76.581*mm, 83.5154*mm, 89.9187*mm, 95.2918*mm, 99.6867*mm, 103.405*mm, 106.303*mm, 107.669*mm, 108.556*mm, 108.658*mm, 109.334*mm, 109.825*mm, 110.*mm};

const G4double r_inner_glue[]= {0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm};


const G4double r_outer_glue[]= {92.7321*mm, 86.4727*mm, 78.9557*mm, 70.9368*mm, 62.0639*mm, 52.8971*mm, 43.1194*mm, 32.596*mm, 27.291*mm, 22.8952*mm, 22.3897*mm, 15.4348*mm, 10.53213*mm, 2.*mm};

  G4Polycone *Glue8Inches_N3DC = new G4Polycone("Glue8Inches_N3DC",
			Phi_start,
			Phi_end,
			N_z_plane_glue,
			z_plane_glue,
			r_inner_glue,
			r_outer_glue);




  // ####################### PMT GLASS SOLID SUBTRACTION #######################
G4int N_z_plane = 36;


const G4double z_plane_extrusion[] = {-110.*mm, -109.829*mm, -108.647*mm, -108.65*mm, -107.416*mm, -106.481*mm, -103.084*mm, -98.6401*mm, -43.6458*mm, -33.8076*mm, -27.3606*mm, -22.9623*mm, -18.0893*mm, -12.2671*mm, -5.65007*mm, 2.09728*mm, 10.6724*mm, 20.0733*mm, 29.9204*mm, 39.9182*mm, 49.8607*mm, 59.4345*mm, 68.3814*mm, 76.581*mm, 83.5154*mm, 89.9187*mm, 95.2918*mm, 99.6867*mm, 103.405*mm, 106.303*mm, 107.669*mm, 108.556*mm, 108.658*mm, 109.334*mm, 109.825*mm, 110.*mm};

const G4double r_inner_extrusion[]= {0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm};

const G4double r_outer_extrusion[]= {0.*mm, 32.0648*mm, 35.6705*mm, 35.6705*mm, 37.4533*mm, 38.4624*mm, 40.5955*mm, 41.5084*mm, 41.9043*mm, 45.2687*mm, 54.2783*mm, 63.2852*mm, 71.4126*mm, 79.2565*mm, 86.3265*mm, 92.3066*mm, 96.803*mm, 99.5055*mm, 100.905*mm, 100.846*mm, 99.3918*mm, 95.9735*mm, 90.7321*mm, 84.4727*mm, 76.9557*mm, 68.9368*mm, 60.0639*mm, 50.8971*mm, 41.1194*mm, 30.596*mm, 25.291*mm, 20.8952*mm, 20.3897*mm, 13.4348*mm, 8.53213*mm, 0.*mm};

  G4Polycone *PMTGlass8InchesExtrusion = new G4Polycone("PMTGlass8InchesExtrusion",
			Phi_start,
			Phi_end,
			N_z_plane,
			z_plane_extrusion,
			r_inner_extrusion,
			r_outer_extrusion);  


  // Subtract smaller sphere from the larger to make shell 
  G4SubtractionSolid* FinalCoupling8Inches_N3DC = new G4SubtractionSolid("FinalCoupling8Inches", 
							     Glue8Inches_N3DC,
							     PMTGlass8InchesExtrusion,
							     G4Transform3D(DontRotate,G4ThreeVector(0, 0, 0)));  
							     
  // Llogical volume: material RTV  
  LogicalN3DC_8InchesCouplingSphere = new G4LogicalVolume(FinalCoupling8Inches_N3DC,Cargille,"LogicalN3DC_8InchesCouplingSphere",0,0,0);


  

  
  return LogicalN3DC_8InchesCouplingSphere;
}
