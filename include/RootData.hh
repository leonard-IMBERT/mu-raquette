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
    void FillTree();

    TFile * GetFileRoot() const { return hfile; };

    // Getter and setter
    G4double  GetMuEDep() const { return MuEDep; };
    void      SetMuEDep(G4double val) { MuEDep = val; };

    G4String GetProcName() const { return procName; };
    void     SetProcName(G4String name) { procName = name; };

    Detect GetDetecteur() const { return detect; };
    void   SetDetecteur(Detect de) { detect = de; };

    static const G4String UserOutputFile;


  private:
    TFile * hfile;
    TH1D * EDepByDes;
    TH1D * EMuDetecteur;
    TH1D * EMuVetoA;
    TH1D * EMuVetoB;

    G4double MuEDep;
    G4String procName;
    Detect detect;
};

#endif
