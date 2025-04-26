#!/bin/bash



# Loop from start_index to end_index
for ((i=; i<36; i++))
do
    echo "Running iteration $i"

    # Run the Geant4 simulation
    ./exampleB4a -m run3.mac

    # Check if the ROOT file was created
    if [ -f "/mnt/c/Users/hnayak/Documents/Pion_100GeV/pione.root" ]; then
        # Rename the ROOT file with the iteration number
        mv "/mnt/c/Users/hnayak/Documents/Pion_100GeV/pione.root" "/mnt/c/Users/hnayak/Documents/Pion_100GeV/pion_$i.root"
        echo "File pion_$i.root moved successfully."
    else
        echo "pion.root not found in iteration $i"
    fi
  
    # Optional delay to avoid overwhelming the system
    # sleep 1
done

