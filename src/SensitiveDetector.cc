#include "SensitiveDetector.hh"

#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VProcess.hh"

SensitiveDetector::SensitiveDetector(const G4String& name, const G4String& hitsCollectionName):
  G4VSensitiveDetector(name),
  desCount(0),
  _hitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
  desCount = 0;
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
  G4StepPoint * prePoint = aStep->GetPostStepPoint();
  G4String name = aStep->GetTrack()->GetParticleDefinition()->GetParticleName();
  G4String procName = "unknow";
  if(prePoint != NULL) {
    procName = prePoint->GetProcessDefinedStep()->GetProcessName();
    if(procName == "Decay" && name == "mu-") {
      desCount += 1;
    }
  }

  RaquetteHit * newHit = new RaquetteHit;

  // Filling the hit
  newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
  newHit->SetEdep     (edep);
  newHit->SetPos      (aStep->GetPostStepPoint()->GetPosition());
  newHit->SetName     (aStep->GetTrack()->GetParticleDefinition()->GetParticleName());
  newHit->SetVertex   (aStep->GetTrack()->GetVertexPosition());
  newHit->SetLength   (aStep->GetTrack()->GetTrackLength());
  newHit->SetIsPrimary(aStep->GetTrack()->GetParentID() == 0);
  newHit->SetDetecteur(Detect::detecteur);
  newHit->SetProcName (procName);

  _hitsCollection->insert(newHit);

  // If you want vis on the hits
  //newHit->Draw();


  return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent *) {
}
