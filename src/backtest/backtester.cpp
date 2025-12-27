#include <iostream>

#include "backtester.h"


Backtester::Backtester(double cash) : portfolio_(cash) {}

void Backtester::run_backtest() {
  if (!curr_feed_) {
    std::cerr << "Backtester ERROR: No feed set.\n";
    return;
  }

  std::vector<Quote> quotes = curr_feed_->loadData();

  //run backtest
  for (Quote& quote : quotes) {
    double price = (quote.bid + quote.ask) / 2;
    std::cout << "CURR QUOTE: PRICE=" << price << std::endl;
    Decision decision = curr_strat_->onBar(quote);
    portfolio_.record(decision);

    last_quote_ = quote;
  }
}

double Backtester::results() {
  // need ticker:price
  double price = (last_quote_.bid + last_quote_.ask) / 2;
  std::string ticker = last_quote_.ticker;
  return portfolio_.get_pnl({{ticker, price}});
}

void Backtester::set_feed(std::unique_ptr<IDataFeed> feed) {
  curr_feed_ = std::move(feed);
}

void Backtester::set_strat(std::unique_ptr<IStrategy> strat) {
  curr_strat_ = std::move(strat);
}


