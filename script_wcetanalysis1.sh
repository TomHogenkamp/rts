#!/bin/bash
#Script for WCET analysis exercise 1

cd /home/rttools/cprog/rts_avr/ArduinoMake/Arduino-Makefile-master/examples/WCETanalysis1
. profile
make clean
make
cd /home/rttools/cprog/rts_avr/boundt/experiments/WCETanalysis1
. profile
./make_graphs.sh
#okular graph.pdf
cd /home/rttools/cprog/rts_avr/simulavr/examples/WCETanalysis1
. profile
./run_simulavr.sh
gtkwave trace.vcd

