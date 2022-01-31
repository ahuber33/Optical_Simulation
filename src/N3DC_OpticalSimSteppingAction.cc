/// SN_OpticalSimSteppingAction.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> 
/// Copyright: 2010 (C) NEMO3/SuperNEMO - University of Texas at Austin
/// Modified and completed : Arnaud HUBER <huber@cenbg.in2p3.fr>
/// Copyright: 2016 (C) NEMO3/SuperNEMO - University of Bordeaux

#include "SN_OpticalSimSteppingAction.hh"
#include "G4DynamicParticle.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4OpticalPhoton.hh"
#include "SN_OpticalSimRunAction.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "PMT.hh"
#include "SN_OpticalSimEventAction.hh"
#include <iostream>
#include <fstream>
#include "G4Material.hh"
#include "G4Trajectory.hh"
#include "SN_OpticalSimTrackInformation.hh"
#include "SN_OpticalSimGeometry.hh"
#include "G4StepPoint.hh"
#include "TRandom3.h"
#include "TF1.h"
#include "SN_OpticalSimCaloMap.hh"
#include "SN_OpticalSimUniformityPMT.hh"



using namespace CLHEP;

const G4String SN_OpticalSimSteppingAction::path = "/home/local1/Optical_Simulation_Arnaud/simulation_input_files/";

SN_OpticalSimSteppingAction::SN_OpticalSimSteppingAction()
{}

//SN_OpticalSimSteppingAction::SN_OpticalSimSteppingAction()
//{}

SN_OpticalSimSteppingAction::~SN_OpticalSimSteppingAction(){}
void SN_OpticalSimSteppingAction::UserSteppingAction(const G4Step *aStep){
  
  G4Track* theTrack = aStep->GetTrack();


  G4double x = aStep->GetTrack()->GetPosition().x();
  G4double y = aStep->GetTrack()->GetPosition().y();   
  G4double z = aStep->GetTrack()->GetPosition().z(); 
  //G4double z_origine = 90. -z; // Limits of photocathode in z plane => Define origine for determine cathode uniformity !!!!
  G4double z_origine = -50. -z; // Limits of photocathode in z plane => Define origine for determine cathode uniformity for N3DC configuration!!!!
  G4double r = sqrt(x*x + y*y +z_origine*z_origine); 
  //G4double d = sqrt(x*x + y*y + pow(z-116,2));
  G4double theta = acos((z_origine)/r)/deg;
  G4double phi = 0;

  //G4cout << "x = " << x << G4endl;
  //G4cout << "y = " << y << G4endl;
  //G4cout << "z = " << z << G4endl;


  if (x < 0)
    {
      //theta = -theta;
    }



  if (x >0 && y > 0)
    {
      phi = atan(y/x)/deg;
    }

  if (x <0 && y > 0)
    {
      phi = 90 + atan(-x/y)/deg;
    }

  if (x <0 && y < 0)
    {
      phi = 180 + atan(y/x)/deg;
    }

  if (x >0 && y < 0)
    {
      phi = 270 + atan(x/-y)/deg;
    }
      






  SN_OpticalSimTrackInformation *trackInformation;
  trackInformation = (SN_OpticalSimTrackInformation*)theTrack->GetUserInformation();

  //  In case you want the previous or next step information...

  //  G4StepPoint* thePrePoint = aStep->GetPreStepPoint();
  //  G4VPhysicalVolume* thePrePV = thePrePoint->GetPhysicalVolume();

  //  G4StepPoint* thePostPoint = aStep->GetPostStepPoint();
  //  G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();

  
  if(0){                       //set to 1 to ignore generated photons
    if(theTrack->GetDefinition()->GetParticleName()=="opticalphoton")
      theTrack->SetTrackStatus(fStopAndKill);



  }
  
  //The following lines are for debugging purposes
  G4String partname = aStep->GetTrack()->GetDefinition()->GetParticleName();
  if(partname == "opticalphoton" && aStep->GetTrack()->GetUserInformation() == 0) 
    G4cout<<"Warning! No User info attached to photon!"<<G4endl;



  //Get the Event manager
  G4EventManager *evtman = G4EventManager::GetEventManager();
  SN_OpticalSimEventAction *evtac = (SN_OpticalSimEventAction*)evtman->GetUserEventAction();


  //Get the Run manager
  SN_OpticalSimRunAction *runac = (SN_OpticalSimRunAction*)(G4RunManager::GetRunManager()->GetUserRunAction());
  


  //****************  ELECTRON BACKSCATTERING!  *********************************
  /*
  
    double electronMomentum = 0.0;
    double electron_b_position;
    double electron_d_position;
    double electron_angle;

    if(theTrack->GetDefinition()->GetParticleName()=="e-"){
    Hep3Vector e_MomDir=theTrack->GetMomentumDirection();
    electronMomentum = e_MomDir[2];
    electron_angle = atan(sqrt(e_MomDir[0]*e_MomDir[0]+e_MomDir[1]*e_MomDir[1])/e_MomDir[2]);

      
    //      SN_OpticalSimTrackInformation* e_info=((SN_OpticalSimTrackInformation*)(theTrack->GetUserInformation()));
      
    G4ThreeVector e_birthpos=aStep->GetPreStepPoint()->GetPosition();
    G4ThreeVector e_deathpos=aStep->GetPostStepPoint()->GetPosition();

    electron_b_position = e_birthpos[2]/mm;
    electron_d_position = e_deathpos[2]/mm;
      
    if ( electronMomentum < 0 && electron_d_position < -86.0 && electron_b_position > -105.5 ) {
	
    G4cout << "Backscatter! " << G4endl;
    G4cout << "the z-momentum is " << electronMomentum <<  G4endl;
    //	G4cout << "the birthpos is " << electron_b_position <<  G4endl;
    //	G4cout << "the deathpos is " << electron_d_position <<  G4endl;

    ofstream Backscatter;
    Backscatter.open("Backscatter.out",ios::app);
    //	Backscatter<<e_deathpos[0]<<" "<<e_deathpos[1]<<" "<<e_deathpos[2]<<G4endl;
    Backscatter << fabs((electron_angle*180)/pi) << G4endl;
    Backscatter.close();
    }
    }
  */
  //**********************	END BACKSCATTERING    *****************************

  /*
    if(aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName()  == "PhysicalPMT")
    {
    G4cout << "Position de detection X = " << x << G4endl;
    G4cout << "Position de detection Y = " << y << G4endl;
    G4cout << "Position de detection Z = " << z << G4endl;
    G4cout << "Angle de detection = " << theta << G4endl;
    G4cout << "Angle plan x/y = " << phi << G4endl;
    }
  */



  SN_OpticalSimTrackInformation* info = (SN_OpticalSimTrackInformation*)(aStep->GetTrack()->GetUserInformation());

  //if more than 1 secondary is generated, update scintillation statistics

  if(fpSteppingManager->GetfN2ndariesPostStepDoIt()>1) {
        
    //these two will give you the number of wls photons
    //    if(theTrack->GetDefinition()->GetParticleName()=="opticalphoton")
    //     evtac->AddGenerated(fpSteppingManager->GetfN2ndariesPostStepDoIt());

    evtac->AddGenerated(fpSteppingManager->GetfN2ndariesPostStepDoIt()); 
    evtac->AddEdep(aStep->GetTotalEnergyDeposit()/MeV);
    //G4cout << "Count ph" << G4endl;
  }






  G4double my_dist_after = aStep->GetTrack()->GetTrackLength()/mm;




  //check for bulk absorption
  G4String endproc = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  if(endproc == "OpAbsorption")
    {
      evtac->CountBulkAbs();

      //G4cout << "Photon BulkAbsorbed" << G4endl;
    }
  
  else if (endproc == "OpWLS"){
    evtac->CountWLS();
    my_dist_after = my_dist_after + aStep->GetTrack()->GetTrackLength()/mm;
    //G4cout << "Count WLS true = " << info->GetWLSCount() << G4endl;
    //G4cout << "Distance d = " << d << G4endl;


    if (info->GetWLSCount() == 0)
      {
	Length_Track = aStep->GetTrack()->GetTrackLength()/mm;
	//G4cout << "Count WLS = " << info->GetWLSCount() << G4endl;
      }

    if (info->GetWLSCount() > 0)
      {
	Length_Track += aStep->GetTrack()->GetTrackLength()/mm;
	//G4cout << "Count WLS = " << info->GetWLSCount() << G4endl;
      }

    //G4cout << "Track Length = " << aStep->GetTrack()->GetTrackLength()/mm << G4endl;
    //G4cout << "Total Length Track (for different OpWLS) = " << Total_Length_Track << G4endl;

  }

  else if(partname == "opticalphoton" && endproc != "Transportation")
    G4cout << endproc << G4endl;
  
  //find the boundary status
  // uncomment

  G4OpBoundaryProcessStatus boundaryStatus=Undefined;
  static G4OpBoundaryProcess* boundary=NULL;


  //find the boundary process only once
  if(!boundary){
    G4ProcessManager* pm = aStep->GetTrack()->GetDefinition()->GetProcessManager();
    G4int nprocesses = pm->GetProcessListLength();
    G4ProcessVector* pv = pm->GetProcessList();
    G4int i;
    for( i = 0; i < nprocesses; i++){
      if((*pv)[i]->GetProcessName()=="OpBoundary"){
	boundary = (G4OpBoundaryProcess*)(*pv)[i];
	break;
      }
    }
  }

  RunTally Statistics;
  RunTallybis Statisticsbis;
  RunTallyter Statisticster;

  G4ParticleDefinition* particleType = aStep->GetTrack()->GetDefinition();
  if(particleType == G4OpticalPhoton::OpticalPhotonDefinition()){
    //query the boundary status
    boundaryStatus = boundary->GetStatus();

    //Tally aTally;

    Statisticsbis.Angle = 0;
    Statisticsbis.PositionX = 0;
    Statisticsbis.PositionY = 0;
    Statisticsbis.PositionZ = 0;
    Statisticsbis.DeathLambda = 0;
    Statisticsbis.BirthLambda = 0;
    Statisticsbis.Theta = 0;

		
      
    if(aStep->GetPostStepPoint()->GetStepStatus()==fGeomBoundary){

      if(aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName()  == "PhysicalPMT")
	{


	  SetTheta(theta);
	  Statisticsbis.Angle = theta;
	  Statisticsbis.PositionX = x;
	  Statisticsbis.PositionY = y;
	  Statisticsbis.PositionZ = z;
	  Statisticsbis.DeathLambda = 1240*eV/(aStep->GetTrack()->GetTotalEnergy());
	  Statisticsbis.BirthLambda = info->GetBirthLambda();
	  Statisticsbis.Theta = theta;
	  Statisticsbis.Phi = phi;


	  //G4cout << "Death Lambda = " << Statisticsbis.DeathLambda << G4endl;


	  //G4cout << "theta (get) = " << GetTheta() << G4endl;
	  //G4cout << "Position de detection X = " << x << G4endl;
	  //G4cout << "Position de detection Y = " << y << G4endl;
	  //G4cout << "Position de detection Z = " << z << G4endl;
	  //G4cout << "Angle de detection = " << theta << G4endl;
	}
      switch(boundaryStatus){
      case Detection: 
	{

	  //G4cout << "we detected a photon" << G4endl;
	  //        write photon statistics to the event ROOT tree

	  evtac->Setcpt_without_CU((evtac->Getcpt_without_CU())+1); //Incrementation of number of photons detected without applicaiton of cathode uniformity !!!	

	  G4double Angle[20], Cathode_Unif[20];
	  G4int entries = 0;
	  ifstream ReadCathodeUniformity;
	  G4String Cathodefile = path+"CathodeUnif_ham_GA0124.txt"; 
	  //G4String Cathodefile = path+"CathodeUnif_ham_GA0154.txt";
	  //G4String Cathodefile = path+"CathodeUnif_ham_GA0190.txt"; 
	  //G4String Cathodefile = path+"CathodeUnif_ham_GA0192.txt"; 
	  //G4String Cathodefile = path+"CathodeUnif_ham_GA0205.txt"; 
	  //G4String Cathodefile = path+"CathodeUnif_ham_GA0494.txt"; 


	  //##############################################################################################
	  //WARNING !!!! Think to change also the datafile in SN_OpticalSimGeometry_8inchesMW.cc !!!!!!! #
	  //##############################################################################################

	  ReadCathodeUniformity.open(Cathodefile);
	  if (ReadCathodeUniformity.is_open()){
	    while(!ReadCathodeUniformity.eof()){
	      G4String filler;
	      G4double angle;
	      G4double cathode_unif;
	      ReadCathodeUniformity >> angle >> filler >> cathode_unif;
	      //Cathode_Unif[entries] = cathode_unif;  //For the GAxxxx file with cathode uniformity in fonction of detection position on photocathode
	      Angle[entries] = angle;
	      entries++;
	    }
	  }				
	  else
	    G4cout << "Error opening file: " << Cathodefile << G4endl; 
	  ReadCathodeUniformity.close();


	  //G4cout << "angle = " << Angle[11] << G4endl;
	  //G4cout << "Valeur = " << Cathode_Unif[11] << G4endl;

	  G4double Uniformity =  1.*UniformityPMT::GetInstance()->Interpolate(theta, phi);
	  //G4cout << "Theta = " << theta << G4endl;
	  //G4cout << "Phi = " << phi << G4endl;
	  //G4cout << "Value Uniformity = " << Uniformity << G4endl;




	  //G4int i =0;

	  //for (i =0; i <20; i++)
	  //{
	  //  if (theta > Angle[i] && theta < Angle[i+1])
	  //	{
		  //evtac->Setcpt((evtac->Getcpt())+1);
	  double fnum = RandFlat::shoot();
	  //if (fnum > Cathode_Unif[i])
	  if (fnum > Uniformity)
	    {
	      evtac->Setcpt_photons_lost((evtac->Getcpt_photons_lost())+1);  //Incrementation of number of photons lost due to application of cathode uniformity !!!
	    }

	  //if (fnum < Cathode_Unif[i])
	  if (fnum < Uniformity)
	    {
	      evtac->CountDetected();
	      //runac->UpdateStatisticster(Statisticsbis);

	      //##############################################################################################################
	      //##############################################################################################################
	      //###################################### BEGIN CODE TIME RESOLUTION ############################################
	      //##############################################################################################################
	      //##############################################################################################################
	      
	      // TRandom3* gen_TTS = new TRandom3(0);
	      // double Transit_Time = 50; //ns
	      // //double TTS = 2.056; //ns for PMT GA0124 
	      // double TTS = 2.272; //ns for PMT GA0154 
	      // //double TTS = 2.411; //ns for PMT GA0190 
	      // //double TTS = 2.334; //ns for PMT GA0192 
	      // //double TTS = 2.180; //ns for PMT GA0205 
	      
	      
	      // /*
	      // !!!!!!!!VALUES FOR DISTRIBUTION !!!!!!!!!!!!!
	      // double Trise_PTP = 0.5; //ns
	      // double Tdecay_PTP = 5. ;//ns
	      // double Trise_POPOP = 0.3 ;//ns
	      // double Tdecay_POPOP = 1.5 ;//ns
	      // */
	      
	      // Time_Emission_Sc_POPOP =0;
	      // Time_Emission_Sc =0;
	      // Total_Length_Track =0;
	      
	      //   TF1 *pTP_distribution = new TF1("pTP_distribution","(1/(5 - 0.5))*(exp(-x/5) - exp(-x/0.5))",0,70);
	      //   TF1 *POPOP_distribution = new TF1("POPOP_distribution","(1/(1.5 - 0.3))*(exp(-x/1.5) - exp(-x/0.3))",0,70);
	      
	      
	      
	      // 		//aTally.TotTrackLen = aTally.DistBefore + aTally.DistAfter + 75*mm + aTally.BirthZ;      => First version for Zac's simulation
	      // 		Total_Length_Track = Length_Track + aStep->GetTrack()->GetTrackLength()/mm;
	      // 		Statisticster.TotTrackLen = Total_Length_Track;
	      // 		Time_Transportation_Sc = (Statisticster.TotTrackLen *1.60)/299.792458; //ns
	      // 		Statisticster.TimeTransSc = Time_Transportation_Sc;
	      // 		Time_Emission_Sc_PTP = pTP_distribution->GetRandom(0,70);
	      
	      // 		for (int h =1; h<=info->GetWLSCount() ; h++)
	      // 			{
	      // 			Time_Emission_Sc_POPOP += POPOP_distribution->GetRandom(0,70);
	      // 			//G4cout << "Number WLS = " << info->GetWLSCount() << G4endl;
	      // 			//G4cout << "Time POPOP = " << Time_Emission_Sc << G4endl;
	      // 			}
	      
	      // 		Statisticster.TimeEmissionScPTP = Time_Emission_Sc_PTP;
	      // 		Statisticster.TimeEmissionScPOPOP = Time_Emission_Sc_POPOP;
	      // 		Time_Emission_Sc = Time_Emission_Sc_PTP + Time_Emission_Sc_POPOP;
	      // 		Statisticster.TimeEmissionSc = Time_Emission_Sc;
	      
	      
	      // 		if (theta > 75 || theta < -75)
	      // 			{
	      // 			Time_Transit_PMT = gen_TTS->Gaus(Transit_Time+1.2,TTS/2.35);
	      // 			}
	      // 		else
	      // 			{
	      // 			Time_Transit_PMT = gen_TTS->Gaus(Transit_Time,TTS/2.35);
	      // 			}
	      // 		Statisticster.TimeTransitPMT = Time_Transit_PMT;
	      // 		Statisticster.TimeTotalSc = Time_Transportation_Sc + Time_Emission_Sc;
	      // 		Statisticster.TimeTotal = Statisticster.TimeTotalSc + Time_Transit_PMT;
	      // 		Statisticster.DeathLambda = 1240*eV/(aStep->GetTrack()->GetTotalEnergy());


	      // 		runac->UpdateStatisticster(Statisticster);
	      
			//evtac->CountDetected(aTally); //BE CAREFUL  ====> ONLY IF YOU WANT TO CONSIDER THE PHOTOCATHODE UNIFORMITY FOR TIME RESOLUTION (RESULTS ON TERMINAL ARE WRONG WITH THIS LINE) IF YOU WANT TO OBTAIN GOOD RESUKTS ON TERMINAL COMMENT THIS LINE AND UNCOMMENT THE LINE AT THE END OF DETECTION PART !!!!
	      
	      
	      //###########################################################################################################
	      //###########################################################################################################
	      //############################### END CODE TIME RESOLUTION ##################################################
	      //###########################################################################################################
	      //###########################################################################################################
	    }
	  //G4cout << "num aleatoire = " << fnum << G4endl;
	  //}
	//}
      
      //G4cout << "compteur = " << evtac->Getcpt() << G4endl;
      


      evtac->CountDetected_without_CU();
      //evtac->CountDetected(aTally);

	
		
	}
	break;
      case Absorption:    // used to get the number TRANSMITTED!!
	    
	if (theTrack->GetNextVolume()->GetName()!="PhysicalTeflon" && 
	    theTrack->GetNextVolume()->GetName()!="PhysicalScint" && 
	    //		  theTrack->GetNextVolume()->GetName()!="p_coupling" && 
	    //		  theTrack->GetNextVolume()->GetName()!="p_top_paint" && 
	    theTrack->GetNextVolume()->GetName()!="PhysicalMylar" && 
	    //		  theTrack->GetNextVolume()->GetName()!="physical_holder" && 
	    //		  theTrack->GetNextVolume()->GetName()!="p_light_guide_forPMT" && 
	    //		  theTrack->GetNextVolume()->GetName()!="p_light_guide_wrap_forPMT" &&
	    theTrack->GetNextVolume()->GetName()!="PhysicalGlue" && 
	    theTrack->GetNextVolume()->GetName()!="PhysicalPMTGlass")
		
	  {
	    evtac->CountFailed();
	    runac->UpdateStatisticsbis(Statisticsbis);

		  
	    //  Note that currently it is not set up to root output...see void CountDetected();
	    //	  G4cout << "Photon bulk absorbed" << G4endl;
	  }
	else{  // if not bulk, transmitted, or detected...it must be surface!
	  evtac->CountAbsorbed();

	  //	  G4cout << "Photon surface absorbed" << G4endl;
		  
	  /*   These lines output the surface absorbed photon information 		
	  //	G4ThreeVector Surf_absorb_position = aStep->GetPostStepPoint()->GetPosition();
		  
	  //	  aTally.SurfAbsDeathX = Surf_absorb_position[0]/mm;
	  //	  aTally.SurfAbsDeathY = Surf_absorb_position[1]/mm;
	  //	  aTally.SurfAbsDeathZ = Surf_absorb_position[2]/mm;
		  
	  ofstream SurfAbs;
	  SurfAbs.open("SurfAbs.out",std::ios::app);
	  SurfAbs<<Surf_absorb_position[0] << " " << Surf_absorb_position[1] << " " << Surf_absorb_position[2] << G4endl;
	  SurfAbs.close();
	  */




	}
	break;
      case Undefined: G4cout<<"Undefined Boundary Process!"<<G4endl;
	break;
      case NoRINDEX: 
	{
	  evtac->CountEscaped();
	  //G4cout << "count escaped" << G4endl;

	}
	break;
	    
	// if we have any kind of reflections, count them
      case LambertianReflection: 
      case LobeReflection:
      case SpikeReflection:
	{
	  ((SN_OpticalSimTrackInformation*)(aStep->GetTrack()->GetUserInformation()))->CountReflections();
	  //G4cout << "Reflection" << G4endl;
	  break;}
      case TotalInternalReflection:
	{
	  ((SN_OpticalSimTrackInformation*)(aStep->GetTrack()->GetUserInformation()))->CountTotalInternalReflections();
	  break;
	}
      default: 
	break;
      }
    }
  }
  //G4cout << "Total Boundary = " <<  evtac->Getcpt() << G4endl;
  //G4cout << "Total Generated = " <<  evtac->Getcpt_tot() << G4endl;






  //G4cout << "Nb bin X = " << Scan->GetNbinsX() << G4endl;
  //G4cout << "Value bin = " << Scan->GetBinContent(11,15) << G4endl;
  /*
  TH2F *Scanning; // interpolated histo with fine binning
  Scanning = new TH2F("Scanning", "", 400, 0, 200, 600, 0, 300);

  for (int binx=1; binx<=Scanning->GetNbinsX(); ++binx) {
    for (int biny=1; biny<=Scanning->GetNbinsY(); ++biny) {
      double x_position = Scanning->GetXaxis()->GetBinCenter(binx);
      double y_position = Scanning->GetYaxis()->GetBinCenter(biny);
      double value = Scan->Interpolate(x_position, y_position);
      Scanning->SetBinContent(binx, biny, value);
    }
  } 
  */


  
  //TH2F * h = new TH2F("h", "h",  40 ,0, 200, 60, 0, 300);
  //h = (TH2F*)f.Get("Scanning");

  RunTallyPosition Statsposition;
  RunTallyGamma Statsgamma;
  RunTallyCompton Statscompton;
  RunTallyConv Statsconv;

  //G4cout << "Position Z = " << z << G4endl;


      

  if ((aStep->GetTrack()->GetParentID() == 0)
      && (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()  == "PhysicalScint")
      && (aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "PhysicalScint"))
    {
      if(partname == "gamma" )
	{
	  if(endproc =="compt" || endproc =="phot")
	    {
	      G4double E_gamma = aStep->GetPreStepPoint()->GetKineticEnergy()/keV - aStep->GetPostStepPoint()->GetKineticEnergy()/keV;
      
	      G4double Correction_BC = 1.;
	      G4double E_gamma_BC_Corrected;
	      G4double E_gamma_Position_Corrected;
	      G4double E_gamma_Corrected;
	      G4double Correction_Position;

	      Statsposition.Position_x = x;
	      Statsposition.Position_y = y;
	      Statsposition.Position_z = z+150;
	      //Correction_Position=  CaloMap::GetInstance()->Interpolate(x, y, z+120.5);
	      
	      // G4cout << "Effet Compton !!!!!" << G4endl;
	      // G4cout << "E pre = " << aStep->GetPreStepPoint()->GetKineticEnergy()/keV << " keV" << G4endl;
	      // G4cout << "E post = " << aStep->GetPostStepPoint()->GetKineticEnergy()/keV << " keV" << G4endl;
	      // G4cout << "E depose par compton = " << E_gamma << " keV" << G4endl;
	      //Statsgammadeposited.Energy_Compton.push_back(E_gamma);
	      //Statsgammadeposited.Energy_Compton = E_gamma;
	      Statscompton.E_Compton = E_gamma;
	      //Correction_BC = 1.00061*(1.05026 - (1.22017/(pow(E_gamma, 0.46))));
	      Correction_BC = 1.00196*(1.08996 - (1.5611/(pow(E_gamma, 0.41)))); // kb=0.25mm/MeV
	      E_gamma_BC_Corrected = E_gamma *Correction_BC;
	      E_gamma_Position_Corrected = E_gamma *Correction_Position;
	      E_gamma_Corrected = E_gamma *Correction_Position * Correction_BC;

	      evtac->E_deposited_Gamma_Corrected(E_gamma_Corrected);
	      evtac->E_deposited_Gamma(E_gamma);
	      evtac->E_deposited_Gamma_BC_Corrected(E_gamma_BC_Corrected);
	      evtac->E_deposited_Gamma_Position_Corrected(E_gamma_Position_Corrected);
	      //G4cout << "E gamma = " << E_gamma << " keV" <<  G4endl;
	      // G4cout << "Correction BC = " << Correction_BC << G4endl;
	      // G4cout << "Correction Position = " << Correction_Position << G4endl;
	      // G4cout << "E corrigé BC = " << E_gamma_BC_Corrected << " keV " <<  G4endl;
	      // G4cout << "E corrigé Position = " << E_gamma_Position_Corrected << " keV " <<  G4endl;
	      // G4cout << "E corrigé  = " << E_gamma_Corrected << " keV " <<  G4endl;
	      // G4cout << "E total gamma dep = " << evtac->Get_E_deposited_Gamma() << " keV " << G4endl;
	      // G4cout << "E total gamma dep CORRECTED with BC = " << evtac->Get_E_deposited_Gamma_BC_Corrected() << " keV " << G4endl;
	      // G4cout << "E total gamma dep CORRECTED with Position = " << evtac->Get_E_deposited_Gamma_Position_Corrected() << " keV " << G4endl;
	      // G4cout << "E total gamma dep CORRECTED = " << evtac->Get_E_deposited_Gamma_Corrected() << " keV " << G4endl;

	      //G4cout << "vector 0 = " << Statsgamma.E_compton.at(0) << G4endl;

	      evtac->Fill_E_Compton(E_gamma);
	      evtac->Fill_Gamma_Interaction_X(x);
	      evtac->Fill_Gamma_Interaction_Y(y);
	      evtac->Fill_Gamma_Interaction_Z(z+120.5);
	      
	      
	      runac->UpdateStatisticsCompton(Statscompton);
	      
	      
	      runac->UpdateStatisticsPosition(Statsposition);
	      evtac->Set_Number_Interaction(); // To count the number of interaction inside the scintillator (gammas)
	      
	      //G4cout << "Position X = " << x << G4endl;
	      //G4cout << "Position Y = " << y << G4endl;
	      //G4cout << "Position Z = " << z +150  << G4endl;      
	      
	    }
	}
      
      // if(partname !="gamma" && evtac->Get_Number_Interaction() ==0)
      // 	{
      // 	  //Statsposition.Position_x = x;
      // 	  //Statsposition.Position_y = y;
      // 	  //Statsposition.Position_z = z;

      // 	  evtac->Set_Position_X(x);
      // 	  evtac->Set_Position_Y(y);
      // 	  evtac->Set_Position_Z(z);

      // 	  //G4cout << "Position X = " << evtac->Get_Position_X() << G4endl;
      // 	  //G4cout << "Position Y = " << evtac->Get_Position_Y() << G4endl;
      // 	  //G4cout << "Position Z = " << evtac->Get_Position_Z() << G4endl;

      // 	  //runac->UpdateStatisticsPosition(Statsposition);
      // 	  evtac->Set_Number_Interaction(); // To count the number of interaction inside the scintillator (gammas)
      
      // 	  //G4cout << "Position X = " << x << G4endl;
      // 	  //G4cout << "Position Y = " << y << G4endl;
      // 	  //G4cout << "Position Z = " << z << G4endl;      
      // 	}
      
    }







  //###################################################################################
  //###################################################################################
  //############### PARTIE DEDIE A L ETUDE DU TAUX DE COMPTAGE ########################
  //###################################################################################
  //###################################################################################


  G4int StepNo = aStep->GetTrack()->GetCurrentStepNumber();

  
  //G4cout << "nph = " << evtac->Getnph() << G4endl;


  
 //####################################################################################
 //################################ PARTIE ELECTRONS ##################################
 //####################################################################################

  if ((partname == "e-")
      && (aStep->GetTrack()->GetParentID() ==0)// ATTENTION : 1 dans le cas de noyau et 0 pour calcul d'efficacité
      && StepNo ==1
      )
    {    
      //evtac->E_emitted_Elec(aStep->GetPreStepPoint()->GetKineticEnergy()/keV);
      evtac->Setnph(2);
      //G4cout << "Electron !!!! " << G4endl;
      //G4cout << "nph = " << evtac->Getnph() << G4endl;

    }
                  
  if(evtac->Getnph()==2 &&  aStep->GetTrack()->GetParentID() !=2)
    {
      if((aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()  == "PhysicalScint")
	 && (aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "PhysicalScint")
	 )
	{
	  evtac->E_deposited_Elec(aStep->GetTotalEnergyDeposit()/keV);
	  //G4cout << "Depot d'energie new !!! " << G4endl;
	}
    }




 //####################################################################################
 //##################################### PARTIE ALPHAS ################################
 //####################################################################################

 if ((partname == "alpha")
      && (aStep->GetTrack()->GetParentID() ==1)// ATTENTION : 1 dans le cas de noyau et 0 pour calcul d'efficacité
      && StepNo ==1
      )
    {    
      evtac->E_emitted_Alpha(aStep->GetPreStepPoint()->GetKineticEnergy()/keV);
      evtac->Setnph(3);
      //G4cout << "Alpha !!!! " << G4endl;
      //G4cout << "nph = " << evtac->Getnph() << G4endl;
    }
                  

  if(evtac->Getnph()==3)
    {
      if((aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()  == "PhysicalScint")
	 && (aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "PhysicalScint")
	 )
	{
	  evtac->E_deposited_Alpha(aStep->GetTotalEnergyDeposit()/keV);
	  //G4cout << "Depot d'energie new !!! " << G4endl;
	}
    }

  


 //####################################################################################
 //################################ PARTIE GAMMAS #####################################
 //####################################################################################

  if (aStep->GetTrack()->GetParentID() >=1)
    {
      if (aStep->GetTrack()->GetCreatorProcess()->GetProcessName() == "RadioactiveDecay")
	{
	  if (StepNo == 1)
	    {
	      if (aStep->GetTrack()->GetDefinition()->GetParticleType() == "gamma")
		{
		  Statsgamma.E_emitted_Gamma = aStep->GetPreStepPoint()->GetKineticEnergy()/keV;
		  //evtac->E_emitted_Gamma(aStep->GetPreStepPoint()->GetKineticEnergy()/keV);
		  //G4cout << "Energie depart gamma = " << aStep->GetPreStepPoint()->GetKineticEnergy() << G4endl;
		  //G4cout << "E emitted gamma = " << Statsgamma.E_emitted_Gamma  << G4endl;
		  runac->UpdateStatisticsGamma(Statsgamma);
		  evtac->Setnph(1);
		}
	    }
	}
    }

 if(evtac->Getnph()==1)
    {
      if((aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()  == "PhysicalScint")
	 && (aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "PhysicalScint")
	 )
	{
	  //evtac->E_deposited_Gamma(aStep->GetTotalEnergyDeposit()/keV);
	  //G4cout << "Depot d'energie new !!! " << G4endl;
	}
    }




 //####################################################################################
 //####################### PARTIE ELECTRONS DE CONVERSION #############################
 //####################################################################################

 if (aStep->GetTrack()->GetParentID() >1)
    {
      if (aStep->GetTrack()->GetCreatorProcess()->GetProcessName() == "RadioactiveDecay")
	{
	  if (StepNo == 1)
	    {
	      if (aStep->GetTrack()->GetDefinition()->GetParticleName() == "e-")
		{
		  evtac->Setnph(4);
		  Statsconv.E_emitted_Conv = aStep->GetPreStepPoint()->GetKineticEnergy()/keV;
		  //evtac->E_emitted_Elec(aStep->GetPreStepPoint()->GetKineticEnergy()/keV);
		  runac->UpdateStatisticsConv(Statsconv);
		  //G4cout << "Energie depart e conv = " << aStep->GetPreStepPoint()->GetKineticEnergy() << G4endl;
		}
	    }
	}
    }


 if(evtac->Getnph()==4)
    {
      if((aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()  == "PhysicalScint")
	 && (aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "PhysicalScint")
	 )
	{
	  evtac->E_deposited_Conv(aStep->GetTotalEnergyDeposit()/keV);
	  //G4cout << "Depot d'energie new !!! " << G4endl;
		}
    }


  



  if((aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()  == "PhysicalScint")
     && (aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "PhysicalScint")
     && (partname != "opticalphoton")
     )
    {
      //TFile *f = new TFile("/home/local1/Optical_Simulation_Arnaud/simulation_input_files/Scan.root");
      //TH2F *Scan = (TH2F*)f->Get("Scan");
      double rho = sqrt(x*x + y*y);
      G4double value=1;
      //double value = CaloMap->Interpolate(rho,z+150);
      //G4double value =  CaloMap::GetInstance()->Interpolate(rho, z+150);
      //G4double value =  CaloMap::GetInstance()->Interpolate(x, y, z+120.5);
      //G4cout << "Position X = " << x << G4endl;
      //G4cout << "Position Y = " << y << G4endl;
      //G4cout << "Position Z = " << z  << G4endl;      
      //G4cout << "Rho = " << rho << G4endl;
      //G4cout << "Factor correction = " << value << G4endl;

      
      evtac->E_deposited_Total(aStep->GetTotalEnergyDeposit()/keV);
      evtac->E_deposited_Total_Corrected((value) * aStep->GetTotalEnergyDeposit()/keV);
      //G4cout << "Energy depose step = " << aStep->GetTotalEnergyDeposit()/keV << G4endl;
      //G4cout << "Energy corrected = " << aStep->GetTotalEnergyDeposit()/keV * value << G4endl;
      //G4cout << "Depot d'energie new !!! " << G4endl;
      //f->Close();
    }



  if((aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()  == "PhysicalScint_bis")
     && (aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "PhysicalScint_bis")
     //&& (partname != "opticalphoton")
     && (aStep->GetTotalEnergyDeposit()/keV) !=0
     )
    {
      evtac->Set_Coinc();
      evtac->E_deposited_Coinc(aStep->GetTotalEnergyDeposit()/keV);
      //G4cout << "Edepbis = " << aStep->GetTotalEnergyDeposit()/keV << G4endl;
      //G4cout << "Somme Edepbis = " << evtac->Get_E_deposited_Coinc() << G4endl;
      //G4cout << "Coinc = " << evtac->Get_Coinc() << G4endl;
    }





  //PART FOR CERENKOV !!!!!!!!!!!!!!!!!!!!!!!


  if(partname == "opticalphoton" && aStep->GetTrack()->GetCreatorProcess()->GetProcessName() == "Scintillation" && StepNo ==1)
    {
      //G4cout << " Photon Scintillation!!!" << G4endl;
      evtac->CountScintillation();
      //G4cout << "n sc = " << evtac->GetCountScintillation() << G4endl;
    }

  if(partname == "opticalphoton" && aStep->GetTrack()->GetCreatorProcess()->GetProcessName() == "Cerenkov" && StepNo ==1)
    {
      //G4cout << " Photon Cerenkov !!!" << G4endl;
      evtac->CountCerenkov();
      //G4cout << "n cerenkov = " << evtac->GetCountCerenkov() << G4endl;
      //G4cout << " Birth = " << info->GetBirthLambda() << G4endl;
      //Statisticsbis.BirthLambda = info->GetBirthLambda();
      //runac->UpdateStatisticsbis(Statisticsbis);
    }


  if ( partname =="e-" && aStep->GetPreStepPoint()->GetKineticEnergy()/keV >= 130)
    {
      //G4cout << " Track Length = " << aStep->GetTrack()->GetTrackLength()/mm << G4endl;
      evtac->AddTrackLength(aStep->GetTrack()->GetStepLength()/mm);
      //G4cout << "Total Track Length = " << evtac->GetTotalTrackLength() << G4endl;
    }




}
