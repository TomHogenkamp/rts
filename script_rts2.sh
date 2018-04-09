#!/bin/bash
#Script for RTS2

cd /home/rttools/omphale_tools/progs/HapiLight/doc/
make ex6 
./ex6
gtkwave ex6.vcd 
