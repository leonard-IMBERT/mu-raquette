#include <iostream>

// Enable multi-threading
// == Run Manager
#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

// Import from G4
// == UI
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
// == Vis
#include "G4VisExecutive.hh"

// Import detector code
// == Construction
#include "DetectorConstruction.hh"
// Import physics
#include "PhysicsList.hh"
// Import big gunz
#include "PrimaryGeneratorAction.hh"
// Import analysis
#include "EventAction.hh"
// Import root utils
#include "RootData.hh"

RootData * rootFile;

int main(int argc, char** argv) {

  // == Initialize ui
  G4UIExecutive * ui = 0;
  if (argc == 1) {
    ui = new G4UIExecutive(argc, argv);
  }

  // == Construct the run manager
  G4RunManager * runManager = new G4RunManager;

  rootFile = new RootData();
  rootFile->Create();

  // == Build the detector
  G4VUserDetectorConstruction * detector = new DetectorConstruction;
  runManager->SetUserInitialization(detector);

  // == Build the Physics
  G4VUserPhysicsList * physics = new PhysicsList;
  runManager->SetUserInitialization(physics);

  // == Set The different actions
  // ==== Mandatory pew pew ====
  G4VUserPrimaryGeneratorAction * prim_action = new PrimaryGeneratorAction;
  runManager->SetUserAction(prim_action);

  // ==== Analysis ====
  G4UserEventAction * action = new EventAction;
  runManager->SetUserAction(action);

  // == Initialize the Run Manager
  runManager->Initialize();

  G4cout << "run manager setup done" << G4endl;

  // == Initialize Vis
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // Get the pointer to the UI Manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (!ui) {
    // If no ui
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command + fileName);
  } else {
    // If interactive mode
    UImanager->ApplyCommand("/control/execute vis.mac");
    ui->SessionStart();
    // When quitting ui
    delete ui;
  }

  // On quit, cleanup
  rootFile->EndOfAction();
  delete runManager;
  delete rootFile;

  return 0;
}
