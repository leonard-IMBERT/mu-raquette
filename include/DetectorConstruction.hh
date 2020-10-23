#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1

// Basic detector construction class
#include "G4VUserDetectorConstruction.hh"

// Volume type
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

class DetectorConstruction : public G4VUserDetectorConstruction {
  public:
    DetectorConstruction();
    ~DetectorConstruction();

    G4VPhysicalVolume * Construct();

    static G4String RaquetteCollectionName;

  private:
    // == Logicals volumes ==
    G4LogicalVolume * experimentalHall_log;
    G4LogicalVolume * raquette_log;

    // == Physicals volumes ==
    G4VPhysicalVolume * experimentalHall_phys;
    G4VPhysicalVolume * raquette_phys;
};

#endif
