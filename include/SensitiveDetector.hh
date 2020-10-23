#ifndef SensitiveDetector_H
#define SensitiveDetector_H 1

#include "RaquetteHit.hh"

#include "G4HCofThisEvent.hh"
#include "G4VSensitiveDetector.hh"

class SensitiveDetector : public G4VSensitiveDetector {
  public:
    SensitiveDetector(const G4String& name, const G4String& collectionName);
    ~SensitiveDetector();

    void Initialize(G4HCofThisEvent * hitCollection);
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    void EndOfEvent(G4HCofThisEvent* hitCollection);

  private:
    HitsCollection* _hitsCollection;
};

#endif
