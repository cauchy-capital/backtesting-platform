#include <unordered_map>
#include <string>
#include <chrono>

#include "iexecution_handler.h"

struct PendingOrder {
  size_t quantity;
  OrderSide order_side;
  std::chrono::sys_time<std::chrono::milliseconds> last_order_ts;
};

class SimulatedExecutionHandler : public IExecutionHandler {
public: 
  SimulatedExecutionHandler();

  void handleOrder(OrderEvent oe) override;

  FillEvent onMarket(MarketEvent market) override;

private:
  std::unordered_map<std::string, PendingOrder> pending_orders_;
};
