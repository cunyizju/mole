## Modification Log

Compared with OOFEM, mole is different in the following aspects:

### Removed modules
* fluid mechanics
* additive manufacturing
* multiphysical problems
* transportation
* OOFEG
* T3D
### Modifications
* CDPM2: For benchmark problems, using secant stiffness significantly increases the number of  iterations.
* When the number of iterations goes beyond the input value, the job will be killed automatically.
* `profileopt=1` is default option to optimize sparse matrix using Sloan algorithm.
* Frameworks of codes.

### Added features
* Ottosen and Menetrey-
Willam equivalent strain (to be done).
* phase field fracture models (to be done).
* export history variables given specified nodes or sets (to be done).

