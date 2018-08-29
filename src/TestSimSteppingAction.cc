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
  fCopperVolume(0),
  fWLSvolume(0),
  fPhotonCatcherVolume(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TestSimSteppingAction::~TestSimSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TestSimSteppingAction::UserSteppingAction(const G4Step* step)
{
  if (!(fPMTvolume && fPlasticVolume && fCopperVolume 
        && fWLSvolume && fPhotonCatcherVolume)) { 

    const TestSimDetectorConstruction* detectorConstruction
      = static_cast<const TestSimDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    
    if (!fPMTvolume)
        fPMTvolume = detectorConstruction->GetPMTvolume(); 
    if (!fPlasticVolume)
        fPlasticVolume = detectorConstruction->GetPlasticVolume();
    if (!fCopperVolume)
        fCopperVolume = detectorConstruction->GetCopperVolume();
    if (!fWLSvolume)
        fWLSvolume = detectorConstruction->GetWLSvolume(); 
    if (!fPhotonCatcherVolume)
        fPhotonCatcherVolume = detectorConstruction->GetPhotonCatcherVolume();
  }

  // Get next volume if there is one
  G4StepPoint* pstep = step->GetPostStepPoint();
  G4LogicalVolume* volume_ps = 0;
  if (pstep) {
    volume_ps = pstep->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
  }

  // Get energy in eV
  G4double energy = step->GetPreStepPoint()->GetTotalEnergy()/eV;
  G4double max_PMT_energy = 4.13281; // eV

  // Get track status
  G4TrackStatus trackStatus = step->GetTrack()->GetTrackStatus();
  
  // Absorbed by box
  if (volume_ps == fPlasticVolume && trackStatus == fStopAndKill) {
    fEventAction->CountPlasticAbsorption();
    G4cout << "Photon was absorbed by plastic" << G4endl;
  }
  // Reflected by box
  if (volume_ps == fPlasticVolume && trackStatus != fStopAndKill) {
    fEventAction->CountPlasticReflection();
    G4cout << "Photon was reflected by plastic" << G4endl;
  }

  // Absorbed by copper
  if (volume_ps == fCopperVolume && trackStatus == fStopAndKill) {
    fEventAction->CountCopperAbsorption();
    G4cout << "Photon was absorbed by copper" << G4endl;
  }
  // Reflected by copper
  if (volume_ps == fCopperVolume && trackStatus != fStopAndKill) {
    fEventAction->CountCopperReflection();
    G4cout << "Photon was reflected by copper" << G4endl;
  }

  // PMT fired/misfired
  if (volume_ps == fPMTvolume && trackStatus == fStopAndKill) {
    if (energy < max_PMT_energy) {
      fEventAction->FirePMT();
      G4cout << "PMT Fired" << G4endl;
    }
    else {
      fEventAction->MisfirePMT();
      G4cout << "PMT Misfired (UV photon)" << G4endl;
    }
  }
 
  // Escapes box
  if (volume_ps == fPhotonCatcherVolume && trackStatus == fStopAndKill) {
    fEventAction->CountEscapedPhoton();
    G4cout << "Photon escaped" << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

