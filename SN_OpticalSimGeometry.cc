// SN_OpticalSimGeometry.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> 
/// Copyright: 2010 (C) NEMO3/SuperNEMO - University of Texas at Austin

#include "SN_OpticalSimGeometry.hh"
#include "SN_OpticalSimRunAction.hh"
#include "SN_OpticalSimMaterials.hh"
#include "Scintillator.hh"
#include "Coupling.hh"
#include "PMT.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4UnitsTable.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4MaterialTable.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4RunManager.hh"
#include "G4Transform3D.hh"
#include "G4SurfaceProperty.hh"
#include "globals.hh"
#include <fstream>
#include <iostream>

using namespace CLHEP;

// Constructor
SN_OpticalSimGeometry::SN_OpticalSimGeometry() {  
}

// Destructor
SN_OpticalSimGeometry::~SN_OpticalSimGeometry()
{
  /*  
  if(scintProp) { delete scintProp; scintProp = 0; }
  if(Vacuum) { delete Vacuum; Vacuum = 0; }
  //  if(invis) { delete invis; invis = 0; }
  //  if(green) { delete green; green = 0; }
  if(theScint) { delete theScint; theScint = 0; }
  //  if(sealant) { delete sealant; sealant = 0; }
  //  if(holder) { delete holder; holder = 0; }
  */  
  // these seem to crash the system!!!
}

// Main Function: Builds Full block, coupling, and PMT geometries
G4VPhysicalVolume* SN_OpticalSimGeometry::Construct( ){
  // Initialize scint classes
  scintProp = new SN_OpticalSimMaterials("Materials.cfg");
  Vacuum = scintProp->GetMaterial("Vacuum");
  VacuumWorld = scintProp->GetMaterial("VacuumWorld");
  theScint = new Scintillator("OpticalModule.cfg");
  
  // ***********************
  // Visualization Colors
  // ***********************
  // Create some colors for visualizations
  invis = new G4VisAttributes( G4Colour(255/255. ,255/255. ,255/255. ));
  invis->SetVisibility(false);

  white = new G4VisAttributes(G4Colour(1,1,1,0.9)); // Sets the color (can be looked up online)
  //  white->SetForceWireframe(true); // Sets to wire frame mode for coloring the volume
  //  white->SetForceSolid(true); // Sets to solid mode for coloring the volume
  white->SetVisibility(true); // Makes color visible in visualization
  
  gray = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.9));
  //  gray->SetForceWireframe(true);
  //  gray->SetForceSolid(true);
  gray->SetVisibility(true);
    
  black = new G4VisAttributes(G4Colour(0,0,0,0.9));
  //  black->SetForceWireframe(true);
  //  black->SetForceSolid(true);
  black->SetVisibility(true);
  
  red = new G4VisAttributes(G4Colour(1,0,0,0.8));
  //  red->SetForceWireframe(true);
  red->SetForceSolid(true);
  red->SetVisibility(true);
  
  orange = new G4VisAttributes(G4Colour(1,0.5,0,0.5));
  //  orange->SetForceWireframe(true);
  orange->SetForceSolid(true);
  orange->SetVisibility(true);
  
  yellow = new G4VisAttributes(G4Colour(1,1,0,0.5));
  //  yellow->SetForceWireframe(true);
  yellow->SetForceSolid(true);
  yellow->SetVisibility(true);    
  
  green = new G4VisAttributes(G4Colour(0,1,0,0.5));
  //  green->SetForceWireframe(true);
  green->SetForceSolid(true);
  green->SetVisibility(true);
  
  cyan = new G4VisAttributes(G4Colour(0,1,1,0.9));
  //  cyan->SetForceWireframe(true);
  cyan->SetForceSolid(true);
  cyan->SetVisibility(true);

  blue = new G4VisAttributes(G4Colour(0,0,1,0.5));
  //  blue->SetForceWireframe(true);
  blue->SetForceSolid(true);
  blue->SetVisibility(true);
  
  magenta = new G4VisAttributes(G4Colour(1,0,1,0.9));
  //  magenta->SetForceWireframe(true);
  magenta->SetForceSolid(true);
  magenta->SetVisibility(true);  
  
  // ***********************
  // Various dimensions
  // ***********************
  //Scint
  SNMW_ScintStepHeight = theScint->GetSNMW_ScintStepHeight();
  SNMW_ScintStepWidth = theScint->GetSNMW_ScintStepWidth();
  SNMW_ScintBodyHeight = theScint->GetSNMW_ScintBodyHeight();
  SNMW_ScintBodyWidth = theScint->GetSNMW_ScintBodyWidth();
  SNMW_FullScintHeight = theScint->GetSNMW_FullScintHeight();
  SNMW_CouplingSphereRadius = theScint->GetSNMW_CouplingRadius();
  SNMW_CouplingSphereDepth = theScint->GetSNMW_CouplingDepth();
  // Wrapping
  TeflonThickness = theScint->GetTeflonThickness();
  TeflonGap = theScint->GetAirGapTeflon();
  MylarThickness = theScint->GetMylarThickness();
  MylarGap = theScint->GetAirGapMylar();
  // Glue
  GlueThickness = theScint->GetGlueThickness();
  // PMT
  PMTGlassRadius = theScint->GetPMTGlassRadius();
  PMTGlassThickness = theScint->GetPMTGlassThickness();
  PMTPhotocathodeThickness = theScint->GetPMTPhotocathodeThickness();
  PMTPhotocathodeRadius = PMTGlassRadius-PMTPhotocathodeThickness; 
  // Light guide if used for different geometries (***Note: should set up a boolean switch to account for light guide properties when true)
  //    LightGuideHeight = theScint->GetLightGuideHeight();
  //    LightGuideRadius = theScint->GetLightGuideRadius();
  //    LightGuideDepth = theScint->GetLightGuideDepth();
  
  // Define common rotations
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);
  G4RotationMatrix Flip;
  Flip.rotateX(180*deg);

  // ***********************
  // Volumes
  // ***********************
  // Create World Volume
  // This is just a big box to place all other logical volumes inside
  G4Box *SolidWorld = new G4Box("SolidWorld", 100*cm, 100*cm, 100*cm );
  LogicalWorld = new G4LogicalVolume(SolidWorld, Vacuum,"LogicalWorld",0,0,0);
  LogicalWorld->SetVisAttributes(invis);

  // Place the world volume: center of world at origin (0,0,0)    
  PhysicalWorld = new G4PVPlacement(G4Transform3D
				    (DontRotate,G4ThreeVector(0,0,0)),
				    "PhysicalWorld",LogicalWorld,NULL,false,0); 
 
  //*********************** 
  // Build scint block volumes
  //*********************** 
  //Simply calls functions from Scintillator()lass
  LogicalSNMW_Scint = theScint->GetSNMW_Scint();
  LogicalSNMW_Teflon = theScint->GetSNMW_Teflon();
  LogicalSNMW_Mylar = theScint->GetSNMW_Mylar();
  
  // Set colors of various block materials
  LogicalSNMW_Scint->SetVisAttributes(cyan); // Set scint color to cyan
  LogicalSNMW_Teflon->SetVisAttributes(white); // Set teflon color to white
  LogicalSNMW_Mylar->SetVisAttributes(gray); // Set mylar color to gray 
  
  // Define Teflon properties
  G4int teflon_entries = 0;
  G4double teflon_energy[500], teflon_reflect[500], zero[500]; // Vectors to store values
  // Properties are read in from data file  
  ifstream Read_teflon;
  G4String teflon_file = "/data/nemo243/Simulation_Optique//simulation_input_files/teflon.dat";
  Read_teflon.open(teflon_file);
  if (Read_teflon.is_open()){
    while(!Read_teflon.eof()){
      G4String filler; // This just skips the coma and space in data files
      G4double wavelength; // x values
      G4double teflon_ref_coeff; // y values
      Read_teflon >> wavelength >> filler >> teflon_ref_coeff;
      teflon_energy[teflon_entries] = (1240/wavelength)*eV;
      teflon_reflect[teflon_entries] = teflon_ref_coeff;
      zero[teflon_entries] = 1e-6;
      teflon_entries++;
    }
  }
  else
    G4cout << "Error opening file: " << teflon_file << G4endl; // throw an error if file is not found
  Read_teflon.close();
  
  // Define Teflon optical boundary surface properties
  OpticalTeflon = new G4OpticalSurface("OpticalTeflon");
  OpticalTeflon->SetModel(unified); // Either glisur (GEANT3 model) or unified surface model: set references online
  OpticalTeflon->SetPolish(0); // Set level of polish 
  OpticalTeflon->SetFinish(groundfrontpainted); // Sets finish to one of 6 options: see references online - based off polished or grounded
  OpticalTeflon->SetType(dielectric_metal); // Sets boundary type: dielectric-dielectric or dielectric-metal
  // Define Teflon material properties
  TeflonMPT = new G4MaterialPropertiesTable(); // Creates table to populate with various material properties: reflectivity, ...
  TeflonMPT->AddProperty("REFLECTIVITY",teflon_energy,teflon_reflect,teflon_entries);
  TeflonMPT->AddProperty("SPECULARLOBECONSTANT",teflon_energy,zero,teflon_entries);
  TeflonMPT->AddProperty("SPECULARSPIKECONSTANT",teflon_energy,zero,teflon_entries);
  TeflonMPT->AddProperty("BACKSCATTERCONSTANT",teflon_energy,zero,teflon_entries);
  // Geometrical implementation of boundary surface
  OpticalTeflon->SetMaterialPropertiesTable(TeflonMPT); 
  SSTeflon = new G4LogicalSkinSurface("b_Teflon",LogicalSNMW_Teflon,OpticalTeflon); // Applies optical Teflon properties to ogical Teflon surface
    
  // Define Mylar properties
  G4int mylar_entries = 0;
  G4double mylar_energy[500], mylar_reflect[500], realzero[500], one[500]; 
  ifstream Read_mylar;
  G4String mylar_file = "/data/nemo243/Simulation_Optique//simulation_input_files/mylar.dat";
  Read_mylar.open(mylar_file);
  if (Read_mylar.is_open()){
    while(!Read_mylar.eof()){
      G4String filler;
      G4double wavelength;
      G4double mylar_ref_coeff;
      Read_mylar >> wavelength >> filler >> mylar_ref_coeff;
      mylar_energy[mylar_entries] = (1240/wavelength)*eV;
      mylar_reflect[mylar_entries] = mylar_ref_coeff;
      realzero[mylar_entries] = 0.0;
      one[mylar_entries] = 1.0;
      mylar_entries++;
    }
  }
  else
    G4cout << "Error opening file: " << mylar_file << G4endl; 
  Read_mylar.close();

  // Define Mylar optical boundary surface properties
  OpticalMylar = new G4OpticalSurface("OpticalMylar");
  OpticalMylar->SetModel(glisur);
  OpticalMylar->SetFinish(polished);
  OpticalMylar->SetType(dielectric_metal);

  // Define Mylar material properties
  MylarMPT = new G4MaterialPropertiesTable();
  MylarMPT->AddProperty("REFLECTIVITY",mylar_energy,mylar_reflect,mylar_entries);
  MylarMPT->AddProperty("SPECULARLOBECONSTANT",mylar_energy,one,mylar_entries);
  MylarMPT->AddProperty("SPECULARSPIKECONSTANT",mylar_energy,one,mylar_entries);
  MylarMPT->AddProperty("DIFFUSELOBECONSTANT",mylar_energy,realzero,mylar_entries);
  MylarMPT->AddProperty("BACKSCATTERCONSTANT",mylar_energy,realzero,mylar_entries);
  
  // Geometrical implementation of boundary surface
  OpticalMylar->SetMaterialPropertiesTable(MylarMPT);
  SSMylar = new G4LogicalSkinSurface("SSMylar",LogicalSNMW_Mylar,OpticalMylar); // Applies Mylar properties to logical mylar surface
  //    SSLightGuideWrap = new G4LogicalSkinSurface("SSLightGuideWrap",LogicalLightGuideWrap,OpticalMylar); // Wraps light guide in Mylar

  // Define Vikuiti properties
  G4int vikuiti_entries = 0;
  G4double vikuiti_energy[500], vikuiti_reflect[500];
  ifstream Read_vikuiti;
  G4String vikuiti_file = "/data/nemo243/Simulation_Optique//simulation_input_files/vikuiti.dat";
  Read_vikuiti.open(vikuiti_file);
  if (Read_vikuiti.is_open()){
    while(!Read_vikuiti.eof()){
      G4String filler;
      G4double wavelength;
      G4double vikuiti_ref_coeff;
      Read_vikuiti >> wavelength >> filler >> vikuiti_ref_coeff;
      vikuiti_energy[vikuiti_entries] = (1240/wavelength)*eV;
      vikuiti_reflect[vikuiti_entries] = vikuiti_ref_coeff;
      realzero[vikuiti_entries] = 0.0;
      one[vikuiti_entries] = 1.0;
      vikuiti_entries++;
    }
  }
  else
    G4cout << "Error opening file: " << vikuiti_file << G4endl; 
  Read_vikuiti.close();

  // Define Vikuiti optical boundary surface properties
  OpticalVikuiti = new G4OpticalSurface("OpticalVikuiti");
  OpticalVikuiti->SetModel(glisur);
  OpticalVikuiti->SetFinish(polished);
  OpticalVikuiti->SetType(dielectric_metal);

  // Define Vikuiti material properties
  VikuitiMPT = new G4MaterialPropertiesTable();
  VikuitiMPT->AddProperty("REFLECTIVITY",vikuiti_energy,vikuiti_reflect,vikuiti_entries);
  VikuitiMPT->AddProperty("SPECULARLOBECONSTANT",vikuiti_energy,one,vikuiti_entries);
  VikuitiMPT->AddProperty("SPECULARSPIKECONSTANT",vikuiti_energy,one,vikuiti_entries);
  VikuitiMPT->AddProperty("DIFFUSELOBECONSTANT",vikuiti_energy,realzero,vikuiti_entries);
  VikuitiMPT->AddProperty("BACKSCATTERCONSTANT",vikuiti_energy,realzero,vikuiti_entries);

  // Geometrical implementation of boundary surface
  OpticalVikuiti->SetMaterialPropertiesTable(VikuitiMPT);
  //  SSMylar = new G4LogicalSkinSurface("SSVikuiti",LogicalMylar,OpticalVikuiti); //<- Use if mylar should have Vikuiti optical properties

  //*********************** 
  // Build light guide volume
  //***********************   
  // NOT USED FOR MAIN WALL OM
  //    LightGuide = new LightGuide();
  //    LogicalLightGuide = LightGuide->BuildLightGuideWithSphereCut();
  //    LogicalLightGuideWrap = LightGuide->BuildLightGuideWrappingWithSphereCut();

  //*********************** 
  // Build coupling medium
  //*********************** 
  CouplingGlue = new Coupling();  // Initialize coupling class
  LogicalSNMW_Glue = CouplingGlue->GetSNMW_SphericalCoupling(); // Call function for building glue
  LogicalSNMW_Glue->SetVisAttributes(red); // Set glue color to red 
  
  //*********************** 
  // Build PMT volumes
  //*********************** 
  // Initialize PMT class object for creating R6594-03 HPK PMT
  R6594 = new PMT();
  
  // Build the PMT glass structure from PMT class
  LogicalPMTGlass = R6594->GetSphericalPMTGlass(); // Call function for PMT glass
  LogicalPMTGlass->SetVisAttributes(yellow); // Set glass color to yellow

  // Build the photocathode from PMT class
  LogicalPMT = R6594->GetPhotocathode(); // Call function for PMT photocathode
  LogicalPMT->SetVisAttributes(orange); // Set photocathode color to orange
  G4int PMTcount = 0; 

  // Define PMT properties
  G4int PMTentries = 0;
  G4double PMTenergy[500], PMTindex[500], PMTreflect[500], PMTdetect[500];
  G4double indexconst = 1.49; // Index currently set to constant for PMT glass
  G4double reflectconst = 0.0;
  ifstream ReadPMT;
  G4String PMTfile = "/data/nemo243/Simulation_Optique//simulation_input_files/qe_ham_sba43.txt"; // Currently for different PMT model
  ReadPMT.open(PMTfile);
  if (ReadPMT.is_open()){
    while(!ReadPMT.eof()){
      G4String filler;
      G4double wavelength;
      G4double quant_eff;
      ReadPMT >> wavelength >> filler >> quant_eff;
      PMTdetect[PMTentries] = 0.79*quant_eff*.65;  //0.79 = correction factor to adjust QE to lower value of 34% for R6594 
      //PMTdetect[PMTentries] = 1; // Use only if you want %100 QE
      PMTenergy[PMTentries] = (1240/wavelength)*eV;
      PMTindex[PMTentries] = indexconst;
      PMTreflect[PMTentries] = reflectconst;
      PMTentries++;
    }
  }
  else
    G4cout << "Error opening file: " << PMTfile << G4endl; 
  ReadPMT.close();

  // Define PMT optical boundary surface properties  
  OpticalPMT = new G4OpticalSurface("OpticalPMT");
  OpticalPMT->SetModel(glisur);
  OpticalPMT->SetFinish(polished);
  OpticalPMT->SetType(dielectric_metal);
  
  // Define PMT material properties
  PMT_MPT = new G4MaterialPropertiesTable();    
  PMT_MPT->AddProperty("EFFICIENCY",PMTenergy,PMTdetect,PMTentries);
  PMT_MPT->AddProperty("REFLECTIVITY",PMTenergy,PMTreflect,PMTentries);
  PMT_MPT->AddProperty("RINDEX",PMTenergy,PMTindex,PMTentries);

  // Geometrical implementation of boundary surface
  OpticalPMT->SetMaterialPropertiesTable(PMT_MPT);
  SSPhotocathode = new G4LogicalSkinSurface("SSVikuiti",LogicalPMT,OpticalPMT);

  //*********************** 
  // Various Positioning values
  //*********************** 
  G4double GlueZpos = SNMW_ScintStepHeight/2 + SNMW_ScintBodyHeight - SNMW_CouplingSphereDepth + SNMW_CouplingSphereRadius;
  G4double PMTGlassZpos = GlueZpos + GlueThickness - (SNMW_CouplingSphereRadius - PMTGlassRadius);
  //G4double LightGuideZpos = ScintStepHeight/2 + ScintBodyHeight; // Need to change glue and PMT values if used
  
  G4cout << "Teflon Zpos = " << (TeflonThickness+TeflonGap)/2
        << "\nMylar Zpos = " << (TeflonThickness+TeflonGap)/2
        << "\nGlue Zpos = " << GlueZpos
	 << "\nPMT Zpos = " << PMTGlassZpos << G4endl;

  //*********************** 
  // Geometry placements
  //*********************** 
  // Scint block placement
  PhysicalScint = new G4PVPlacement(G4Transform3D
				    (DontRotate,G4ThreeVector(0,0,0)), // Set at origin as basis of everything else
				    LogicalSNMW_Scint,"PhysicalScint",
				    LogicalWorld,false,0);
  
  // Teflon wrapping placement
  PhysicalTeflon = new G4PVPlacement(G4Transform3D 
				     (DontRotate,G4ThreeVector(0,0,(TeflonThickness+TeflonGap)/2)), //For teflon step thickness
				     LogicalSNMW_Teflon,"PhysicalTeflon",
				     LogicalWorld,false,0);  
  
  // Mylar wrapping placement
  PhysicalMylar = new G4PVPlacement(G4Transform3D
				    (DontRotate,G4ThreeVector(0,0,(TeflonThickness+TeflonGap)/2)), //same shift as Teflon 
				    LogicalSNMW_Mylar,"PhysicalMylar",
				    LogicalWorld,false,0);  
  /* 
  // Light guide placement
  PhysicalGuide = new G4PVPlacement(G4Transform3D
  (DontRotate,G4ThreeVector(0,0,LightGuideZpos)),
  LogicalGuide,"PhysicalGuide",
  LogicalWorld,false,0);
  */
  
  // Glue placement
  PhysicalGlue = new G4PVPlacement(G4Transform3D
				   (Flip,G4ThreeVector(0,0,GlueZpos)),
				   LogicalSNMW_Glue,"PhysicalGlue",
				   LogicalWorld,false,0);

  // PMT placement
  PhysicalPMTGlass = new G4PVPlacement(G4Transform3D
				       (Flip,G4ThreeVector(0,0,PMTGlassZpos)),
				       LogicalPMTGlass,"PhysicalPMTGlass",
				       LogicalWorld,false,0);     

  // PMT photocathode placement      
  PhysicalPMT = new G4PVPlacement(G4Transform3D
				  (Flip,G4ThreeVector(0,0,PMTGlassZpos)),
				  LogicalPMT,"PhysicalPMT",LogicalWorld,true,0); 
  PMTcount++; // Keeps count of number of PMTs in simulation
 
  // Give number of PMTs to the RunAction for statistics compilation
  G4RunManager *runman = G4RunManager::GetRunManager();
  SN_OpticalSimRunAction *runac = (SN_OpticalSimRunAction*)runman->GetUserRunAction();
  runac->SetNumPMTs(PMTcount);
  
  // Returns world with everything in it and all properties set
  return PhysicalWorld;
}
