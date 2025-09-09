#!/bin/bash

# ===== SLURM options =====
#SBATCH --job-name=208Tl
#SBATCH --mem=600M
#SBATCH --licenses=sps
#SBATCH --time=06:00:00
#SBATCH --cpus-per-task=1



source "${THRONG_DIR}/config/supernemo_profile.bash"
snswmgr_load_setup falaise@5.1.5


i=$1


SNCUTS_CONF="/sps/nemo/scratch/ayanko/kink_track_study_Anna/208Tl"
WORKDIR="${SNCUTS_CONF}/DATA/${i}"
FALAISE_BIN="/sps/nemo/sw/redhat-9-x86_64/snsw/opt2/falaise-5.1.5/bin"
MYDIRKK="${WORKDIR}/cutsKK"
MYDIRSS="${WORKDIR}/cutsSS"
MYDIRSK="${WORKDIR}/cutsSK"

INPUT="${WORKDIR}/reco.brio"


mkdir -p "${MYDIRKK}" "${MYDIRSS}" "${MYDIRSK}"

# ===== Run three cut pipelines =====
cd $MYDIRKK 
"${FALAISE_BIN}/flreconstruct" -i "${INPUT}" -p "${SNCUTS_CONF}/SNCutsPipelineKK.conf" -o "${MYDIRKK}/reco_cutsKK.brio"
cd $MYDIRSS
"${FALAISE_BIN}/flreconstruct" -i "${INPUT}" -p "${SNCUTS_CONF}/SNCutsPipelineSS.conf" -o "${MYDIRSS}/reco_cutsSS.brio"
cd $MYDIRSK
"${FALAISE_BIN}/flreconstruct" -i "${INPUT}" -p "${SNCUTS_CONF}/SNCutsPipelineSK.conf" -o "${MYDIRSK}/reco_cutsSK.brio"
