#ifndef EventAction_H
#define EventAction_H 1

#include "G4Event.hh"
#include "G4UserEventAction.hh"

class EventAction : public G4UserEventAction {
  public:
    EventAction();
    ~EventAction();

    void BeginOfEventAction(const G4Event *);
    void EndOfEventAction(const G4Event *);

  private:
    G4int eventID;
    G4int collidID;
};

#endif
