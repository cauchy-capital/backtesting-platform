#include <iostream>
#include <vector>

#include "market/bar.h"
#include "io/csv_data_feed.h"
#include "backtest/backtester.h"
#include "strategy/buy_everything_strategy.h"

int main(int argc, char *argv[]) {
  if (argc > 2) {
    std::cerr << "Usage: " << argv[0] << " <csv_file>" << std::endl;
    return 1;
  }

  std::string filename = argv[1];
  std::string ticker = "0005.HKHKD";

  // TODO: use quotes for backtester
  Backtester b(100);

  b.set_feed(std::make_unique<CsvDataFeed>(filename));
  b.set_strat(std::make_unique<BuyEverythingStrategy>(ticker));

  // TODO: print total profit/loss!
  b.run_backtest();
  double pnl = b.results();

  std::cout << "==== BACKTEST DONE ====\n" << "PNL: " << pnl << std::endl;

  return 0;
}
