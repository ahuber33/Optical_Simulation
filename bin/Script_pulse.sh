#!/bin/bash


for (( i=1 ; i<=10 ; i++ ))
do

 while [[ $(pgrep -x SN_OpticalSim | wc -l) -gt 0 ]] #gt= > # voir ici si on doit changer le Ge en Simulation
#ligne pour faire tourner plusieurs simu
    do
	sleep 5
    done
 ./SN_OpticalSim 100 GA0154_gamma_Central_${i} & 
 sleep 1

done
