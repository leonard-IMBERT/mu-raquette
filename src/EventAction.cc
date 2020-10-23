#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "RaquetteHit.hh"

#include "G4SDManager.hh"

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

  G4cout << ">>> Event No " << evt->GetEventID() << G4endl;


  if(collidID<0) return;

  G4HCofThisEvent * HCE = evt->GetHCofThisEvent();
  HitsCollection* THC = 0;


  if(HCE)
  {
    THC = (HitsCollection*)(HCE->GetHC(collidID));
  }

  //Scintill.
  //G4double totE = 0;
  if(THC)
  {
    int n_hit = THC->entries();
    G4cout << "     " << n_hit << " hits are stored in the collections." << G4endl;


    for (G4int i=0;i<n_hit;i++) { // For each hit
      G4double edep = (*THC)[i]->GetEdep();
    }
  }
}
