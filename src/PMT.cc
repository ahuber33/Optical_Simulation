/// PMT.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu>
/// Copyright: 2010 (C) NEMO3/SuperNEMO - University of Texas at Austin

#include "SN_OpticalSimMaterials.hh"
#include "PMT.hh"
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
#include <G4Polycone.hh>
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"

using namespace CLHEP;

const G4String PMT::path_bin = "/home/local1/Optical_Simulation_Arnaud/bin/";

// ***********************
// Constructor
// ***********************
PMT::PMT(){}
// ***********************
// Destructor
// ***********************
PMT::~PMT(){
  if(scintProp) { delete scintProp; scintProp = 0; }
  if(Vacuum) { delete Vacuum; Vacuum = 0; }
  if(Borosilicate_Glass) { delete Borosilicate_Glass; Borosilicate_Glass = 0; }
  if(theScint) { delete theScint; theScint = 0; }
}
// #######################
// SuperNEMO PMT
// #######################

// ***********************
// Builds R6594 Glass
// ***********************
G4LogicalVolume *PMT::GetSphericalPMTGlass(){
  // Materials properties for PMT
  Borosilicate_Glass = scintProp->GetMaterial("bs_glass");
  Vacuum = scintProp->GetMaterial("Vacuum");
  Cargille = scintProp->GetMaterial("cargille");
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");

  // Read in and define dimensions
  PMTGlassRadius = theScint->GetPMTGlassRadius();
  PMTGlassThickness = theScint->GetPMTGlassThickness();
  PMTRearGlassRadius = theScint->GetPMTRearGlassRadius();
  PMTGlassTubeRadius = theScint->GetPMTGlassTubeRadius();
  PMTGlassTubeHeight = theScint->GetPMTGlassTubeHeight();


  // Define some rotations
  G4RotationMatrix Rotate;
  Rotate.rotateX(180.0*deg);
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);
  
  // ####################### PMT GLASS SOLID #######################
  // PMT made of two spheres: one for the front face and one for the rear face
  // Front face sphere of PMT glass
  G4Sphere *SolidPMTGlassFace = new G4Sphere("SolidPMTGlassFace", 
					     PMTGlassRadius-PMTGlassThickness, // Inner radius
					     PMTGlassRadius, // Outer radius 
					     0, 
					     twopi*rad, // 0 to 2pi in theta
					     0, 
					     0.364140*pi*rad); // Value calculated to mate with rear glass sphere

  // Rear sphere of PMT glass
  G4Sphere *SolidPMTGlassRear = new G4Sphere("SolidPMTGlassRear", 
					     PMTRearGlassRadius-PMTGlassThickness,
					     PMTRearGlassRadius, 
					     0, 
					     twopi*rad, 
					     0, 
					     0.53470*pi*rad); // Value calculated to mate with face glass sphere

  // Union to make whole PMT bulb: front + rear face -> center of union at center of front sphere
  G4UnionSolid* SolidPMTBulb = new G4UnionSolid("SolidPMTBulb",SolidPMTGlassFace,SolidPMTGlassRear,G4Transform3D(Rotate,G4ThreeVector(0,0,22.919108*mm)));   

  // Subtraction of rear of bulb to attach dynode tube
  G4Tubs *TubeSubtraction = new G4Tubs("TubeSubtraction",
				       0, // Inner radius
				       PMTGlassTubeRadius, // Outer radius
				       90, // Height - arbitrary since its just used for subtraction
				       0,
				       2*pi*rad); // Full span in theta
  // Performs subtraction
  G4SubtractionSolid* FinalBulb = new G4SubtractionSolid("FinalBulb",SolidPMTBulb,TubeSubtraction,G4Transform3D(DontRotate,G4ThreeVector(0,0,-100*mm))); 
  
  // Rear dynode glass tube
  G4Tubs *PMTGlassTube = new G4Tubs("PMTGlassTube",
				    PMTGlassTubeRadius-PMTGlassThickness,
				    PMTGlassTubeRadius,
				    PMTGlassTubeHeight,
				    0,
				    2*pi*rad);

  // Full PMT glass structure: bulb + tube -> center of union at center of bulb = center of front sphere
  G4UnionSolid* FullPMTGlass = new G4UnionSolid("FullPMTGlass",FinalBulb,PMTGlassTube,G4Transform3D(DontRotate,G4ThreeVector(0,0,-67.06335*mm)));  

  // Logical PMT glass: material borosilicate glass
  LogicalSphericalPMTGlass = new G4LogicalVolume(FullPMTGlass,Borosilicate_Glass,"LogicalSphericalPMTGlass",0,0,0);
  
  G4VisAttributes *blue;
  blue = new G4VisAttributes(G4Colour(0,0,1));
   
  return LogicalSphericalPMTGlass;
}

// ***********************
// Builds R6594 tube portion
// ***********************
G4LogicalVolume *PMT::GetPMTTubeGlass(){
  // Materials properties for PMT
  Borosilicate_Glass = scintProp->GetMaterial("bs_glass");
  Vacuum = scintProp->GetMaterial("Vacuum");
  Cargille = scintProp->GetMaterial("cargille");
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");

  // Read in and define dimensions
  PMTGlassThickness = theScint->GetPMTGlassThickness();
  PMTGlassTubeRadius = 52.*mm;   
  PMTGlassTubeHeight = 50.*mm;  

  // Define some rotations
  G4RotationMatrix Rotate;
  Rotate.rotateX(180.0*deg);
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);

  // ####################### PMT GLASS TUBE SOLID #######################
  // Solid PMT tube
  G4Tubs *PMTGlassTube = new G4Tubs("PMTGlassTube",
				    PMTGlassTubeRadius-PMTGlassThickness,
				    PMTGlassTubeRadius,
				    PMTGlassTubeHeight,
				    0,
				    2*pi*rad);

  // Logical PMT tube:  material borosilicate glass
  LogicalPMTGlassTube = new G4LogicalVolume(PMTGlassTube,Borosilicate_Glass,"LogicalPMTGlassTube",0,0,0);
  
  G4VisAttributes *blue;
  blue = new G4VisAttributes(G4Colour(0,0,1));
  
  return LogicalPMTGlassTube;
}

// ***********************
// Builds R6594 photocathode
// ***********************
G4LogicalVolume *PMT::GetPhotocathode(){
  // Materials properties for PMT
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Vacuum = scintProp->GetMaterial("Vacuum");
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");

  // Read in and define dimensions
  PMTGlassRadius = theScint->GetPMTGlassRadius();
  PMTGlassThickness = theScint->GetPMTGlassThickness();
  PMTPhotocathodeThickness = theScint->GetPMTPhotocathodeThickness();


  // Define some rotations
  G4RotationMatrix Rotate;
  Rotate.rotateX(180.0*deg);
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);

  // ####################### PMT GLASS TUBE SOLID #######################
  // Solid PMT photocathode sphere
  G4Sphere *SolidPhotocathode = new G4Sphere("SolidPhotocathode", 
					     PMTPhotocathodeRadius-PMTPhotocathodeThickness-PMTGlassThickness, 
					     PMTPhotocathodeRadius-PMTGlassThickness, 
					     0, 
					     twopi*rad, 
					     0, 
					     0.36*pi*rad );
  
  //G4LogicalVolume *logical_spherical_PMT;
  LogicalPhotocathode = new G4LogicalVolume(SolidPhotocathode,Vacuum,"LogicalPhotocathode",0,0,0);
  
  return LogicalPhotocathode;
}

// #####################################################################
// ############ 5 INCHES PMT GEOMETRY (from Falaise BiPo) ##############
// #####################################################################



G4LogicalVolume *PMT::Get5InchesPMTGlass(){
  // Materials properties for PMT
  Borosilicate_Glass = scintProp->GetMaterial("bs_glass");
  Vacuum = scintProp->GetMaterial("Vacuum");
  Cargille = scintProp->GetMaterial("cargille");
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");



  // Define some rotations
  G4RotationMatrix Rotate;
  Rotate.rotateX(180.0*deg);
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);
  
  // ####################### PMT GLASS SOLID #######################
G4int N_z_plane = 30;
G4double Phi_start = 0.*deg;
G4double Phi_end = 2*180*deg;


 const G4double z_plane[] = {-89.*mm, -88.75*mm, -87.5*mm, -87.51*mm, -86.*mm, -85.4*mm, -84.6*mm, -84.*mm, -82.5187*mm, -7.54324*mm, -2.54324*mm, 2.45676*mm, 7.19658*mm, 11.0775*mm, 13.9256*mm, 21.252*mm, 30.2224*mm, 40.0277*mm, 50.0224*mm, 59.6324*mm, 67.6679*mm, 74.4474*mm, 77.7374*mm, 80.3403*mm, 82.8689*mm, 84.6888*mm, 86.3574*mm, 87.4988*mm, 88.4534*mm, 89.*mm};

const G4double r_inner[]= {0.*mm, 0.*mm, 0.*mm, 35.5*mm, 36.*mm, 36.3*mm, 36.7*mm, 40.5*mm, 40.7936*mm, 40.9457*mm, 40.9462*mm, 41.6396*mm, 44.1647*mm, 48.4733*mm, 52.397*mm, 58.0089*mm, 61.56*mm, 62.4995*mm, 61.9301*mm, 58.1609*mm, 51.1608*mm, 43.0239*mm, 38.0377*mm, 33.0694*mm, 26.7013*mm, 20.3997*mm, 10.8782*mm, 0.*mm, 0.*mm, 0.*mm};

const G4double r_outer[]= {0.*mm, 36.5*mm, 37.*mm, 37.*mm, 37.5*mm, 40.8*mm, 41.75*mm, 42.*mm, 42.3589*mm, 42.4425*mm, 42.4425*mm, 43.1688*mm, 46.0797*mm, 51.0241*mm, 54.6315*mm, 59.7845*mm, 63.1125*mm, 64.*mm, 63.473*mm, 59.9087*mm, 53.4484*mm, 45.4762*mm, 40.948*mm, 36.5916*mm, 31.0122*mm, 26.1237*mm, 19.9433*mm, 13.889*mm, 7.70708*mm, 0.*mm};

  G4Polycone *PMTGlass5Inches = new G4Polycone("PMTGlass5Inches",
			Phi_start,
			Phi_end,
			N_z_plane,
			z_plane,
			r_inner,
			r_outer);  

  // Logical PMT glass: material borosilicate glass
  Logical5InchesPMTGlass = new G4LogicalVolume(PMTGlass5Inches,Borosilicate_Glass,"Logical5InchesPMTGlass",0,0,0);

   
  return Logical5InchesPMTGlass;
}





G4LogicalVolume *PMT::Get5InchesPhotocathode(){
  // Materials properties for PMT
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Vacuum = scintProp->GetMaterial("Vacuum");
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");

  // Read in and define dimensions
  PMTGlassRadius = theScint->GetPMTGlassRadius();
  PMTGlassThickness = theScint->GetPMTGlassThickness();
  PMTPhotocathodeThickness = theScint->GetPMTPhotocathodeThickness();


  // Define some rotations
  G4RotationMatrix Rotate;
  Rotate.rotateX(180.0*deg);
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);

  // ####################### PMT GLASS TUBE SOLID #######################
  // Solid PMT photocathode sphere
G4int N_z_plane = 13;
G4double Phi_start = 0.*deg;
G4double Phi_end = 2*180*deg;


 const G4double z_plane[] = {40.0277*mm, 50.0224*mm, 59.6324*mm, 67.6679*mm, 74.4474*mm, 77.7374*mm, 80.3403*mm, 82.8689*mm, 84.6888*mm, 86.3574*mm, 87.4988*mm, 88.4534*mm, 89.*mm};

const G4double r_inner[]= {61.4995*mm, 60.9301*mm, 57.1609*mm, 50.1608*mm, 42.0239*mm, 37.0377*mm, 32.0694*mm, 25.7013*mm, 19.3997*mm, 9.8782*mm, 0.*mm, 0.*mm, 0.*mm};

const G4double r_outer[]= {62.4995*mm, 61.9301*mm, 58.1609*mm, 51.1608*mm, 43.0239*mm, 38.0377*mm, 33.0694*mm, 26.7013*mm, 20.3997*mm, 10.8782*mm, 0.*mm, 0.*mm, 0.*mm};


  G4Polycone *Photocathode5Inches = new G4Polycone("Photocathode5Inches",
			Phi_start,
			Phi_end,
			N_z_plane,
			z_plane,
			r_inner,
			r_outer);  
  
  //G4LogicalVolume *logical_spherical_PMT;
  Logical5InchesPhotocathode = new G4LogicalVolume(Photocathode5Inches,Vacuum,"Logical5InchesPhotocathode",0,0,0);
  
  return Logical5InchesPhotocathode;
}









// #####################################################################
// ###################### 8 INCHES PMT GEOMETRY ########################
// #####################################################################



G4LogicalVolume *PMT::Get8InchesPMTGlass(){
  // Materials properties for PMT
  Borosilicate_Glass = scintProp->GetMaterial("bs_glass");
  Vacuum = scintProp->GetMaterial("Vacuum");
  Cargille = scintProp->GetMaterial("cargille");
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");

  // Read in and define dimensions
  PMTGlassRadius = theScint->GetPMTGlassRadius();
  PMTGlassThickness = theScint->GetPMTGlassThickness();
  PMTRearGlassRadius = theScint->GetPMTRearGlassRadius();
  PMTGlassTubeRadius = theScint->GetPMTGlassTubeRadius();
  PMTGlassTubeHeight = theScint->GetPMTGlassTubeHeight();


  // Define some rotations
  G4RotationMatrix Rotate;
  Rotate.rotateX(180.0*deg);
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);
  
  // ####################### PMT GLASS SOLID #######################
G4int N_z_plane = 36;
G4double Phi_start = 0.*deg;
G4double Phi_end = 2*180*deg;


const G4double z_plane[] = {-110.*mm, -109.829*mm, -108.647*mm, -108.65*mm, -107.416*mm, -106.481*mm, -103.084*mm, -98.6401*mm, -43.6458*mm, -33.8076*mm, -27.3606*mm, -22.9623*mm, -18.0893*mm, -12.2671*mm, -5.65007*mm, 2.09728*mm, 10.6724*mm, 20.0733*mm, 29.9204*mm, 39.9182*mm, 49.8607*mm, 59.4345*mm, 68.3814*mm, 76.581*mm, 83.5154*mm, 89.9187*mm, 95.2918*mm, 99.6867*mm, 103.405*mm, 106.303*mm, 107.669*mm, 108.556*mm, 108.658*mm, 109.334*mm, 109.825*mm, 110.*mm};

const G4double r_inner[]= {0.*mm, 0.*mm, 0.*mm, 31.*mm, 35.*mm, 36.0841*mm, 38.8759*mm, 39.9976*mm, 40.4027*mm, 43.5152*mm, 51.0925*mm, 59.8796*mm, 68.6233*mm, 76.826*mm, 84.2188*mm, 90.4598*mm, 95.1553*mm, 97.9604*mm, 99.4007*mm, 99.3384*mm, 97.8127*mm, 94.2893*mm, 88.9034*mm, 82.4206*mm, 74.5809*mm, 66.2397*mm, 56.6831*mm, 46.8337*mm, 35.3909*mm, 23.8773*mm, 13.4135*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm};


/*const G4double r_inner[]= {0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm};
*/

const G4double r_outer[]= {0.*mm, 32.0648*mm, 35.6705*mm, 35.6705*mm, 37.4533*mm, 38.4624*mm, 40.5955*mm, 41.5084*mm, 41.9043*mm, 45.2687*mm, 54.2783*mm, 63.2852*mm, 71.4126*mm, 79.2565*mm, 86.3265*mm, 92.3066*mm, 96.803*mm, 99.5055*mm, 100.905*mm, 100.846*mm, 99.3918*mm, 95.9735*mm, 90.7321*mm, 84.4727*mm, 76.9557*mm, 68.9368*mm, 60.0639*mm, 50.8971*mm, 41.1194*mm, 30.596*mm, 25.291*mm, 20.8952*mm, 20.3897*mm, 13.4348*mm, 8.53213*mm, 0.*mm};

  G4Polycone *PMTGlass8Inches = new G4Polycone("PMTGlass8Inches",
			Phi_start,
			Phi_end,
			N_z_plane,
			z_plane,
			r_inner,
			r_outer);  

  // Logical PMT glass: material borosilicate glass
  Logical8InchesPMTGlass = new G4LogicalVolume(PMTGlass8Inches,Borosilicate_Glass,"Logical8InchesPMTGlass",0,0,0);

   
  return Logical8InchesPMTGlass;
}


G4LogicalVolume *PMT::Get8InchesPhotocathode(){
  // Materials properties for PMT
  //scintProp = new SN_OpticalSimMaterials("Materials.cfg");
  Vacuum = scintProp->GetMaterial("Vacuum");
  //theScint = new Scintillator("OpticalModule.cfg");

  // Read in and define dimensions
 // PMTGlassRadius = theScint->GetPMTGlassRadius();
 // PMTGlassThickness = theScint->GetPMTGlassThickness();
 // PMTPhotocathodeThickness = theScint->GetPMTPhotocathodeThickness();


  // Define some rotations
  G4RotationMatrix Rotate;
  Rotate.rotateX(180.0*deg);
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);

  // ####################### PMT GLASS TUBE SOLID #######################
  // Solid PMT photocathode sphere
G4int N_z_plane = 18;
G4double Phi_start = 0.*deg;
G4double Phi_end = 2*180*deg;


const G4double z_plane[] = {29.9204*mm, 39.9182*mm, 49.8607*mm, 59.4345*mm, 68.3814*mm, 76.581*mm, 83.5154*mm, 89.9187*mm, 95.2918*mm, 99.6867*mm, 103.405*mm, 106.303*mm, 107.669*mm, 108.556*mm, 108.658*mm, 109.334*mm, 109.825*mm, 110.*mm};

const G4double r_inner[]= {98.4007*mm, 98.3384*mm, 96.8127*mm, 93.2893*mm, 87.9034*mm, 81.4206*mm, 73.5809*mm, 65.2397*mm, 55.6831*mm, 45.8337*mm, 34.3909*mm, 22.8773*mm, 12.4135*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm};

const G4double r_outer[]= {99.4007*mm, 99.3384*mm, 97.8127*mm, 94.2893*mm, 88.9034*mm, 82.4206*mm, 74.5809*mm, 66.2397*mm, 56.6831*mm, 46.8337*mm, 35.3909*mm, 23.8773*mm, 13.4135*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm};

  G4Polycone *Photocathode8Inches = new G4Polycone("Photocathode8Inches",
			Phi_start,
			Phi_end,
			N_z_plane,
			z_plane,
			r_inner,
			r_outer);  
  
  //G4LogicalVolume *logical_spherical_PMT;
  Logical8InchesPhotocathode = new G4LogicalVolume(Photocathode8Inches,Vacuum,"Logical8InchesPhotocathode",0,0,0);
  
  return Logical8InchesPhotocathode;
}






// #####################################################################
// ####################### OTHER PMT GEOMETRIES ########################
// #####################################################################

// Creates tube shaped PMT?
G4LogicalVolume *PMT::GetTubePMT(){
  
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Vacuum = scintProp->GetMaterial("Vacuum");
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");
  
  // Read in PMT radius, define other useful values
  PMTGlassRadius = theScint->GetPMTGlassRadius();
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);
  
  // Create solid PMT tube
  G4Tubs *SolidTubePMT = new G4Tubs("SolidTubePMT",0,PMTGlassRadius,0.5*cm,0,twopi*rad);
  // Create logical PMT tube
  LogicalTubePMT = new G4LogicalVolume(SolidTubePMT,Vacuum,"LogicalTubePMT",0,0,0);
  
  // Visualization options
  G4VisAttributes *invis = new G4VisAttributes( G4Colour(255/255. ,255/255. ,255/255. ));
  invis->SetVisibility(false);
  G4VisAttributes *blue = new G4VisAttributes(G4Colour(0,0,1));
  G4VisAttributes *black = new G4VisAttributes(G4Colour(0,0,0)); 
  blue->SetForceWireframe(true);
  LogicalTubePMT->SetVisAttributes(black);
  
  return LogicalTubePMT;
}

// Creates tube shaped PMT glass?
G4LogicalVolume *PMT::GetPMTGlass(){
  
  scintProp = new SN_OpticalSimMaterials(path_bin+"Materials.cfg");
  Borosilicate_Glass = scintProp->GetMaterial("borosilicate_glass");
  theScint = new Scintillator(path_bin+"OpticalModule.cfg");
  
  // Read in PMT radius, define other useful values
  PMTGlassRadius = theScint->GetPMTGlassRadius();
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);
  
  // Create solid PMT glass tube
  G4Tubs *SolidTubePMTGlass = new G4Tubs("SolidTubePMTGlass",0,PMTGlassRadius,10.0*cm,0,twopi*rad);
  // Create logical PMT glass tube
  LogicalTubePMTGlass = new G4LogicalVolume(SolidTubePMTGlass,Borosilicate_Glass,"logical_PMT_glass",0,0,0);
  
  // Visualization options
  G4VisAttributes *invis = new G4VisAttributes( G4Colour(255/255. ,255/255. ,255/255. ));
  invis->SetVisibility(false);
  G4VisAttributes *blue = new G4VisAttributes(G4Colour(0,0,1));
  LogicalTubePMTGlass->SetVisAttributes(blue);
  
  return LogicalTubePMTGlass;
}
