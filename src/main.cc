#include <iostream>
#include <fstream>
#include <string>

#include "CLUEAlgo.h"
#include "CLUEAlgoGPU.h"

void testRun(int n, float* x, float* y, int* layer, float* weight, float dc, float d0, float rhoc, bool useGPU=false, bool verbose=false) {
  if (useGPU) {
    CLUEAlgoGPU clueAlgo(dc, d0, rhoc);
    clueAlgo.setPoints(n,x,y,layer,weight);
    clueAlgo.makeClusters();
    if(verbose) clueAlgo.verboseResults();
  } else {
    CLUEAlgo clueAlgo(dc, d0, rhoc);
    clueAlgo.setPoints(n,x,y,layer,weight);
    clueAlgo.makeClusters();
    if(verbose) clueAlgo.verboseResults();
  }
}

int main(void) {

  //////////////////////////////
  // read toy data from csv file
  //////////////////////////////
  std::vector<float> x;
  std::vector<float> y;
  std::vector<int> layer;
  std::vector<float> weight;
  // open csv file
  std::ifstream file("data/input.csv");
  std::string value = "";
	// Iterate through each line and split the content using delimeter
	while (getline(file, value, ',')) {
    x.push_back(std::stof(value)) ;
    getline(file, value, ','); y.push_back(std::stof(value));
    getline(file, value, ','); layer.push_back(std::stoi(value));
    getline(file, value); weight.push_back(std::stof(value));
	}
	file.close();

  //////////////////////////////
  // test run
  //////////////////////////////
  float dc=5.0, d0=10, rhoc=2;
  bool useGPU=false, verbose=false;
  testRun(x.size(), &x[0],&y[0],&layer[0],&weight[0], dc,d0,rhoc, useGPU,verbose);
  return 0;
}