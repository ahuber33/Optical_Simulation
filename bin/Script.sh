#!/bin/bash





for (( i=1 ; i<=4 ; i++ ))
do

    
 while [[ $(pgrep -x SN_OpticalSim | wc -l) -gt 3 ]] #gt= > # voir ici si on doit changer le Ge en Simulation
#ligne pour faire tourner plusieurs simu
    do
	sleep 10
    done


   ./SN_OpticalSim 250 t_${i} &
   sleep 3



done



