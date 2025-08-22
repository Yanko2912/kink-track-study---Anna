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

SRC_DIR="/sps/nemo/scratch/ayanko/kink_track_study_Anna/0nu/"

mkdir -p DATA

for ((i = start; i <= end; i++)); do
  d="DATA/$i"
  mkdir -p "$d"

  if [ "$job" = "send_simu.sh" ]; then
    cp "$SRC_DIR/simu_setup.conf" "$d/"
    cp "$SRC_DIR/simu.profile"   "$d/"
    cp "$SRC_DIR/reco.conf"       "$d/"
    cp "$SRC_DIR/pipeline.conf"   "$d/"
  fi

 
  if [ "$job" = "send_cuts.sh" ]; then
    cp "$SRC_DIR/SNCutsPipelineTEST.conf" "$d/"
  fi

  (
    cd "$d" || exit 3
    sbatch --output="slurm-%j.out" "../../$job" "$i"
  )
done
