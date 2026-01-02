#include "buy_everything_strategy.h"


BuyEverythingStrategy::BuyEverythingStrategy(std::string ticker_to_buy)
  : ticker_to_buy_(ticker_to_buy) {}

SignalEvent BuyEverythingStrategy::onBar(const MarketEvent& e) {
  Bar bar = e.bar;
  double price = bar.low;

  if (bought_tickers_.find(bar.ticker) ==  bought_tickers_.end()) {
    // buy 
    //Decision buy{bar.ticker, 10, price};
    SignalEvent buy = SignalEvent();
    bought_tickers_[bar.ticker] = {10, price};

    return buy;
  }

  if (bought_tickers_[bar.ticker].second < price) {
    //sell
    SignalEvent sell = SignalEvent();
    //Decision sell{bar.ticker, -10, price};
    bought_tickers_.erase(bar.ticker);
    
    return sell;
  }

  // already own, dont do anything
  SignalEvent nothing = SignalEvent();
  //Decision nothing{bar.ticker, 0, price};
  return nothing;
}
