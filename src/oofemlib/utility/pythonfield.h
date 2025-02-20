// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef pythonfield_h
#define pythonfield_h

#include "fields/field.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "engng/classfactory.h"
#include "input/dynamicinputrecord.h"
#include "dofman/dofmanager.h"
#include "error/error.h"
#include <iostream>


///@name Input fields for PythonField
//@{
#define _IFT_PythonField_Name "pythonmaterial"
#define _IFT_PythonField_moduleName "module" /// The name of the module with the supplied functions (i.e. the name of the python script, without file extension)
//@}

namespace oofem {
/**
 * Custom user supplied python scripts for field.
 * The python module should contain the functions
 * @code{.py}
 * evaluateAt(FloatArray &answer, const FloatArray &coords, ValueModeType mode, TimeStep *tStep) # returns int 
 * @endcode
 * 
 * @author Vit Smilauer
 */
class OOFEM_EXPORT PythonField : public Field
{
private:
    /// Name of python module containing evaluating function function
    std::string moduleName;
    std::string functionName;

public:
//     Constructor.
    PythonField(void);

    void setFunctionName(std::string functionName);
    void setModuleName(std::string moduleName);
    
    int evaluateAt(FloatArray &answer, const FloatArray &coords,
                           ValueModeType mode, TimeStep *tStep) override;
                           
                           
    int evaluateAt(FloatArray &answer, DofManager *dman,
                           ValueModeType mode, TimeStep *tStep) override;
                           
    void saveContext(DataStream &stream) override { }
    void restoreContext(DataStream &stream) override { }
    
    const char *giveClassName() const override { return "PythonField"; }
};

} // end namespace oofem
#endif // pythonfield_h
