#include <globals.hh>
#include <CLHEP/Units/SystemofUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>

class G4MaterialPropertiesTable;

class OpticalMaterialProperties
{
    public:
        static G4MaterialPropertiesTable* Air();
        static G4MaterialPropertiesTable* Teflon();

    private:
        // Constructor (hidden)
        OpticalMaterialProperties();
        // Destructor (hidden)
        ~OpticalMaterialProperties();
};
