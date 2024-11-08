#!/bin/bash

for input in ./inputs/*.in; 
do
  echo "Running input file $input"
  output="${input%.in}.sol"
  ./mydht < $input | diff $output -
done
