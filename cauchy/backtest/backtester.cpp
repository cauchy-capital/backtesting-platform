#include <iostream>
#include <queue>
#include <optional>

#include <cauchy/backtest/backtester.h>
#include <cauchy/market/bar_builder.h>
#include <cauchy/core/events/market_event.h>
#include <cauchy/core/events/event.h>
#include <cauchy/core/events/signal_event.h>


Backtester::Backtester(double cash) : portfolio_(cash) {}

void Backtester::run_backtest() {
  if (!curr_feed_) {
    std::cerr << "Backtester ERROR: No feed set.\n";
    return;
  }

  // parse CSV into bars.
  std::chrono::milliseconds interval{3600000};
  BarBuilder bar_builder{interval};

  std::vector<Quote> quotes = curr_feed_->loadData();
  std::vector<Bar> bars = bar_builder.buildBars(quotes);

  // create market events out of bars.
  std::vector<MarketEvent> market_events;
  for (Bar bar : bars) {
    MarketEvent m(bar);
    market_events.push_back(m);
  }

  std::cerr << "quotes=" << quotes.size() << "\n";
  std::cerr << "bars=" << bars.size() << "\n";
  std::cerr << "market_events=" << market_events.size() << "\n";
  //run backtest
  Dispatcher dispatcher{*this};
  for (MarketEvent market_event : market_events) {
    event_queue_.push(market_event);


    while (!event_queue_.empty()) {
      auto e = std::move(event_queue_.front());
      event_queue_.pop();

      std::visit(dispatcher,e);
    }
  }
  std::cout << "last BAR: PRICE=" << last_bar_.close << std::endl;
}

double Backtester::results() {
  // need ticker:price
  std::cout << std::endl;
  std::string ticker = last_bar_.ticker;
  return portfolio_.get_pnl({{ticker, last_bar_.low}});
}

void Backtester::set_feed(std::unique_ptr<IDataFeed> feed) {
  curr_feed_ = std::move(feed);
}

void Backtester::set_strat(std::unique_ptr<IStrategy> strat) {
  curr_strat_ = std::move(strat);
}

void Backtester::set_execution_handler(std::unique_ptr<IExecutionHandler> exec_handler) {
  curr_execution_handler_ = std::move(exec_handler);
}


