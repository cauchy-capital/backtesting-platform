#pragma once

#include <cauchy/core/events/event_type.h>

/*
 * Event is the base class for all events that drive the backtest. It contains
 * an EventType, defining what type the inhereting event is. 
 *
 * To create a new event, inherit from this class, define a new EventType, 
 * and route it within the backtester.
 */

class Event {
public:
  virtual ~Event() = default;
  explicit Event(EventType type);
  EventType type;
};

