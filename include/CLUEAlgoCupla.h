#ifndef CLUEAlgoCupla_h
#define CLUEAlgoCupla_h


#define DALPAKA_ACC_GPU_CUDA_ENABLED 
#ifndef CUPLA_HEADER_ONLY
#   define CUPLA_HEADER_ONLY 1
#endif
#include <cupla/config/GpuCudaRt.hpp>

#define ALPAKA_ACC_GPU_CUDA_ENABLED 1
#include <cuda_to_cupla.hpp>

#include "LayerTilesCupla.h"
#include "CLUEAlgo.h"

static const int maxNSeeds = 10000; 
static const int maxNFollowers = 20; 
static const int localStackSizePerSeed = 20; 

struct PointsPtr {
  float *x; 
  float *y ;
  int *layer ;
  float *weight ;

  float *rho ; 
  float *delta; 
  int *nearestHigher;
  int *clusterIndex; 
  int *isSeed;
};

class CLUEAlgoCupla : public CLUEAlgo {
  // inheriate from CLUEAlgo

  public:
    // constructor
    CLUEAlgoCupla(float dc, float d0, float deltac, float rhoc) : CLUEAlgo(dc,d0,deltac,rhoc) {
      init_device();
    }
    // distrcutor
    ~CLUEAlgoCupla(){
      free_device();
    }

    // public methods
    void makeClusters(); // overwrite base class


  private:
    // private variables

    // #ifdef __CUDACC__
    // // CUDA functions

    // algorithm internal variables
    PointsPtr d_points;
    LayerTilesCupla *d_hist;
    GPU::VecArray<int,maxNSeeds> *d_seeds;
    GPU::VecArray<int,maxNFollowers> *d_followers;

    // private methods
    void init_device(){
      unsigned int reserve = 1000000;
      // input variables
      cudaMalloc((void**)&d_points.x, sizeof(float)*reserve);
      cudaMalloc((void**)&d_points.y, sizeof(float)*reserve);
      cudaMalloc((void**)&d_points.layer, sizeof(int)*reserve);
      cudaMalloc((void**)&d_points.weight, sizeof(float)*reserve);
      // result variables
      cudaMalloc((void**)&d_points.rho, sizeof(float)*reserve);
      cudaMalloc((void**)&d_points.delta, sizeof(float)*reserve);
      cudaMalloc((void**)&d_points.nearestHigher, sizeof(int)*reserve);
      cudaMalloc((void**)&d_points.clusterIndex, sizeof(int)*reserve);
      cudaMalloc((void**)&d_points.isSeed, sizeof(int)*reserve);
      // algorithm internal variables
      cudaMalloc((void**)&d_hist, sizeof(LayerTilesCupla) * NLAYERS);
      cudaMalloc((void**)&d_seeds, sizeof(GPU::VecArray<int,maxNSeeds>) );
      cudaMalloc((void**)&d_followers, sizeof(GPU::VecArray<int,maxNFollowers>)*reserve);
    }

    void free_device(){
      // input variables
      cudaFree(d_points.x);
      cudaFree(d_points.y);
      cudaFree(d_points.layer);
      cudaFree(d_points.weight);
      // result variables
      cudaFree(d_points.rho);
      cudaFree(d_points.delta);
      cudaFree(d_points.nearestHigher);
      cudaFree(d_points.clusterIndex);
      cudaFree(d_points.isSeed);
      // algorithm internal variables
      cudaFree(d_hist);
      cudaFree(d_seeds);
      cudaFree(d_followers);
    }

    void copy_todevice(){
      // input variables
      cudaMemcpy(d_points.x, points_.x.data(), sizeof(float)*points_.n, cudaMemcpyHostToDevice);
      cudaMemcpy(d_points.y, points_.y.data(), sizeof(float)*points_.n, cudaMemcpyHostToDevice);
      cudaMemcpy(d_points.layer, points_.layer.data(), sizeof(int)*points_.n, cudaMemcpyHostToDevice);
      cudaMemcpy(d_points.weight, points_.weight.data(), sizeof(float)*points_.n, cudaMemcpyHostToDevice);
    }

    void clear_set(){
      // result variables
      cudaMemset(d_points.rho, 0x00, sizeof(float)*points_.n);
      cudaMemset(d_points.delta, 0x00, sizeof(float)*points_.n);
      cudaMemset(d_points.nearestHigher, 0x00, sizeof(int)*points_.n);
      cudaMemset(d_points.clusterIndex, 0x00, sizeof(int)*points_.n);
      cudaMemset(d_points.isSeed, 0x00, sizeof(int)*points_.n);
      // algorithm internal variables
      cudaMemset(d_hist, 0x00, sizeof(LayerTilesCupla) * NLAYERS);
      cudaMemset(d_seeds, 0x00, sizeof(GPU::VecArray<int,maxNSeeds>));
      cudaMemset(d_followers, 0x00, sizeof(GPU::VecArray<int,maxNFollowers>)*points_.n);
    }

    void copy_tohost(){
      // result variables
      cudaMemcpy(points_.rho.data(), d_points.rho, sizeof(float)*points_.n, cudaMemcpyDeviceToHost);
      cudaMemcpy(points_.delta.data(), d_points.delta, sizeof(float)*points_.n, cudaMemcpyDeviceToHost);
      cudaMemcpy(points_.nearestHigher.data(), d_points.nearestHigher, sizeof(int)*points_.n, cudaMemcpyDeviceToHost);
      cudaMemcpy(points_.clusterIndex.data(), d_points.clusterIndex, sizeof(int)*points_.n, cudaMemcpyDeviceToHost);
      cudaMemcpy(points_.isSeed.data(), d_points.isSeed, sizeof(int)*points_.n, cudaMemcpyDeviceToHost);
    }

    // #endif // __CUDACC__

};

#endif
