#!/bin/bash

echo "# Normal ####################################################"
time ./main -n 62 10000000  
echo "#############################################################"
echo ""
echo "# Optimised #################################################"
time ./main -o 62 10000000
echo "#############################################################"
