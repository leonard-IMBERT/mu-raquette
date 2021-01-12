#ifndef ActionInitializer_h
#define ActionInitializer_h = 1

#include "G4VUserActionInitialization.hh"

class ActionInitializer : public G4VUserActionInitialization {
  public:
    ActionInitializer();
    ~ActionInitializer();

    void Build() const;
    void BuildForMaster() const;
};


#endif
