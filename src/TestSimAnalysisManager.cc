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
/// \file analysis/AnaEx01/src/TestSimAnalysisManager.cc
/// \brief Implementation of the TestSimAnalysisManager class
//
//
// $Id: TestSimAnalysisManager.cc 105494 2018-08-23 09:02:56Z $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

#include "TestSimAnalysisManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TestSimAnalysisManager::TestSimAnalysisManager()
 : fFactoryOn(false)
{
  fPMTinsertion = 0.;
  fCopperRotation = 0.;
  fPrimaryEnergy = 0.;
  fPrimaryTheta = 0.;
  fPrimaryPhi = 0.;
  fWLSabsorbed = 0;
  fNemitted = 0.;
  fPMThits = 0.;
  fPMTduds = 0.;
  fPlasticReflections = 0.;
  fPlasticAbsorptions = 0.;
  fCopperReflections = 0.;
  fCopperAbsorptions = 0.;
  fEscapedPhotons = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TestSimAnalysisManager::~TestSimAnalysisManager()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TestSimAnalysisManager::Book()
{
  // Create or get analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in TestSimAnalysisManager.hh
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
      
  // Create directories 
  analysisManager->SetNtupleDirectoryName("ntuples");
    
  // Open an output file
  //
  G4bool fileOpen = analysisManager->OpenFile("NEXTPlasticTestSim");
  if (! fileOpen) {
    G4cerr << "\n---> TestSimAnalysisManager::Book(): cannot open "
           << analysisManager->GetFileName() << G4endl;
    return;
  }
  
  // Create ntuples.
  // Ntuples ids are generated automatically starting from 0.

  // Create 1st ntuple (id = 0)
  analysisManager->CreateNtuple("ntuple", "BoxEvents");
  analysisManager->CreateNtupleDColumn("PMTinsertion"); // column id = 0
  analysisManager->CreateNtupleDColumn("CopperRotation"); // column id = 1
  analysisManager->CreateNtupleDColumn("PrimaryEnergy"); // column Id = 2
  analysisManager->CreateNtupleDColumn("PrimaryTheta"); // column Id = 3
  analysisManager->CreateNtupleDColumn("PrimaryPhi"); // id = 4
  analysisManager->CreateNtupleIColumn("WLSabsorbed"); // id = 5
  analysisManager->CreateNtupleIColumn("Nemitted");  // id = 6
  analysisManager->CreateNtupleDColumn("PMThits"); // id = 7
  analysisManager->CreateNtupleDColumn("PMTduds"); // id = 8
  analysisManager->CreateNtupleDColumn("PlasticReflections"); // id = 9
  analysisManager->CreateNtupleDColumn("PlasticAbsorptions"); // id = 10
  analysisManager->CreateNtupleDColumn("CopperReflections"); // id = 11
  analysisManager->CreateNtupleDColumn("CopperAbsorptions"); // id = 12
  analysisManager->CreateNtupleDColumn("EscapedPhotons"); // id = 13
  analysisManager->FinishNtuple();

  fFactoryOn = true;

  G4cout << "\n----> Output file is open in " 
         << analysisManager->GetFileName() << "." 
         << analysisManager->GetFileType() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TestSimAnalysisManager::Save()
{
  if (! fFactoryOn) return;

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
   
  G4cout << "\n----> Ntuples are saved\n" << G4endl;
      
  delete G4AnalysisManager::Instance();
  fFactoryOn = false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TestSimAnalysisManager::FillNtuple()
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  // Fill 1st ntuple ( id = 0)
  analysisManager->FillNtupleDColumn(0, 0, fPMTinsertion); // Separation of PMT face with box opening
  analysisManager->FillNtupleDColumn(0, 1, fCopperRotation); // Rotation of copper piece around PMT axis
  analysisManager->FillNtupleDColumn(0, 2, fPrimaryEnergy); // Energy of initial VUV photon (eV)
  analysisManager->FillNtupleDColumn(0, 3, fPrimaryTheta); // Theta of initial photon
  analysisManager->FillNtupleDColumn(0, 4, fPrimaryPhi); // Phi of initial photon
  analysisManager->FillNtupleIColumn(0, 5, fWLSabsorbed); // 0 or 1 flag for initial photon's
                                                         // being absorbed by the WLS
  analysisManager->FillNtupleIColumn(0, 6, fNemitted); // Number of photons emitted by the WLS
  analysisManager->FillNtupleDColumn(0, 7, fPMThits); // Number of PMT hits (counted photons)
  analysisManager->FillNtupleDColumn(0, 8, fPMTduds); // Number of photons incident to PMT that don't fire
  analysisManager->FillNtupleDColumn(0, 9, fPlasticReflections); // Number of reflections off of the
                                                                // plastic box surface
  analysisManager->FillNtupleDColumn(0, 10, fPlasticAbsorptions); // Number of photons absorbed by the plastic
  analysisManager->FillNtupleDColumn(0, 11, fCopperReflections); // Number of photons reflected by copper
  analysisManager->FillNtupleDColumn(0, 12, fCopperAbsorptions); // Number of photons absorbed by copper
  analysisManager->FillNtupleDColumn(0, 13, fEscapedPhotons); // Number of photons that escape past the PMT
  analysisManager->AddNtupleRow(0);
}

void TestSimAnalysisManager::Reset()
{
  //fPrimaryEnergy = 0.;
  //fPrimaryTheta = 0.;
  //fPrimaryPhi = 0.;
  fWLSabsorbed = 0;
  fNemitted = 0.;
  fPMThits = 0.;
  fPMTduds = 0.;
  fPlasticReflections = 0.;
  fPlasticAbsorptions = 0.;
  fCopperReflections = 0.;
  fCopperAbsorptions = 0.;
  fEscapedPhotons = 0;
}
