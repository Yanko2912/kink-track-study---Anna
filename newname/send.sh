#!/bin/sh

# SLURM options:

#SBATCH --job-name=0nubb_Se82_10p2eve_single_job         	 # Job name
#SBATCH --mem=10G                     	 # RAM
#SBATCH --licenses=sps                   # When working on sps, must declare license!!

#SBATCH --time=50:00                 	 # Time for the job in format “minutes:seconds” or  “hours:minutes:seconds”, “days-hours”
#SBATCH --cpus-per-task=1                # Number of CPUs








/sps/nemo/sw/redhat-9-x86_64/snsw/opt2/falaise-5.1.5/bin/flreconstruct -i /sps/nemo/scratch/ayanko/kink_track_study_Anna/newname/reco.brio -p reco.conf -o /sps/nemo/scratch/ayanko/kink_track_study_Anna/newname/reco2.brio
# /sps/nemo/sw/redhat-9-x86_64/snsw/opt2/falaise-5.1.5/bin/flreconstruct -i /pbs/home/a/ayanko/hihihaha/pb/Reco_pb1000.brio -p /pbs/home/a/ayanko/private/MiModule/p_MiModule_v00.conf

