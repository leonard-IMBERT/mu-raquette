#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include <cmath>

PrimaryGeneratorAction::PrimaryGeneratorAction(G4int nparts) {
  _nparts = nparts;
  particleGun = new G4ParticleGun(_nparts);

  run = new TFile("../ressources/Mu.root");
  gammaSpec = new TFile("../ressources/GammaSpec.root");
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
  delete particleGun;
  delete run;
  delete gammaSpec;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event * evt) {
  G4ParticleTable * particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;

  G4double EMu = 0*MeV;
  G4double theta = 0.;
  G4double phi = 0.;
  G4double startX = 0.01;//(G4UniformRand() * 1.) - 0.5;
  G4double startY = 0.01;//(G4UniformRand() * 1.5) - 0.75;

  particleGun->SetParticleDefinition(particleTable->FindParticle(particleName="mu-"));

  TH1D * distE = (TH1D*)run->Get("Emu");
  TH1D * distang = (TH1D*)run->Get("tethamu");

  EMu = distE->GetRandom()*GeV;
  //theta = distang->GetRandom();
  //phi = G4UniformRand() * 2 * M_PI;


  particleGun->SetParticleEnergy(EMu);
  particleGun->SetParticleMomentumDirection(G4ThreeVector(
        sin(theta) * cos(phi),
        sin(theta) * sin(phi),
        1//cos(theta)
      ));
  particleGun->SetParticlePosition(G4ThreeVector(startX*m, startY*m, -20*cm));

  // === part postion ===

  particleGun->GeneratePrimaryVertex(evt);
}
