#pragma once

#include <vector>

#include <cauchy/market/quote.h>

/*
 * @brief Interface for data feeds.
 *
 * A data feed is responsible for ingesting market data from some source
 * (e.g., CSV files, databases, live APIs) and parsing/transforming it into a
 * sequence of Quote objects consumable by the rest of the system.
 *
 * Contract / behavior:
 * - loadData() reads the underlying data source and returns the resulting Quotes,
 *   in chronological order. The implementation owns any parsing, validation, and 
 *   formatting required to construct valid Quote instances.
 */

class IDataFeed {
public:
  virtual ~IDataFeed() = default;
  virtual std::vector<Quote> loadData() = 0;
};

