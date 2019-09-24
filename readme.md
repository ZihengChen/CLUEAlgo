# Standalone CLUE Algorithm


## 1. Setup on Felk40 or Vinavx2

This needs `gcc version 7.4.1` and `CUDA_10_1 with driver V418`.
Note that `gcc v4.8` is the defaul gcc version on Felk40 or Vinavx2, so one has to 
switch to `gcc version 7.4.1` by setting up CMSSW_11_0_X. The only purpose of CMSSW 
here is to prove a correct version of gcc.

```
cmsrel CMSSW_11_0_X 
cd CMSSW_11_0_X/src
cmsenv
cd ../..
```

First build your standalone CUDA development workplace.
```
cp -r /usr/local/cuda-10.1/samples ./CUDA_Sample_10_1
cd CUDA_Sample_10_1
make -j
bin/x86_64/linux/release/vectorAdd
```
This should take a few minutes to build.
And running the example program `vectorAdd` should be good.

Then setup this project. in `CUDA_Sample_10_1`
```
mkdir 8_MyProject
cd 8_MyProject
git clone https://github.com/ZihengChen/CLUEAlgo.git
cd CLUEAlgo
make
```