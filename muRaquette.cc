#include <iostream>

// Enable multi-threading
// == Run Manager
// #define G4MULTITHREADED = 1
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
// == Random
#include "Randomize.hh"

// Import detector code
// == Construction
#include "DetectorConstruction.hh"
// Import physics
#include "PhysicsList.hh"
// Import Init
#include "ActionInitializer.hh"
// Import root utils
#include "RootData.hh"
// Import source
#include "SourceReader.hh"
//

RootData * rootFile;
SourceReader * source;

int main(int argc, char** argv) {

  // == Initialize ui
  G4UIExecutive * ui = 0;
  if (argc == 1) {
    ui = new G4UIExecutive(argc, argv);
  }

  // == Read source
  source = new SourceReader;

  // == Set the seed
  // long seed = 23356131;
  // G4Random::setTheSeed(seed);

  // == Construct the run manager
  //G4MTRunManager * runManager = new G4MTRunManager;
  //runManager->SetNumberOfThreads(4);
  G4RunManager * runManager = new G4RunManager;

  rootFile = new RootData();
  rootFile->Create();

  // == Build the detector
  G4VUserDetectorConstruction * detector = new DetectorConstruction;
  runManager->SetUserInitialization(detector);

  // == Build the Physics
  G4VUserPhysicsList * physics = new PhysicsList;
  runManager->SetUserInitialization(physics);

  // = Initialization =
  G4VUserActionInitialization * init = new ActionInitializer();


  // == Initialize the Run Manager
  runManager->SetUserInitialization(init);
  runManager->Initialize();

  // G4cout << "run manager setup done" << G4endl;

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
    UImanager->ApplyCommand("/control/execute ./vis.mac");
    ui->SessionStart();
    // When quitting ui
    delete ui;
  }

  // On quit, cleanup
  // G4cout << " === Done ! === " << G4endl;
  rootFile->EndOfAction();
  // G4cout << " === Results saved ! === " << G4endl;
  delete runManager;
  delete rootFile;
  delete source;

  return 0;
}
