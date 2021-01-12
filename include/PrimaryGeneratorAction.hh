#ifndef PrimaryGeneratorAction_H 
#define PrimaryGeneratorAction_H 1

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

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
