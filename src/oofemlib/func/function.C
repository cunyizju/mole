// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "func/function.h"
#include "solvers/timestep.h"
#include "error/error.h"
#include "input/domain.h"
#include "math/gausspoint.h"
#include "export/datastream.h"
#include "utility/contextioerr.h"

namespace oofem {
Function :: Function(int n, Domain *d) :
    FEMComponent(n, d)
{ this->parameterType=0; }

double
Function :: giveFunctionParameter(int paramID)
{
    if(this->giveDomain()->giveElement(this->giveNumber())!=NULL)
        return 1.;
    return 0.;
}

double
Function :: evaluate(TimeStep *tStep, ValueModeType mode)
{
    if ( mode == VM_Total ) {
        return this->evaluateAtTime( tStep->giveIntrinsicTime() );
    } else if ( mode == VM_Velocity ) {
        return this->evaluateVelocityAtTime( tStep->giveIntrinsicTime() );
    } else if ( mode == VM_Acceleration ) {
        return this->evaluateAccelerationAtTime( tStep->giveIntrinsicTime() );
    } else if ( mode == VM_Incremental ) {
        return this->evaluateAtTime( tStep->giveTargetTime() ) - this->evaluateAtTime( tStep->giveTargetTime() - tStep->giveTimeIncrement() );
    } else if (mode == VM_Intermediate) {
      return this->evaluateAtTime( tStep->giveIntrinsicTime() );
    } else {
        OOFEM_ERROR("unsupported mode(%d)", mode);
    }

    return 0.;
}


double
Function :: evaluateAtTime(double t)
{
    FloatArray v;
    this->evaluate(v, {{"t",t}});
    if ( v.giveSize() != 1 ) {
        OOFEM_ERROR("Function doesn't return scalar results.");
    }
    return v.at(1);
}


void
Function :: evaluate(FloatArray &answer, const std :: map< std :: string, FunctionArgument > &valDict, GaussPoint *gp, double param)
{
    if(this->parameterType==0)
    {
        auto it = valDict.find("t");
        if ( it == valDict.end() )
        {
            OOFEM_ERROR("Missing necessary argument \"t\"");
        }else{
            answer = FloatArray{this->evaluateAtTime(it->second.val0)};
        }
    }
    else if(this->parameterType==1)
    {
        answer = FloatArray{this->evaluateAtTime(param)};
    }
    else
        OOFEM_ERROR("Unknown parameter type %d",parameterType);
}

double
Function :: evaluate(const std :: map< std :: string, FunctionArgument > &valDict)
{
    FloatArray ans;
    this->evaluate(ans, valDict);
    if ( ans.giveSize() != 1 ) {
        OOFEM_ERROR("Function does not return scalar value");
    }
    return ans[0];
}

void
Function :: saveContext(DataStream &stream, ContextMode mode)
{
    FEMComponent :: saveContext(stream, mode);

    if ( mode & CM_Definition ) {
        if ( !stream.write(parameterType) ) {
          THROW_CIOERR(CIO_IOERR);
        }
    }
}


void
Function :: restoreContext(DataStream &stream, ContextMode mode)
{
    FEMComponent :: restoreContext(stream, mode);

    if ( mode & CM_Definition ) {
        if ( !stream.read(parameterType) ) {
          THROW_CIOERR(CIO_IOERR);
        }
    }
}

} // end namespace oofem
