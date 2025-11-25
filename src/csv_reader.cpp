#include "csv_reader.h"

#include <fstream>
#include <iostream>

bool ReadCsv(const std::string& file_name, std::vector<Line>* result) { 
  std::ifstream file(file_name);

  if (!file.is_open()) {
    std::cerr << "Error: file failed to open!" << std::endl;
    return false;
  }

  std::string line;
  while(getline(file, line)) {
    std::cout << line << std::endl;

    //TODO: parse line into Line, add to result vector.
  }

  if (file.eof()) {
    std::cout << "reached end of file" << std::endl;
  }
  else {
    std::cerr << "Error: File reading failed!" << std::endl;
  }

  file.close();
  return true;
}
