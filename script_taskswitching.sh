#!/bin/bash
#Script for taskswitching exercise

cd /home/rttools/cprog/rts_avr/ArduinoMake/Arduino-Makefile-master/examples/Taskswitching
. profile
make
cd /home/rttools/cprog/rts_avr/simulavr/examples/Taskswitching
. profile
./run_simulavr.sh
gtkwave trace.vcd
