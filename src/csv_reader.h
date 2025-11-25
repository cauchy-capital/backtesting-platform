#ifndef CAUCHY_CSV_READER_H
#define CAUCHY_CSV_READER_H

#include <ctime>
#include <string>
#include <vector>

//types use UpperCamelCase 
struct Line {
  time_t timestamp;
  float ask;
  float bid;
  float ask_volume;
  float bid_volume;
};

// UpperCamelCase, Inputs by const reference, outputs by pointer (google style)
bool ReadCsv(const std::string& file_name, std::vector<Line>* result);

#endif //CAUCHY_CSV_READER_H
