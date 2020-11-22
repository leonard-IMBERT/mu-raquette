#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "RaquetteHit.hh"
#include "RootData.hh"

#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"

extern RootData * rootFile;

EventAction::EventAction() {
  eventID = -1;
  collidID = -1;
}

EventAction::~EventAction() {;}

void EventAction::BeginOfEventAction(const G4Event * evt) {

  eventID = evt->GetEventID();
  // test->SetEvent(event);

  G4SDManager * SDman = G4SDManager::GetSDMpointer();

  if(collidID)
  {
    G4String colNam;

    collidID = SDman->GetCollectionID(colNam=DetectorConstruction::RaquetteCollectionName);
  }
}

void EventAction::EndOfEventAction(const G4Event * evt) {

  // G4cout << ">>> Event No " << evt->GetEventID() << G4endl;
  //
  G4String primName = evt->GetPrimaryVertex()->GetPrimary()->GetParticleDefinition()->GetParticleName();


  if(collidID<0) return;

  G4HCofThisEvent * HCE = evt->GetHCofThisEvent();
  HitsCollection* THC = 0;


  if(HCE) {
    THC = (HitsCollection*)(HCE->GetHC(collidID));
  }

  G4double MuEDep = 0.*MeV;
  G4double GEDep = 0.*MeV;
  G4double TotEDep = 0.*MeV;
  if(THC) {

    int n_hit = THC->entries();
    //G4cout << "     " << n_hit << " hits are stored in the collections." << G4endl;
    for (G4int ii=0; ii < n_hit; ii++) { // For each hit
      G4double edep = (*THC)[ii]->GetEdep();
      G4String name = (*THC)[ii]->GetName();
      if(primName == "mu-") {
        MuEDep += edep;
      }
      if(primName == "gamma") {
        GEDep += edep;
      }
      if(primName == "mu-" || primName == "gamma") {
        TotEDep += edep;
      }
    }

  }
  // if (MuEDep != 0. || GEDep != 0.) {
    rootFile->SetMuEDep(MuEDep);
    rootFile->SetGEDep(GEDep);
    rootFile->SetTotEDep(TotEDep);
    rootFile->FillTree();
  // }
}
