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

  G4SDManager * SDman = G4SDManager::GetSDMpointer();

  // Fetch collection ids
  if(detectID && vetoAID && vetoBID)
  {
    G4String colNam;

    detectID = SDman->GetCollectionID(colNam=DetectorConstruction::RaquetteCollectionName);
    vetoAID = SDman->GetCollectionID(colNam=DetectorConstruction::VetoACollectionName);
    vetoBID = SDman->GetCollectionID(colNam=DetectorConstruction::VetoBCollectionName);
  }
}

void EventAction::EndOfEventAction(const G4Event * evt) {

  HitsCollection * THC = 0;

  // Should not happened but if collections does not exists
  if(detectID < 0 || vetoAID < 0 || vetoBID < 0) {
    G4cout << "[Error] Can't fetch collections" << G4endl;
    return;
  }

  // Fetch all collections relatives to this event
  G4HCofThisEvent * HCE = evt->GetHCofThisEvent();


  // Energy deposit in detector (D2)
  G4double MuEDepD;

  // Energy deposit in vetoA (D1)
  G4double MuEDepVA;

  // Energy deposit in vetoB (D3)
  G4double MuEDepVB;

  // Does Geant4 tell us this event decayed in the detector ? (D2)
  G4bool hasDecay = false;


  // Temporary var to store process name and detector
  G4String procName;
  Detect detecteur;


  // Time of last signal in detector
  G4double lastSignalD = 0;
  // Time of last signal in vetoA
  G4double lastSignalVA = 0;

  // Regex to detect "Decay" in process name
  std::regex decay_Reg("Decay");

  //  ========== Detector ============

  if(HCE) { THC = (HitsCollection*)(HCE->GetHC(detectID)); }

  MuEDepD = 0.*MeV;
  procName = "";
  detecteur = Detect::other;

  if(THC) {

    int n_hit = THC->entries();
    for (G4int ii=0; ii < n_hit; ii++) { // For each hit
      G4double edep = (*THC)[ii]->GetEdep();
      G4String name = (*THC)[ii]->GetName();
      G4double dTime = (*THC)[ii]->GetDeltaTime();
      detecteur = (*THC)[ii]->GetDetecteur();
      procName = (*THC)[ii]->GetProcName();


      // If energy deposit in detector, actualize the time of last signal (if needed)
      if(edep > 0 && dTime > lastSignalD) {
        lastSignalD = dTime;
      }

      // If a mu- decayed in the detector, flag it
      if(hasDecay == false && std::regex_search(procName, decay_Reg) && name == "mu-") {
        hasDecay = true;
      }

      MuEDepD += edep;
    }
  }

  // Store the sum of energy deposit in detector in an hist
  rootFile->FillHist(MuEDepD, procName, detecteur);


  //  ========== VetoA ============

  if(HCE) { THC = (HitsCollection*)(HCE->GetHC(vetoAID)); }

  MuEDepVA = 0.*MeV;
  procName = "";
  detecteur = Detect::other;
  if(THC) {

    int n_hit = THC->entries();
    for (G4int ii=0; ii < n_hit; ii++) { // For each hit
      G4double edep = (*THC)[ii]->GetEdep();
      G4String name = (*THC)[ii]->GetName();
      G4double dTime = (*THC)[ii]->GetDeltaTime();
      procName = (*THC)[ii]->GetProcName();
      detecteur = (*THC)[ii]->GetDetecteur();

      // If energy deposit in vetoA, actualize the time of last signal (if needed)
      if(edep > 0 && dTime > lastSignalVA) {
        lastSignalVA = dTime;
      }

      MuEDepVA += edep;
    }

  }

  // Store the sum of energy deposit in vetoA in an hist
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

      MuEDepVB += edep;
    }

  }


  // Store the sum of energy deposit in vetoB in an hist
  rootFile->FillHist(MuEDepVB, procName, detecteur);

  // ========== Detection of decay ===============

  /**
   * Here we veto some events.
   * First part of the condition is:
   *
   * D1 && D2 && !D3 (energy deposit in detector, and vetoA but none in vetoB)
   *
   * Second part is to ignore retrodiffusion (the oldest signal must come from the detector)
   * The experiment would ignore those signals anyway
   **/
  if(MuEDepD > 0. && MuEDepVA > 0. && MuEDepVB == 0 && lastSignalD > lastSignalVA) {

    if(HCE) { THC = (HitsCollection*)(HCE->GetHC(detectID)); }

    // Tell the manager to keep an event for review, use it to debug mainly
    /*
    if(hasDecay == false) {
      manager->KeepTheCurrentEvent();
    }*/

    if(THC) {
      int n_hit = THC->entries();

      // Flag in the TTree if it was a decay in Geant4
      rootFile->tDecay = hasDecay;

      // Store the hits in the detector into a time table (binned by 10 ns)
      for (G4int ii=0; ii < n_hit; ii++) { // For each hit
        auto entry = (*THC)[ii];
        /**
         * Time of deposit is floored
         * bin 0: 0 to 10 ns
         * |
         * bin n: (n - 1) * 10 ns to n * 10 ns
         * |
         * ...
         **/
        G4int time = floor(entry->GetDeltaTime() * ns / 10);

        // If the time of deposit is > to 20ms, ignore but print for debug
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
