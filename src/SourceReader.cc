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

#include <math.h>

/**
 * Used to read root files asynchronously
 **/
SourceReader::SourceReader() {

  // If multithreading
  #ifdef G4MULTITHREADED
  int n_threads = 4;
  ROOT::EnableImplicitMT(n_threads);
  #endif

  // ===== File describing mu- energy and angle =====
  muData = new TFile("../ressources/Mu.root");

  // == Energy distribution ==
  distE = (TH1D*)muData->Get("Emu");
  // == Incident angular distribution ==
  distang = (TH1D*)muData->Get("tethamu");

  // Keep track of the number of particules created
  // (only for visualization)
  nRead = 0;
}

SourceReader::~SourceReader() {
  delete muData;
}


/**
 * Get a random energy from the source file
 **/
G4double SourceReader::GetRandomMuE() {
  // Used to print percentage when running
  double nTot = 6000000.;
  nRead += 1;
  G4cout << "\rNombre d'evenement produits: " << floor((((double) nRead)/nTot) * 100.) << " % (" << nRead << ")";

  G4double ret = distE->GetRandom();
  return ret;
}

/**
 * Get a random angle from the source file
 **/
G4double SourceReader::GetRandomMuTheta() {
  G4double ret = distang->GetRandom();
  return ret;
}
