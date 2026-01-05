#pragma once

#include <cauchy/core/events/event_type.h>

class Event {
public:
  virtual ~Event() = default;
  explicit Event(EventType type);
  EventType type;
};

