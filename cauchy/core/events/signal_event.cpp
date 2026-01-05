#include <cauchy/core/events/signal_event.h>

SignalEvent::SignalEvent(std::string ticker, Direction direction, Bar bar) 
  : Event(SIGNAL), ticker(ticker), direction(direction), bar(bar) {}
