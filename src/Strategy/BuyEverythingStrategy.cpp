#include "BuyEverythingStrategy.h"


BuyEverythingStrategy::BuyEverythingStrategy(std::string ticker_to_buy)
  : ticker_to_buy_(ticker_to_buy) {}

Decision BuyEverythingStrategy::onBar(Quote quote) {
  double price = (quote.bid + quote.ask) / 2;

  if (bought_tickers_.find(quote.ticker) ==  bought_tickers_.end()) {
    // buy 
    Decision buy{quote.ticker, 10, price};
    bought_tickers_[quote.ticker] = {10, price};

    return buy;
  }

  if (bought_tickers_[quote.ticker].second < price) {
    //sell
    Decision sell{quote.ticker, -10, price};
    bought_tickers_.erase(quote.ticker);
    
    return sell;
  }

  // already own, dont do anything
  Decision nothing{quote.ticker, 0, price};
  return nothing;
}
