#include "CsvDataFeed.h"
#include <fstream>
#include <iostream>


CsvDataFeed::CsvDataFeed(std::string filepath) 
  : filepath_(filepath) {}

std::vector<Quote> CsvDataFeed::loadData() {
  std::vector<Quote> bars;

  std::ifstream file(filepath_);

  if (!file.is_open()) {
    std::cerr << "Error: file failed to open!" << std::endl;
    return bars;
  }

  std::string line;
  while(getline(file, line)) {
    std::cout << line << std::endl;

    //TODO: parse line into quotes
  }

  if (file.eof()) {
    std::cout << "reached end of file" << std::endl;
  }
  else {
    std::cerr << "Error: File reading failed!" << std::endl;
  }

  file.close();

  return bars;
}
