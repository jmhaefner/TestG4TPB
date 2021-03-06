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
// $Id: TestSimPrimaryGeneratorAction.cc 94307 2015-11-11 13:42:46Z gcosmo $
//
/// \file TestSimPrimaryGeneratorAction.cc
/// \brief Implementation of the TestSimPrimaryGeneratorAction class

#include <cmath>
#include <typeinfo>
#include "TestSimPrimaryGeneratorAction.hh"
#include "TestSimAnalysisManager.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TestSimPrimaryGeneratorAction::TestSimPrimaryGeneratorAction(TestSimAnalysisManager* analysisManager)
: G4VUserPrimaryGeneratorAction(),
  fAnalysisManager(analysisManager),
  fParticleGun(0), 
  fCopperThickness(0)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);
  fCopperTranslation = G4ThreeVector();

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="opticalphoton");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleEnergy(6.*MeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TestSimPrimaryGeneratorAction::~TestSimPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TestSimPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of ecah event
  //

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get Envelope volume
  // from G4LogicalVolumeStore.

  G4VPhysicalVolume* CuPV = G4PhysicalVolumeStore::GetInstance()->GetVolume("Cu");
  if ( CuPV ) {
      fCopperTranslation = CuPV->GetObjectTranslation();
  }

  if (!(fCopperTranslation.getX() == 0. && fCopperTranslation.getY() == 0. && fCopperTranslation.getZ() == 0))
  {
      G4Box* CopperBox = dynamic_cast<G4Box*>(G4SolidStore::GetInstance()->GetSolid("fullCu"));
      fCopperThickness = CopperBox->GetZHalfLength()*2.;
  }

  else  {
    G4ExceptionDescription msg;
    msg << "Couldn't find the copper piece named Cu.\n"; 
    msg << "Perhaps you have changed geometry.\n";
    msg << "The gun will be place at the center.";
    G4Exception("TestSimPrimaryGeneratorAction::GeneratePrimaries()",
     "MyCode0002",JustWarning,msg);
  }

  //G4double size = 0.8; 
  //G4double x0 = size * envSizeXY * (G4UniformRand()-0.5);
  //G4double y0 = size * envSizeXY * (G4UniformRand()-0.5);
  // Create a point source with a 120 degree spread, placed in the center
  // of the (X,Y) of the box
  //G4double x0 = 0.06*envSizeXY;
  //G4double y0 = 0.06*envSizeXY;
  G4double cu_x0 = fCopperTranslation.getX();
  G4double cu_y0 = fCopperTranslation.getY();
  G4double cu_radius = std::sqrt(std::pow(cu_x0,2) + std::pow(cu_y0,2));

  G4double hole_radius = cu_radius + 15; // assume the hole for LED is 10 mm into the copper piece

  //G4double x0 = cu_x0;
  //G4double y0 = cu_y0;
  G4double x0 = hole_radius*(cu_x0/cu_radius); // cu_x0/cu_radius is cos(angle b/w x-axis and radius vector of cu)
  G4double y0 = hole_radius*(cu_y0/cu_radius); // same idea as above, but with sin(angle)

  G4double copperZ = fCopperTranslation.getZ();
  G4double z0 = copperZ + (fCopperThickness/2.);

  
  G4double sigma = 0.445;
  // "azimuthal" angle w.r.t. z-axis:
  G4double theta = std::abs(G4RandGauss::shoot(0.,sigma));
  // polar angle in the x-y plane:
  G4double phi = G4UniformRand()*2*CLHEP::pi;
  // now calculate the momentum components:
  G4double px = sin(theta)*cos(phi);
  G4double py = sin(theta)*sin(phi);
  G4double pz = cos(theta);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(px,py,pz));
  fParticleGun->SetParticlePosition(G4ThreeVector(x0*mm,y0*mm,z0*mm));
  
  // model the spectrum of the LED
  G4double central_wl = 290.*nm;
  G4double wl_spread = 5.1*nm; // FWHM 12 nm
  G4double wl = std::abs(G4RandGauss::shoot(central_wl, wl_spread));
  //G4double energy = (G4PhysicalConstants::h_Planck*G4PhysicalConstants::c_light)/wl;
  G4double energy = (h_Planck*c_light)/wl;
  fParticleGun->SetParticleEnergy(energy);// MeV?
  //fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,0.));
  //G4RandGauss* gaussian(0.0,1.0);
  // The gaussian from which we sample theta below has std dev of 0.445 in order
  // to place the full width half max (= 2.355*sigma) at +/- 60 degrees (pi/3 rad)

  // Fill branches of analysis tree
  fAnalysisManager->SetPrimaryEnergy(energy/eV);
  fAnalysisManager->SetPrimaryTheta(theta);
  fAnalysisManager->SetPrimaryPhi(phi);

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

