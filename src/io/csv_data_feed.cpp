#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>

#include "csv_data_feed.h"

CsvDataFeed::CsvDataFeed(std::string filepath, std::string ticker) 
  : filepath_(filepath), ticker_(ticker) {}

std::vector<Quote> CsvDataFeed::loadData() {
  std::vector<Quote> bars;

  std::ifstream file(filepath_);

  if (!file.is_open()) {
    std::cerr << "Error: file failed to open!" << std::endl;
    return bars;
  }

  // loop through csv contents
  std::string line;
  bool first_line = true;
  while(getline(file, line)) {
    if (first_line) {
      first_line = false;
      continue;
    }

    if (line.empty()) {
      continue;
    }

    auto fields = splitTab(line);

    //expecting 5 columns
    if (fields.size() != 5) {
      std::cerr << "Malformed line: " << line << std::endl;
      continue;
    }

    // create quote
    Quote q;
    q.ticker = ticker_;
    q.ts = parseTimeStamp(fields[0]);
    q.ask = std::stod(fields[1]);
    q.bid = std::stod(fields[2]);
    q.askVol = std::stod(fields[3]);
    q.bidVol = std::stod(fields[4]);

    bars.push_back(q);
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

std::vector<std::string> CsvDataFeed::splitTab(const std::string& line) {
  std::vector<std::string> fields;
  std::stringstream ss(line);
  std::string field;

  while (std::getline(ss, field, ',')) {
    fields.push_back(field);
  }

  return fields;
}

std::chrono::sys_time<std::chrono::milliseconds> CsvDataFeed::parseTimeStamp(const std::string& s) {
  // Example input:
  // 17.11.2025 01:30:00.786 GMT-0000

  // take only: 17.11.2025 01:30:00.786
  std::string datetime = s.substr(0,23);
  std::cout << "parsing: " << datetime << std::endl;


  std::istringstream stream(datetime);
  std::chrono::sys_time<std::chrono::milliseconds> time;
  std::chrono::from_stream(stream, "%d.%m.%Y %H:%M:%S", time);

  if (stream.fail()) {
    throw std::runtime_error("Failed to parse timestamp: " + s);
  }
  std::cout << "parsed: " << time << std::endl;

  return time;
}
