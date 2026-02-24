#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>

#include <cauchy/io/csv_data_feed.h>

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

std::chrono::sys_time<std::chrono::milliseconds>
CsvDataFeed::parseTimeStamp(const std::string& s) {
  // Example input:
  // 17.11.2025 01:30:00.786 GMT-0000

  // Take only: "17.11.2025 01:30:00.786" (length 23)
  if (s.size() < 19) {
    throw std::runtime_error("Timestamp too short: " + s);
  }

  // Parse the base datetime part (no millis)
  // "17.11.2025 01:30:00" -> length 19
  const std::string base = s.substr(0, 19);

  std::tm tm{};
  tm.tm_isdst = -1; // let conversion decide; for UTC this is ignored
  std::istringstream iss(base);
  iss >> std::get_time(&tm, "%d.%m.%Y %H:%M:%S");
  if (iss.fail()) {
    throw std::runtime_error("Failed to parse timestamp (base): " + s);
  }

  // Parse milliseconds if present: ".786"
  int millis = 0;
  if (s.size() >= 23 && s[19] == '.') {
    const std::string ms_str = s.substr(20, 3);
    if (ms_str.size() != 3 ||
        ms_str[0] < '0' || ms_str[0] > '9' ||
        ms_str[1] < '0' || ms_str[1] > '9' ||
        ms_str[2] < '0' || ms_str[2] > '9') {
      throw std::runtime_error("Failed to parse timestamp (milliseconds): " + s);
    }
    millis = (ms_str[0] - '0') * 100 + (ms_str[1] - '0') * 10 + (ms_str[2] - '0');
  }

  // Convert to time_t in UTC.
  // On Linux/WSL, timegm() is available (GNU extension).
  std::time_t tt = timegm(&tm);
  if (tt == -1) {
    throw std::runtime_error("Failed to convert timestamp to UTC time_t: " + s);
  }

  auto tp_sec = std::chrono::system_clock::from_time_t(tt);
  auto tp_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(tp_sec)
             + std::chrono::milliseconds(millis);

  return std::chrono::sys_time<std::chrono::milliseconds>(tp_ms);
}

