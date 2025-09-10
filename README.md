Repository Structure

At the root are 4 main directories (e.g., 0nu, 2nu, 208Tl, 214Bi). Example for 0nu/:

0nu/
├─ manager.sh              # Master script: creates DATA/<i> folders and launches tasks
├─ send_simu.sh            # Simulation + base reconstruction (simu_setup.conf → simu.brio → reco.brio)
├─ send_cuts.sh            # Apply three cut pipelines (KK/SS/SK) to reco.brio
├─ dump_energy.C           # Reads energies from Default.root and builds histograms (dump)
├─ energy_overly.c         # Overlay visualization of pre-generated histograms
├─ get_eps.C               # Counts events in 2.7–3.5 MeV window and computes ε
├─ send_eps.sh             # Batch launcher for get_eps.C across subfolders
├─ run.sh                  # Example SLURM script for running dump_energy on the cluster
├─ simu_setup.conf         # flsimulate configuration
├─ reco.conf               # flreconstruct configuration (standard chain with Zimmermann track fit)
├─ SNCutsPipelineKK.conf   # Selection pipeline for KK 
├─ SNCutsPipelineSS.conf   # Selection pipeline for SS 
└─ SNCutsPipelineSK.conf   # Selection pipeline for SK 


After execution, a tree DATA/<i>/ is created with intermediate and final outputs.

Dependencies & Environment

Falaise 5.1.x (e.g., /sps/nemo/sw/redhat-9-x86_64/snsw/opt2/falaise-5.1.5/bin)


1) Managing jobs with manager.sh

manager.sh creates subfolders in DATA/ and launches the requested task (simulation or cuts).

Usage:

# Create and run jobs for DATA/0..49 (50 total)
./manager.sh send_simu.sh 0 49     # Simulation + primary reconstruction
./manager.sh send_cuts.sh 0 49     # Apply SNCuts (KK/SS/SK) to reco.brio

2) Simulation & Reconstruction — send_simu.sh

Key steps for folder i:

WORKDIR=/sps/nemo/scratch/ayanko/kink_track_study_Anna/0nu/DATA/$i

$FALAISE_BIN/flsimulate -c $WORKDIR/simu_setup.conf -o $WORKDIR/simu.brio
$FALAISE_BIN/flreconstruct -i $WORKDIR/simu.brio -p reco.conf -o $WORKDIR/reco.brio
rm -f "$WORKDIR/simu.brio"


Generates simu.brio from simu_setup.conf.

Reconstructs reco.brio using reco.conf.

Deletes simu.brio to save space.

reco.conf uses the standard Falaise reconstruction chain with track fitting (Zimmermann) and clustering; its outputs feed the cut pipelines.

3) Applying Selections — send_cuts.sh

For each DATA/<i>:

INPUT="${WORKDIR}/reco.brio"

mkdir -p "${MYDIRKK}" "${MYDIRSS}" "${MYDIRSK}"

cd $MYDIRKK; "${FALAISE_BIN}/flreconstruct" -i "${INPUT}" -p "${SNCUTS_CONF}/SNCutsPipelineKK.conf" -o "${MYDIRKK}/reco_cutsKK.brio"
cd $MYDIRSS; "${FALAISE_BIN}/flreconstruct" -i "${INPUT}" -p "${SNCUTS_CONF}/SNCutsPipelineSS.conf" -o "${MYDIRSS}/reco_cutsSS.brio"
cd $MYDIRSK; "${FALAISE_BIN}/flreconstruct" -i "${INPUT}" -p "${SNCUTS_CONF}/SNCutsPipelineSK.conf" -o "${MYDIRSK}/reco_cutsSK.brio"


Each of the three directories (cutsKK/, cutsSS/, cutsSK/) contains:

reco_cuts*.brio — reconstructed events after cuts

Default.root — ROOT tree for further analysis

Analysis

A) Energy Dump & Overlay — dump_energy.C → energy_overly.c

dump_energy.C scans DATA/*/cuts{KK,SS,SK}/Default.root, extracts total energies, and builds histograms (saving ROOT/PNG/PDF outputs).

energy_overly.c overlays the pre-generated histograms (e.g., combined KK/SS/SK spectra with normalization options).

Run on cluster:

chmod +x run.sh
sbatch run.sh

root -l energy_overly.c


(run.sh is an example batch script that calls dump_energy.C in batch mode.)

B) Efficiency Calculation — get_eps.C → send_eps.sh

get_eps.C opens each Default.root, counts events with total energy 2.7–3.5 MeV, writes them to a text file (only those events in range), and computes ε = N(2.7–3.5) / N_total.

send_eps.sh batch-launches get_eps.C for multiple subfolders.

Example:

# Compute efficiencies for all subfolders 0..49
./manager.sh send_cuts.sh 0 49       # If cuts were not yet applied


Data Format & Outputs

After a full cycle, expect:

DATA/<i>/
├─ reco.brio               # Base reconstruction result
├─ cutsKK/
│  ├─ reco_cutsKK.brio
│  └─ Default.root         # Analysis data (KK)
├─ cutsSS/
│  ├─ reco_cutsSS.brio
│  └─ Default.root         # (SS)
└─ cutsSK/
   ├─ reco_cutsSK.brio
   └─ Default.root         # (SK)


Analysis artifacts may include:

DATA/energy_dump.root, DATA/energy_overlay.png, DATA/energy_overlay.pdf

DATA/get_eps_summary.txt 

Quick Start 
cd 0nu

# 1) Simulate + reconstruct into DATA/0..49
./manager.sh send_simu.sh 0 49

# 2) Apply three selection pipelines
./manager.sh send_cuts.sh 0 49

#4) chmod +x run.sh
sbatch run.sh

# 4) Overlay spectra
root -l energy_overly.c

#5)
chmod +x  send_eps.sh 
sbatch send_eps.sh

# 6) All summary plots and .txt are collected in:
results/

