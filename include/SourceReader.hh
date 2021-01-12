#ifndef SourceReader_h
#define SourceReader_h = 1

#include "TFile.h"
#include "TH1.h"
#include "G4Types.hh"

class SourceReader {
  public:
    SourceReader();
    ~SourceReader();

    G4double GetRandomMuE();
    G4double GetRandomMuTheta();

  private:
    TFile * muData;
    TH1D * distE;
    TH1D * distang;

    G4int nRead;
};

#endif
