#pragma once

enum EventType { MARKET, SIGNAL, ORDER, FILL };

class Event {
public:
  virtual ~Event() = default;
  explicit Event(EventType type);
  EventType type;
};

