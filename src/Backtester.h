#pragma once

#include <memory>

#include "Portfolio.h"
#include "IO/IDataFeed.h"
#include "Strategy/IStrategy.h"
#include "Decision.h"

class Backtester {
  public:
    Backtester(double cash);

    void run_backtest();

    double results();

    void set_feed(std::unique_ptr<IDataFeed> feed);

    void set_strat(std::unique_ptr<IStrategy> strat);

  private:
    Portfolio portfolio_;
    std::unique_ptr<IDataFeed> curr_feed_;
    std::unique_ptr<IStrategy> curr_strat_;
    Quote last_quote_;
};

