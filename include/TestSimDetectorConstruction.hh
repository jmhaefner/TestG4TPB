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
// $Id: TestSimDetectorConstruction.hh 69565 2013-05-08 12:35:31Z gcosmo $
//
/// \file TestSimDetectorConstruction.hh
/// \brief Definition of the TestSimDetectorConstruction class

#ifndef TestSimDetectorConstruction_h
#define TestSimDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class TestSimAnalysisManager;

/// Detector construction class to define materials and geometry.

class TestSimDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    TestSimDetectorConstruction();
    virtual ~TestSimDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    
    G4LogicalVolume* GetPMTvolume() const { return fPMTvolume; }
    G4LogicalVolume* GetPlasticVolume() const { return fPlasticVolume; }
    G4LogicalVolume* GetCopperVolume() const { return fCopperVolume; }
    G4LogicalVolume* GetWLSvolume() const { return fWLSvolume; }
    G4LogicalVolume* GetPhotonCatcherVolume() const { return fPhotonCatcherVolume; }

  protected:
    G4LogicalVolume*  fPMTvolume;
    G4LogicalVolume*  fPlasticVolume;
    G4LogicalVolume*  fCopperVolume;
    G4LogicalVolume*  fWLSvolume;
    G4LogicalVolume*  fPhotonCatcherVolume;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

