#pragma once

#include <cauchy/core/events/event.h>
#include <cauchy/core/order_side.h>
#include <cauchy/market/bar.h>


class OrderEvent : public Event {
public: 
  OrderEvent(std::string ticker, std::size_t mkt_quantity, OrderSide order_side, Bar bar);

  std::string ticker;
  std::size_t mkt_quantity;
  OrderSide order_side;
  Bar bar;
};
