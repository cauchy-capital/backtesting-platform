#pragma once

#include "event.h"
#include "../market/bar.h"

enum Direction { LONG, SHORT, EXIT, NOACT };

class SignalEvent : public Event {
public:
  SignalEvent(std::string ticker, Direction direction, Bar bar);

  std::string ticker;
  Direction direction;
  Bar bar;
};
