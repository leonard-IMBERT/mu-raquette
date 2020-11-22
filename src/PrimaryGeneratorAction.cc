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


  // ==> Choose part
  if (G4UniformRand() > 0.3) {
    TH1D * distE = (TH1D*)gammaSpec->Get("GammaSpec");
    G4double Egamma = distE->GetRandom();
    G4double startX = (G4UniformRand() * 0.5) - 0.25;
    G4double startY = (G4UniformRand() * 0.5) - 0.25;
    particleGun->SetParticleDefinition(particleTable->FindParticle(particleName="gamma"));


    particleGun->SetParticleEnergy(Egamma*keV);
    particleGun->SetParticleMomentumDirection(G4ThreeVector(
          0.,
          0.,
          -1.
        ));
    particleGun->SetParticlePosition(G4ThreeVector(startX*m, startY*m, 2.6*cm));
  } else {
    G4double EMu = 0*MeV;
    G4double theta = 0.;
    G4double phi = 0.;
    G4double startX = (G4UniformRand() * 2.) - 1.;
    G4double startY = (G4UniformRand() * 2.) - 1.;

    particleGun->SetParticleDefinition(particleTable->FindParticle(particleName="mu-"));

    TH1D * distE = (TH1D*)run->Get("Emu");
    TH1D * distang = (TH1D*)run->Get("tethamu");

    EMu = distE->GetRandom()*GeV;
    theta = distang->GetRandom();
    phi = G4UniformRand() * 2 * M_PI;


    particleGun->SetParticleEnergy(EMu);
    particleGun->SetParticleMomentumDirection(G4ThreeVector(
          sin(theta) * cos(phi),
          sin(theta) * sin(phi),
          cos(theta)
        ));
    particleGun->SetParticlePosition(G4ThreeVector(startX*m, startY*m, 10*cm));
  }

  // === part postion ===

  particleGun->GeneratePrimaryVertex(evt);
}
