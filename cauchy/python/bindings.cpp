#include <memory>
#include <string>
#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/chrono.h>

#include "backtest/backtester.h"
#include "io/csv_data_feed.h"
#include "strategy/istrategy.h"
#include "strategy/sma_cross_strategy.h"

// If Quote is in ../market/quote.h (as your IDataFeed includes it), include it too:
#include "market/quote.h"

// If Bar / Decision are in market/
#include "market/bar.h"
#include "market/decision.h"

namespace py = pybind11;

// --------------------
// Trampolines (so Python can override virtuals)
// --------------------
struct PyIStrategy : public IStrategy {
  using IStrategy::IStrategy;
  Decision onBar(const Bar& bar) override {
    PYBIND11_OVERRIDE_PURE(
      Decision,   // Return
      IStrategy,  // Parent
      onBar,      // Name
      bar         // Args
    );
  }
};

struct PyIDataFeed : public IDataFeed {
  using IDataFeed::IDataFeed;
  std::vector<Quote> loadData() override {
    PYBIND11_OVERRIDE_PURE(
      std::vector<Quote>,
      IDataFeed,
      loadData
    );
  }
};

// --------------------
// Adapters: Backtester wants unique_ptr<Interface>
// but Python objects are managed by shared_ptr.
// These adapters are uniquely owned by Backtester,
// and internally keep the Python object alive.
// --------------------
class StrategyAdapter : public IStrategy {
public:
  explicit StrategyAdapter(std::shared_ptr<IStrategy> s) : strat_(std::move(s)) {}
  Decision onBar(const Bar& bar) override {
    py::gil_scoped_acquire gil;
    return strat_->onBar(bar);
  }
private:
  std::shared_ptr<IStrategy> strat_;
};

class FeedAdapter : public IDataFeed {
public:
  explicit FeedAdapter(std::shared_ptr<IDataFeed> f) : feed_(std::move(f)) {}
  std::vector<Quote> loadData() override {
    py::gil_scoped_acquire gil;
    return feed_->loadData();
  }
private:
  std::shared_ptr<IDataFeed> feed_;
};

PYBIND11_MODULE(cauchybacktest, m) {
  m.doc() = "Backtesting framework bindings";

  // --------------------
  // POD structs
  // --------------------
  py::class_<Decision>(m, "Decision")
    .def(py::init<>())
    .def(py::init<std::string, int, double>(),
         py::arg("ticker"), py::arg("quantity"), py::arg("price"))
    .def_readwrite("ticker", &Decision::ticker)
    .def_readwrite("quantity", &Decision::quantity)
    .def_readwrite("price", &Decision::price);

  py::class_<Bar>(m, "Bar")
    .def(py::init<>())
    .def_readwrite("ticker", &Bar::ticker)
    // pybind11/chrono.h usually converts system_clock::time_point <-> datetime
    .def_readwrite("start_ts", &Bar::start_ts)
    .def_readwrite("end_ts", &Bar::end_ts)
    .def_readwrite("open", &Bar::open)
    .def_readwrite("high", &Bar::high)
    .def_readwrite("low", &Bar::low)
    .def_readwrite("close", &Bar::close)
    .def_readwrite("data_gap", &Bar::data_gap);

  // Quote: bind minimally so std::vector<Quote> can cross the boundary.
  // IMPORTANT: customize fields/constructors to match YOUR Quote definition.
  py::class_<Quote>(m, "Quote")
    .def(py::init<>());

  // --------------------
  // Interfaces
  // --------------------
  py::class_<IStrategy, PyIStrategy, std::shared_ptr<IStrategy>>(m, "IStrategy")
    .def(py::init<>())
    .def("onBar", &IStrategy::onBar);

  py::class_<IDataFeed, PyIDataFeed, std::shared_ptr<IDataFeed>>(m, "IDataFeed")
    .def(py::init<>())
    .def("loadData", &IDataFeed::loadData);

  // --------------------
  // Concrete implementations
  // --------------------
  py::class_<CsvDataFeed, IDataFeed, std::shared_ptr<CsvDataFeed>>(m, "CsvDataFeed")
    .def(py::init<std::string, std::string>(),
         py::arg("filepath"), py::arg("ticker"));

  py::class_<SmaCrossStrategy, IStrategy, std::shared_ptr<SmaCrossStrategy>>(m, "SmaCrossStrategy")
    .def(py::init<std::string, std::size_t, std::size_t, int>(),
         py::arg("ticker"),
         py::arg("short_window") = 10,
         py::arg("long_window") = 30,
         py::arg("trade_qty") = 10);

  // --------------------
  // Backtester
  // --------------------
  py::class_<Backtester>(m, "Backtester")
    .def(py::init<double>(), py::arg("starting_cash"))
    .def("run_backtest", &Backtester::run_backtest)
    .def("results", &Backtester::results)

    // Accept shared_ptr in Python and wrap into unique_ptr adapter for C++.
    .def("set_feed",
         [](Backtester& b, std::shared_ptr<IDataFeed> feed) {
           b.set_feed(std::make_unique<FeedAdapter>(std::move(feed)));
         },
         py::arg("feed"))
    .def("set_strat",
         [](Backtester& b, std::shared_ptr<IStrategy> strat) {
           b.set_strat(std::make_unique<StrategyAdapter>(std::move(strat)));
         },
         py::arg("strategy"));
}

