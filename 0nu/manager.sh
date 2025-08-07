#!/bin/bash

mkdir -p DATA

for ((i = 0; i < $1; i++)); do
  mkdir -p "DATA/$i"
  sbatch send.sh $i
done
