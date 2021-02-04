#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"
#include "SensitiveVeto.hh"

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
G4String DetectorConstruction::VetoACollectionName = "VetoAHitsCollection";
G4String DetectorConstruction::VetoBCollectionName = "VetoBHitsCollection";

DetectorConstruction::DetectorConstruction() :
  experimentalHall_log(0), rs1_log(0), r2_log(0), rs3_log(0),
  experimentalHall_phys(0),
  r1s1_phys(0), r1s2_phys(0), r2_phys(0), r3s1_phys(0), r3s2_phys(0) {;}

DetectorConstruction::~DetectorConstruction() {}

void DetectorConstruction::ConstructSDandField() {
  // ====== Sensible volume ======
  G4String sensitiveName = "RaquetteSD";
  SensitiveDetector * sd = new SensitiveDetector(sensitiveName, RaquetteCollectionName);
  G4String vetoAName = "VetoASD";
  SensitiveVeto * vA = new SensitiveVeto(vetoAName, VetoACollectionName);
  G4String vetoBName = "VetoBSD";
  SensitiveVeto * vB = new SensitiveVeto(vetoBName, VetoBCollectionName);
  G4SDManager::GetSDMpointer()->AddNewDetector(sd);
  G4SDManager::GetSDMpointer()->AddNewDetector(vA);
  G4SDManager::GetSDMpointer()->AddNewDetector(vB);
  rs1_log->SetSensitiveDetector(vA);
  r2_log->SetSensitiveDetector(sd);
  rs3_log->SetSensitiveDetector(vB);
}

G4VPhysicalVolume * DetectorConstruction::Construct() {
  // ====== Materials =======

  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;
  G4int nel;

  // Air
  G4Element * N = new G4Element("Nitrogen", "N", z=7., a= 14.01*g/mole);
  G4Element * O = new G4Element("Oxygen"  , "O", z=8., a= 16.00*g/mole);

  G4Material * Air = new G4Material("Air", density= 1.29, nel=2);
  Air->AddElement(N, 70*perCent);
  Air->AddElement(O, 30*perCent);

  // C9H10
  G4Element * C = new G4Element("Carbon", "C", z=6., a= 12.00*g/mole);
  G4Element * H = new G4Element("Hydrogen", "H", z =1., a= 1.0079*g/mole);

  G4Material * Detect = new G4Material("Detector", density= 1.03*g/cm3, nel=2);
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
  // ======== Raquette 1
  // =========== Square 1
  G4double rs_z = (3./2.)*cm;
  G4double rs_x = (47.5/2.)*cm;
  G4double rs_y = (49./2.)*cm;
  G4double r2_z = 2.5*cm;
  G4double r2_x = 13.1*cm;
  G4double r2_y = 74.8*cm;

  G4Box * rs_box   = new G4Box("rs", rs_x, rs_y, rs_z);
  rs1_log           = new G4LogicalVolume(rs_box, Detect, "rs1_log");
  r1s1_phys        = new G4PVPlacement(0, G4ThreeVector(0, rs_y+0.01, -rs_z - r2_z - 0.01), rs1_log, "r1s1", experimentalHall_log, false, 0);

  // =========== Square 2
  r1s2_phys        = new G4PVPlacement(0, G4ThreeVector(0, -rs_y-0.01, -rs_z - r2_z - 0.01), rs1_log, "r1s2", experimentalHall_log, false, 1);

  // ======== Raquette 2
  G4Box * r2_box   = new G4Box("r2", r2_x, r2_y, r2_z);
  r2_log           = new G4LogicalVolume(r2_box, Detect, "r2_log");
  r2_phys          = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), r2_log, "r2", experimentalHall_log, false, 0);

  // ======== Raquette 3
  rs3_log           = new G4LogicalVolume(rs_box, Detect, "rs3_log");
  // =========== Square 1
  r3s1_phys        = new G4PVPlacement(0, G4ThreeVector(0, rs_y+0.01, +rs_z + r2_z + 0.01), rs3_log, "r3s1", experimentalHall_log, false, 2);
  // =========== Square 2
  r3s2_phys        = new G4PVPlacement(0, G4ThreeVector(0, -rs_y-0.01, +rs_z + r2_z + 0.01), rs3_log, "r3s2", experimentalHall_log, false, 3);

  // ====== Vis attributes ======
  
  experimentalHall_log->SetVisAttributes(new G4VisAttributes(false));
  rs1_log             ->SetVisAttributes(new G4VisAttributes(G4Colour(0., 1., 1.)));
  r2_log              ->SetVisAttributes(new G4VisAttributes(G4Colour(0., 1., 0.5)));
  rs3_log             ->SetVisAttributes(new G4VisAttributes(G4Colour(0., 0.5, 1.)));

  return experimentalHall_phys;
}
