#include "order_event.h"

OrderEvent::OrderEvent(std::string ticker, std::size_t mkt_quantity, 
                       OrderSide order_side, Bar bar) 
  : Event("ORDER_EVENT"), ticker(ticker), 
    mkt_quantity(mkt_quantity), order_side(order_side), 
    bar(bar) {}


