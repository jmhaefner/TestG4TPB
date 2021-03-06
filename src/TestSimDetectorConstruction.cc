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
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TestSimDetectorConstruction::TestSimDetectorConstruction()
: G4VUserDetectorConstruction(),
  // fPMTvolume(0), [edit]
  fPlasticVolume(0),
  fCopperVolume(0),
  fTPBVolume(0),
  fWLSvolume(0),
  fPhotonCatcherVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TestSimDetectorConstruction::~TestSimDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* TestSimDetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Plastic box placement parameters
  G4double insertion_z = 0.*mm; // how far PMT face inserted into the box
  G4double box_thick = 5.1*mm;
  G4double int_boxXY = 69*mm, int_boxZ = 150*mm;

  // Plastic box material parameters
  G4Material* box_mat = nist->FindOrBuildMaterial("G4_TEFLON");
  box_mat->SetMaterialPropertiesTable(OpticalMaterialProperties::Teflon());
  //G4Material* box_mat = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
  //box_mat->SetMaterialPropertiesTable(OpticalMaterialProperties::Polyethylene());
  //G4Material* box_mat = nist->FindOrBuildMaterial("G4_POLYPROPYLENE");
  //box_mat->SetMaterialPropertiesTable(OpticalMaterialProperties::Polypropylene());


  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = true;


  // World
  //
  G4double world_sizeXY = 1.3*(int_boxXY + 2*box_thick);
  G4double world_sizeZ  = 2.*(int_boxZ + box_thick);
  G4double assembly_shift = - 0.1*world_sizeZ;
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


  // Plastic box
  //
  G4Box* fullBox =
    new G4Box("fullBox",                         // its name
        0.5*(int_boxXY + 2*box_thick), 0.5*(int_boxXY + 2*box_thick),
        0.5*(int_boxZ + box_thick)); //its size

  G4Box* hollow =
    new G4Box("hollow",
        0.5*int_boxXY, 0.5*int_boxXY, 0.5*int_boxZ);

  G4VSolid* solidBox =
    new G4SubtractionSolid("Box", fullBox, hollow,
        0, G4ThreeVector(0,0,-box_thick/2-0.01*mm));

  G4LogicalVolume* logicBox =
    new G4LogicalVolume(solidBox,            // its solid
                        box_mat,             // its material
                        "Box");              // its name

  G4VPhysicalVolume* physBox =
    new G4PVPlacement(0,                       // no rotation
                    G4ThreeVector(0.,0.,0.5*(int_boxZ + box_thick)+assembly_shift-insertion_z),
                    logicBox,                // its logical volume
                    "Box",                   // its name
                    logicWorld,              // its mother  volume
                    false,                   // no boolean operation
                    0,                       // copy number
                    checkOverlaps);          // overlaps checking

  // Plastic box optical surface
  G4OpticalSurface* boxSurface =  new G4OpticalSurface("boxSurface");

  new G4LogicalBorderSurface("boxSurface", physWorld, physBox, boxSurface);

  // these options are not sacred
  boxSurface->SetType(dielectric_metal);
  boxSurface->SetFinish(ground);
  boxSurface->SetModel(unified);
  boxSurface->SetMaterialPropertiesTable(OpticalMaterialProperties::Teflon());


  // Lead volume for PMT
  //
  G4double PMT_radius = 30.*mm;
  G4double PMT_thick = 5.*mm;
  G4Material* pmt_mat = nist->FindOrBuildMaterial("G4_Pb");
  pmt_mat->SetMaterialPropertiesTable(OpticalMaterialProperties::PMT());
  /* [edit]

  G4VSolid* solidPMT =
    new G4Tubs("PMT",                       // its name
        0, PMT_radius, PMT_thick/2, 0.0*deg, 360.0*deg);      // its size

  G4LogicalVolume* logicPMT =
    new G4LogicalVolume(solidPMT,           // its solid
                        pmt_mat,          // its material
                        "PMT");             // its name

  new G4PVPlacement(0,
                    G4ThreeVector(0, 0, -PMT_thick/2 + assembly_shift), // face of PMT at (0,0,0)
                    logicPMT,
                    "PMT",
                    logicWorld,
                    false,
                    0,
                    checkOverlaps);
  */



  // Copper heat sink
  //
  G4Material* Cu_mat = nist->FindOrBuildMaterial("G4_Cu");

  G4Box* fullCu =
    new G4Box("fullCu",                          // its name
        2*mm/2, 2*mm/2, 2.5*mm/2);         // its size

  G4VSolid* holeCu =
    new G4Tubs("holeCu",                       // its name
        0,1*mm, 2.5*mm/2, 0.0*deg, 360.0*deg);      // its size

  G4VSolid* solidCu =
    new G4SubtractionSolid("Cu", fullCu, holeCu,
        0, G4ThreeVector(2*mm/2, 15*mm/2, 0));

  G4LogicalVolume* logicCu =
    new G4LogicalVolume(solidCu,             // its solid
                        Cu_mat,              // its material
                        "Cu");               // its name

  // specify the angle w.r.t. the +x-axis (about the +z-axis)
  // at which the copper piece should be positioned
  G4double rot = 135.;
  G4double offset = 135.; // copper rotation angle
  // build the rotation matrix for the copper piece
  G4RotationMatrix rotm = G4RotationMatrix();
  rotm.rotateZ((rot+offset)*deg);
  // center of copper piece needs to be 23.33 mm from center of PMT in order to be
  // correctly positioned
  G4double cu_radius = 23.33;
  G4ThreeVector position = G4ThreeVector(cu_radius*std::cos(rot*deg)*mm, cu_radius*std::sin(rot*deg)*mm,
                                         -PMT_thick/2 + assembly_shift);
  // create the overall transform
  G4Transform3D transform = G4Transform3D(rotm, position);

  G4VPhysicalVolume* physCu =
  new G4PVPlacement(transform,               // rotation and position
                    logicCu,                 // its logical volume
                    "Cu",                    // its name
                    logicWorld,                // its mother  volume
                    false,                   // no boolean operation
                    0,                       // copy number
                    checkOverlaps);          // overlaps checking


  // Copper mount optical surface
  //
  /* [edit] I got rid of the copper
  G4OpticalSurface* cuSurface =  new G4OpticalSurface("cuSurface");

  new G4LogicalBorderSurface("cuSurface", physWorld, physCu, cuSurface);

  // these options are not sacred
  cuSurface->SetType(dielectric_metal);
  cuSurface->SetFinish(ground);
  cuSurface->SetModel(unified);
  //cuSurface->SetMaterialPropertiesTable(OpticalMaterialProperties::Copper());
  */

  // WLS coating from nexus

  G4Box* coating_solid =
    new G4Box("DB_WLS_COATING", 70.*mm/2, 70.*mm/2, 0.05*mm);

  G4Material* TPB = nist->FindOrBuildMaterial("G4_TEFLON");
  TPB->SetMaterialPropertiesTable(OpticalMaterialProperties::TPB());

  G4LogicalVolume* coating_logic =
    new G4LogicalVolume(coating_solid, TPB, "DB_WLS_COATING");

  new G4PVPlacement(0, G4ThreeVector(), coating_logic,
        "DB_WLS_COATING", logicWorld, false, 0, checkOverlaps);


  // Photon catcher
  //
  G4Box* fullBox2 =
    new G4Box("fullBox2",                         // its name
        0.49*world_sizeXY, 0.49*world_sizeXY, 0.49*world_sizeZ); //its size

  G4Box* hollow2 =
    new G4Box("hollow2",
        0.47*world_sizeXY, 0.47*world_sizeXY, 0.47*world_sizeZ);

  G4VSolid* solidCatcher =
    new G4SubtractionSolid("Chatcher", fullBox2, hollow2,
        0, G4ThreeVector(0,0,0));

  G4LogicalVolume* logicCatcher =
    new G4LogicalVolume(solidCatcher,            // its solid
                        pmt_mat,             // its material
                        "Catcher");              // its name

  G4VPhysicalVolume* physCatcher =
    new G4PVPlacement(0,                       // no rotation
                    G4ThreeVector(0.,0.,0.), // centered at world center
                    logicCatcher,                // its logical volume
                    "Catcher",                   // its name
                    logicWorld,              // its mother  volume
                    false,                   // no boolean operation
                    0,                       // copy number
                    checkOverlaps);          // overlaps checking

  // PMT volume
  //
  // fPMTvolume = logicPMT; //[edit]

  // Plastic volume
  //
  fPlasticVolume = logicBox;

  // Copper volume
  //
  fCopperVolume = logicCu;


  // TPB volume
  //
  fTPBVolume = coating_logic;

  // WLS volume
  //

  // Photon catcher volume
  //
  fPhotonCatcherVolume = logicCatcher;

  //always return the physical World
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
