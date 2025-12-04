#include "BuyEverythingStrategy.h"


BuyEverythingStrategy::BuyEverythingStrategy(std::string ticker_to_buy)
  : ticker_to_buy_(ticker_to_buy) {}

Decision BuyEverythingStrategy::onBar(Quote quote) {
  if (bought_tickers_.find(quote.ticker) ==  bought_tickers_.end()) {
    // buy 
    Decision buy{10, quote.ticker};
    bought_tickers_.insert({quote.ticker, 10});

    return buy;
  }

  // already own, dont do anything
  Decision nothing{0, quote.ticker};
  return nothing;
}
