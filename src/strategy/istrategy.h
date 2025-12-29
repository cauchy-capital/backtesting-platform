#pragma once

#include "../market/decision.h"
#include "../market/quote.h"
#include "../market/bar.h"

// Interface for strategy. traders should implement onBar()
class IStrategy {
public:
  // ignore the irony of it taking a quote when the name is onBar. WILL FIX L8R!
  virtual Decision onBar(Bar) = 0;
};
