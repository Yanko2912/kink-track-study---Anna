#!/bin/bash
set -euo pipefail


if [ "$#" -ne 3 ]; then
  exit 1
fi

start="$1"
end="$2"
sel="$3"

case "$sel" in
  send_simu.sh) job="send_simu.sh" ;;
  send_cuts.sh) job="send_cuts.sh" ;;
  *) exit 2 ;;
esac

mkdir -p DATA

for ((i = start; i <= end; i++)); do
  d="DATA/$i"
  mkdir -p "$d"

  cp simu_setup.conf     "$d/"
  cp reco.conf           "$d/"
  cp pipeline.conf       "$d/"
  cp SNCutsPipeline.conf "$d/"
  cp simu_setup.conf "$d/"
  sbatch --output="$d/slurm-%j.out" "$job" "$i"
done

