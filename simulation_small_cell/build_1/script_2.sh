#!/bin/bash



# Loop from start_index to end_index
for ((i=11; i<21; i++))
do
    echo "Running iteration $i"

    # Run the Geant4 simulation
    ./exampleB4a -m run3.mac

    # Check if the ROOT file was created
    if [ -f "/mnt/c/Users/hnayak/Documents/Proton_100GeV/protone.root" ]; then
        # Rename the ROOT file with the iteration number
        mv "/mnt/c/Users/hnayak/Documents/Proton_100GeV/protone.root" "/mnt/c/Users/hnayak/Documents/Proton_100GeV/proton_$i.root"
        echo "File proton_$i.root moved successfully."
    else
        echo "proton.root not found in iteration $i"
    fi
  
    # Optional delay to avoid overwhelming the system
    # sleep 1
done

