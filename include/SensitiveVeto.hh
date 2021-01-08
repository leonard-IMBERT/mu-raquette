#ifndef SensitiveVeto_H
#define SensitiveVeto_H 1

#include "RaquetteHit.hh"

#include "G4HCofThisEvent.hh"
#include "G4VSensitiveDetector.hh"

class SensitiveVeto : public G4VSensitiveDetector {
  public:
    SensitiveVeto(const G4String& name, const G4String& collectionName);
    ~SensitiveVeto();

    void Initialize(G4HCofThisEvent * hitCollection);
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    void EndOfEvent(G4HCofThisEvent* hitCollection);

    G4int desCount;

  private:
    HitsCollection* _hitsCollection;
};

#endif
