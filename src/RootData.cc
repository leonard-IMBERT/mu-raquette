#include "RootData.hh"

#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TNtuple.h"
#include "TRandom.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TApplication.h"
#include "TSystem.h"
#include "TH1.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TFile.h"
#include "G4ios.hh"
#include <TTree.h>
#include <TMath.h>
#include "G4ios.hh"

const G4String RootData::UserOutputFile = "Results.root";

RootData::RootData() {
  hfile = (TFile *)0;
  tree = (TTree *)0;

  MuEDep = 0.;
  GEDep = 0.;
  TotEDep = 0.;

}

RootData::~RootData() {}


void RootData::Create() {
  hfile = new TFile("Results.root"/*RootData::UserOutputFile*/, "RECREATE", "ROOT file with histograms");
  Mu = new TH1D("HMuEDep", "Énergie déposée par les muons;Nombre d'evenements;Énergie (MeV)", 300, 0, 65);
  Gamma = new TH1D("HGEDep", "Énergie déposée par les gamma;Nombre d'evenements;Énergie (MeV)", 300, 0, 65);
  Total = new TH1D("HTotEDep", "Énergie déposée par les gamma;Nombre d'evenements;Énergie (MeV)", 300, 0, 65);
  tree = new TTree("SourceTree","MuAnalysis");

  tree->Branch("MuEDep", &MuEDep, "MuEDep/D");
  tree->Branch("GEDep", &GEDep, "GEDep/D");
  tree->Branch("TotEDep", &TotEDep, "TotEDep/D");
}

void RootData::FillTree() {
  if(MuEDep > 0) { Mu->Fill(MuEDep); }
  if(GEDep > 0) { Gamma->Fill(GEDep); }
  if(TotEDep > 0) { Total->Fill(TotEDep); }
  tree->Fill();
}

void RootData::EndOfAction() {
  hfile->Write();
  hfile->Close();
}
