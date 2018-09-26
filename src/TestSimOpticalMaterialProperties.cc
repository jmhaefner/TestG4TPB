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
  G4double REFLECTIVITY[REFL_NUMENTRIES] = {.0, .0}; //{.72, .72}; [edit]
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
    G4double reflectivity[entries] = {0., 0.}; // {0.32, 0.58}; // [edit]

    // refractive index
    G4double rindex[entries] = {0.94, 1.22};

    // extinction coefficient
    G4double extinction_coeff[entries] = {1.337, 2.56};

    copper_mpt->AddProperty("REFLECTIVITY", energy, reflectivity, entries);
    copper_mpt->AddProperty("RINDEX", energy, rindex, entries);
    copper_mpt->AddProperty("EXTINCTION", energy, extinction_coeff, entries);

    return copper_mpt;
}

G4MaterialPropertiesTable* OpticalMaterialProperties::TPB(G4double pressure, G4double temperature)
{
  /// This is the simulation of the optical properties of TPB (tetraphenyl butadiene)
  /// a wavelength shifter which allows to converts VUV photons to blue photons.
  /// A WLS material is characterized by its photon absorption and photon emission spectrum
  /// and by a possible time delay between the absorption and re-emission of the photon.

  G4MaterialPropertiesTable* tpb_mpt = new G4MaterialPropertiesTable();

  const G4int ri_entries = 9;
  //const G4int ABSL_NUMENTRIES = 8;
  //const G4int WLSABSL_NUMENTRIES = 7;
  const G4int EMISSION_NUMENTRIES = 55;

  // XenonGasProperties GXe_prop(pressure, temperature); // [edit] I've removed the references to GXe
  G4double ri_energy[ri_entries]
    = {1*eV, 2*eV, 3*eV, 4*eV, 5*eV, 6*eV, 7*eV, 8*eV, 9*eV};
  G4double rindex[ri_entries];
  for (G4int i=0; i<ri_entries; i++) {
    rindex[i] = 1; // GXe_prop.RefractiveIndex(ri_energy[i]); // [edit] I've removed the references to GXe
    //   G4cout << ri_energy[i] << ", " << rindex[i] << G4endl;
  }

/* Right border of the bins*/
  G4double WLS_Emission_Energies[EMISSION_NUMENTRIES] = {1.96800306364 *eV,  1.98230541148 *eV,  1.99681716413 *eV,  2.01154295443 *eV,  2.0264875529 *eV,  2.04165587291 *eV,  2.05705297602 *eV,  2.07268407766 *eV,  2.08855455299 *eV,  2.10466994306 *eV,  2.12103596128 *eV,  2.13765850016 *eV,  2.15454363839 *eV,  2.17169764825 *eV,  2.18912700337 *eV,  2.2068383869 *eV,  2.2248387 *eV,  2.24313507089 *eV,  2.26173486418 *eV,  2.28064569081 *eV,  2.29987541838 *eV,  2.31943218215 *eV,  2.3393243964 *eV,  2.35956076661 *eV,  2.3801503021 *eV,  2.4011023294 *eV,  2.4224265064 *eV,  2.4441328371 *eV,  2.46623168735 *eV,  2.48873380128 *eV,  2.51165031879 *eV,  2.53499279387 *eV,  2.55877321408 *eV,  2.58300402103 *eV,  2.60769813212 *eV,  2.63286896352 *eV,  2.65853045439 *eV,  2.68469709272 *eV,  2.71138394255 *eV,  2.7386066729 *eV,  2.76638158844 *eV,  2.7947256621 *eV,  2.82365656957 *eV,  2.85319272616 *eV,  2.8833533258 *eV,  2.91415838269 *eV,  2.9456287756 *eV,  2.97778629498 *eV,  3.01065369338 *eV,  3.04425473906 *eV,  3.07861427337 *eV,  3.11375827192 *eV,  3.14971391017 *eV,  3.18650963341 *eV,  3.22417523192 *eV};
;

 G4double TPB_Emission[EMISSION_NUMENTRIES] = {5e-05 , 5e-05 ,  5e-05 ,  5e-05 ,  5e-05 ,  5e-05 ,  5e-05 ,  5e-05 ,  5e-05 ,  5e-05 ,  5e-05 ,  5e-05 ,  5e-05 ,  0.0001 ,  0.0002 ,  0.0003 ,  0.0003 ,  0.0003 ,  0.0003 ,  0.0003 ,  0.0002 ,  0.0007 ,  0.0007 ,  0.001 ,  0.0011 ,  0.0013 ,  0.0016 ,  0.0018 ,  0.0022 ,  0.0029 ,  0.0035 ,  0.0034 ,  0.0041 ,  0.0046 ,  0.0053 ,  0.0061 ,  0.0069 ,  0.008 ,  0.0087 ,  0.0101 ,  0.0101 ,  0.0103 ,  0.0105 ,  0.0112 ,  0.0117 ,  0.0112 ,  0.011 ,  0.014 ,  0.008 ,  0.008 ,  0.007 ,  0.0038 ,  0.0012 ,  0.0008, 0.0004};


 // Values taken from Gehman's paper http://arxiv.org/pdf/1104.3259.pdf
 const G4int wls_numentries = 14;
 G4double WLS_Energies[wls_numentries] =
   {1.*eV, 2.8*eV, 4.*eV, 4.95937*eV, 5.16601*eV, 5.39062*eV,
    5.63565*eV, 5.90401*eV, 6.19921*eV, 6.52548*eV, 6.88801*eV, 7.29319*eV,
    7.74901*eV, 8.26561*eV};
 G4double TPB_ConvEfficiency[wls_numentries] =
   {0., 0., 0., .86, .90, .94,
    .90, .80, .75, .70, .75, .82,
    .85, .92};

 tpb_mpt->AddProperty("RINDEX", ri_energy, rindex, ri_entries);
 tpb_mpt->AddProperty("WLSCOMPONENT", WLS_Emission_Energies,
		      TPB_Emission, EMISSION_NUMENTRIES);
 tpb_mpt->AddProperty("WLSABSLENGTH", WLS_Energies,
		      TPB_ConvEfficiency, wls_numentries);
 tpb_mpt->AddConstProperty("WLSTIMECONSTANT",1.*ns);

  return tpb_mpt;
}
