# Calorimeter_Data_Gen

This repository is focused on generating data related to calorimeter simulations, for machine learning based proton and pion classification model. The project leverages CMake for build management and references dependencies such as CLHEP.

## Features

- Simulation setup for calorimeter systems.
- Uses CMake and supports C++ compilation.
- Integration with CLHEP for physics calculations.
- Organized build directories for different simulation configurations.

## Directory Structure

- `simulation_small_cell/`: Primary source directory for simulation code and build configurations.
- `simulation_small_cell/build/`, `build_1/`, `build_2/`, `build_3/`, `build_4/`: Separate build directories, for different test cases, configurations, or parameter sweeps.

## Requirements

- CMake (build system)
- C++ compiler (e.g., GCC)
- CLHEP physics library
- CERN ROOT for output files.
- Screen for backgroud run.

## Building

To build the project, you typically need to:

```bash
cd simulation_small_cell
mkdir build && cd build
cmake ..
make
```

Ensure that the CLHEP library is installed and available on your system.

## Running Simulation
```bash
./exampleb4a -f "path/to/your/directory/Output.root" -m run3.mac
```
run3.mac-> This file is available in build_1 directory. It is a macrofile contains parameters like: particle species, number of events and energy of particle. 


---

> 
