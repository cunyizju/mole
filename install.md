## Install and config petsc

```
./configure PETSC_ARCH=linux-gnu --with-cc=gcc --with-cxx=g++ --with-fc=gfortran --download-mpich --download-fblaslapack

make PETSC_DIR=/home/bruce/Videos/Repo2024/petsc PETSC_ARCH=linux-gnu all

make PETSC_DIR=/home/bruce/Videos/Repo2024/petsc PETSC_ARCH=linux-gnu check
```
Note: please update /home/bruce/Videos/Repo2024/petsc based on your case