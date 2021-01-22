#!/bin/sh
#PBS -l nodes=4:ppn=2
#PBS -q Q1
#PBS -j oe

cd $PBS_O_WORKDIR
mpirun -machinefile $PBS_NODEFILE -np 4 ./test2
exit 0 





