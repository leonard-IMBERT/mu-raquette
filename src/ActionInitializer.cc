#include "ActionInitializer.hh"
// Import big gunz
#include "PrimaryGeneratorAction.hh"
// Import analysis
#include "EventAction.hh"


ActionInitializer::ActionInitializer(): G4VUserActionInitialization() {;}

ActionInitializer::~ActionInitializer() {;}

void ActionInitializer::Build() const {
  // == Set The different actions
  // ==== Mandatory pew pew ====
  G4VUserPrimaryGeneratorAction * prim_action = new PrimaryGeneratorAction;
  SetUserAction(prim_action);

  // ==== Analysis ====
  G4UserEventAction * action = new EventAction;
  SetUserAction(action);
}

void ActionInitializer::BuildForMaster() const {
  ;
}
