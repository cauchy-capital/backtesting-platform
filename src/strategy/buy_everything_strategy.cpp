#include "buy_everything_strategy.h"


BuyEverythingStrategy::BuyEverythingStrategy(std::string ticker_to_buy)
  : ticker_to_buy_(ticker_to_buy) {}

Decision BuyEverythingStrategy::onBar(Bar bar) {
  double price = bar.low;

  if (bought_tickers_.find(bar.ticker) ==  bought_tickers_.end()) {
    // buy 
    Decision buy{bar.ticker, 10, price};
    bought_tickers_[bar.ticker] = {10, price};

    return buy;
  }

  if (bought_tickers_[bar.ticker].second < price) {
    //sell
    Decision sell{bar.ticker, -10, price};
    bought_tickers_.erase(bar.ticker);
    
    return sell;
  }

  // already own, dont do anything
  Decision nothing{bar.ticker, 0, price};
  return nothing;
}
