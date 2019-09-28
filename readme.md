# Standalone CLUE Algorithm on GPU and CPU

Z.Chen[1], A. Di Pilato[2], F. Pantaleo[3], M. Rovere[3]

*[1] Northwestern University, [2]University of Bari, [3] CERN*

## 1. Setup on felk40 or vinavx2

This needs `gcc version 7.4.1` and `CUDA_10_1 with driver V418`.
Note that `gcc v4.8` is the defaul gcc version on Felk40 or Vinavx2, 
so you have to switch to `gcc version 7.4.1` by setting up CMSSW_11_0_X. 
The only purpose of CMSSW here is to prove a correct version of gcc.
If you already have `gcc version 7.4.1` ad default gcc, you can
skip this step.

```bash
# skip this if your defaut gcc is V7.4.1
cmsrel CMSSW_11_0_X 
cd CMSSW_11_0_X/src
cmsenv
cd ../..
```

First build your standalone CUDA development workplace. 
This should take a few minutes to compile.
And test run of `vectorAdd` should be good.
```bash
cp -r /usr/local/cuda-10.1/samples ./CUDA_Sample_10_1
cd CUDA_Sample_10_1
make -j
bin/x86_64/linux/release/vectorAdd
```



Then setup this project. **Fork this reop if developers**. In `CUDA_Sample_10_1`
```bash
mkdir 8_MyProject
cd 8_MyProject
git clone https://github.com/YOUR_FOLK/CLUEAlgo.git
cd CLUEAlgo
make
```

If the projects compiles without errors, you can go run CLUEAlgo by
```bash
# bin/main [fileName] [dc] [deltao] [deltac] [rhoc] [useGPU] [totalNumberOfEvent] [verbose]
bin/main aniso_1000 20 20 50 50 0 10 1
```

### Format of Input and output
The input files are `data/input/*.csv` with columns 
* `x`, `y`, `layer`, `weight`

The output files are `data/output/*.csv` with columns
* `x`, `y`,`layer`, `weight`,` rho`, `delta`, `nh`, `isSeed`, `clusterId`

If you meet any error when compiling or running this project, 
contact us at zihengchen2015@u.northwestern.edu
## 2. Implementation

<p align=center><img width="50%" src=https://github.com/ZihengChen/CLUEAlgo/blob/master/plots/implementations.png/></p> 
Result of CPU and GPU are tested to identical.


## 3. Examples
The clustering result of a few sythetic dataset is shown below
<p align=center><img width="100%" src=https://github.com/ZihengChen/CLUEAlgo/blob/master/plots/examples.png/></p> 

## 4. Performannce on Toy Events
We generate toy events on toy detector consist of 1 layer (left) or 100 layers (right).
A toy event with 4000 points per layer is shown as below.
<p align=center><img width="40%" src=https://github.com/ZihengChen/CLUEAlgo/blob/master/plots/toyDetector_4000_5_10_5_8.png/></p>
The average execution time of toy events on CPU and GPU are shown below
<p align=center><img width="100%" src=https://github.com/ZihengChen/CLUEAlgo/blob/master/plots/executionTime0.png/></p> 

