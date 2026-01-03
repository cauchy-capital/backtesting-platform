#include "iexecution_handler.h"

class SimulatedExecutionHandler : public IExecutionHandler {
public: 
  SimulatedExecutionHandler();

  void handleOrder(OrderEvent order) override;

  FillEvent onMarket(MarketEvent market) override;
};
