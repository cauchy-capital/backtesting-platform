#include "simulated_execution_handler.h"

SimulatedExecutionHandler::SimulatedExecutionHandler()  {}

void SimulatedExecutionHandler::handleOrder(OrderEvent order) {

}

FillEvent SimulatedExecutionHandler::onMarket(MarketEvent market) {
  return FillEvent{};
}

