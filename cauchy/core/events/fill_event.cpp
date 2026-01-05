#include <cauchy/core/events/fill_event.h>

FillEvent::FillEvent(std::string ticker, std::size_t quantity, 
                     double fill_price, OrderSide order_side) 
  : Event(FILL), ticker(ticker), quantity(quantity), 
    fill_price(fill_price), order_side(order_side) {}

