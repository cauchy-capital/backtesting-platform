#pragma once

#include "../market/decision.h"
#include "../market/bar.h"

// Interface for strategy. traders should implement onBar()
class IStrategy {
public:
  virtual Decision onBar(Bar) = 0;
};
