// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

//#include <Python.h>
#include <pybind11/embed.h>

#include "fields/field.h"
#include "utility/pythonfield.h"
#include "math/floatarray.h"
#include "solvers/timestep.h"

namespace py = pybind11;

namespace oofem {
// REGISTER_Material(PythonField);


/// Constructor.
PythonField :: PythonField(void) : Field(FieldType::FT_Unknown)
{
    //pybind11::print("Hello, World!"); // use the Python API
}

void PythonField :: setFunctionName(std::string functionName){
    this->functionName = functionName;
}


void PythonField :: setModuleName(std::string moduleName){
    this->moduleName = moduleName;
    //this->moduleName.resize(this->moduleName.size()); //remove trailing quotes
}


int PythonField :: evaluateAt(FloatArray &answer, const FloatArray &coords, ValueModeType mode, TimeStep *tStep)
{
    // Do not use the raw CPython API functions Py_Initialize and Py_Finalize as these do not properly handle the lifetime of pybind11’s internal data.
    // https://pybind11.readthedocs.io/en/stable/advanced/embedding.html
    
    // py::scoped_interpreter guard{}; // start the interpreter and keep it alive
    
//     py::initialize_interpreter();
    
    py::module calc = py::module::import(moduleName.c_str());
    py::object result = calc.attr(functionName.c_str())(coords, mode, tStep);
    answer = result.cast<FloatArray>();
//     py::finalize_interpreter();
    
    return 0;
}

int PythonField :: evaluateAt(FloatArray &answer, DofManager *dman, ValueModeType mode, TimeStep *tStep)
{
    OOFEM_ERROR("Not implemented");
    return 1;
}



} // end namespace oofem
