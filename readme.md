# Standalone CLUE Algorithm on GPU and CPU

Z.Chen[1], A. Di Pilato[2], F. Pantaleo[3], M. Rovere[3]

*[1] Northwestern University, [2]University of Bari, [3] CERN*

## 1. Setup on a CERN machine with GPUs

Source the LCG View containing the correct version of GCC and Boost:
```bash
source /cvmfs/sft.cern.ch/lcg/views/LCG_96/x86_64-centos7-gcc8-opt/setup.sh
```

Then setup this project. **Fork this reop if developers**.
```bash
git clone --recurse-submodules https://github.com/ZihengChen/CLUEAlgo.git
cd CLUEAlgo
make
```

### 1.2. Setup on a Ubuntu Machine with CUDA and GCC>7.0
```
sudo apt-get install libtbb-dev
sudo apt-get install libboost-all-dev
git clone --recurse-submodules https://github.com/ZihengChen/CLUEAlgo.git
cd CLUEAlgo
make
```

### 2. Run CLUE
CLUE needs four parameters `dc, deltao, deltac, rhoc`

_dc_ is the critical distance used to compute the local density.
_deltao_ is the maximum distance for a point to be linked to a nearest higher
point.
_deltac_ is the minimum distance for a local high density point to be promoted
as a Seed.
_rhoc_ is the minimum local density for a point to be promoted as a Seed.

If the projects compiles without errors, you can go run CLUEAlgo by
```bash
# ./main [fileName] [dc] [deltao] [deltac] [rhoc] [useGPU] [totalNumberOfEvent] [verbose]
./main aniso_1000 20 20 50 50 0 10 1
```

The input files are `data/input/*.csv` with columns 
* x, y, layer, weight

The output files are `data/output/*.csv` with columns
* x, y, layer, weight, rho, delta, nh, isSeed, clusterId

If you meet any error when compiling or running this project, 
contact us at zihengchen2015@u.northwestern.edu
## 3. Implementation

<p align=center><img width="50%" src=https://github.com/ZihengChen/CLUEAlgo/blob/master/plots/implementations.png/></p> 
Result of CPU and GPU are tested to identical.


## 4. Examples
The clustering result of a few sythetic dataset is shown below
<p align=center><img width="100%" src=https://github.com/ZihengChen/CLUEAlgo/blob/master/plots/examples.png/></p> 

## 5. Performannce on Toy Events
We generate toy events on toy detector consist of 1 layer (left) or 100 layers (right).
A toy event with 4000 points per layer is shown as below.
<p align=center><img width="40%" src=https://github.com/ZihengChen/CLUEAlgo/blob/master/plots/toyDetector_4000_5_10_5_8.png/></p>
The average execution time of toy events on CPU and GPU are shown below
<p align=center><img width="100%" src=https://github.com/ZihengChen/CLUEAlgo/blob/master/plots/executionTime0.png/></p> 

