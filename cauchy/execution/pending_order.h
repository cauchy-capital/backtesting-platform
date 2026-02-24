#include <chrono>

#include <cauchy/core/order_side.h>

struct PendingOrder {
  size_t quantity;
  OrderSide order_side;
  std::chrono::sys_time<std::chrono::milliseconds> last_order_ts;
};
