# Standalone CLUE Algorithm on GPU and CPU


## 1. Setup on Felk40 or Vinavx2

This needs `gcc version 7.4.1` and `CUDA_10_1 with driver V418`.
Note that `gcc v4.8` is the defaul gcc version on Felk40 or Vinavx2, 
so you have to switch to `gcc version 7.4.1` by setting up CMSSW_11_0_X. 
The only purpose of CMSSW here is to prove a correct version of gcc.
If you already have `gcc version 7.4.1` ad default gcc, you can
skip this step.

```
# skip this if your defaut gcc is V7.4.1
cmsrel CMSSW_11_0_X 
cd CMSSW_11_0_X/src
cmsenv
cd ../..
```

First build your standalone CUDA development workplace. 
This should take a few minutes to compile.
And test run of `vectorAdd` should be good.
```
cp -r /usr/local/cuda-10.1/samples ./CUDA_Sample_10_1
cd CUDA_Sample_10_1
make -j
bin/x86_64/linux/release/vectorAdd
```



Then setup this project. **fork this reop for developers**. In `CUDA_Sample_10_1`
```
mkdir 8_MyProject
cd 8_MyProject
git clone https://github.com/YOUR_FOLK/CLUEAlgo.git
cd CLUEAlgo
make
```

## 2. Implementation

Result of CPU and GPU are tested to identical

## 3. Performance
