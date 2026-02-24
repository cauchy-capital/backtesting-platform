#include <cauchy/strategy/buy_everything_strategy.h>


BuyEverythingStrategy::BuyEverythingStrategy(std::string ticker_to_buy)
  : ticker_to_buy_(ticker_to_buy) {}

SignalEvent BuyEverythingStrategy::onBar(const MarketEvent& e) {
  Bar bar = e.bar;
  double price = bar.low;

  if (bought_tickers_.find(bar.ticker) ==  bought_tickers_.end()) {
    // buy 
    SignalEvent buy = SignalEvent(bar.ticker, LONG, bar);
    bought_tickers_[bar.ticker] = {10, price};

    return buy;
  }

  if (bought_tickers_[bar.ticker].second < price) {
    //sell
    SignalEvent sell = SignalEvent(bar.ticker, SHORT, bar);
    bought_tickers_.erase(bar.ticker);
    
    return sell;
  }

  // already own, dont do anything
  SignalEvent nothing = SignalEvent(bar.ticker, NOACT, bar);
  return nothing;
}
