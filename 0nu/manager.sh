#!/bin/bash

mkdir -p DATA

for ((i = 0; i < $1; i++)); do
  mkdir -p "DATA/$i"
cp simu_setup.conf  "DATA/$i/"
  cp reco.conf        "DATA/$i/"
  cp pipeline.conf    "DATA/$i/"
	sbatch \
	--output="DATA/$i/slurm-%j.out" \
	send.sh "$i"


done
