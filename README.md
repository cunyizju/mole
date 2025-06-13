<h2 align="center">
  MOLE (<b>M</b>odified <b>O</b>OFEM for Ine<b>l</b>astic Mat<b>e</b>rials)
</h2>



### What is mole?
**mole** is a finite element package, inherented from [OOFEM](https://github.com/oofem/oofem), but with a primary focus on inelastic solid mechanics.

### Why Choose Mole?

* Concise and Organized: Mole is streamlined for individual developers, making it easier to use and extend.
* Accuracy: All examples have been thoroughly tested to ensure reliable results.
* High Performance: **Mole** is faster than [OOFEM](https://github.com/oofem/oofem). With [PETSc](https://petsc.org/release/overview/) and [Open MPI](https://github.com/open-mpi/ompi), its computational efficiency is comparable with commercial packages.

### Installation (Linux OS Only)
#### Pre-requisites
* [CMake](https://cmake.org/)
* [gcc](https://gcc.gnu.org/)
#### Step 1: Download source code
```
git clone https://github.com/cunyizju/mole --depth=1
git clone https://github.com/petsc/petsc.git --depth=1

```
#### Step 2: Install PETSc
```
./configure PETSC_ARCH=linux-gnu --with-cc=gcc --with-cxx=g++ --with-fc=gfortran --download-mpich --download-fblaslapack

make PETSC_DIR=/home/bruce/Repo2024/petsc PETSC_ARCH=linux-gnu all

make PETSC_DIR=/your/install/path/petsc PETSC_ARCH=linux-gnu check
```

#### Step 3: Config mole
```
cd mole
mkdir build/opt
cd build/opt
ccmake ../..
```
Please use following configurations:
```
CMAKE_BUILD_TYPE   Release
PETSC_ARCH         linux-gnu
PETSC_DIR          /your/path/to/petsc/source/code
PETSC_CURRENT      ON
USE_DSS            ON
USE_IML            ON
USE_MPI_PARALLEL   ON
USE_PETSC          ON
USE_SHARED_LIB     ON
USE_SM             ON
```

#### Step 4: Build mole
```
cmake --build ./ -jn (n the number of cpu cores)
```
#### Run
```
oofem -f input_file_name
```
or
```
oofem -f input_file_name > output.log
```
if you want to record the log.

#### Debug with VS CODE in Linux

When configure the project, just set ```CMAKE_BUILD_TYPE``` as ```Debug```, as shown below,
```
 BUILD_TESTING                    ON
 CCACHE_PROGRAM                   CCACHE_PROGRAM-NOTFOUND
 CMAKE_BUILD_TYPE                 Debug
 CMAKE_INSTALL_PREFIX             /usr/local
 DL_LIB                           /usr/lib64/libdl.so
```

Then please add a file launch.json in folder .vscode to specify the package and input file for debugging,
```
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "(gdb) Launch",
            "type": "cppdbg",
            "request": "launch",
            "program": "/home/licunyi/LCY/oofem/build/debug/oofem",
            "stopAtEntry": false,
            "cwd": "/home/licunyi/LCY/oofem/Debug",
            "args": [
                "-f",
                "MW.in"
            ],
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                },
                {
                    "description": "Set Disassembly Flavor to Intel",
                    "text": "-gdb-set disassembly-flavor intel",
                    "ignoreFailures": true
                }
            ]
        }
    ]
}
```
