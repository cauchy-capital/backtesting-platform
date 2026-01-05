#pragma once

#include <cauchy/core/events/event.h>
#include <cauchy/market/bar.h>

class MarketEvent : public Event {
public: 
  MarketEvent(Bar bar);

  Bar bar;
};
