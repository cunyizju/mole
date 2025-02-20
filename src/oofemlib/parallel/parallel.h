// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef parallel_h
#define parallel_h

// turn on parallel verbose mode if uncommented
//#define __VERBOSE_PARALLEL

// turn on using MPI if uncommented
#define __USE_MPI

//
// ===========>  Do not modify bellow this line <==============
//

// macro for verbose print in parallel mode
//#define VERBOSEPARALLEL_PRINT(service, str,rank) fprintf(stderr, "\n[process rank %3d] %s - %s",rank, service, str);
#define VERBOSEPARALLEL_PRINT(service, str, rank) fprintf(stderr, "\n[%d] %s - %s", rank, service, str);


#ifdef __USE_MPI
// if MPI used, include headers
 #include <mpi.h>
 #define PROCESSOR_NAME_LENGTH MPI_MAX_PROCESSOR_NAME
#else
 #define PROCESSOR_NAME_LENGTH 1024
#endif

#endif // parallel_h
