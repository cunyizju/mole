// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/Elements/Beams/beambaseelement.h"
#include "dofman/node.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/intarray.h"
#include "math/floatarray.h"
#include "engng/engngm.h"
#include "bc/boundaryload.h"
#include "math/mathfem.h"
#include "bc/bctracker.h"

#include "bc/bodyload.h"
#include "bc/boundaryload.h"

namespace oofem {


BeamBaseElement :: BeamBaseElement (int n, Domain *aDomain) : StructuralElement(n, aDomain)
{}

BeamBaseElement :: ~BeamBaseElement()
{}

void
BeamBaseElement :: computeLocalForceLoadVector(FloatArray &answer, TimeStep *tStep, ValueModeType mode)
// computes the part of load vector, which is imposed by force loads acting
// on element volume (surface).
// Why is this function taken separately ?
// When reactions forces are computed, they are computed from element::GiveRealStressVector
// in this vector a real forces are stored (temperature part is subtracted).
// so we need further subtract part corresponding to non-nodal loading.
{
    FloatArray helpLoadVector(1);
    answer.clear();

    // loop over body load array first
    int nBodyLoads = this->giveBodyLoadArray()->giveSize();
    for ( int i = 1; i <= nBodyLoads; i++ ) {
        int id = bodyLoadArray.at(i);
        Load *load = domain->giveLoad(id);
        bcGeomType ltype = load->giveBCGeoType();
        if ( ( ltype == BodyLoadBGT ) && ( load->giveBCValType() == ForceLoadBVT ) ) {
            this->computeBodyLoadVectorAt(helpLoadVector, load, tStep, mode);
            if ( helpLoadVector.giveSize() ) {
                answer.add(helpLoadVector);
            }
        } else {
            if ( load->giveBCValType() != TemperatureBVT && load->giveBCValType() != EigenstrainBVT ) {
                // temperature and eigenstrain is handled separately at computeLoadVectorAt subroutine
                OOFEM_ERROR("body load %d is of unsupported type (%d)", id, ltype);
            }
        }
    }

    // loop over boundary load array
    int nBoundaryLoads = this->giveBoundaryLoadArray()->giveSize() / 2;
    for ( int i = 1; i <= nBoundaryLoads; i++ ) {
        int n = boundaryLoadArray.at(1 + ( i - 1 ) * 2);
        int id = boundaryLoadArray.at(i * 2);
        Load *load = domain->giveLoad(n);
	BoundaryLoad* bLoad;
	if ((bLoad = dynamic_cast<BoundaryLoad*> (load))) {
	  bcGeomType ltype = load->giveBCGeoType();
	  if ( ltype == EdgeLoadBGT ) {
	    this->computeBoundaryEdgeLoadVector(helpLoadVector, bLoad, id, ExternalForcesVector, mode, tStep, false);
            if ( helpLoadVector.giveSize() ) {
	      answer.add(helpLoadVector);
            }
	  } else if ( ltype == SurfaceLoadBGT ) {
	    this->computeBoundarySurfaceLoadVector(helpLoadVector, bLoad, id, ExternalForcesVector, mode, tStep, false);
            if ( helpLoadVector.giveSize() ) {
	      answer.add(helpLoadVector);
            }
	  } else if ( ltype == PointLoadBGT ) {
            // id not used
	    this->computePointLoadVectorAt(helpLoadVector, load, tStep, mode, false);
            if ( helpLoadVector.giveSize() ) {
	      answer.add(helpLoadVector);
            }
	  } else {
            OOFEM_ERROR("boundary load %d is of unsupported type (%d)", id, ltype);
	  }
	}
    }


    // add exact end forces due to nonnodal loading applied indirectly (via sets)
    BCTracker *bct = this->domain->giveBCTracker();
    BCTracker::entryListType bcList = bct->getElementRecords(this->number);
    FloatArray help;
    
    for (BCTracker::entryListType::iterator it = bcList.begin(); it != bcList.end(); ++it) {
      GeneralBoundaryCondition *bc = this->domain->giveBc((*it).bcNumber);
      BodyLoad *bodyLoad;
      BoundaryLoad *boundaryLoad;
      if (bc->isImposed(tStep)) {
        if ((bodyLoad = dynamic_cast<BodyLoad*>(bc))) { // body load
          this->computeBodyLoadVectorAt(help,bodyLoad, tStep, VM_Total); // this one is local
          answer.add(help);
        } else if ((boundaryLoad = dynamic_cast<BoundaryLoad*>(bc))) {
          // compute Boundary Edge load vector in GLOBAL CS !!!!!!!
          this->computeBoundaryEdgeLoadVector(help, boundaryLoad, (*it).boundaryId,
					      ExternalForcesVector, VM_Total, tStep, false);
          // get it transformed back to local c.s.
          // this->computeGtoLRotationMatrix(t);
          // help.rotatedWith(t, 'n');
          answer.add(help);
        }
      }
    }
}

} // end namespace oofem
