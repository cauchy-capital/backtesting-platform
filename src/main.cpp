#include <iostream>
#include <vector>

#include "csv_reader.h"

int main(int argc, char *argv[]) {
  if (argc > 2) {
    std::cerr << "Usage: " << argv[0] << " <csv_file>" << std::endl;
    return 1;
  }

  std::vector<Line> parsed;

  bool success = ReadCsv(argv[1], &parsed);

  return success;
}
