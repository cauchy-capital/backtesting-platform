#include "signal_event.h"

SignalEvent::SignalEvent(std::string ticker, Call call, Bar bar) 
  : Event("SIGNAL"), ticker(ticker), call(call), bar(bar) {}
