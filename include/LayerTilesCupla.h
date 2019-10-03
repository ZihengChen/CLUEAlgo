#ifndef LayerTilesCupla_h
#define LayerTilesCupla_h

#include <memory>
#include <cmath>
#include <algorithm>
#include <cstdint>

#include "GPUVecArrayCupla.h"
#include "LayerTilesConstants.h"

using GPUVect = GPU::VecArray<int, LayerTilesConstants::maxTileDepth>;

class LayerTilesCupla {

  public:

    // constructor
    LayerTilesCupla(){};

    // #ifdef __CUDACC__
    // overload the fill function on device
    // __device__


    ALPAKA_FN_HOST_ACC int getXBin(float x) const {
      int xBin = (x-LayerTilesConstants::minX)*LayerTilesConstants::rX;
      xBin = (xBin<LayerTilesConstants::nColumns ? xBin:LayerTilesConstants::nColumns-1);
      xBin = (xBin>0 ? xBin:0);
      return xBin;
    }

    ALPAKA_FN_HOST_ACC int  getYBin(float y) const {
      int yBin = (y-LayerTilesConstants::minY)*LayerTilesConstants::rY;
      yBin = (yBin<LayerTilesConstants::nRows ? yBin:LayerTilesConstants::nRows-1);
      yBin = (yBin>0 ? yBin:0);;
      return yBin;
    }

    ALPAKA_FN_HOST_ACC int getGlobalBin(float x, float y) const{
      return getXBin(x) + getYBin(y)*LayerTilesConstants::nColumns;
    }

    ALPAKA_FN_HOST_ACC int getGlobalBinByBin(int xBin, int yBin) const {
      return xBin + yBin*LayerTilesConstants::nColumns;
    }

    ALPAKA_FN_HOST_ACC int4 searchBox(float xMin, float xMax, float yMin, float yMax){
      return int4{ getXBin(xMin), getXBin(xMax), getYBin(yMin), getYBin(yMax)};
    }

    ALPAKA_FN_HOST_ACC void clear() {
      for(auto& t: layerTiles_) t.reset();
    }

    ALPAKA_FN_HOST_ACC GPUVect & operator[] (int globalBinId) {
      return layerTiles_[globalBinId];
    }

  private:
    GPU::VecArray<GPU::VecArray<int, LayerTilesConstants::maxTileDepth>, LayerTilesConstants::nColumns * LayerTilesConstants::nRows > layerTiles_;
};



#endif
