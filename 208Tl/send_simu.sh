#!/bin/sh

# SLURM options:

#SBATCH --job-name=208Tl         	 # Job name
#SBATCH --mem=6G                     	 # RAM
#SBATCH --licenses=sps                   # When working on sps, must declare license!!

#SBATCH --time=15:00:00                	 # Time for the job in format “minutes:seconds” or  “hours:minutes:seconds”, “days-hours”
#SBATCH --cpus-per-task=1                # Number of CPUs



source ${THRONG_DIR}/config/supernemo_profile.bash
snswmgr_load_setup falaise@5.1.5

i=$1

FALAISE_BIN=/sps/nemo/sw/redhat-9-x86_64/snsw/opt2/falaise-5.1.5/bin
WORKDIR=/sps/nemo/scratch/ayanko/kink_track_study_Anna/208Tl/DATA/$i

$FALAISE_BIN/flsimulate -c $WORKDIR/simu_setup.conf -o $WORKDIR/simu.brio

$FALAISE_BIN/flreconstruct -i $WORKDIR/simu.brio -p reco.conf -o $WORKDIR/reco.brio
rm -f "$WORKDIR/simu.brio"
