#pragma once

#include <memory>

#include <cauchy/portfolio/portfolio.h>
#include <cauchy/io/idata_feed.h>
#include <cauchy/strategy/istrategy.h>
#include <cauchy/execution/iexecution_handler.h>

/*
 * Backtester orchestrates an event-driven simulation of a trading strategy over
 * historical (or replayed) market data.
 *
 * It wires together a data feed, strategy, and execution handler. It reads
 * Quotes from the data feed, converting them into bars, and then into 
 * marketEvents. the events are then processed as follows:
 *
 *  MARKET -> (ExecutionHandler::onMarket -> FILL) + (Strategy::onBar -> SIGNAL)
 *  SIGNAL -> (Portfolio::handle_signal -> ORDER) [if actionable]
 *  ORDER  -> (ExecutionHandler::handleOrder)
 *  FILL   -> (Portfolio::on_fill)
 * 
 * Typical usage:
 *  - Construct with initial cash (creates/initializes the Portfolio).
 *  - Inject dependencies via set_feed(), set_strat(), set_execution_handler().
 *  - Call run_backtest() to process the full dataset.
 *  - Call results() to retrieve final performance (currently P&L).
 *
 * Internally, last_bar_ tracks the most recently processed market bar to support
 * the final calculation of unrealized PNL.
 */

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

