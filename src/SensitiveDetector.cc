#include "SensitiveDetector.hh"

#include "G4SDManager.hh"

SensitiveDetector::SensitiveDetector(const G4String& name, const G4String& hitsCollectionName):
  G4VSensitiveDetector(name),
  _hitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
}

SensitiveDetector::~SensitiveDetector() {}

void SensitiveDetector::Initialize(G4HCofThisEvent * hce) {
  _hitsCollection = new HitsCollection(SensitiveDetectorName, collectionName[0]);

  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);

  hce->AddHitsCollection(hcID, _hitsCollection);
}

G4bool SensitiveDetector::ProcessHits(G4Step * aStep, G4TouchableHistory * ) {
  G4double edep = aStep->GetTotalEnergyDeposit();

  if(edep == 0.) return false; // If there is no energy deposit, ignore the step

  RaquetteHit * newHit = new RaquetteHit;

  // Filling the hit
  newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
  newHit->SetEdep     (edep);
  newHit->SetPos      (aStep->GetPostStepPoint()->GetPosition());
  newHit->SetName     (aStep->GetTrack()->GetParticleDefinition()->GetParticleName());
  newHit->SetVertex   (aStep->GetTrack()->GetVertexPosition());
  newHit->SetLength   (aStep->GetTrack()->GetTrackLength());
  newHit->SetIsPrimary(aStep->GetTrack()->GetParentID() == 0);

  _hitsCollection->insert(newHit);

  // If you want vis on the hits
  newHit->Draw();

  return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent *) {
}
