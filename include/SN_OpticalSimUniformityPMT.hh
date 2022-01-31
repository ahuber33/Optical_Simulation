#ifndef UNIFORMITYPMT_HH
#define UNIFORMITYPMT_HH

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
class UniformityPMT
{
private:
  UniformityPMT() {
    TFile *file = new TFile("/home/local1/Optical_Simulation_Arnaud/simulation_input_files/Anode_Uniformity/AU_GA0192_TH2.root");
    Histo = (TH2F*) file->Get("UniformityPM");

    //TFile *file = new TFile("/home/local1/Optical_Simulation_Arnaud/simulation_input_files/Mean_AU.root");
    //TFile *file = new TFile("/home/local1/Optical_Simulation_Arnaud/simulation_input_files/Anode_Uniformity/AU_GA0190_TH2.root");
    //Histo = (TH2F*) file->Get("Mean_Uniformity");
    //Histo = (TH2F*) file->Get("UniformityPM");
    gROOT->cd() ;
  }

  ~UniformityPMT();

  TH2F *Histo;

public:
  static UniformityPMT *Instance;

  static UniformityPMT *GetInstance() {
    if (Instance == NULL) Instance = new UniformityPMT;
    return Instance;}

  double Interpolate(double x, double y) {
    return Histo->Interpolate(x,y);}

};


UniformityPMT *UniformityPMT::Instance = NULL; 


#endif
