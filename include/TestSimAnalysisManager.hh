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

    void FillNtuple(G4double PrimaryEnergy, G4double PrimaryTheta,
                    G4double PrimaryPhi, G4int WLSabsorbed, G4int NumEmitted, 
                    G4double PMThits, G4double PlasticReflections,
                    G4double PlasticAbsorptions, G4double AirScatters, G4double EscapedPhotons);

  private:
    G4bool fFactoryOn;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
