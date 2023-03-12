#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks=4
#SBATCH --exclusive
#SBATCH --partition=compute2011
MPIDIR=/cluster/mpi/openmpi/1.6.5-gcc4.8.2/
GCCDIR=/cluster/gcc/4.8.2/
export PATH=$PATH:${MPIDIR}/bin:${GCCDIR}/bin
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${MPIDIR}/lib:\
${GCCDIR}/lib:${GCCDIR}/lib64
export VIADEV_USE_AFFINITY=0 # affinity control for mvapich1
export MV2_ENABLE_AFFINITY=0 # affinity control for mvapich2
export OMP_NUM_THREADS=8
# should use omp_set_num_threads
mpirun a.out 4