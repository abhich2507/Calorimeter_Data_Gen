#!/bin/bash

INPUT_DIR="/mnt/d/Positron"
OUTPUT_DIR="/mnt/d/Positron_col"

for i in {80..80}
do
    INPUT_FILE="$INPUT_DIR/positron_$i.root"
    OUTPUT_FILE="$OUTPUT_DIR/result_positron_$i.root"
    echo "Processing file: $INPUT_FILE"

    ./Ntuple_8 -m 1 -v 1 -e 1000 -f $INPUT_FILE -o $OUTPUT_FILE
  

    if [ $? -eq 0 ]; then
        echo "File $i processed successfully"
        rm $INPUT_FILE
    else
        echo "Error processing file $i"
    fi

done

echo "All files processed"