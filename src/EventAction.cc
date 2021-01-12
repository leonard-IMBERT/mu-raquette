#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "RaquetteHit.hh"
#include "RootData.hh"

#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"

extern RootData * rootFile;

EventAction::EventAction() {
  detectID = -1;
  vetoAID = -1;
  vetoBID = -1;
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

  //  ========== Detecteur ============

  if(HCE) { THC = (HitsCollection*)(HCE->GetHC(detectID)); }

  MuEDepD = 0.*MeV;
  procName = "";
  detecteur = Detect::other;

  if(THC) {

    int n_hit = THC->entries();
    for (G4int ii=0; ii < n_hit; ii++) { // For each hit
      G4double edep = (*THC)[ii]->GetEdep();
      G4String name = (*THC)[ii]->GetName();
      if(name == "mu-" && edep > 0.) {
        procName = (*THC)[ii]->GetProcName();
        if(hasDecay == false && procName == "Decay") {
          hasDecay = true;
        }
        detecteur = (*THC)[ii]->GetDetecteur();
        MuEDepD += edep;
      }
    }

  }
  // if (MuEDep != 0. || GEDep != 0.) {
  rootFile->FillTree(MuEDepD, procName, detecteur);
  // }

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
      if(name == "mu-" && edep > 0.) {
        procName = (*THC)[ii]->GetProcName();
        detecteur = (*THC)[ii]->GetDetecteur();
        MuEDepVA += edep;
      }
    }

  }
  // if (MuEDepVA != 0. || GEDep != 0.) {
  rootFile->FillTree(MuEDepVA, procName, detecteur);
  // }

  //  ========== VetoB ============

  if(HCE) { THC = (HitsCollection*)(HCE->GetHC(vetoBID)); }

  MuEDepVB = 0.*MeV;
  procName = "";
  detecteur = Detect::other;
  if(THC) {

    int n_hit = THC->entries();
    //G4cout << "     " << n_hit << " hits are stored in the collections." << G4endl;
    for (G4int ii=0; ii < n_hit; ii++) { // For each hit
      G4double edep = (*THC)[ii]->GetEdep();
      G4String name = (*THC)[ii]->GetName();
      if(name == "mu-" && edep > 0.) {
        procName = (*THC)[ii]->GetProcName();
        detecteur = (*THC)[ii]->GetDetecteur();
        MuEDepVB += edep;
      }
    }

  }
  // if (MuEDepVB != 0. || GEDep != 0.) {
  rootFile->FillTree(MuEDepVB, procName, detecteur);
  // }
  if(MuEDepD > 0. && MuEDepVA > 0. && MuEDepVB == 0) {
    // G4cout << "Veto trigered, true decay ? : " << hasDecay << G4endl;
  }
}
