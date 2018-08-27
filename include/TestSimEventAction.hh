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
// $Id: TestSimEventAction.hh 93886 2015-11-03 08:28:26Z gcosmo $
//
/// \file TestSimEventAction.hh
/// \brief Definition of the TestSimEventAction class

#ifndef TestSimEventAction_h
#define TestSimEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class TestSimRunAction;
class TestSimAnalysisManager;

/// Event action class
///

class TestSimEventAction : public G4UserEventAction
{
  public:
    TestSimEventAction(TestSimRunAction* runAction, TestSimAnalysisManager* analysisManager);
    virtual ~TestSimEventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    void SetPrimaryEnergy(G4double e) { fPrimaryEnergy = e; }
    void SetPrimaryTheta(G4double t) { fPrimaryTheta = t; }
    void SetPrimaryPhi(G4double p) { fPrimaryPhi = p; }
    void SetPrimaryAbsorbed() { fWLSabsorbed = 1; }
    void AddSecondaryPhoton() { fNemitted += 1.0; }
    void FirePMT() { fPMThits += 1.0; }
    void CountPlasticReflection() { fPlasticReflections += 1.0; }
    void PlasticAbsorbed() { fPlasticAbsorbed += 1.0; }
    void AirScatter() { fAirScatters += 1.0; }

  private:
    TestSimRunAction* fRunAction;
    TestSimAnalysisManager* fAnalysisManager;

    G4double    fPrimaryEnergy;
    G4double    fPrimaryTheta;
    G4double    fPrimaryPhi;
    G4int       fWLSabsorbed;
    G4double    fNemitted;
    G4double    fPMThits;
    G4double    fPlasticReflections;
    G4double    fPlasticAbsorbed;
    G4double    fAirScatters;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
