#ifndef CALOMAP_HH
#define CALOMAP_HH

#include "TFile.h"
#include "TROOT.h"
#include "TH2F.h"
#include "TH3F.h"
class CaloMap
{
private:
  CaloMap() {
    //TFile *file = new TFile("/home/local1/Optical_Simulation_Arnaud/simulation_input_files/Scan.root");
    TFile *file = new TFile("/nemo243/Optical_Simulation_Arnaud/simulation_input_files/Scan_3D.root", "READ");
    //Map = (TH2F*) file->Get("Scan");
    Map = (TH3F*) file->Get("Correction_3D");
    gROOT->cd() ;
  }

  ~CaloMap();

  TH3F *Map;

public:
  static CaloMap *Instance;

  static CaloMap *GetInstance() {
    if (Instance == NULL) Instance = new CaloMap;
    return Instance;}

  //double Interpolate(double x, double y) {
  //return Map->Interpolate(x, y);}

  double Interpolate(double x, double y, double z) {
  return Map->Interpolate(x, y, z);}

  // double Interpolate(double x, double y, double z) {
  //   int binX = ((x+150)/5) +1;
  //   int binY = ((y+150)/5) +1;
  //   int binZ = ((z)/5) +1;

  //   return Map->GetBinContent(binX, binY, binZ);}


};


CaloMap *CaloMap::Instance = NULL; 


#endif
