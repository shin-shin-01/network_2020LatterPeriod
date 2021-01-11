#!/bin/bash
#PJM -L "vnode=4"
#PJM -L "vnode-core=36"
#PJM -L "rscunit=ito-a"
#PJM -L "rscgrp=ito-s-dbg"
#PJM -L "elapse=00:10:00"

module load openmpi/3.1.1-nocuda-gcc4.8.5

mpirun -np 4 -mca plm_rsh_agent /bin/pjrsh -machinefile ${PJM_O_NODEINF} ./test





