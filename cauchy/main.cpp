#include <iostream>
#include <iomanip>

#include <cauchy/io/csv_data_feed.h>
#include <cauchy/backtest/backtester.h>
#include <cauchy/strategy/buy_everything_strategy.h>
#include <cauchy/strategy/sma_cross_strategy.h>
#include <cauchy/execution/simulated_execution_handler.h>

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <csv_file> <ticker_name>" << std::endl;
    return 1;
  }

  std::string filename = argv[1];
  std::string ticker = argv[2];

  // TODO: use quotes for backtester
  double starting_cash = 100;
  Backtester b(starting_cash);

  b.set_feed(std::make_unique<CsvDataFeed>(filename, ticker));
  b.set_strat(std::make_unique<SmaCrossStrategy>(ticker));
  b.set_execution_handler(std::make_unique<SimulatedExecutionHandler>());

  b.run_backtest();
  double pnl = b.results();

  // get rid of floating point noise
  double eps = 1e-5;
  if (std::abs(pnl) < eps) {
    pnl = 0.0;
  }

  std::cout << "==== BACKTEST DONE ====\n" << "PNL: " << std::fixed <<
    std::setprecision(2) <<std::fixed <<
    std::setprecision(2) << pnl << std::endl;

  return 0;
}

