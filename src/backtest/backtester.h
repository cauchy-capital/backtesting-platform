#pragma once

#include <memory>

#include "../portfolio/portfolio.h"
#include "../io/idata_feed.h"
#include "../strategy/istrategy.h"
#include "../execution/iexecution_handler.h"

class Backtester {
  public:
    Backtester(double cash);

    void run_backtest();

    double results();

    void set_feed(std::unique_ptr<IDataFeed> feed);

    void set_strat(std::unique_ptr<IStrategy> strat);

    void set_execution_handler(std::unique_ptr<IExecutionHandler> exec_handler);

  private:
    Portfolio portfolio_;
    std::unique_ptr<IDataFeed> curr_feed_;
    std::unique_ptr<IStrategy> curr_strat_;
    std::unique_ptr<IExecutionHandler> curr_execution_handler_;
    Bar last_bar_;
};

