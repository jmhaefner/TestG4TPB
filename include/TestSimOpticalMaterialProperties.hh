#include <globals.hh>
#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>
//#include <SystemofUnits.h>
//#include <PhysicalConstants.h>

class G4MaterialPropertiesTable;

class OpticalMaterialProperties
{
    public:
        static G4MaterialPropertiesTable* Air();
        static G4MaterialPropertiesTable* Teflon();
        static G4MaterialPropertiesTable* PMT();
        static G4MaterialPropertiesTable* Copper();

    private:
        // Constructor (hidden)
        OpticalMaterialProperties();
        // Destructor (hidden)
        ~OpticalMaterialProperties();
};
