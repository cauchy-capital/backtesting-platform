#include <iostream>
#include <queue>
#include <variant>

#include "backtester.h"
#include "../market/bar_builder.h"
#include "market_event.h"
#include "event.h"
#include "signal_event.h"


Backtester::Backtester(double cash) : portfolio_(cash) {}

void Backtester::run_backtest() {
  if (!curr_feed_) {
    std::cerr << "Backtester ERROR: No feed set.\n";
    return;
  }

  // parse CSV into bars.
  std::chrono::milliseconds interval{1000};
  BarBuilder bar_builder{interval};

  std::vector<Quote> quotes = curr_feed_->loadData();
  std::vector<Bar> bars = bar_builder.buildBars(quotes);

  // create market events out of bars.
  std::vector<MarketEvent> market_events;
  for (Bar bar : bars) {
    MarketEvent m(bar);
    market_events.push_back(m);
  }

  //run backtest
  std::queue<std::unique_ptr<Event>> event_queue;
  while (market_events.size() > 0) {
    auto me = std::make_unique<MarketEvent>(std::move(market_events.front()));
    market_events.erase(market_events.begin());
    event_queue.push(std::move(me));


    while (!event_queue.empty()) {
      auto e = std::move(event_queue.front());
      event_queue.pop();

      // find the correct handler for event
      // TODO: Use enums
      if (e->type == MARKET) {
        auto* me = dynamic_cast<MarketEvent*>(e.get());
        if (me) {
          SignalEvent se = curr_strat_->onBar(*me);
          auto cse = std::make_unique<SignalEvent>(std::move(se));
          event_queue.push(std::move(cse));
        }
      } else if (e->type == SIGNAL) {
        auto* se = dynamic_cast<SignalEvent*>(e.get());
        if (se) {
          OrderEvent oe = portfolio_.handle_signal(*se);
          auto coe = std::make_unique<OrderEvent>(std::move(oe));
          event_queue.push(std::move(coe));
        }
      }
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


