/// SN_OpticalSimRunAction.hh
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> 
/// Copyright: 2010 (C) NEMO3/SuperNEMO - University of Texas at Austin

#ifndef SN_OpticalSimRunAction_h
#define SN_OpticalSimRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "TROOT.h"
#include "TTree.h"
#include "SN_OpticalSimEventAction.hh"
#include "TFile.h"
#include <vector>

class G4Run;

class SN_OpticalSimRunAction : public G4UserRunAction
{

  public:
    SN_OpticalSimRunAction(char*);
   ~SN_OpticalSimRunAction();

  public:
  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);

  //adds the photon fates from an event to the run tree
  void UpdateStatistics(RunTally);
  void UpdateStatisticsbis(RunTallybis);
  void UpdateStatisticster(RunTallyter);
  void UpdateStatisticsDeposited(RunTallyDeposited);
  void UpdateStatisticsEmitted(RunTallyEmitted);
  void UpdateStatisticsGamma(RunTallyGamma);
  void UpdateStatisticsCompton(RunTallyCompton);
  void UpdateStatisticsConv(RunTallyConv);
  void UpdateStatisticsPosition(RunTallyPosition);

  //makes the individual histrograms for each PMT at the end of the run
  void MakePMTHisto(TFile *f);

  //sets the number of PMTs for MakePMTHisto, used by SN_OpticalSimGeometry
  void SetNumPMTs(G4int PMTcount){NumPMTs=PMTcount;}

  
  private:
  G4String suffixe;
  G4int NumPMTs;
  G4int NumFibers;
  RunTally Stats;
  RunTallybis Statsbis;
  RunTallyter Statster;
  RunTallyDeposited Statsdeposited;
  RunTallyEmitted Statsemitted;
  RunTallyGamma Statsgamma;
  RunTallyCompton Statscompton;
  RunTallyConv Statsconv;
  RunTallyPosition Statsposition;
  TFile *f;
  TTree *theRunTree;
  TTree *theRunTree_bis;
  TTree *theRunTree_ter;
  TTree *Tree_deposited;
  TTree *Tree_emitted;
  TTree *Tree_gamma;
  TTree *Tree_compton;
  TTree *Tree_interaction;
  TTree *Tree_conv;
  TTree *Tree_position;
  TBranch *RunBranch;
  time_t start;
  
};

#endif





