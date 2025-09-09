#!/bin/sh

# SLURM options:

#SBATCH --job-name=0nu         	 # Job name
#SBATCH --mem=1G                    	 # RAM
#SBATCH --licenses=sps                   # When working on sps, must declare license!!

#SBATCH --time=50:00                	 # Time for the job in format “minutes:seconds” or  “hours:minutes:seconds”, “days-hours”
#SBATCH --cpus-per-task=1                # Number of CPUs

source "${THRONG_DIR}/config/supernemo_profile.bash"
snswmgr_load_setup falaise@5.1.5

root dump_energy.C
