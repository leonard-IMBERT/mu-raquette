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
    void ConstructSDandField();

    static G4String RaquetteCollectionName;
    static G4String VetoACollectionName;
    static G4String VetoBCollectionName;

  private:
    // == Logicals volumes ==
    G4LogicalVolume * experimentalHall_log;
    G4LogicalVolume * rs1_log;
    G4LogicalVolume * r2_log;
    G4LogicalVolume * rs3_log;

    // == Physicals volumes ==
    G4VPhysicalVolume * experimentalHall_phys;
    G4VPhysicalVolume * r1s1_phys;
    G4VPhysicalVolume * r1s2_phys;
    G4VPhysicalVolume * r2_phys;
    G4VPhysicalVolume * r3s1_phys;
    G4VPhysicalVolume * r3s2_phys;
};

#endif
