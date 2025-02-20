// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "math/floatarray.h"
#include "math/mathfem.h"
#include "index.h"
#pragma once

using namespace FTensor;



/**
 * Functions providing basic operations with tensors 
 * based on FTensor library
 * It provides transformation from the second and fourth order tensors in 3d to FloatArrayF<9> and FloatArray<9,9> 
 * It provides calculation of determinant, cofactor, inverse, and tensor cross product
 * More to be added gradually
 * @author Martin Horak
 **/

namespace oofem {


  class Tensor1_3d : public Tensor1<double, 3>
  {
  public:
    using Tensor1<double, 3> :: Tensor1;
    Tensor1_3d(const oofem::FloatArrayF<3> &array){
      this->data[0] = array.at(1);
      this->data[1] = array.at(2);
      this->data[2] = array.at(3);
    }

    const inline FloatArrayF<3> to_voigt_form() 
    {
      return {
	this->operator()(0),
	  this->operator()(1),
	  this->operator()(2),
	  };
    }

  };
  
    
} // end namespace oofem


