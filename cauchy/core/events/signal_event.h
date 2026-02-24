#pragma once

#include <cauchy/core/events/event.h>
#include <cauchy/core/direction.h>
#include <cauchy/market/bar.h>


class SignalEvent : public Event {
public:
  SignalEvent(std::string ticker, Direction direction, Bar bar);

  std::string ticker;
  Direction direction;
  Bar bar;
};
