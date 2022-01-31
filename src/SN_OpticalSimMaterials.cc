/// SN_OpticalSimMaterials.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu>
/// Copyright: 2010 (C) NEMO3/SuperNEMO - University of Texas at Austin

#include "SN_OpticalSimMaterials.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4MaterialPropertyVector.hh"
#include "G4NistManager.hh"
#include <fstream>
#include <iostream>
#include "G4UnitsTable.hh"
#include "G4ThreeVector.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4Scintillation.hh"

using namespace CLHEP;

const G4String SN_OpticalSimMaterials::path = "/home/local1/Optical_Simulation_Arnaud/simulation_input_files/";

// Used to set up all material properties.  Some materials
// are left over from previous simulations and may not be
// needed.  Code to read in material properties from file
// is poorly done.  Can improve using vectors.  Careful of 
// memory leaks.  Careful of files and how they are read in.

SN_OpticalSimMaterials::SN_OpticalSimMaterials(G4String prop_buildfile):bs_glass(0),
							      polysty(0),
							      Ti02(0),
							      coating(0),
							      scintillator(0),
							      Vacuum(0),
							      plastic(0),
							      mylar(0),
							      grease(0),
							      cargille(0),
							      PMMA(0)


{
 
  
  //G4cout << "Test" << G4endl;

  std::ifstream config_prop;
  config_prop.open(prop_buildfile);
  if (!config_prop.is_open())
    G4cout << "Error opening file " << prop_buildfile << G4endl;
  else{
          //G4cout << "Accessing the properties file. " << G4endl;

    while(!config_prop.eof()){
      G4String variable,unit;
      G4double value;
      
      config_prop >> variable;
      if(!config_prop.good()) break;
      if(variable == "scintIndexconst")
	{
	  config_prop>>scintIndexconst;
	}      
      if(variable == "scalingfactor_low")
	{
	  config_prop>>scalingfactor_low;
	}
      if(variable == "scalingfactor_high")
	{
	  config_prop>>scalingfactor_high;
	}
      if(variable == "pop_scalingfactor_low")
	{
	  config_prop>>pop_scalingfactor_low;
	}
      if(variable == "pop_scalingfactor_high")
	{
	  config_prop>>pop_scalingfactor_high;
	}
      if(variable == "lightyield")
	{
	  config_prop>>lightyield;
	}
      if(variable == "paint_ref_coeff")
	{
	  config_prop>>paint_ref_coeff;
	}
      if(variable == "mylar_ref_coeff")
	{
	  config_prop>>mylar_ref_coeff;
	}
      else if(variable == "scintAbsorbconst")
	{
	  config_prop >> value >> unit;
	  scintAbsorbconst = value*G4UnitDefinition::GetValueOf(unit);
	}
      
    }
  }
  
  config_prop.close();
  Construct();


}

SN_OpticalSimMaterials::~SN_OpticalSimMaterials()
{
  if(scintillator) { delete scintillator; scintillator = 0; }
  if (Vacuum){ delete Vacuum; Vacuum = 0; }
  if (polysty){ delete polysty; polysty = 0; }
  if (coating){ delete coating; coating = 0; }
  if (Ti02){ delete Ti02; Ti02 = 0; }
  if (plastic){ delete plastic; plastic = 0; }
  if (mylar){ delete mylar; mylar = 0; }
  if (grease){ delete grease; grease = 0; }
  if (bs_glass){ delete bs_glass; bs_glass = 0; }
  if (PMMA){ delete PMMA; PMMA = 0;}
}


void SN_OpticalSimMaterials::Construct()
{

  //Define the Elements

  elementH = new G4Element( "Hydrogen", "H", 1, 1.00794*g/mole );
  elementC = new G4Element( "Carbon", "C", 6, 12.011*g/mole );
  elementO = new G4Element( "Oxygen", "O", 8, 15.9994*g/mole );
  elementTi = new G4Element( "Titanium","Ti", 22, 47.88*g/mole);

  elementSi = new G4Element("Silicon","Si",14,28.0855*g/mole);
  elementB = new G4Element("Boron","B",5,10.811*g/mole);
  elementNa = new G4Element("Sodium","Na",11,22.989770*g/mole);
  elementAl = new G4Element("Aluminum","Al",13,26.981538*g/mole);

  elementN = new G4Element ("Nitrogen", "N", 7., 14.01*g/mole);

  elementFe = new G4Element ("Fer", "Fe", 26., 55.845*g/mole);
  elementF = new G4Element ("Fluor", "F", 9., 18.9984032*g/mole);



  // AIR
  Air = new G4Material("Air"  , 1.290*mg/cm3, 2);
  Air->AddElement(elementN, 0.7);
  Air->AddElement(elementO, 0.3);

  // FER
  Fer = new G4Material("Fer"  , 7.87*g/cm3, 1);
  Fer->AddElement(elementFe, 1);


  
  // Silicon Dioxide
  SiO2 = new G4Material("SiO2",2.6*g/cm3,2,kStateSolid);
  SiO2->AddElement(elementO,2);
  SiO2->AddElement(elementSi,1);

  // DiBoron TriOxide
  B2O3 = new G4Material("B2O3",2.46*g/cm3,2,kStateSolid);
  B2O3->AddElement(elementB,2);
  B2O3->AddElement(elementO,3);

  // DiSodium Monoxide
  Na2O = new G4Material("Na2O",2.27*g/cm3,2,kStateSolid);
  Na2O->AddElement(elementNa,2);
  Na2O->AddElement(elementO,1);

  // Aluminum Peroxide
  Al2O3 = new G4Material("Al2O3",3.97*g/cm3,2,kStateSolid);
  Al2O3->AddElement(elementAl,2);
  Al2O3->AddElement(elementO,3);

  //Aluminium plates
  Alu = new G4Material("Alu", 2.6989*g/cm3, 1, kStateSolid);
  Alu->AddElement(elementAl, 1);

  //Mylar
  mylar = new G4Material("mylar", 1.40*g/cm3, 3);
  mylar->AddElement(elementH, 4);
  mylar->AddElement(elementC, 5);
  mylar->AddElement(elementO, 2);

  //Teflon
  teflon = new G4Material("teflon", 0.40*g/cm3, 2);
  teflon->AddElement(elementC, 2);
  teflon->AddElement(elementF, 4);

  

  //Borosilicate Glass  ******************************************************

  //  G4cout << "starting borosilicate glass... " << G4endl;

  
  bs_glass = new G4Material("bs_glass",
				      2.23*g/cm3,
				      4,
				      kStateSolid, 
				      273.15*kelvin, 
				      1.0*atmosphere );
 
  bs_glassMPT = new G4MaterialPropertiesTable();

  bs_glass->AddMaterial(SiO2, 0.81); 
  bs_glass->AddMaterial(B2O3, 0.13);
  bs_glass->AddMaterial(Na2O, 0.04);
  bs_glass->AddMaterial(Al2O3, 0.02);

  G4double glassabsorblength;
  G4double glassrefindex = 1.49;
  G4int glassEntries = 0;
  //G4double pWavelength;

  G4double glassEnergy[52];
  G4double glassbulkAbsorb[52];
  G4double bsgindexEnergy[52]; 
  G4double bsgindexvalue[52];
  
  
  for (int i = 0; i < 52; i++){

    glassEnergy[i] = 0;
    glassbulkAbsorb[i] = 0;
    bsgindexEnergy[i] = 0;
    bsgindexvalue[i] = 0;
  }


  glassEntries = 0;

  std::ifstream ReadGlassBulk;
  G4String GlassBulk = path+"Borosilicate_GlassBulkAbsorb.cfg";
  ReadGlassBulk.open(GlassBulk);
  ReadGlassBulk.clear();
  ReadGlassBulk.seekg(0);
  if(ReadGlassBulk.is_open()){
    while(!ReadGlassBulk.eof()){
      G4String filler;
      ReadGlassBulk >> pWavelength >> filler >> glassabsorblength;
      glassEnergy[glassEntries] = (1240/pWavelength)*eV;
      glassbulkAbsorb[glassEntries] = glassabsorblength*m;
      if (glassEnergy[glassEntries] == glassEnergy[glassEntries+1]){break;}
      if (glassEnergy[glassEntries] == glassEnergy[glassEntries-1]){break;}
      glassEntries++;
      //   G4cout << "Energy = " << glassEnergy[glassEntries] 
      //<< "    BulkAbsorb = " << glassbulkAbsorb[glassEntries] << G4endl;
    }
  }
  else
    G4cout << "Error opening file: " << GlassBulk << G4endl; 

  ReadGlassBulk.close();

  
  G4int bsgindexEntries = 0;
  
  std::ifstream ReadGlassIndex;
  G4String GlassIndexFile = path+"BSG_ref_index.dat";
  //  had problems with G4String in the past using this method.
  //  G4String GlassIndexFile = "/work/02262/jcesar/simulation_input_files/BSG_ref_index.dat";
  //  ReadGlassIndex.open(GlassIndexFile);  
  ReadGlassIndex.open(GlassIndexFile);
  ReadGlassIndex.clear();
  ReadGlassIndex.seekg(0);
  if(ReadGlassIndex.is_open()){
    while(!ReadGlassIndex.eof()){
      G4String filler;
      ReadGlassIndex >> pWavelength >> filler >> bsgindexvalue[bsgindexEntries];
      bsgindexEnergy[bsgindexEntries] = (1240/pWavelength)*eV;
      //      G4cout << " Energy = " << bsgindexEnergy[bsgindexEntries] << "    Index = " << bsgindexvalue[bsgindexEntries] << G4endl;
      bsgindexEntries++;
    }
  }
  
  else
    //    G4cout << "Error opening file: " << GlassIndexFile << G4endl; 
  
  ReadGlassIndex.close();
  
  bs_glassMPT->AddProperty("ABSLENGTH",glassEnergy,glassbulkAbsorb,glassEntries);
  //  bs_glassMPT->AddProperty("RINDEX",glassEnergy,bsgindexvalue,glassEntries);
  bs_glassMPT->AddProperty("RINDEX",bsgindexEnergy,bsgindexvalue,bsgindexEntries);

  bs_glass->SetMaterialPropertiesTable(bs_glassMPT);

  


  //Polystyrene  ********************************************************

  polysty = new G4Material("polysty",  
			   1.053*g/cm3, 
			   2, 
			   kStateSolid, 
			   273.15*kelvin, 
			   1.0*atmosphere );
  
  polysty->AddElement( elementH, 0.498 );
  polysty->AddElement( elementC, 0.502 );

  Ti02 = new G4Material("Ti02",4.23*g/cm3,2,kStateSolid);
  Ti02->AddElement(elementO,2);
  Ti02->AddElement(elementTi,1);

  coating = new G4Material("coating",1.4316*g/cm3,2,kStateSolid);
  coating->AddMaterial(Ti02,.12);
  coating->AddMaterial(polysty,.88);
  

  //Scintillator *******************************************************

  scintillator = new G4Material("scintillator",
				1.053*g/cm3, //1.032
				2, 
				kStateSolid, 
				273.15*kelvin, 
				1.0*atmosphere );
  
  scintMPT = new G4MaterialPropertiesTable();

  scintillator->AddElement( elementH, 0.5254 );
  //scintillator->AddElement( elementH, 0.5 );
  scintillator->AddElement( elementC, 0.4746 );
  //scintillator->AddElement( elementC, 0.5 );


  G4int absorbEntries = 0;
  G4int wlsAbEntries  = 0;
  G4int wlsEmEntries  = 0;
  G4double varabsorblength;
  G4double absorbEnergy[500];
  G4double Absorb[500];
  G4double wlsEnergy[500];
  G4double wlsEmit[500];
  G4double wlsAbsorb[500];
  G4double scintEnergy[500];
  G4double scintEmit[500];
  G4double scintAbsorb[500];
  G4double scintSlow[500];
  G4double ref_index_Energy[500];
  G4double ref_index_value[500];
  G4double wlsabsorblength;

  for (int i = 0; i < 500; i++){
    wlsEnergy[i] = 0;
    wlsEmit[i] = 0;
    wlsAbsorb[i] = 0;
    scintEnergy[i] = 0;
    scintEmit[i] = 0;
    scintAbsorb[i] = 0;
    scintSlow[i] = 0;
    ref_index_Energy[i]  =  0;
    ref_index_value[i] = 0;
  }

  // Read primary emission spectrum

  G4int scintEntries = 0;
  std::ifstream ReadScint;

  G4String Scint_file = path+"pTP_emission.dat";


  ReadScint.open(Scint_file);
  if(ReadScint.is_open()){
    while(!ReadScint.eof()){
      G4String filler;
      ReadScint >> pWavelength >> filler >> scintEmit[scintEntries];
      scintEnergy[scintEntries] = (1240/pWavelength)*eV;         //convert wavelength to eV
      // scintIndex[scintEntries] = scintIndexconst;                //read from Scintillator.cfg if you want constant.
      // scintAbsorb[scintEntries] = scintAbsorbconst;              //read from Scintillator.cfg or check bulk absorb below
      scintSlow[scintEntries] = 0.0;
      scintEntries++;
    }
  }
  else
    {
      G4cout << "Error opening file: " << Scint_file << G4endl; 
    }
  ReadScint.close();

  // Read primary bulk absorption

  absorbEntries = 0;
  std::ifstream Readabsorb;
    G4String Readabsorblength = path+"PSTBulkAbsorb.cfg";


  Readabsorb.open(Readabsorblength);
  if (Readabsorb.is_open()){
    while(!Readabsorb.eof()){
      G4String filler;
      Readabsorb >> pWavelength >> filler >> varabsorblength;
      absorbEnergy[absorbEntries] = (1240/pWavelength)*eV;
      Absorb[absorbEntries] = 1*1.3*varabsorblength*m; //MW
      //Absorb[absorbEntries] = 0.31*varabsorblength*m; //XW 0.31
      

      absorbEntries++;
    }
  }
  else

      G4cout << "Error opening file: "<< Readabsorblength << G4endl;  

  Readabsorb.close();

  // Read WLS absorption
  
  wlsAbEntries = 0;
  std::ifstream ReadWLSa;
  G4String WLSabsorb = path+"UPS923.cfg";

  ReadWLSa.open(WLSabsorb);
  if (ReadWLSa.is_open()){
    while(!ReadWLSa.eof()){
      G4String filler;
      ReadWLSa>>pWavelength>>filler>>wlsabsorblength;
      wlsEnergy[wlsAbEntries] = (1240/pWavelength)*eV;
      
            
      if (wlsAbEntries < 200){
	wlsAbsorb[wlsAbEntries] = wlsabsorblength*m; 
      }
      else{
	wlsAbsorb[wlsAbEntries] = wlsabsorblength*m; 
      }
    
      wlsAbEntries++;
    }
  }
  else
    {
      G4cout << "Error opening file: " << WLSabsorb << G4endl;  
    }

  ReadWLSa.close();

  // Read WLS emission

  wlsEmEntries = 0;
  std::ifstream ReadWLSe;
  G4String WLSemit = path+"full_popop_emission.cfg";
  ReadWLSe.open(WLSemit);
  if(ReadWLSe.is_open()){
    while(!ReadWLSe.eof()){
      G4String filler;
      ReadWLSe >> pWavelength >> filler >> wlsEmit[wlsEmEntries];
      wlsEnergy[wlsEmEntries] = (1240/pWavelength)*eV;
      wlsEmEntries++;
    }
  }
  else
    G4cout << "Error opening file: " << WLSemit << G4endl; 
  ReadWLSe.close();

  // Read scintillator refractive index

  G4int ref_index_Entries = 0;

  std::ifstream Read_ref_index;
  //G4String ref_index_emit = path+"PST_ref_index.dat";
  G4String ref_index_emit = path+"PS_index_geant.dat";
  Read_ref_index.open(ref_index_emit);
  if(Read_ref_index.is_open()){
    while(!Read_ref_index.eof()){
      G4String filler;
      Read_ref_index >> pWavelength >> filler >> ref_index_value[ref_index_Entries];
      //ref_index_value[ref_index_Entries]=1.59;
      ref_index_Energy[ref_index_Entries] = (1240/pWavelength)*eV;
      ref_index_Entries++;
    }
  }
  else
    G4cout << "Error opening file: " << ref_index_emit << G4endl; 
  Read_ref_index.close();


  // Now apply the properties table

  scintMPT->AddProperty("WLSCOMPONENT",wlsEnergy,wlsEmit,wlsEmEntries);
  scintMPT->AddProperty("WLSABSLENGTH",wlsEnergy,wlsAbsorb,wlsAbEntries);   // the WLS absorption spectrum
  scintMPT->AddConstProperty("WLSTIMECONSTANT",12*ns);
  scintMPT->AddProperty("RINDEX",ref_index_Energy,ref_index_value,ref_index_Entries);

  scintMPT->AddProperty("ABSLENGTH",absorbEnergy,Absorb,absorbEntries);    // the bulk absorption spectrum
  scintMPT->AddProperty("FASTCOMPONENT",scintEnergy,scintEmit,scintEntries);  // not needed in sim
  scintMPT->AddProperty("SLOWCOMPONENT",scintEnergy,scintSlow,scintEntries);  // not needed in sim

  G4double efficiency = 1.0;
  scintMPT->AddConstProperty("EFFICIENCY",efficiency);

  scintMPT->AddConstProperty("SCINTILLATIONYIELD",lightyield/MeV);
  //scintMPT->AddConstProperty("ALPHASCINTILLATIONYIELD",0.01*lightyield/MeV);
  G4double scintRes = 1;
  scintMPT->AddConstProperty("RESOLUTIONSCALE",scintRes);
  G4double scintFastconst = 2.1*ns;
  scintMPT->AddConstProperty("FASTTIMECONSTANT",scintFastconst);
  G4double scintSlowconst = 10*ns;
  scintMPT->AddConstProperty("SLOWTIMECONSTANT",scintSlowconst);
  scintMPT->AddConstProperty("YIELDRATIO",1.0);

  scintillator->SetMaterialPropertiesTable(scintMPT);
  //scintillator->GetIonisation()->SetBirksConstant(0.0872*mm/MeV); //0.126->base; 0.0872->article BiPO
  scintillator->GetIonisation()->SetBirksConstant(0.25*mm/MeV); // Choisi pour validation modèle avec LY 11737!!!
  //scintillator->GetIonisation()->SetBirksConstant(0.22*mm/MeV); 
  //scintillator->GetIonisation()->SetBirksConstant(0.01*mm/MeV); // TEST ELECTRONS !!! => Maxime


  //Vacuum *************************************************************

  // Be careful of this vacuum definition.  This is only used to define
  // a refractive index so that the detector boundaries are defined.

  Vacuum = new G4Material ("Vacuum", //"Vacuum"   => SIMU OPTIQUE BASE 
			   1.0,  //1.0
			   1.01*g/mole,  //1.01*g/mole
			   universe_mean_density,   //universe_mean_density
			   kStateGas,   //kStateGas
			   3.e-18*pascal, //3.e-18*pascal
			   2.73*kelvin);  //2.73*kelvin

  G4int vacEntries = 0;
  G4double vacEmit[500];
  G4double vacIndex[500];
  G4double vacAbsorb[500];
  G4double vacEnergy[500];
  G4double vacAbsorbconst = 100*m;

  std::ifstream ReadVac;
  G4String Vac = path+"pTP_emission.dat";  // simply index filler values...not pTP values
  ReadVac.open(Vac);
  if(ReadVac.is_open()){
    while(!ReadVac.eof()){
      G4String filler;
      ReadVac >> pWavelength >> filler >> vacEmit[vacEntries];
      vacEnergy[vacEntries] = (1240/pWavelength)*eV; //convert wavelength to eV
      vacIndex[vacEntries] = 1.0;
      vacAbsorb[vacEntries] = vacAbsorbconst;
      //    G4cout<<vacEntries<<" "<<vacEnergy[vacEntries]<<" "<<vacIndex[vacEntries]<<G4endl;
      vacEntries++;
    }
  }
  else
  G4cout << "Error opening file: " << Vac << G4endl; 
  ReadVac.close();

  vacMPT = new G4MaterialPropertiesTable();
  vacMPT->AddProperty("RINDEX",vacEnergy,vacIndex,vacEntries);
  vacMPT->AddProperty("ABSLENGTH",vacEnergy,vacAbsorb,vacEntries);
  Air->SetMaterialPropertiesTable(vacMPT);
  Vacuum->SetMaterialPropertiesTable(vacMPT);

  //Fake Vacuum *************************************************************

  // Be careful of this vacuum definition.  This is only used to define
  // a refractive index so that the detector boundaries are defined.

  VacuumWorld = new G4Material ("VacuumWorld", 
			   1.0, 
			   1.01*g/mole,
			   universe_mean_density, 
			   kStateGas, 
			   3.e-18*pascal,
			   2.73*kelvin);
  
  //Plastic **************************************************************

  plastic = new G4Material("plastic",  
			   1.05*g/cm3, 
			   2, 
			   kStateSolid, 
			   273.15*kelvin, 
			   1.0*atmosphere );
  
  plastic->AddElement( elementH, 0.498 );
  plastic->AddElement( elementC, 0.502 );
  
  G4int plasticEntries = 0;
  G4double plasticEnergy[501];
  G4double absorblength;
  G4double bulkAbsorb[501];
  G4double plasticIndex[501];
  G4double plasticIndexconst = 1.49;

  for (int i = 0; i < 501; i++){
    plasticEnergy[i] = 0;
    bulkAbsorb[i] = 0;
    plasticIndex[i] = 0;
  }

  plasticEntries = 0;
  std::ifstream ReadBulk;
  G4String Bulk = path+"PSTBulkAbsorb.cfg";

  ReadBulk.open(Bulk);
  if(ReadBulk.is_open()){
    while(!ReadBulk.eof()){
      G4String filler;
      ReadBulk>>pWavelength>>filler>>absorblength;
      plasticEnergy[plasticEntries] = (1240/pWavelength)*eV;
      if (plasticEnergy[plasticEntries] == plasticEnergy[plasticEntries+1]){break;} //checks for inconsistencies in file
      if (plasticEnergy[plasticEntries] == plasticEnergy[plasticEntries-1]){break;} // typically not needed
      bulkAbsorb[plasticEntries] = absorblength*m;
      plasticIndex[plasticEntries] = plasticIndexconst;
      plasticEntries++;
    }
  }
  else
    G4cout << "Error opening file: " << Bulk << G4endl; 
  ReadBulk.close();

  plasticMPT = new G4MaterialPropertiesTable();
  plasticMPT->AddProperty("RINDEX",plasticEnergy,plasticIndex,plasticEntries);
  plasticMPT->AddProperty("ABSLENGTH",plasticEnergy,bulkAbsorb,plasticEntries);
  plastic->SetMaterialPropertiesTable(plasticMPT);


  
  //Cargille**************************************************************

  cargille = new G4Material("cargille",  
			    0.99*g/cm3, 
			    4, 
			    kStateSolid, 
			    273.15*kelvin, 
			    1.0*atmosphere );

  // cargille->AddElement( elementH, 0.498 );
  // cargille->AddElement( elementC, 0.502 );
  cargille->AddElement( elementH, 6 );
  cargille->AddElement( elementC, 2 );
  cargille->AddElement( elementO, 1 );
  cargille->AddElement( elementSi, 1 );


  G4int cargilleEntries = 0;
  G4double cargilleEnergy[501];
  G4double cargille_absorblength;
  G4double cargille_bulkAbsorb[501];
  G4double cargilleIndex[501];
  G4double cargilleIndexconst = 1.406;//1.49 1.406 RTV

  for (int i = 0; i < 501; i++){
    cargilleEnergy[i] = 0;
    cargille_bulkAbsorb[i] = 0;
    cargilleIndex[i] = 0;
  }

  cargilleEntries = 0;
  std::ifstream ReadCargille_Bulk;
  G4String Cargille_Bulk = path+"CargilleBulkAbsorb.cfg";

  ReadCargille_Bulk.open(Cargille_Bulk);
  if(ReadCargille_Bulk.is_open()){
    while(!ReadCargille_Bulk.eof()){
      G4String filler;
      ReadCargille_Bulk>>pWavelength>>filler>>cargille_absorblength;
      cargilleEnergy[cargilleEntries] = (1240/pWavelength)*eV;
      cargille_bulkAbsorb[cargilleEntries] = cargille_absorblength*m;
      cargilleIndex[cargilleEntries] = cargilleIndexconst;
      cargilleEntries++;
    }
  }
  else
    G4cout << "Error opening file: " << Cargille_Bulk << G4endl; 
  ReadCargille_Bulk.close();

  G4int cargille_ref_index_Entries = 0;

  std::ifstream Read_cargille_ref_index;
  G4String cargille_ref_index_emit = path+"Cargille_ref_index.dat";
  Read_cargille_ref_index.open(cargille_ref_index_emit);
  if(Read_cargille_ref_index.is_open()){
    while(!Read_cargille_ref_index.eof()){
      G4String filler;
      Read_cargille_ref_index >> pWavelength >> filler >> ref_index_value[cargille_ref_index_Entries];
      ref_index_Energy[cargille_ref_index_Entries] = (1240/pWavelength)*eV;
      cargille_ref_index_Entries++;
    }
  }
  else
    G4cout << "Error opening file: " << cargille_ref_index_emit << G4endl; 
  Read_cargille_ref_index.close();

  cargilleMPT = new G4MaterialPropertiesTable();
  cargilleMPT->AddProperty("RINDEX",ref_index_Energy,cargilleIndex,cargille_ref_index_Entries);
  //cargilleMPT->AddProperty("RINDEX",ref_index_Energy,ref_index_value,cargille_ref_index_Entries);
  cargilleMPT->AddProperty("ABSLENGTH",cargilleEnergy,cargille_bulkAbsorb,cargilleEntries);
  cargille->SetMaterialPropertiesTable(cargilleMPT);

  //end of Cargille
 
  //Grease *************************************************************

  grease = new G4Material("grease",  
			  1.05*g/cm3, 
			  2, 
			  kStateSolid, 
			  273.15*kelvin, 
			  1.0*atmosphere );

  grease->AddElement( elementH, 0.498 );
  grease->AddElement( elementC, 0.502 );
  
  G4int greaseEntries = 0;
  G4double greaseEnergy[500];
  G4double greasebulkAbsorb[500];
  G4double greaseIndex[500];
  G4double greaseabsorblength;
  G4double greaseIndexconst = 1.465;// 1.465 grease and 1.56 BC600

  for (int i = 0; i < 500; i++){
    greaseEnergy[i] = 0;
    greasebulkAbsorb[i] = 0;
    greaseIndex[i] = 0;
  }

  greaseEntries = 0;
  std::ifstream ReadGreaseBulk;
  G4String GreaseBulk = path+"GreaseBulkAbsorb.cfg";
  ReadGreaseBulk.open(GreaseBulk);
  if(ReadGreaseBulk.is_open()){
    while(!ReadGreaseBulk.eof()){
      G4String filler;
      //G4double pWavelength;
      ReadGreaseBulk >> pWavelength >> filler >> greaseabsorblength;
      greaseEnergy[greaseEntries] = (1240/pWavelength)*eV;
      greasebulkAbsorb[greaseEntries] = greaseabsorblength*m;
      greaseIndex[greaseEntries] = greaseIndexconst;
      greaseEntries++;
    }
  }
  else
    G4cout << "Error opening file: " << GreaseBulk << G4endl; 
  ReadGreaseBulk.close();

  greaseMPT = new G4MaterialPropertiesTable();
  greaseMPT->AddProperty("RINDEX",greaseEnergy,greaseIndex,greaseEntries);
  greaseMPT->AddProperty("ABSLENGTH",greaseEnergy,greasebulkAbsorb,greaseEntries);
  grease->SetMaterialPropertiesTable(greaseMPT);

  //  G4cout << "we got the grease" << G4endl;

  
  //PMMA
 
  PMMA = new G4Material("PMMA",  1.19*g/cm3, 3, kStateSolid, 273.15*kelvin, 1.0*atmosphere );
  PMMA->AddElement( elementH, 0.532 );
  PMMA->AddElement( elementC, 0.336 );
  PMMA->AddElement( elementO, 0.132 );
  

  G4MaterialPropertiesTable *innercladMPT;
  
  G4int pmmaEntries = 0;
  G4double cladIndex1[500];
  G4double pmmabulkAbsorb[500];
  G4double pmmaEnergy[500];


 G4int PMMA_ref_index_Entries = 0;

  std::ifstream Read_PMMA_ref_index;
  G4String PMMA_ref_index_emit = path+"PMMA_ref_index.dat";
  Read_PMMA_ref_index.open(PMMA_ref_index_emit);
  if(Read_PMMA_ref_index.is_open()){
    while(!Read_PMMA_ref_index.eof()){
      G4String filler;
      Read_PMMA_ref_index >> pWavelength >> filler >> ref_index_value[PMMA_ref_index_Entries];
      ref_index_Energy[PMMA_ref_index_Entries] = (1240/pWavelength)*eV;
      PMMA_ref_index_Entries++;
    }
  }
  else
    G4cout << "Error opening file: " << PMMA_ref_index_emit << G4endl; 
  Read_PMMA_ref_index.close();
  
  pmmaEntries = 0;
  std::ifstream Read_pmma_Bulk;
  G4String PMMA_bulk = path+"PMMABulkAbsorb.dat";
  
  //  Read_pmma_Bulk.open(pmma_Bulk);
  Read_pmma_Bulk.open(PMMA_bulk);
  
  if(Read_pmma_Bulk.is_open()){
  while(!Read_pmma_Bulk.eof()){
  G4String filler;
  Read_pmma_Bulk >> pWavelength >> filler >> absorblength;
  pmmaEnergy[pmmaEntries] = (1240/pWavelength)*eV;
  cladIndex1[pmmaEntries] = 1.51;
  //pmmabulkAbsorb[pmmaEntries] = absorblength*m;
  pmmabulkAbsorb[pmmaEntries] = 1*5.0*m;
  //      G4cout<<pmmaEntries<<" "<<pmmaEnergy[pmmaEntries]<<" "<<cladIndex1[pmmaEntries]<<G4endl;
  
  pmmaEntries++;
  }
  }
  else
    //    G4cout << "Error opening file: " << Bulk << G4endl;
    G4cout << "Error opening file: " << "PMMABulkAbsorb.dat" << G4endl;
  
  Read_pmma_Bulk.close();
  



  
  //PMMA
  innercladMPT = new G4MaterialPropertiesTable();
  
  innercladMPT->AddProperty("ABSLENGTH",pmmaEnergy,pmmabulkAbsorb,pmmaEntries);
  //innercladMPT->AddProperty("RINDEX",pmmaEnergy,cladIndex1,pmmaEntries); 
  innercladMPT->AddProperty("RINDEX",pmmaEnergy,ref_index_value,PMMA_ref_index_Entries);
  PMMA->SetMaterialPropertiesTable(innercladMPT);
  
  

  // Vikuiti ***********************************************************




  // end Vikuiti
  
  // READ OUT THE VECTORS!
  //    G4cout << "Energy" << "Emission Data" << "Absorption Data" << "WLS Emission Data" << "WLS Absorption Data"<< G4endl;
  //  for (int i = 0; i < 60; i++){
  //    G4cout <<scintEnergy[i]<<"   "<<scintEmit[i]<<"   "<<scintAbsorb[i]<<"   "<<wlsEmit[i]<<"   "<<wlsAbsorb[i]<< G4endl;
  //  }

//}  
//end of Materials


}
  G4Material* SN_OpticalSimMaterials::GetMaterial(G4String material)
    {
      G4Material* pttoMaterial = G4Material::GetMaterial(material); 
      return pttoMaterial; 


    }
  



