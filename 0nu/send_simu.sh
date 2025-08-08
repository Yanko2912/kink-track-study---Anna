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
FALAISE_BIN=/sps/nemo/sw/redhat-9-x86_64/snsw/opt2/falaise-5.1.5/bin
CONF_FAL=/sps/nemo/sw/Falaise/install_develop/share/Falaise-4.1.0/resources/snemo/demonstrator/reconstruction
WORKDIR=/sps/nemo/scratch/ayanko/kink_track_study_Anna/0nu/DATA/$i
$FALAISE_BIN/flsimulate -c $WORKDIR/simu_setup.conf -o $WORKDIR/simu.brio

#$FALAISE_BIN/flreconstruct -i $WORKDIR/simu_setup.brio -p $CONF_FAL/official-2.0.0.conf -o $WORKDIR/reco.brio

$FALAISE_BIN/flreconstruct -i $WORKDIR/simu.brio -p reco.conf -o $WORKDIR/reco.brio
