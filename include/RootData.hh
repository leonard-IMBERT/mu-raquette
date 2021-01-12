#ifndef RootData_H
#define RootData_H 1

#include "G4String.hh"
#include "G4Types.hh"

#include "Detect.hh"

#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"

class RootData {
  public:

    RootData();
    ~RootData();

    void Create();
    void EndOfAction();
    void FillTree(G4double MuEDep, G4String procName, Detect detect);

    TFile * GetFileRoot() const { return hfile; };

    static const G4String UserOutputFile;


  private:
    TFile * hfile;
    TH1D * EDepByDes;
    TH1D * EMuDetecteur;
    TH1D * EMuVetoA;
    TH1D * EMuVetoB;
};

#endif
