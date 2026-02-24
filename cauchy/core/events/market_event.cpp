#include <cauchy/core/events/market_event.h>

MarketEvent::MarketEvent(Bar bar) 
  : Event(MARKET), bar(bar) {}


