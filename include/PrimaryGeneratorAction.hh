#ifndef PrimaryGeneratorAction_H 
#define PrimaryGeneratorAction_H 1

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
  public:
    PrimaryGeneratorAction(G4int nparts = 1);
    ~PrimaryGeneratorAction();

    void GeneratePrimaries(G4Event * evt);

  private:
    G4ParticleGun * particleGun;
    G4int _nparts;
};

#endif
