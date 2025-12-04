#include <iostream>
#include <vector>

#include "Bar.h"
#include "IO/CsvDataFeed.h"

int main(int argc, char *argv[]) {
  if (argc > 2) {
    std::cerr << "Usage: " << argv[0] << " <csv_file>" << std::endl;
    return 1;
  }

  std::string filename = argv[1];

  // read data
  CsvDataFeed data_feed(filename);
  std::vector<Quote> quotes = data_feed.loadData();

  // TODO: use quotes for backtester

  // TODO: print total profit/loss!

  return 0;
}
