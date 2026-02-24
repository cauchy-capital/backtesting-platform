#include <memory>
#include <string>
#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/chrono.h>

#include <cauchy/backtest/backtester.h>
#include <cauchy/io/csv_data_feed.h>
#include <cauchy/strategy/istrategy.h>
#include <cauchy/strategy/sma_cross_strategy.h>
#include <cauchy/execution/simulated_execution_handler.h>

// If Quote is in ../market/quote.h (as your IDataFeed includes it), include it too:
#include <cauchy/market/quote.h>

// If Bar / Decision are in market/
#include <cauchy/market/bar.h>
#include <cauchy/core/events/market_event.h>
#include <cauchy/core/events/signal_event.h>

namespace py = pybind11;

// --------------------
// Trampolines (so Python can override virtuals)
// --------------------
struct PyIStrategy : public IStrategy {
  using IStrategy::IStrategy;
  SignalEvent onBar(const MarketEvent& market_event) override {
    PYBIND11_OVERRIDE_PURE(
      SignalEvent,   // Return type
      IStrategy,     // Parent class
      onBar,         // Method name
      market_event   // Arguments
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

struct PyIExecutionHandler : public IExecutionHandler {
  using IExecutionHandler::IExecutionHandler;

  void handleOrder(OrderEvent oe) override {
    PYBIND11_OVERRIDE_PURE(
      void,
      IExecutionHandler,
      handleOrder,
      oe
    );
  }

  std::optional<FillEvent> onMarket(MarketEvent market) override {
    PYBIND11_OVERRIDE_PURE(
      std::optional<FillEvent>,
      IExecutionHandler,
      onMarket,
      market
    );
  }
};

// --------------------
// Adapters
// --------------------
class StrategyAdapter : public IStrategy {
public:
  explicit StrategyAdapter(std::shared_ptr<IStrategy> s) : strat_(std::move(s)) {}
  SignalEvent onBar(const MarketEvent& market_event) override {
    py::gil_scoped_acquire gil;
    return strat_->onBar(market_event);
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

class ExecutionHandlerAdapter : public IExecutionHandler {
public:
  explicit ExecutionHandlerAdapter(std::shared_ptr<IExecutionHandler> exec_handler)
      : exec_handler_(std::move(exec_handler)) {}

  void handleOrder(OrderEvent oe) override {
    py::gil_scoped_acquire gil;
    exec_handler_->handleOrder(oe);
  }

  std::optional<FillEvent> onMarket(MarketEvent market) override {
    py::gil_scoped_acquire gil;
    return exec_handler_->onMarket(market);
  }

private:
  std::shared_ptr<IExecutionHandler> exec_handler_;
};

PYBIND11_MODULE(cauchybacktest, m) {
  m.doc() = "Backtesting framework bindings";

  // --------------------
  // POD structs
  // --------------------
  py::class_<SignalEvent>(m, "SignalEvent")
    .def(py::init<std::string, Direction, Bar>(),
         py::arg("ticker"), py::arg("direction"), py::arg("bar"))
    .def_readwrite("ticker", &SignalEvent::ticker)
    .def_readwrite("direction", &SignalEvent::direction)
    .def_readwrite("bar", &SignalEvent::bar);

  py::class_<MarketEvent>(m, "MarketEvent")
    .def(py::init<Bar>())
    .def_readwrite("bar", &MarketEvent::bar);

  py::class_<Bar>(m, "Bar")
    .def(py::init<>())
    .def_readwrite("ticker", &Bar::ticker)
    .def_readwrite("start_ts", &Bar::start_ts)
    .def_readwrite("end_ts", &Bar::end_ts)
    .def_readwrite("open", &Bar::open)
    .def_readwrite("high", &Bar::high)
    .def_readwrite("low", &Bar::low)
    .def_readwrite("close", &Bar::close)
    .def_readwrite("data_gap", &Bar::data_gap);

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

  py::class_<IExecutionHandler, PyIExecutionHandler, std::shared_ptr<IExecutionHandler>>(m, "IExecutionHandler")
  // If it's an interface / pure virtual, don't expose a default constructor:
  // .def(py::init<>())
  .def("handleOrder", &IExecutionHandler::handleOrder)
  .def("onMarket", &IExecutionHandler::onMarket);

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

  py::class_<SimulatedExecutionHandler, IExecutionHandler, std::shared_ptr<SimulatedExecutionHandler>>(m, "SimulatedExecutionHandler")
    .def(py::init<>());

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
         py::arg("strategy"))
    .def("set_execution_handler",
         [](Backtester& b, std::shared_ptr<IExecutionHandler> exec_handler) {
           b.set_execution_handler(std::make_unique<ExecutionHandlerAdapter>(std::move(exec_handler)));
         },
         py::arg("execution_handler"));
}


