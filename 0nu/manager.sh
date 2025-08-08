#!/bin/bash

if [ $# -ne 2 ]; then
  exit 1
fi

start=$1
end=$2

mkdir -p DATA

for ((i = start; i <= end; i++)); do
  mkdir -p "DATA/$i"
  cp simu_setup.conf  "DATA/$i/"
  cp reco.conf        "DATA/$i/"
  cp pipeline.conf    "DATA/$i/"
  cp SNCutsPipeline.conf "DATA/$i/"

  sbatch \
    --output="DATA/$i/slurm-%j.out" \
    send_simu.sh "$i"
  sbatch \
    --output="DATA/$i/slurm-%j.out" \
    send_cuts.sh "$i"
done
