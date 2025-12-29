#include <iostream>

#include "io/csv_data_feed.h"
#include "backtest/backtester.h"
#include "strategy/buy_everything_strategy.h"

int main(int argc, char *argv[]) {
  if (argc > 3) {
    std::cerr << "Usage: " << argv[0] << " <csv_file> <ticker_name>" << std::endl;
    return 1;
  }

  std::string filename = argv[1];
  std::string ticker = argv[2];

  // TODO: use quotes for backtester
  double starting_cash = 100;
  Backtester b(starting_cash);

  b.set_feed(std::make_unique<CsvDataFeed>(filename, ticker));
  b.set_strat(std::make_unique<BuyEverythingStrategy>(ticker));

  b.run_backtest();
  double pnl = b.results();

  std::cout << "==== BACKTEST DONE ====\n" << "PNL: " << pnl << std::endl;

  return 0;
}
