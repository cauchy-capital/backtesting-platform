#include "Portfolio.h"

#include <iostream>

Portfolio::Portfolio(double cash) : cash_(cash)  {}

void Portfolio::record(const Decision& decision) {
  if (decision.quantity == 0) {
    //dont do anything
    return;
  }


  std::string ticker = decision.ticker;
  double units = decision.quantity;
  double transact_amount = units * decision.price;

  std::cout << "recording decision:\n" << "buy:" << ticker  << ", " << units << "@" << decision.price << std::endl;

  //update cash
  cash_ -= transact_amount;
  std::cout << "cash: " << cash_ << std::endl;
  
  //update portfolio holding
  std::pair<int, double> old_ticker_stash = holdings_[ticker];
  int ticker_held = old_ticker_stash.first;
  double val = old_ticker_stash.second;

  std::pair<int, double> new_ticker_stash = {ticker_held + units, val + transact_amount};
  holdings_[ticker] = new_ticker_stash;

  //add decision to history
  history_.push_back(decision);
}

double Portfolio::calculate_pnl() const {
  double pnl = cash_;
  /*
  for (const std::pair<const std::string, std::pair<int, double>>& n : holdings_) {
    std::pair<int, double> holding = n.second;
    pnl += holding.second;
  }
  */
  return pnl;
}
