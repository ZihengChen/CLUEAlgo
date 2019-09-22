#include "CLUEAlgo.h"


void CLUEAlgo::makeClusters(){
  std::array<LayerTiles, NLAYERS> allLayerTiles;
  // start clustering
  prepareDataStructures(allLayerTiles);
  calculateLocalDensity(allLayerTiles);
  calculateDistanceToHigher(allLayerTiles);
  findAndAssignClusters();
}


void CLUEAlgo::prepareDataStructures( std::array<LayerTiles, NLAYERS> & allLayerTiles ){
  for (int i=0; i<points_.n; i++){
    // push index of points into tiles
    allLayerTiles[points_.layer[i]].fill( points_.x[i], points_.y[i], i);
  }
}


void CLUEAlgo::calculateLocalDensity( std::array<LayerTiles, NLAYERS> & allLayerTiles ){
  
  // loop over all points
  for(int i = 0; i < points_.n; i++) {
    LayerTiles& lt = allLayerTiles[points_.layer[i]];
    
    // get search box
    std::array<int,4> search_box = lt.searchBox(points_.x[i]-dc_, points_.x[i]+dc_, points_.y[i]-dc_, points_.y[i]+dc_);
    
    // loop over bins in the search box
    for(int xBin = search_box[0]; xBin < search_box[1]+1; ++xBin) {
      for(int yBin = search_box[2]; yBin < search_box[3]+1; ++yBin) {
        
        // get the id of this bin
        int binId = lt.getGlobalBinByBin(xBin,yBin);
        // get the size of this bin
        int binSize = lt[binId].size();
        
        // interate inside this bin
        for (int binIter = 0; binIter < binSize; binIter++) {
          int j = lt[binId][binIter];
          // query N_{dc_}(i)
          float dist_ij = distance(i, j);
          if(dist_ij < dc_) {
            // sum weights within N_{dc_}(i)
            points_.rho[i] += points_.weight[j];
          }
        } // end of interate inside this bin
      
      }
    } // end of loop over bins in search box
  } // end of loop over points
}


void CLUEAlgo::calculateDistanceToHigher( std::array<LayerTiles, NLAYERS> & allLayerTiles ){
  // loop over all points
  for(int i = 0; i < points_.n; i++) {
    // default values of delta and nearest higher for i
    float maxDelta = std::numeric_limits<float>::max();
    float delta_i = maxDelta;
    int nearestHigher_i = -1;

    LayerTiles& lt = allLayerTiles[points_.layer[i]];

    // get search box 
    std::array<int,4> search_box = lt.searchBox(points_.x[i]-d0_, points_.x[i]+d0_, points_.y[i]-d0_, points_.y[i]+d0_);
    
    // loop over all bins in the search box
    for(int xBin = search_box[0]; xBin < search_box[1]+1; ++xBin) {
      for(int yBin = search_box[2]; yBin < search_box[3]+1; ++yBin) {
        
        // get the id of this bin
        int binId = lt.getGlobalBinByBin(xBin,yBin);
        // get the size of this bin
        int binSize = lt[binId].size();

        // interate inside this bin
        for (int binIter = 0; binIter < binSize; binIter++) {
          int j = lt[binId][binIter];
          // query N'_{dc_}(i)
          bool foundHigher = (points_.rho[j] > points_.rho[i]);
          // in the rare case where rho is the same, use detid
          foundHigher = foundHigher || ((points_.rho[j] == points_.rho[i]) && (j>i) );
          float dist_ij = distance(i, j);
          if(foundHigher && dist_ij < d0_) { // definition of N'_{dc_}(i)
            // find the nearest point within N'_{dc_}(i)
            if (dist_ij < delta_i) {
              // update delta_i and nearestHigher_i
              delta_i = dist_ij;
              nearestHigher_i = j;
            }
          }
        } // end of interate inside this bin
      }
    } // end of loop over bins in search box
    
    points_.delta[i] = delta_i;
    points_.nearestHigher[i] = nearestHigher_i;
  } // end of loop over points
}

void CLUEAlgo::findAndAssignClusters(){
  
  int nClusters = 0;
  
  // find cluster seeds and outlier  
  std::vector<int> localStack;
  // loop over all points
  for(unsigned int i = 0; i < points_.n; i++) {
    // initialize clusterIndex
    points_.clusterIndex[i] = -1;
    // determine seed or outlier
    bool isSeed = (points_.delta[i] > dc_) && (points_.rho[i] >= rhoc_);
    bool isOutlier = (points_.delta[i] > d0_) && (points_.rho[i] < rhoc_);
    if (isSeed) {
      // set isSeed as 1
      points_.isSeed[i] = 1;
      // set cluster id
      points_.clusterIndex[i] = nClusters;
      // increment number of clusters
      nClusters++;
      // add seed into local stack
      localStack.push_back(i);
    } else if (!isOutlier) {
      // register as follower at its nearest higher
      points_.followers[points_.nearestHigher[i]].push_back(i);   
    }
  }

  // expend clusters from seeds
  while (!localStack.empty()) {
    int i = localStack.back();
    auto& followers = points_.followers[i];
    localStack.pop_back();

    // loop over followers
    for( int j : followers){
      // pass id from i to a i's follower
      points_.clusterIndex[j] = points_.clusterIndex[i];
      // push this follower to localStack
      localStack.push_back(j);
    }
  }
}

inline float CLUEAlgo::distance(int i, int j) const {

  // 2-d distance on the layer
  if(points_.layer[i] == points_.layer[j] ) {
    const float dx = points_.x[i] - points_.x[j];
    const float dy = points_.y[i] - points_.y[j];
    return std::sqrt(dx * dx + dy * dy);
  } else {
    return std::numeric_limits<float>::max();
  }

}
