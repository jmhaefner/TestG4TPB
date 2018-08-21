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
// $Id: TestSimRunAction.cc 99560 2016-09-27 07:03:29Z gcosmo $
//
/// \file TestSimRunAction.cc
/// \brief Implementation of the TestSimRunAction class

#include "TestSimRunAction.hh"
#include "TestSimPrimaryGeneratorAction.hh"
#include "TestSimDetectorConstruction.hh"
// #include "TestSimRun.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TestSimRunAction::TestSimRunAction()
: G4UserRunAction(),
  fNphoton(0.),
  fNphoton2(0.),
  fNreflection(0.),
  fNreflection2(0.)
{ 
  // Register accumulable to the accumulable manager
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fNphoton);
  accumulableManager->RegisterAccumulable(fNphoton2); 
  accumulableManager->RegisterAccumulable(fNreflection);
  accumulableManager->RegisterAccumulable(fNreflection2);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TestSimRunAction::~TestSimRunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TestSimRunAction::BeginOfRunAction(const G4Run*)
{ 
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // reset accumulables to their initial values
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TestSimRunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // Merge accumulables 
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  // Compute number of absorbed photons and number of reflections (average and RMS)
  //
  G4double Nphoton  = fNphoton.GetValue()/nofEvents;
  G4double Nphoton2 = fNphoton2.GetValue()/nofEvents;

  G4double Nreflection = fNreflection.GetValue()/nofEvents;
  G4double Nreflection2 = fNreflection2.GetValue()/nofEvents;
  
  G4double p_rms = Nphoton2 - Nphoton*Nphoton;
  if (p_rms > 0.) p_rms = std::sqrt(p_rms); else p_rms = 0.;  

  G4double r_rms = Nreflection2 - Nreflection*Nreflection;
  if (r_rms > 0.) r_rms = std::sqrt(r_rms); else r_rms = 0.;
  

  /*const TestSimDetectorConstruction* detectorConstruction
   = static_cast<const TestSimDetectorConstruction*>
     (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  */

  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const TestSimPrimaryGeneratorAction* generatorAction
   = static_cast<const TestSimPrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  if (generatorAction)
  {
    const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
    runCondition += particleGun->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    G4double particleEnergy = particleGun->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy,"Energy");
  }
        
  // Print
  //  
  if (IsMaster()) {
    G4cout
     << G4endl
     << "--------------------End of Global Run-----------------------";
  }
  else {
    G4cout
     << G4endl
     << "--------------------End of Local Run------------------------";
  }
  
  G4cout
     << G4endl
     << " The run consists of " << nofEvents << " "<< runCondition
     << G4endl
     << " Cumulated PMT fires per event : " 
     << Nphoton<< " +/- " << p_rms
     << G4endl
     << " Cumulated photon incidences to box per event : "
     << Nreflection<< " +/- " << r_rms
     << G4endl
     << "------------------------------------------------------------"
     << G4endl
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TestSimRunAction::AddNphoton(G4double Nphoton)
{
  fNphoton  += Nphoton;
  fNphoton2 += Nphoton*Nphoton;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TestSimRunAction::AddNreflection(G4double Nreflection)
{
  fNreflection  += Nreflection;
  fNreflection2 += Nreflection*Nreflection;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

