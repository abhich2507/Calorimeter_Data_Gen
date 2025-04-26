#!/bin/bash

INPUT_DIR="/mnt/c/Users/hnayak/Documents/Proton_100GeV"
#OUTPUT_DIR="/mnt/c/Users/hnayak/Documents/Proton_100GeV_Col"
OUTPUT_DIR="/mnt/d/Proton_100GeV_Col"

for i in {96..100}
do
    INPUT_FILE="$INPUT_DIR/proton_$i.root"
    OUTPUT_FILE="$OUTPUT_DIR/result_proton_$i.root"
    echo "Processing file: $INPUT_FILE"

    ./Ntuple_8 -m 0 -v 1 -e 1000 -f $INPUT_FILE -o $OUTPUT_FILE

    if [ $? -eq 0 ]; then
        echo "File $i processed successfully"
        rm $INPUT_FILE
    else
        echo "Error processing file $i"
    fi

done

echo "All files processed" 