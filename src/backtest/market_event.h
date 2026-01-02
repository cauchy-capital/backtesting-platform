#pragma once

#include "event.h"
#include "../market/bar.h"

class MarketEvent : public Event {
public: 
  MarketEvent(Bar bar);

  Bar bar;
};
