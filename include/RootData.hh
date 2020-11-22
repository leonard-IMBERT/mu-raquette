#ifndef RootData_H
#define RootData_H 1

#include "G4String.hh"
#include "G4Types.hh"

#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"

class RootData {
  public:

    RootData();
    ~RootData();

    void Create();
    void EndOfAction();
    void FillTree();

    TTree * GetTree() const { return tree; };
    TFile * GetFileRoot() const { return hfile; };

    // Getter and setter
    G4double  GetMuEDep() const { return MuEDep; };
    void      SetMuEDep(G4double val) { MuEDep = val; };

    G4double  GetGEDep() const { return GEDep; };
    void      SetGEDep(G4double val) { GEDep = val; };

    G4double  GetTotEDep() const { return TotEDep; };
    void      SetTotEDep(G4double val) { TotEDep = val; };

    static const G4String UserOutputFile;


  private:
    TFile * hfile;
    TTree * tree;

    TH1D * Mu;
    TH1D * Gamma;
    TH1D * Total;

    G4double MuEDep;
    G4double GEDep;
    G4double TotEDep;
};

#endif
