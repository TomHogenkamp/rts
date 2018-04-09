#!/bin/bash
#Script for HAPI

cd /home/rttools/omphale_tools/progs/HapiLight/doc/
make rma1 
./rma1
gtkwave rma1.vcd