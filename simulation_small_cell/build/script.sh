#!/bin/bash

# Create the target directory if it doesn't exist
mkdir -p /mnt/d/Pion/

# Loop 100 times
for i in {1..100}
  do
    echo "Running iteration $i"
    
    # Run the Geant4 simulation
    ./exampleB4a -m run3.mac

    # Check if the ROOT file was created
    if [ -f /mnt/d/pion.root ]; then
      # Rename the ROOT file with the iteration number
      mv /mnt/d/pion.root /mnt/d/Pion/pion_$i.root
      echo "File pion_$i.root moved successfully."
    else
      echo "pion.root not found in iteration $i"
    fi
  
    # Optional: Add a small delay between iterations (uncomment if needed)
    # sleep 1

  done

echo "All iterations completed."