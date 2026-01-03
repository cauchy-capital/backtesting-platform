#pragma once

#include <string> 

#include "event.h"
#include "order_event.h"

class FillEvent : public Event {
public: 
  FillEvent(std::string ticker, std::size_t quantity, double fill_price, OrderSide order_side);

  std::string ticker;
  std::size_t quantity;
  double fill_price;
  OrderSide order_side;
};
