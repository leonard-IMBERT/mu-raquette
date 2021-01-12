#include "SourceReader.hh"

#include "G4SystemOfUnits.hh"

#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TNtuple.h"
#include "TRandom.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TApplication.h"
#include "TSystem.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TFile.h"
#include "G4ios.hh"

SourceReader::SourceReader() {
  #ifdef G4MULTITHREADED
  int n_threads = 4;
  ROOT::EnableImplicitMT(n_threads);
  #endif

  muData = new TFile("../ressources/Mu.root");
  distE = (TH1D*)muData->Get("Emu");
  distang = (TH1D*)muData->Get("tethamu");
  nRead = 0;
}

SourceReader::~SourceReader() {
  delete muData;
}

G4double SourceReader::GetRandomMuE() {
  nRead += 1;
  G4cout << "Nombre d'evenement ecrits: " << nRead << G4endl;
  G4double ret = distE->GetRandom();
  return ret;
}

G4double SourceReader::GetRandomMuTheta() {
  G4double ret = distang->GetRandom();
  return ret;
}
