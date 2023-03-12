#!/bin/bash
#
#SBATCH --nodes=2
#SBATCH --ntasks=4
#SBATCH --exclusive
#SBATCH --partition=compute2011

mpirun ./a.out
