#include <unordered_map>
#include <string>
#include <chrono>

#include <cauchy/execution/iexecution_handler.h>
#include <cauchy/execution/pending_order.h>


class SimulatedExecutionHandler : public IExecutionHandler {
public: 
  SimulatedExecutionHandler();

  void handleOrder(OrderEvent oe) override;

  std::optional<FillEvent> onMarket(MarketEvent market) override;

private:
  std::unordered_map<std::string, PendingOrder> pending_orders_;
};
