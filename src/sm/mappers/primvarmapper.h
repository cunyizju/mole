// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef PRIMVARMAPPER_H_
#define PRIMVARMAPPER_H_

#include "input/valuemodetype.h"

namespace oofem {
class FloatArray;
class Domain;
class TimeStep;

/**
 * Base class for mapping of primary variables between domains.
 * The reason for not using the existing PrimaryUnknownMapper is that I need to handle
 * cases where the number of dofs per node may change (for example XFEM enrichments).
 *
 * Created on: Nov 7, 2013
 * @author Erik Svenning
 */
class PrimaryVariableMapper
{
public:
    PrimaryVariableMapper();
    virtual ~PrimaryVariableMapper();

    virtual void mapPrimaryVariables(FloatArray &oU, Domain &iOldDom, Domain &iNewDom, ValueModeType iMode, TimeStep &iTStep) = 0;
};

/**
 * LSPrimaryVariableMapper: Least-squares primary variable mapper.
 *
 * @author Erik Svenning
 */
class LSPrimaryVariableMapper
{
public:
    LSPrimaryVariableMapper();
    virtual ~LSPrimaryVariableMapper();

    virtual void mapPrimaryVariables(FloatArray &oU, Domain &iOldDom, Domain &iNewDom, ValueModeType iMode, TimeStep &iTStep);
};
} /* namespace oofem */
#endif /* PRIMVARMAPPER_H_ */
