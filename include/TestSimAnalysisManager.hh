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
/// \file NEXTPlasticTestSim/include/TestSimAnalysisManager.hh
/// \brief Definition of the TestSimAnalysisManager class
//
//
// $Id: TestSimAnalysisManager.hh 99607 2018-08-23 13:33:42Z $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

#ifndef TestSimAnalysisManager_h
#define TestSimAnalysisManager_h 1

#include "globals.hh"

#include "g4root.hh"
//#include "g4csv.hh"
//#include "g4xml.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TestSimAnalysisManager
{
  public:
    TestSimAnalysisManager();
   ~TestSimAnalysisManager();

    void Book();
    void Save();

    void FillNtuple();
    void Reset();
    
    void SetPMTinsertion(G4double d) { fPMTinsertion = d; }
    void SetCopperRotation(G4double r) { fCopperRotation = r; }
    void SetPrimaryEnergy(G4double e) { fPrimaryEnergy = e; }
    void SetPrimaryTheta(G4double t) { fPrimaryTheta = t; }
    void SetPrimaryPhi(G4double p) { fPrimaryPhi = p; }
    void SetPrimaryAbsorbed() { fWLSabsorbed = 1; }
    void AddSecondaryPhoton() { fNemitted += 1.0; }
    void FirePMT() { fPMThits += 1.0; }
    void MisfirePMT() { fPMTduds += 1.0; }
    void CountPlasticReflection() { fPlasticReflections += 1.0; }
    void CountPlasticAbsorption() { fPlasticAbsorptions += 1.0; }
    void CountCopperReflection() { fCopperReflections += 1.0; }
    void CountCopperAbsorption() { fCopperAbsorptions += 1.0; }
    void CountEscapedPhoton() { fEscapedPhotons += 1.0; }

  private:
    G4bool fFactoryOn;

    G4double    fPMTinsertion;
    G4double    fCopperRotation;
    G4double    fPrimaryEnergy;
    G4double    fPrimaryTheta;
    G4double    fPrimaryPhi;
    G4int       fWLSabsorbed;
    G4double    fNemitted;
    G4double    fPMThits;
    G4double    fPMTduds;
    G4double    fPlasticReflections;
    G4double    fPlasticAbsorptions;
    G4double    fCopperReflections;
    G4double    fCopperAbsorptions;
    G4double    fEscapedPhotons;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
