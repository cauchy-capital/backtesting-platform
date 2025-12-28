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

  std::chrono::sys_time<std::chrono::milliseconds> begin_ts = this->floorMsTimestamp(quotes[0].ts);
  Quote last_quote;
  for (Quote& q : quotes) {
    double price = this->calc_price(q);

    auto curr_ts = q.ts;
    if (curr_ts <= begin_ts + ms_interval_) {
      // current quote part of same bar. 
      high = std::max(high, price);
      low = std::min(low, price);

    } else {
      //current quote part of new (sequential/ non sequential) bar.
      //close previous bar, open new one.
      std::chrono::sys_time<std::chrono::milliseconds> close_ts = begin_ts + ms_interval_;
      close = this->calc_price(last_quote);

      Bar bar {ticker, begin_ts, close_ts, open, high, low, close};
      bars.push_back(bar);

      open = price;
      high = price;
      low = price;

      // find begin_ts of new bar.
      if (curr_ts <= (begin_ts + ms_interval_) + ms_interval_) { 
        // sequential bar.
        begin_ts = close_ts;
      } else {
        // non sequential bar.
        begin_ts = this->floorMsTimestamp(q.ts);
      }
    } 
    last_quote = q;
  }

  return bars;
}

double BarBuilder::calc_price(const Quote& quote) {
  return (quote.bid + quote.ask) / 2;
}

//EUGH
std::chrono::sys_time<std::chrono::milliseconds> BarBuilder::floorMsTimestamp(
    std::chrono::sys_time<std::chrono::milliseconds> ts) {

  std::chrono::sys_time<std::chrono::seconds> floored = std::chrono::floor<std::chrono::seconds>(ts);
  return time_point_cast<std::chrono::milliseconds>(floored);
}


