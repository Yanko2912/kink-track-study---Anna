#!/bin/sh

# SLURM options:

#SBATCH --job-name=0nubb_Se82_10p2eve_single_job         	 # Job name
#SBATCH --mem=10G                     	 # RAM
#SBATCH --licenses=sps                   # When working on sps, must declare license!!

#SBATCH --time=50:00                 	 # Time for the job in format “minutes:seconds” or  “hours:minutes:seconds”, “days-hours”
#SBATCH --cpus-per-task=1                # Number of CPUs



source ${THRONG_DIR}/config/supernemo_profile.bash
snswmgr_load_setup falaise@5.1.5

i=$1

WORKDIR=/sps/nemo/scratch/ayanko/kink_track_study_Anna/2nu/DATA/$i
FALAISE_BIN=/sps/nemo/sw/redhat-9-x86_64/snsw/opt2/falaise-5.1.5/bin
SNCUTS_CONF=/sps/nemo/scratch/ayanko/kink_track_study_Anna/2nu/

$FALAISE_BIN/flreconstruct -i $WORKDIR/reco.brio -p /sps/nemo/scratch/ayanko/kink_track_study_Anna/2nu/SNCutsPipeline.conf -o $WORKDIR/reco_cuts.brio
