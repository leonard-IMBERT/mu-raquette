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

const G4String RootData::UserOutputFile = "Results.root";

RootData::RootData() {
  hfile = (TFile *)0;
}

RootData::~RootData() {
}


void RootData::Create() {
  // Create the output file
  hfile = new TFile("Results.root"/*RootData::UserOutputFile*/, "RECREATE", "ROOT file with histograms");

  gFile = hfile;

  // Create the differents histograms and tree
  // == Histograms about energy deposit
  EDepByDes =  new TH1D("EDepByDes", "Energie deposee lors de desintegration;Energie (MeV);Nombre d'evenements", 300, 0, 30);
  EMuDetecteur = new TH1D("EMuDetecteur", "Energie deposee par les muons dans le detecteur;Energie (MeV);Nombre d'evenements", 300, 0, 65);
  EMuVetoA = new TH1D("EMuVetoA", "Energie deposee par les muons dans le veto A;Energie (MeV);Nombre d'evenements", 300, 0, 65);
  EMuVetoB = new TH1D("EMuVetoB", "Energie deposee par les muons dans le veto B;Energie (MeV);Nombre d'evenements", 300, 0, 65);

  /** == Tree containing the hits in the detector
   *
   * Hits are discriminated in EventAction.cc
   *
   * event
   * ├──── {boolean} TrueDecay : Flag if the hit is a decay in Geant4
   * └──── {double[2000]} EventsLog : Array containing the desposited energy (MeV) by time (each cell is 10ns since the begining of the event)
   **/
  events = new TTree("event", "The event log of signals");

  // Initialize the eventsLog array;
  EmptyLog();

  tDecay = false;

  // Linking TTree
  ELB = events->Branch("EventsLog", &eventsLog, "eventsLog[2000]/D");
  TDB = events->Branch("TrueDecay", &tDecay, "trueDecay/O");
}

G4double * RootData::GetLogs() {
  return eventsLog;
}

/**
 * Set the eventsLog array to 0
 **/
void RootData::EmptyLog() {
  for(int ii = 0; ii < 2000; ii ++) {
    eventsLog[ii] = 0.;
  }
}

/**
 * Fill the differents histograms depending of the detector and the processus
 **/
void RootData::FillHist(G4double MuEDep, G4String procName, Detect detect) {
  if(MuEDep > 0 && detect == Detect::vetoA) { EMuVetoA->Fill(MuEDep); };
  if(MuEDep > 0 && detect == Detect::vetoB) { EMuVetoB->Fill(MuEDep); };
  if(MuEDep > 0 && detect == Detect::detecteur) { EMuDetecteur->Fill(MuEDep); };
  if(MuEDep > 0 && detect == Detect::detecteur && procName == "Decay") { EDepByDes->Fill(MuEDep); };
}

void RootData::SaveLog() {
  events->Fill();
  EmptyLog();
  tDecay = false;
}

void RootData::EndOfAction() {
  hfile->Write();
  hfile->Close();
}
