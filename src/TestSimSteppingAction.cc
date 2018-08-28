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
// $Id: TestSimSteppingAction.cc 74483 2013-10-09 13:37:06Z gcosmo $
//
/// \file TestSimSteppingAction.cc
/// \brief Implementation of the TestSimSteppingAction class

#include "TestSimSteppingAction.hh"
#include "TestSimEventAction.hh"
#include "TestSimDetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TestSimSteppingAction::TestSimSteppingAction(TestSimEventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fPMTvolume(0),
  fPlasticVolume(0),
  fWLSvolume(0),
  fPhotonCatcherVolume(0),
  fAirVolume(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TestSimSteppingAction::~TestSimSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TestSimSteppingAction::UserSteppingAction(const G4Step* step)
{
  if (!(fPMTvolume && fPlasticVolume && fWLSvolume 
        && fPhotonCatcherVolume && fAirVolume)) { 

    const TestSimDetectorConstruction* detectorConstruction
      = static_cast<const TestSimDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    
    if (!fPMTvolume)
        fPMTvolume = detectorConstruction->GetPMTvolume(); 
    if (!fPlasticVolume)
        fPlasticVolume = detectorConstruction->GetPlasticVolume();
    if (!fWLSvolume)
        fWLSvolume = detectorConstruction->GetWLSvolume(); 
    if (!fPhotonCatcherVolume)
        fPhotonCatcherVolume = detectorConstruction->GetPhotonCatcherVolume();
    if (!fAirVolume)
        fAirVolume = detectorConstruction->GetAirVolume(); 
  }

  // Get current volume
  G4LogicalVolume* volume = step->GetPreStepPoint()
    ->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

  // Get energy in eV
  G4double energy = step->GetPreStepPoint()->GetTotalEnergy()/eV;
  G4double max_PMT_energy = 4.13281; // eV

  // Get track status
  G4TrackStatus trackStatus = step->GetTrack()->GetTrackStatus();
  
  // In PMT volume
  if (volume == fPMTvolume) {
    if (energy < max_PMT_energy) fEventAction->FirePMT();
  }

  // In the box volume
  if (volume == fPlasticVolume) {
    // Count photon is absorbed
    if (trackStatus == fStopAndKill) {
      G4cout << "Track was absorbed" << G4endl; 
      fEventAction->CountPlasticAbsorption();
    }
    // Count photon reflected
    else { 
      G4cout << "Track was reflected" << G4endl;
      fEventAction->CountPlasticReflection();
    }
  }

  // In the photon-catcher volume
  if (volume == fPhotonCatcherVolume) {
    fEventAction->CountEscapedPhoton();
  }

  // In the world volume
  if (volume == fAirVolume) {
    fEventAction->AirScatter();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

