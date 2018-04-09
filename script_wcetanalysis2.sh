#!/bin/bash
#Script for WCET analysis exercise 2 

cd /home/rttools/cprog/rts_avr/ArduinoMake/Arduino-Makefile-master/examples/WCETanalysis2
. profile
make clean
make
cd /home/rttools/cprog/rts_avr/boundt/experiments/WCETanalysis2
. profile
./make_graphs.sh
okular graph.pdf