#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "CLUEAlgo.h"
#include "CLUEAlgoGPU.h"

void testRun( int n, float* x, float* y, int* layer, float* weight, 
              float dc, float d0, float deltac, float rhoc, 
              bool useGPU, bool verbose ) {
  if (useGPU) {
    CLUEAlgoGPU clueAlgo(dc, d0, deltac, rhoc);
    clueAlgo.setPoints(n,x,y,layer,weight);
    // measure excution time of makeClusters
    auto start = std::chrono::high_resolution_clock::now();
    clueAlgo.makeClusters();
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cerr << "Elapsed time: " << elapsed.count() *1000 << " ms\n";
    if(verbose) clueAlgo.verboseResults();
  } else {
    CLUEAlgo clueAlgo(dc, d0, deltac, rhoc);
    clueAlgo.setPoints(n,x,y,layer,weight);
    // measure excution time of makeClusters
    auto start = std::chrono::high_resolution_clock::now();
    clueAlgo.makeClusters();
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cerr << "Elapsed time: " << elapsed.count() *1000 << " ms\n";
    if(verbose) clueAlgo.verboseResults();
  }
}

int main(int argc, char *argv[]) {
  //////////////////////////////
  // set algorithm parameters
  //////////////////////////////

  float dc=20, d0=20, deltac=20, rhoc=80;
  bool useGPU=false;

  std::string fileName = "data/input/";
  if (argc == 7) {
    fileName.append(argv[1]);
    dc = std::stof(argv[2]);
    d0 = std::stof(argv[3]);
    deltac = std::stof(argv[4]);
    rhoc = std::stof(argv[5]);
    if (std::stof(argv[6])==1) useGPU = true;

  } else {
    std::cout << "bin/main [dc] [d0] [deltac] [rhoc]" << std::endl;
    return 0;
  }

  if (deltac > d0) {
    std::cout << "must satisfy deltac<=d0 " << std::endl;
    return 0;
  }

  //////////////////////////////
  // read toy data from csv file
  //////////////////////////////
  std::cout << "Start Loading input points" << std::endl;
  std::vector<float> x;
  std::vector<float> y;
  std::vector<int> layer;
  std::vector<float> weight;

  for (int l=0; l<NLAYERS; l++){
    // open csv file

    std::ifstream file(fileName);
    std::string value = "";
    // Iterate through each line and split the content using delimeter
    while (getline(file, value, ',')) {
      x.push_back(std::stof(value)) ;
      getline(file, value, ','); y.push_back(std::stof(value));
      getline(file, value, ','); layer.push_back(std::stoi(value) + l);
      getline(file, value); weight.push_back(std::stof(value));
    }
    file.close();
  }


  //////////////////////////////
  // test run
  //////////////////////////////
  std::cout << "Start Running CLUE" << std::endl;
  bool verbose=false;
  int totoalNumberOfEvents = 10;
  for (int nev = 0; nev<totoalNumberOfEvents; nev++){
    testRun(x.size(), &x[0],&y[0],&layer[0],&weight[0], 
            dc,d0,deltac,rhoc, 
            useGPU,verbose);
  }

  return 0;
}