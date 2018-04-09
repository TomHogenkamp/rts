#!/bin/bash
#Script for dual core ARM7 exercise

cd /home/rttools/cprog/rts/apps/examples/prod_cons/arm 
. profile
make clean
make
./swarm test1
