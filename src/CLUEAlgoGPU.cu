#include <math.h>
#include <limits>
#include <iostream>

//GPU Add
#include <cuda_runtime.h>
#include <cuda.h>
// for timing
#include <chrono>
#include <ctime>
// user include

#include "CLUEAlgoGPU.h"


__global__ void kernel_compute_histogram( LayerTilesGPU *d_hist, 
                                          PointsPtr d_points, 
                                          int numberOfPoints
                                          )
{
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if(i < numberOfPoints) {
    // push index of points into tiles
    d_hist[d_points.layer[i]].fill(d_points.x[i], d_points.y[i], i);
  }
} //kernel


__global__ void kernel_compute_density( LayerTilesGPU *d_hist, 
                                        PointsPtr d_points, 
                                        float dc,
                                        int numberOfPoints
                                        ) 
{ 
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if (i < numberOfPoints){
    double rhoi{0.};
    int layeri = d_points.layer[i];
    float xi = d_points.x[i];
    float yi = d_points.y[i];

    // get search box 
    int4 search_box = d_hist[layeri].searchBox(xi-dc, xi+dc, yi-dc, yi+dc);

    // loop over bins in the search box
    for(int xBin = search_box.x; xBin < search_box.y+1; ++xBin) {
      for(int yBin = search_box.z; yBin < search_box.w+1; ++yBin) {

        // get the id of this bin
        int binId = d_hist[layeri].getGlobalBinByBin(xBin,yBin);
        // get the size of this bin
        int binSize  = d_hist[layeri][binId].size();

        // interate inside this bin
        for (int binIter = 0; binIter < binSize; binIter++) {
          int j = d_hist[layeri][binId][binIter];
          // query N_{dc_}(i)
          float xj = d_points.x[j];
          float yj = d_points.y[j];
          float dist_ij = std::sqrt((xi-xj)*(xi-xj) + (yi-yj)*(yi-yj));
          if(dist_ij < dc) { 
            // sum weights within N_{dc_}(i)
            rhoi += d_points.weight[j];              
          }
        } // end of interate inside this bin
      }
    } // end of loop over bins in search box
    d_points.rho[i] = rhoi;
  }
} //kernel


__global__ void kernel_compute_distanceToHigher(LayerTilesGPU* d_hist, 
                                                PointsPtr d_points, 
                                                float d0,
                                                int numberOfPoints
                                                ) 
{
  int i = blockIdx.x * blockDim.x + threadIdx.x;

  if (i < numberOfPoints){
    int layeri = d_points.layer[i];

    float deltai = std::numeric_limits<float>::max();
    int nearestHigheri = -1;
    float xi = d_points.x[i];
    float yi = d_points.y[i];
    float rhoi = d_points.rho[i];

    // get search box 
    int4 search_box = d_hist[layeri].searchBox(xi-d0, xi+d0, yi-d0, yi+d0);

    // loop over all bins in the search box
    for(int xBin = search_box.x; xBin < search_box.y+1; ++xBin) {
      for(int yBin = search_box.z; yBin < search_box.w+1; ++yBin) {
        // get the id of this bin
        int binId = d_hist[layeri].getGlobalBinByBin(xBin,yBin);
        // get the size of this bin
        int binSize  = d_hist[layeri][binId].size();

        // interate inside this bin
        for (int binIter = 0; binIter < binSize; binIter++) {
          int j = d_hist[layeri][binId][binIter];
          // query N'_{dc_}(i)
          float xj = d_points.x[j];
          float yj = d_points.y[j];
          float dist_ij = std::sqrt((xi-xj)*(xi-xj) + (yi-yj)*(yi-yj));
          bool foundHigher = (d_points.rho[j] > rhoi);
          // in the rare case where rho is the same, use detid
          foundHigher = foundHigher || ( (d_points.rho[j] == rhoi) && (j>i));
          if(foundHigher && dist_ij < d0) { // definition of N'_{dc_}(i)
            // find the nearest point within N'_{dc_}(i)
            if (dist_ij<deltai) {
              // update deltai and nearestHigheri
              deltai = dist_ij;
              nearestHigheri = j;
            }
          }
        } // end of interate inside this bin
      }
    } // end of loop over bins in search box
    d_points.delta[i] = deltai;
    d_points.nearestHigher[i] = nearestHigheri;
  }
} //kernel



__global__ void kernel_find_clusters( GPU::VecArray<int,maxNSeeds>* d_seeds,
                                      GPU::VecArray<int,maxNFollowers>* d_followers,
                                      PointsPtr d_points,
                                      float dc, float d0, float rhoc,
                                      int numberOfPoints
                                      ) 
{
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  
  if (i < numberOfPoints) {
    // initialize clusterIndex
    d_points.clusterIndex[i] = -1;
    // determine seed or outlier
    float deltai = d_points.delta[i];
    float rhoi = d_points.rho[i];
    bool isSeed = (deltai > dc) && (rhoi >= rhoc);
    bool isOutlier = (deltai > d0) && (rhoi < rhoc);

    if (isSeed) {
      // set isSeed as 1
      d_points.isSeed[i] = 1;
      d_seeds[0].push_back(i); // head of d_seeds
    } else {
      if (!isOutlier) {
        // register as follower at its nearest higher
        d_followers[d_points.nearestHigher[i]].push_back(i);  
      }
    }
  }
} //kernel


__global__ void kernel_assign_clusters( GPU::VecArray<int,maxNSeeds>* d_seeds, 
                                        GPU::VecArray<int,maxNFollowers>* d_followers,
                                        PointsPtr d_points
                                        )
{

  int idxCls = blockIdx.x * blockDim.x + threadIdx.x;

  if (idxCls < d_seeds[0].size()){

    int localStack[localStackSizePerSeed] = {-1};
    int localStackSize = 0;

    // asgine cluster to seed[idxCls]
    int idxThisSeed = d_seeds[0][idxCls];
    d_points.clusterIndex[idxThisSeed] = idxCls;
    // push_back idThisSeed to localStack
    localStack[localStackSize] = idxThisSeed;
    localStackSize++;

    // process all elements in localStack
    while (localStackSize>0){
      // get last element of localStack
      int idxEndOflocalStack = localStack[localStackSize-1];

      int temp_clusterIndex = d_points.clusterIndex[idxEndOflocalStack];
      GPU::VecArray<int,maxNFollowers> temp_followers = d_followers[idxEndOflocalStack];
              
      // pop_back last element of localStack
      localStack[localStackSize-1] = -1;
      localStackSize--;

      // loop over followers of last element of localStack
      for( int j : temp_followers){
        // pass id to follower
        d_points.clusterIndex[j] = temp_clusterIndex;
        // push_back follower to localStack
        localStack[localStackSize] = j;
        localStackSize++;
      }
    }
  }
} //kernel






void CLUEAlgoGPU::makeClusters( ) {

  copy_todevice();
  clear_set();

  ////////////////////////////////////////////
  // calcualte rho, delta and find seeds 
  // 1 point per thread
  ////////////////////////////////////////////
  const dim3 blockSize(1024,1,1);
  const dim3 gridSize(ceil(points_.n/1024.0),1,1);
  kernel_compute_histogram <<<gridSize,blockSize>>>(d_hist, d_points, points_.n);
  kernel_compute_density <<<gridSize,blockSize>>>(d_hist, d_points, dc_, points_.n);
  kernel_compute_distanceToHigher <<<gridSize,blockSize>>>(d_hist, d_points, d0_, points_.n);
  kernel_find_clusters <<<gridSize,blockSize>>>(d_seeds, d_followers, d_points, dc_,d0_,rhoc_, points_.n);  
  
  ////////////////////////////////////////////
  // assign clusters
  // 1 point per seeds
  ////////////////////////////////////////////
  const dim3 gridSize_nseeds(ceil(maxNSeeds/1024.0),1,1);
  kernel_assign_clusters <<<gridSize_nseeds,blockSize>>>(d_seeds, d_followers, d_points);

  copy_tohost();
}
