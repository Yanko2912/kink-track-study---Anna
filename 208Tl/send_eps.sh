#!/bin/sh

# SLURM options:

#SBATCH --job-name=208TlEps         	 # Job name
#SBATCH --mem=1G                    	 # RAM
#SBATCH --licenses=sps                   # When working on sps, must declare license!!

#SBATCH --time=50:00                	 # Time for the job in format “minutes:seconds” or  “hours:minutes:seconds”, “days-hours”
#SBATCH --cpus-per-task=1                # Number of CPUs

root get_eps.C
