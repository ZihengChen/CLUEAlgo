#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "CLUEAlgo.h"
#include "CLUEAlgoGPU.h"

void mainRun( std::string inputFileName, std::string outputFileName,
              float dc, float deltao, float deltac, float rhoc, 
              bool useGPU, int repeats, bool verbose  ) {

  //////////////////////////////
  // read toy data from csv file
  //////////////////////////////
  std::cout << "Start to load input points" << std::endl;
  std::vector<float> x;
  std::vector<float> y;
  std::vector<int> layer;
  std::vector<float> weight;

  // make dummy layers
  for (int l=0; l<NLAYERS; l++){
    // open csv file
    std::ifstream iFile(inputFileName);
    std::string value = "";
    // Iterate through each line and split the content using delimeter
    while (getline(iFile, value, ',')) {
      x.push_back(std::stof(value)) ;
      getline(iFile, value, ','); y.push_back(std::stof(value));
      getline(iFile, value, ','); layer.push_back(std::stoi(value) + l);
      getline(iFile, value); weight.push_back(std::stof(value));
    }
    iFile.close();
  }
  std::cout << "Finished loading input points" << std::endl;

  //////////////////////////////
  // run CLUE algorithm
  //////////////////////////////   
  std::cout << "Start to run CLUE algorithm" << std::endl;      
  if (useGPU) {
    CLUEAlgoGPU clueAlgo(dc, deltao, deltac, rhoc);
    for (int r = 0; r<repeats; r++){
      clueAlgo.setPoints(x.size(), &x[0],&y[0],&layer[0],&weight[0]);
      // measure excution time of makeClusters
      auto start = std::chrono::high_resolution_clock::now();
      clueAlgo.makeClusters();
      auto finish = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> elapsed = finish - start;
      std::cout << "Elapsed time: " << elapsed.count() *1000 << " ms\n";
    }
    // output result to outputFileName. -1 means all points. 
    if (verbose) clueAlgo.verboseResults(outputFileName, -1);
  } else {
    CLUEAlgo clueAlgo(dc, deltao, deltac, rhoc);
    for (int r = 0; r<repeats; r++){
      clueAlgo.setPoints(x.size(), &x[0],&y[0],&layer[0],&weight[0]);
      // measure excution time of makeClusters
      auto start = std::chrono::high_resolution_clock::now();
      clueAlgo.makeClusters();
      auto finish = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> elapsed = finish - start;
      std::cout << "Elapsed time: " << elapsed.count() *1000 << " ms\n";
    }
    // output result to outputFileName. -1 means all points. 
    if (verbose) clueAlgo.verboseResults(outputFileName, -1);
  }
  std::cout << "Finished running CLUE algorithm" << std::endl; 
  std::cout << std::endl; 
} // end of testRun()




int main(int argc, char *argv[]) {

  //////////////////////////////
  // MARK -- set algorithm parameters
  //////////////////////////////
  float dc=20, deltao=20, deltac=20, rhoc=80;
  bool useGPU=false;
  int totalNumberOfEvent = 10;
  bool verbose=false;

  if (argc == 9) {
    dc = std::stof(argv[2]);
    deltao = std::stof(argv[3]);
    deltac = std::stof(argv[4]);
    rhoc = std::stof(argv[5]);
    useGPU = (std::stoi(argv[6])==1)? true:false;
    totalNumberOfEvent = std::stoi(argv[7]);
    verbose = (std::stoi(argv[8])==1)? true:false;
  } else {
    std::cout << "bin/main [fileName] [dc] [deltao] [deltac] [rhoc] [useGPU] [totalNumberOfEvent] [verbose]" << std::endl;
    return 0;
  }


  //////////////////////////////
  // MARK -- set input and output files
  //////////////////////////////
  std::string underscore="_", suffix = ".csv";

  std::string inputFileName = "data/input/";
  inputFileName.append(argv[1]);
  inputFileName.append(suffix);
  std::cout << "input file " << inputFileName << std::endl;


  std::string outputFileName = "data/output/";
  outputFileName.append(argv[1]);
  outputFileName.append(underscore);
  outputFileName.append(std::to_string(int(dc)));
  outputFileName.append(underscore);
  outputFileName.append(std::to_string(int(deltao)));
  outputFileName.append(underscore);
  outputFileName.append(std::to_string(int(deltac)));
  outputFileName.append(underscore);
  outputFileName.append(std::to_string(int(rhoc)));
  outputFileName.append(suffix);
  std::cout << "output file " << outputFileName << std::endl;


  //////////////////////////////
  // MARK -- test run
  //////////////////////////////
  mainRun(inputFileName, outputFileName,
          dc, deltao, deltac, rhoc, 
          useGPU, totalNumberOfEvent,verbose);

  return 0;
}