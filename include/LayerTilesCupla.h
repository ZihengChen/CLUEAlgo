#ifndef LayerTilesCupla_h
#define LayerTilesCupla_h

#include <memory>
#include <cmath>
#include <algorithm>
#include <cstdint>

#include "GPUVecArray.h"
#include "LayerTilesConstants.h"


class LayerTilesCupla {

  public:

    // constructor
    LayerTilesCupla(){};

    // #ifdef __CUDACC__
    // overload the fill function on device
    // __device__

    
    template< typename T_Acc>
    ALPAKA_FN_HOST_ACC int getXBin(T_Acc const & acc, float x)const {
      int xBin = (x-LayerTilesConstants::minX)*LayerTilesConstants::rX;
      xBin = (xBin<LayerTilesConstants::nColumns ? xBin:LayerTilesConstants::nColumns-1);
      xBin = (xBin>0 ? xBin:0);
      return xBin;
    }
    
    template< typename T_Acc>
    ALPAKA_FN_HOST_ACC int getXBin(T_Acc const & acc, float x) const {
      int xBin = (x-LayerTilesConstants::minX)*LayerTilesConstants::rX;
      xBin = (xBin<LayerTilesConstants::nColumns ? xBin:LayerTilesConstants::nColumns-1);
      xBin = (xBin>0 ? xBin:0);
      return xBin;
    }

    template< typename T_Acc>
    ALPAKA_FN_HOST_ACC int  getYBin(T_Acc const & acc, float y) const {
      int yBin = (y-LayerTilesConstants::minY)*LayerTilesConstants::rY;
      yBin = (yBin<LayerTilesConstants::nRows ? yBin:LayerTilesConstants::nRows-1);
      yBin = (yBin>0 ? yBin:0);;
      return yBin;
    }

    template< typename T_Acc>
    ALPAKA_FN_HOST_ACC int getGlobalBin(T_Acc const & acc, float x, float y) const{
      return getXBin(x) + getYBin(y)*LayerTilesConstants::nColumns;
    }

    template< typename T_Acc>
    ALPAKA_FN_HOST_ACC int getGlobalBinByBin(T_Acc const & acc, int xBin, int yBin) const {
      return xBin + yBin*LayerTilesConstants::nColumns;
    }

    template< typename T_Acc>
    ALPAKA_FN_HOST_ACC int4 searchBox(T_Acc const & acc, float xMin, float xMax, float yMin, float yMax){
      return int4{ getXBin(xMin), getXBin(xMax), getYBin(yMin), getYBin(yMax)};
    }

    template< typename T_Acc>
    ALPAKA_FN_HOST_ACC void clear(T_Acc const & acc) {
      for(auto& t: layerTiles_) t.reset();
    }

    template< typename T_Acc>
    ALPAKA_FN_HOST_ACC GPU::VecArray<int, LayerTilesConstants::maxTileDepth>& operator[](T_Acc const & acc, int globalBinId) {
      return layerTiles_[globalBinId];
    }
    
   

  private:
    GPU::VecArray<GPU::VecArray<int, LayerTilesConstants::maxTileDepth>, LayerTilesConstants::nColumns * LayerTilesConstants::nRows > layerTiles_;
};


  
#endif 
