#include "bar_builder.h"

BarBuilder::BarBuilder(std::chrono::milliseconds ms_interval) : ms_interval_(ms_interval) {}

std::vector<Bar> BarBuilder::buildBars(std::vector<Quote>& quotes) {
  std::vector<Bar> bars;

  if (quotes.size() == 0) {
    return bars;
  }


  std::string ticker = quotes[0].ticker;
  double first_price = this->calc_price(quotes[0]);
  double open = first_price;
  double high = first_price;
  double low = first_price;
  double close;

  std::chrono::sys_time<std::chrono::milliseconds> begin_ts = this->floorTsByMagnitude(quotes[0].ts);
  std::chrono::sys_time<std::chrono::milliseconds> close_ts = begin_ts + ms_interval_;
  Quote last_quote;
  for (Quote& q : quotes) {
    double price = this->calc_price(q);

    auto curr_ts = q.ts;
    if (curr_ts <= close_ts) {
      // current quote part of same bar. 
      high = std::max(high, price);
      low = std::min(low, price);

    } else {
      //current quote part of new (sequential/ non sequential) bar.
      //close previous bar, open new one.
      close = this->calc_price(last_quote);
      
      bool data_gap = false;
      Bar bar {ticker, begin_ts, close_ts, open, high, low, close, data_gap};
      bars.push_back(bar);

      begin_ts = close_ts;
      close_ts = begin_ts + ms_interval_;

      while (!(curr_ts <= close_ts)) {
        data_gap = true;
        // while next bar is not sequential, fill with empty bars.
        open = close;
        high = close;
        low = close;
        Bar bar{ticker, begin_ts, close_ts, open, high, low, close, data_gap};
        bars.push_back(bar);

        begin_ts = close_ts;
        close_ts = begin_ts + ms_interval_;
      }

      open = price;
      high = price; 
      low = price;
     }
    last_quote = q;
  } 
  //close final bar
  close = this->calc_price(last_quote);
  Bar bar{ticker, begin_ts, close_ts, open, high, low, close};
  bars.push_back(bar);

  return bars;
}

double BarBuilder::calc_price(const Quote& quote) {
  return (quote.bid + quote.ask) / 2;
}

//EUGH

static std::chrono::milliseconds orderOfMagnitude(std::chrono::milliseconds interval) {
  auto n = interval.count();
  
  if (n <= 0) {
    return std::chrono::milliseconds{0};
  }

  std::int64_t mag = 1;
  while (mag <= n / 10) {
    mag *= 10;
  }
  return std::chrono::milliseconds{mag};
}

std::chrono::sys_time<std::chrono::milliseconds> BarBuilder::floorTsByMagnitude(
    std::chrono::sys_time<std::chrono::milliseconds> ts) {
  std::chrono::milliseconds step = orderOfMagnitude(ms_interval_);
  if (step.count() == 0) {
    return ts;
  }
  
  auto d = ts.time_since_epoch();
  auto floored = d - (d % step);
  return std::chrono::sys_time<std::chrono::milliseconds>{floored};
}
    

