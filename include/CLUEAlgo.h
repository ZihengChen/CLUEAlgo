#ifndef CLUEAlgo_h
#define CLUEAlgo_h

// C/C++ headers
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "LayerTiles.h"
#include "Points.h"

#define NLAYERS 1

class CLUEAlgo{

  public:
    // constructor
    CLUEAlgo(float dc, float d0, float rhoc ){ 
      dc_ = dc; 
      d0_ = d0; 
      rhoc_ = rhoc;
    }
    // distrcutor
    ~CLUEAlgo(){} 
    
    // public variables
    float dc_, d0_, rhoc_;
    Points points_; 

    // public methods
    void setPoints(int n, float* x, float* y, int* layer, float* weight) {
      points_.clear();
      points_.n = n;
      // input variables
      points_.x.assign(x, x + n);
      points_.y.assign(y, y + n);
      points_.layer.assign(layer, layer + n);
      points_.weight.assign(weight, weight + n);
      // result variables
      points_.rho.resize(n,0);
      points_.delta.resize(n,std::numeric_limits<float>::max());
      points_.nearestHigher.resize(n,-1);
      points_.isSeed.resize(n,0);
      points_.followers.resize(n);
      points_.clusterIndex.resize(n,-1);
    }

    void clearPoints(){ points_.clear(); }

    void makeClusters();

    void verboseResults(int nVerbose = -1 ){
      
      if (nVerbose ==-1) nVerbose=points_.n;

      for(int i = 0; i < nVerbose; i++) {
        std::cout << i << ", ("<<points_.x[i]<< ","<<points_.y[i]<< "), rho="<<points_.rho[i];
        if (points_.delta[i] < 999)
          std::cout << ", delta="<<points_.delta[i];
        else
          std::cout << ", delta=+inf";
        std::cout << ", nh="<<points_.nearestHigher[i];
        std::cout << ", clID="<<points_.clusterIndex[i];
        std::cout << std::endl;
      }
    }
    
    
  private:

    // private variables


    // private member methods
    void prepareDataStructures(std::array<LayerTiles, NLAYERS> & );
    void calculateLocalDensity(std::array<LayerTiles, NLAYERS> & );
    void calculateDistanceToHigher(std::array<LayerTiles, NLAYERS> & );
    void findAndAssignClusters();
      
    inline float distance(int , int) const ;
};

#endif
