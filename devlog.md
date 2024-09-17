### Revision Log
* Remove PFEM
* AM can be compiled, yet I have removed it.
* MPM can be compiled, yet I have removed it.
* remove FM, TM, visco in SM

CDPM 2
For benchmark problems, using secant stiffness significantly increases the number of iterations.

### How to deal with below warnings?
```
profiling:/home/licunyi/LCY/oofem/build/src/sm/Materials/CMakeFiles/sm_materials.dir/lsmastermatgrad.C.gcda:Merge mismatch for function 192
profiling:/home/licunyi/LCY/oofem/build/src/oofemlib/CMakeFiles/core.dir/fei2dquadquad.C.gcda:Merge mismatch for function 144
profiling:/home/licunyi/LCY/oofem/build/src/oofemlib/CMakeFiles/core.dir/fei1dhermite.C.gcda:Merge mismatch for function 58
```

* Remove CMakeFiles and rebuild