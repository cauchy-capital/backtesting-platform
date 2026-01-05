#pragma once

#include <string> 

#include <cauchy/core/events/event.h>
#include <cauchy/core/order_side.h>

class FillEvent : public Event {
public: 
  FillEvent(std::string ticker, std::size_t quantity, double fill_price, OrderSide order_side);

  std::string ticker;
  std::size_t quantity;
  double fill_price;
  OrderSide order_side;
};
