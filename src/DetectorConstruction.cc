#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"

#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VisAttributes.hh"

G4String DetectorConstruction::RaquetteCollectionName = "RaquetteHitsCollection";

DetectorConstruction::DetectorConstruction() :
  experimentalHall_log(0), raquette_log(0),
  experimentalHall_phys(0), raquette_phys(0) {;}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume * DetectorConstruction::Construct() {
  // ====== Materials =======

  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;
  G4int nel;

  // Air
  G4Element * N = new G4Element("Nitrogen", "N", z=7., a= 14.01*g/mole);
  G4Element * O = new G4Element("Oxygen"  , "O", z=8., a= 16.00*g/mole);

  G4Material * Air = new G4Material("Air", density= 1.29*mg/cm3, nel=2);
  Air->AddElement(N, 70*perCent);
  Air->AddElement(O, 30*perCent);

  // C9H10
  G4Element * C = new G4Element("Carbon", "C", z=6., a= 12.00*g/mole);
  G4Element * H = new G4Element("Hydrogen", "H", z =1., a= 1.0079*g/mole);

  G4Material * Detect = new G4Material("Detector", density= 0.8*g/cm3, nel=2);
  Detect->AddElement(C, 47.37*perCent);
  Detect->AddElement(H, 52.63*perCent);

  // ====== Volumes ======
  // === Experimental Hall
  G4double expHall_x = 3.0*m;
  G4double expHall_y = 3.0*m;
  G4double expHall_z = 3.0*m;

  G4Box * experimentalHall_box = new G4Box("expHall_box", expHall_x, expHall_y, expHall_z);
  experimentalHall_log         = new G4LogicalVolume(experimentalHall_box, Air, "expHall_log");
  //                                 G4PVPlacement(rot, translate, logicalVolume, name, mother volume, many ?, nCopy)
  experimentalHall_phys        = new G4PVPlacement(0, G4ThreeVector(), experimentalHall_log, "expHall", 0, false, 0);

  // === Raquette
  // Z -> height axis
  G4double raquette_z = 2.5*cm;
  G4double raquette_x = 25*cm;
  G4double raquette_y = 25*cm;

  G4Box * raquette_box = new G4Box("raquette", raquette_x, raquette_y, raquette_z);
  raquette_log         = new G4LogicalVolume(raquette_box, Detect, "raquette_log");
  raquette_phys        = new G4PVPlacement(0, G4ThreeVector(), raquette_log, "raquette", experimentalHall_log, false, 0);

  // ====== Sensible volume ======
  G4String sensitiveName = "RaquetteSD";
  SensitiveDetector * sd = new SensitiveDetector(sensitiveName, RaquetteCollectionName);
  G4SDManager::GetSDMpointer()->AddNewDetector(sd);
  raquette_log->SetSensitiveDetector(sd);

  // ====== Vis attributes ======
  
  experimentalHall_log->SetVisAttributes(new G4VisAttributes(false));
  raquette_log        ->SetVisAttributes(new G4VisAttributes(G4Colour(0., 1., 1.)));

  return experimentalHall_phys;
}
