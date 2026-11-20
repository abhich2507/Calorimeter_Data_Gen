#!/bin/bash
#SBATCH --job-name=b4a_sim
#SBATCH --output=/farm_out/aabhishe/sim_%A_%a.out
#SBATCH --error=/farm_out/aabhishe/sim_%A_%a.err
#SBATCH --partition=production
#SBATCH --time=00:10:00
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=8G
#SBATCH --array=0-1

# 1. Environment setup (uncomment if needed)
# source /etc/profile.d/modules.sh
module purge
module use /cvmfs/oasis.opensciencegrid.org/jlab/geant4/modules
module load geant4/11.3.2
module load root/6.38.04

# 2. Define arrays and paths
PARTICLES=("pi+" "proton")
CURRENT_PARTICLE=${PARTICLES[$SLURM_ARRAY_TASK_ID]}
MACRO_FILE="run_${CURRENT_PARTICLE}.mac"

EXE="/work/eic/users/aabhishe/Calorimeter_Data_Gen/simulation_small_cell/build/exampleB4a"
OUT_DIR="/work/eic/users/aabhishe/Calorimeter_Data_Gen/simulation_small_cell/Data"

# Create the output directory if it doesn't already exist
mkdir -p $OUT_DIR

echo "Starting job for ${CURRENT_PARTICLE}"

# 3. Generate the macro file dynamically
cat <<EOF > $MACRO_FILE
/run/numberOfThreads 1
/random/setSeeds 8904138 $((1546569 + SLURM_ARRAY_TASK_ID))
/run/initialize
/gun/particle ${CURRENT_PARTICLE}
/gun/energy 100 GeV
/run/beamOn 1
EOF

# 4. Execute the Geant4 application with the -f flag
OUT_FILE="${OUT_DIR}/data_100k_${CURRENT_PARTICLE}.root"

echo "Executing: $EXE -m $MACRO_FILE -f $OUT_FILE"
$EXE -m $MACRO_FILE -f $OUT_FILE

echo "Job completed successfully. Data saved to $OUT_FILE"