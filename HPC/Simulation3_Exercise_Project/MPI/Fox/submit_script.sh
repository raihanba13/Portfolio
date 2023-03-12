#!/bin/bash
#
#SBATCH --nodes=1
#SBATCH --ntasks=9
#SBATCH --exclusive
#SBATCH --partition=compute2011

mpirun ./a.out
