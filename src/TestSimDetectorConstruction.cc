//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: TestSimDetectorConstruction.cc 94307 2015-11-11 13:42:46Z gcosmo $
//
/// \file TestSimDetectorConstruction.cc
/// \brief Implementation of the TestSimDetectorConstruction class

#include "TestSimDetectorConstruction.hh"
#include "TestSimOpticalMaterialProperties.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TestSimDetectorConstruction::TestSimDetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TestSimDetectorConstruction::~TestSimDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* TestSimDetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // Plastic box parameters
  //
  G4double box_thick = 5.1*mm;
  G4double int_boxXY = 69*mm, int_boxZ = 150*mm;
  G4Material* box_mat = nist->FindOrBuildMaterial("G4_TEFLON");
  box_mat->SetMaterialPropertiesTable(OpticalMaterialProperties::Teflon());
  //G4Material* box_mat = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
  //box_mat->SetMaterialPropertiesTable(OpticalMaterialProperties::Polyethylene());
  //G4Material* box_mat = nist->FindOrBuildMaterial("G4_POLYPROPYLENE");
  //box_mat->SetMaterialPropertiesTable(OpticalMaterialProperties::Polypropylene());
  
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 1.2*(int_boxXY + 2*box_thick);
  G4double world_sizeZ  = 1.2*(int_boxZ + box_thick);
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  world_mat->SetMaterialPropertiesTable(OpticalMaterialProperties::Air());
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                     
  //     
  // Plastic box
  //  
  G4Box* solidBox =    
    new G4Box("Box",                         // its name
        0.5*(int_boxXY + 2*box_thick), 0.5*(int_boxXY + 2*box_thick), 0.5*(int_boxZ + box_thick)); //its size
      
  G4LogicalVolume* logicBox =                         
    new G4LogicalVolume(solidBox,            // its solid
                        box_mat,             // its material
                        "Box");              // its name

  new G4PVPlacement(0,                       // no rotation
                    G4ThreeVector(),         // at (0,0,0)
                    logicBox,                // its logical volume
                    "Box",                   // its name
                    logicWorld,              // its mother  volume
                    false,                   // no boolean operation
                    0,                       // copy number
                    checkOverlaps);          // overlaps checking

  //
  // Box of air inside plastic box
  //
  G4Material* hollow_mat = nist->FindOrBuildMaterial("G4_AIR");
  hollow_mat->SetMaterialPropertiesTable(OpticalMaterialProperties::Air());

  G4Box* hollow = 
    new G4Box("Hollow",
        0.5*int_boxXY, 0.5*int_boxXY, 0.5*int_boxZ);

  G4LogicalVolume* logicHollow = 
    new G4LogicalVolume(hollow,
                        hollow_mat,
                        "Hollow");

  new G4PVPlacement(0,
                    G4ThreeVector(0,0,-box_thick/2-0.01*mm),
                    logicHollow,
                    "Hollow",
                    logicBox,
                    false,
                    0,
                    checkOverlaps);

  //     
  // Copper heat sink
  // 

  G4double PMT_Z = -int_boxZ/2;
 
  G4Material* Cu_mat = nist->FindOrBuildMaterial("G4_Cu");

  G4Box* solidCu =
    new G4Box("Cu",                          // its name
        33*mm/2, 33*mm/2, 2.5*mm/2);         // its size

  G4LogicalVolume* logicCu =
    new G4LogicalVolume(solidCu,             // its solid
                        Cu_mat,              // its material
                        "Cu");               // its name

  new G4PVPlacement(0,                       // no rotation
                    G4ThreeVector(-30*mm/2,-30*mm/2,PMT_Z),         // at (0,0,0)
                    logicCu,                 // its logical volume
                    "Cu",                    // its name
                    logicBox,                // its mother  volume
                    false,                   // no boolean operation
                    0,                       // copy number
                    checkOverlaps);          // overlaps checking
                        
  //
  // Hole in copper
  //
  G4VSolid* solidHole = 
    new G4Tubs("Hole",                       // its name
        0,30*mm, 2.5*mm/2, 180.0*deg,90.0*deg);      // its size

  G4LogicalVolume* logicHole = 
    new G4LogicalVolume(solidHole,           // its solid
                        hollow_mat,          // its material
                        "Hole");             // its name

  new G4PVPlacement(0,
                    G4ThreeVector(33*mm/2, 33*mm/2, 0),
                    logicHole,
                    "Hole",
                    logicCu,
                    false,
                    0,
                    checkOverlaps);
 
  // scoring volume
  //
  fScoringVolume = logicBox;

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
