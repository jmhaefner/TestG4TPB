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
// $Id: TestSimEventAction.cc 93886 2015-11-03 08:28:26Z gcosmo $
//
/// \file TestSimEventAction.cc
/// \brief Implementation of the TestSimEventAction class

#include "TestSimEventAction.hh"
#include "TestSimRunAction.hh"
#include "TestSimAnalysisManager.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TestSimEventAction::TestSimEventAction(TestSimRunAction* runAction, 
                                       TestSimAnalysisManager* analysisManager)
: G4UserEventAction(),
  fRunAction(runAction),
  fAnalysisManager(analysisManager)
{
  fPrimaryEnergy = 0.;
  fPrimaryTheta = 0.;
  fPrimaryPhi = 0.;
  fWLSabsorbed = 0;
  fNemitted = 0.;
  fPMThits = 0.;
  fPlasticReflections = 0.;
  fPlasticAbsorptions = 0.;
  fAirScatters = 0.;
  fEscapedPhotons = 0;
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TestSimEventAction::~TestSimEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TestSimEventAction::BeginOfEventAction(const G4Event*)
{    
  fPrimaryEnergy = 0.;
  fPrimaryTheta = 0.;
  fPrimaryPhi = 0.;
  fWLSabsorbed = 0;
  fNemitted = 0.;
  fPMThits = 0.;
  fPlasticReflections = 0.;
  fPlasticAbsorptions = 0.;
  fAirScatters = 0.;
  fEscapedPhotons = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TestSimEventAction::EndOfEventAction(const G4Event*)
{  
  // Fill analysis tree 
  fAnalysisManager->FillNtuple(fPrimaryEnergy, fPrimaryTheta, fPrimaryPhi, 
                    fWLSabsorbed, fNemitted, fPMThits, fPlasticReflections, 
                    fPlasticAbsorptions, fAirScatters, fEscapedPhotons);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
