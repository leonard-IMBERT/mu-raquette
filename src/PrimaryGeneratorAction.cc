#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
//#include "Randomize.hh"

#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TNtuple.h"
#include "TRandom.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TApplication.h"
#include "TSystem.h"
#include "TH1.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TFile.h"

PrimaryGeneratorAction::PrimaryGeneratorAction(G4int nparts) {
  _nparts = nparts;
  particleGun = new G4ParticleGun(_nparts);

}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
  delete particleGun;

}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event * evt) {
  G4ParticleTable * particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;

  // ==> Choose part
  //if (G4UniformRand() > 0.7) {
  //} else {
  //}

  // === part definition ===
  particleGun->SetParticleDefinition(particleTable->FindParticle(particleName="mu-"));
  particleGun->SetParticleEnergy(20.*MeV);

  // === part postion ===
  particleGun->SetParticlePosition(G4ThreeVector(0.*m,0.*m,-2.5*m));
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));

  particleGun->GeneratePrimaryVertex(evt);
}
