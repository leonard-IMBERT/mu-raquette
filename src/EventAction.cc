#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "RaquetteHit.hh"
#include "RootData.hh"

#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"

#include <math.h>
#include <regex>

extern RootData * rootFile;

EventAction::EventAction() {
  detectID = -1;
  vetoAID = -1;
  vetoBID = -1;
  manager = G4EventManager::GetEventManager();
}

EventAction::~EventAction() {;}

void EventAction::BeginOfEventAction(const G4Event * evt) {

  // test->SetEvent(event);

  G4SDManager * SDman = G4SDManager::GetSDMpointer();

  if(detectID && vetoAID && vetoBID)
  {
    G4String colNam;

    detectID = SDman->GetCollectionID(colNam=DetectorConstruction::RaquetteCollectionName);
    vetoAID = SDman->GetCollectionID(colNam=DetectorConstruction::VetoACollectionName);
    vetoBID = SDman->GetCollectionID(colNam=DetectorConstruction::VetoBCollectionName);
  }
}

void EventAction::EndOfEventAction(const G4Event * evt) {

  if(detectID < 0 || vetoAID < 0 || vetoBID < 0) {
    G4cout << "[Error] Can't fetch collections" << G4endl;
    return;
  }

  G4HCofThisEvent * HCE = evt->GetHCofThisEvent();
  HitsCollection* THC = 0;


  G4double MuEDepD;
  G4double MuEDepVA;
  G4double MuEDepVB;
  G4bool hasDecay = false;
  G4String procName;
  Detect detecteur;

  std::regex decay_Reg("Decay");

  //  ========== Detecteur ============

  if(HCE) { THC = (HitsCollection*)(HCE->GetHC(detectID)); }

  MuEDepD = 0.*MeV;
  procName = "";
  detecteur = Detect::other;
  G4double lastSignalD = 0;

  if(THC) {

    int n_hit = THC->entries();
    for (G4int ii=0; ii < n_hit; ii++) { // For each hit
      G4double edep = (*THC)[ii]->GetEdep();
      G4String name = (*THC)[ii]->GetName();
      G4double dTime = (*THC)[ii]->GetDeltaTime();
      detecteur = (*THC)[ii]->GetDetecteur();
      procName = (*THC)[ii]->GetProcName();


      if(edep > 0 && dTime > lastSignalD) {
        lastSignalD = dTime;
      }

      if(hasDecay == false && std::regex_search(procName, decay_Reg) && name == "mu-") {
        hasDecay = true;
      }

      MuEDepD += edep;
    }
  }
  rootFile->FillHist(MuEDepD, procName, detecteur);

  //  ========== VetoA ============

  if(HCE) { THC = (HitsCollection*)(HCE->GetHC(vetoAID)); }

  MuEDepVA = 0.*MeV;
  procName = "";
  detecteur = Detect::other;
  G4double lastSignalVA = 0;
  if(THC) {

    int n_hit = THC->entries();
    for (G4int ii=0; ii < n_hit; ii++) { // For each hit
      G4double edep = (*THC)[ii]->GetEdep();
      G4String name = (*THC)[ii]->GetName();
      G4double dTime = (*THC)[ii]->GetDeltaTime();
      procName = (*THC)[ii]->GetProcName();
      detecteur = (*THC)[ii]->GetDetecteur();

      if(edep > 0 && dTime > lastSignalVA) {
        lastSignalVA = dTime;
      }
      if(name == "mu-" && edep > 0.) {
        MuEDepVA += edep;
      }
    }

  }
  rootFile->FillHist(MuEDepVA, procName, detecteur);

  //  ========== VetoB ============

  if(HCE) { THC = (HitsCollection*)(HCE->GetHC(vetoBID)); }

  MuEDepVB = 0.*MeV;
  procName = "";
  detecteur = Detect::other;
  if(THC) {

    int n_hit = THC->entries();
    for (G4int ii=0; ii < n_hit; ii++) { // For each hit
      G4double edep = (*THC)[ii]->GetEdep();
      G4String name = (*THC)[ii]->GetName();
      procName = (*THC)[ii]->GetProcName();
      detecteur = (*THC)[ii]->GetDetecteur();

      if(name == "mu-" && edep > 0.) {
        MuEDepVB += edep;
      }
    }

  }
  rootFile->FillHist(MuEDepVB, procName, detecteur);

  // ========== Detection of decay ===============

  if(MuEDepD > 0. && MuEDepVA > 0. && MuEDepVB == 0 && lastSignalD > lastSignalVA) {
    if(HCE) { THC = (HitsCollection*)(HCE->GetHC(detectID)); }
    if(hasDecay == false) {
      manager->KeepTheCurrentEvent();
    }
    //G4cout << "Veto trigered, true decay ? : " << hasDecay << G4endl;
    if(THC) {
      int n_hit = THC->entries();
      rootFile->tDecay = hasDecay;
      for (G4int ii=0; ii < n_hit; ii++) { // For each hit
        auto entry = (*THC)[ii];

        G4int time = floor(entry->GetDeltaTime() * ns / 10);
        if (time >= 2000) {
          G4cout << time << G4endl;
        }
        if (time < 2000 && time >= 0) {
          (rootFile->GetLogs())[time] += entry->GetEdep();
        }
      }

      rootFile->SaveLog();
    }
  }
}
