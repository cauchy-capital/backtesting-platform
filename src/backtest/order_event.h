#pragma once

#include "event.h"
#include "../market/bar.h"

enum OrderSide { BUY, SELL };

class OrderEvent : public Event {
public: 
  OrderEvent(std::string ticker, std::size_t mkt_quantity, OrderSide order_side, Bar bar);

  std::string ticker;
  std::size_t mkt_quantity;
  OrderSide order_side;
  Bar bar;
};
