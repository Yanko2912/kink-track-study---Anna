#!/bin/sh

# SLURM options:

#SBATCH --job-name=0nubb_Se82_10p2eve_single_job         	 # Job name
#SBATCH --mem=10G                     	 # RAM
#SBATCH --licenses=sps                   # When working on sps, must declare license!!

#SBATCH --time=50:00                 	 # Time for the job in format “minutes:seconds” or  “hours:minutes:seconds”, “days-hours”
#SBATCH --cpus-per-task=1                # Number of CPUs


CONF_FAL=/sps/nemo/sw/Falaise/install_develop/share/Falaise-4.1.0/resources/snemo/demonstrator/reconstruction/ 

/sps/nemo/sw/redhat-9-x86_64/snsw/opt2/falaise-5.1.5/bin/flsimulate -c /sps/nemo/scratch/ayanko/kink_track_study_Anna/Bi/simu_setup.conf -o /sps/nemo/scratch/ayanko/kink_track_study_Anna/Bi/simu_setup.brio

/sps/nemo/sw/redhat-9-x86_64/snsw/opt2/falaise-5.1.5/bin/flreconstruct -i /sps/nemo/scratch/ayanko/kink_track_study_Anna/Bi/simu_setup.brio -p $CONF_FAL/official-2.0.0.conf -o /sps/nemo/scratch/ayanko/kink_track_study_Anna/Bi/reco.brio





/sps/nemo/sw/redhat-9-x86_64/snsw/opt2/falaise-5.1.5/bin/flreconstruct -i /sps/nemo/scratch/ayanko/kink_track_study_Anna/Bi/reco.brio -p reco.conf -o /sps/nemo/scratch/ayanko/kink_track_study_Anna/Bi/reco2.brio
/sps/nemo/sw/redhat-9-x86_64/snsw/opt2/falaise-5.1.5/bin/flreconstruct -i /sps/nemo/scratch/ayanko/kink_track_study_Anna/Bi/reco2.brio -p /sps/nemo/scratch/ayanko/software/SNCuts/build/SNCutsPipeline.conf -o CDFiltered.brio
# /sps/nemo/sw/redhat-9-x86_64/snsw/opt2/falaise-5.1.5/bin/flreconstruct -i /pbs/home/a/ayanko/hihihaha/pb/Reco_pb1000.brio -p /pbs/home/a/ayanko/private/MiModule/p_MiModule_v00.conf

