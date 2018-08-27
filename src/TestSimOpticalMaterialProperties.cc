#include "TestSimOpticalMaterialProperties.hh"
#include <G4MaterialPropertiesTable.hh>

using namespace CLHEP;

G4MaterialPropertiesTable* OpticalMaterialProperties::Air()
{
    G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();

    const G4int entries = 2;
    G4double energy[entries] = {0.01*eV, 100.*eV};

    // REFRACTIVE INDEX
    G4double rindex[entries] = {1., 1.};
    mpt->AddProperty("RINDEX", energy, rindex, entries);

    // ABSORPTION LENGTH
    G4double abslen[entries] = {1.e8*m, 1.e8*m};
    mpt->AddProperty("ABSLENGTH", energy, abslen, entries);

    return mpt;
}

G4MaterialPropertiesTable* OpticalMaterialProperties::Teflon()
{
  G4MaterialPropertiesTable* teflon_mpt = new G4MaterialPropertiesTable();

  const G4int REFL_NUMENTRIES = 2;

  G4double ENERGIES[REFL_NUMENTRIES] = {1.0*eV, 30.*eV};
  /// This is for non-coated teflon panels
  G4double REFLECTIVITY[REFL_NUMENTRIES] = {.72, .72};
  G4double specularlobe[REFL_NUMENTRIES] = {0., 0.}; // specular reflection about the normal to a
  //microfacet. Such a vector is chosen according to a gaussian distribution with
  //sigma = SigmaAlhpa (in rad) and centered in the average normal.
  G4double specularspike[REFL_NUMENTRIES] = {0., 0.}; // specular reflection about the average normal
  G4double backscatter[REFL_NUMENTRIES] = {0., 0.}; //180 degrees reflection
  // 1 - the sum of these three last parameters is the percentage of Lambertian reflection

  teflon_mpt->AddProperty("REFLECTIVITY", ENERGIES, REFLECTIVITY, REFL_NUMENTRIES);
  teflon_mpt->AddProperty("SPECULARLOBECONSTANT",ENERGIES,specularlobe,REFL_NUMENTRIES);
  teflon_mpt->AddProperty("SPECULARSPIKECONSTANT",ENERGIES,specularspike,REFL_NUMENTRIES);
  teflon_mpt->AddProperty("BACKSCATTERCONSTANT",ENERGIES,backscatter,REFL_NUMENTRIES);

  return teflon_mpt;
}

G4MaterialPropertiesTable* OpticalMaterialProperties::PMT()
{
    G4MaterialPropertiesTable* pmt_mpt = new G4MaterialPropertiesTable();

    const G4int entries = 2;
    G4double energy[entries] = {0.01*eV, 100.*eV};

    // REFRACTIVE INDEX
    G4double rindex[entries] = {1., 1.};
    pmt_mpt->AddProperty("RINDEX", energy, rindex, entries);

    // ABSORPTION LENGTH
    G4double abslen[entries] = {0.01*mm, 0.01*mm};
    pmt_mpt->AddProperty("ABSLENGTH", energy, abslen, entries);

    return pmt_mpt;
}

// source: refractiveindex.info
G4MaterialPropertiesTable* OpticalMaterialProperties::Copper()
{
    G4MaterialPropertiesTable* copper_mpt = new G4MaterialPropertiesTable();

    const G4int entries = 2;
    G4double energy[entries] = {2.5*eV, 124.*eV};
    // so 10nm to 495nm
    // but these all go from 188nm to 495nm:
    
    // reflectivity
    G4double reflectivity[entries] = {0.32, 0.58};

    // refractive index
    G4double rindex[entries] = {0.94, 1.22};

    // extinction coefficient
    G4double extinction_coeff[entries] = {1.337, 2.56};

    copper_mpt->AddProperty("REFLECTIVITY", energy, reflectivity, entries);
    copper_mpt->AddProperty("RINDEX", energy, rindex, entries);
    copper_mpt->AddProperty("EXTINCTION", energy, extinction_coeff, entries);

    return copper_mpt;
}

