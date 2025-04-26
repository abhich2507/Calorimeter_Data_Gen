#!/bin/bash

INPUT_DIR="/mnt/c/Users/hnayak/Documents/Proton"
OUTPUT_DIR="/mnt/d/Proton_col"

for i in {81..100}
do
    INPUT_FILE="$INPUT_DIR/proton_$i.root"
    OUTPUT_FILE="$OUTPUT_DIR/result_proton_$i.root"
    echo "Processing file: $INPUT_FILE"

    ./Ntuple_8 -m 0 -v 1 -e 1000 -f $INPUT_FILE -o $OUTPUT_FILE

    if [ $? -eq 0 ]; then
        echo "File $i processed successfully"
    else
        echo "Error processing file $i"
    fi

done

echo "All files processed" 