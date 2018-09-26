#include <globals.hh>
#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>
//#include <SystemofUnits.h>
//#include <PhysicalConstants.h>

#include <globals.hh>

#include <CLHEP/Units/SystemOfUnits.h> // [edit] needed to get bar units
#include <CLHEP/Units/PhysicalConstants.h> // [edit] needed to get STP_Temperature

using namespace CLHEP;

class G4MaterialPropertiesTable;

class OpticalMaterialProperties
{
    public:
        static G4MaterialPropertiesTable* Air();
        static G4MaterialPropertiesTable* Teflon();
        static G4MaterialPropertiesTable* PMT();
        static G4MaterialPropertiesTable* Copper();
        static G4MaterialPropertiesTable* TPB(G4double pressure=10.*bar,
					  G4double temperature=STP_Temperature);

    private:
        // Constructor (hidden)
        OpticalMaterialProperties();
        // Destructor (hidden)
        ~OpticalMaterialProperties();
};
