# Cauchy In‑House Backtesting Platform

A C++ backtesting platform with Python bindings, created for **Cauchy Capital**.

---


## Build

From the root of the project:

```bash
cmake -B build
cmake --build build
```

After a successful build, the executable will be available in the `build/` directory.

---

## Usage (Python)

Once built, the Python package **`cauchybacktest`** lives inside the `build/` directory.  
Create and run your Python backtest scripts from within `build/`.

### 1) Create a `Backtester`

Create a `cauchybacktest.Backtester`, specifying the `starting_cash`:

```python
import cauchybacktest as cb

b = cb.Backtester(starting_cash=100.0)
```

A backtester requires:

- a **data feed**
- a **strategy**

---

### 2) Create a data feed

Create a `cauchybacktest.CsvDataFeed`, specifying:

- `filepath`: path to a CSV containing quotes (see **CSV format** below)
- `ticker`: ticker name to associate with the feed

```python
import cauchybacktest as cb

data_feed = cb.CsvDataFeed(
    filepath="<path/to/file.csv>",
    ticker="<ticker>"
)
```

#### CSV format

The CSV must have **comma-delimited** columns in this order:

1. **Local Time** (`DD.MM.YYYY hh:mm:ss.f`)
2. **Ask**
3. **Bid**
4. **AskVolume**
5. **BidVolume**

Example header:

```text
Local Time,Ask,Bid,AskVolume,BidVolume
```

---

### 3) Create an execution handler

A default execution handler is available:

- `cauchybacktest.SimulatedExecutionHandler`

create it as follows:

```python
import cauchybacktest as cb
execution_handler = cb.SimulatedExecutionHandler()
```
---

### 4) Create a strategy

A default strategy is available:

- `cauchybacktest.SmaCrossStrategy`

To implement your own strategy, implement the `cauchybacktest.IStrategy` interface and define:

- `onBar(bar: cauchybacktest.Bar) -> cauchybacktest.Decision`

#### Minimal custom strategy skeleton

```python
import cauchybacktest as cb

class ExampleStrat(cb.IStrategy):
    def __init__(self, ticker: str):
        super().__init__()
        self.ticker = ticker

    def onBar(self, bar: cb.Bar) -> cb.Decision:
        price = bar.close
        qty = 100

        # ... your logic here ...

        # Buy `bar.ticker` at `price` for quantity `qty`
        return cb.Decision(bar.ticker, qty, price)
```

> **Note:** Returning `cb.Decision(bar.ticker, 0, price)` indicates **no trade** on this bar.

---

### 5) Run a backtest

Set the feed, strategy and execution handler using `set_feed()`, `set_strat()` and `set_execution_handler()`, then run:

```python
import cauchybacktest as cb

b = cb.Backtester(starting_cash=100.0)

data_feed = cb.CsvDataFeed(
    filepath="../data/0005.HKHKD_Ticks_17.11.2025-17.11.2025.csv",
    ticker="0005.HKHKD",
)
b.set_feed(data_feed)

strat = cb.SmaCrossStrategy(ticker="0005.HKHKD")  # or your custom strategy
execution_handler = cb.SimulatedExecutionHandler()

b.set_strat(strat)
b.set_execution_handler(execution_handler)

b.run_backtest()
```

---

## Results

Currently, the only available statistic is **PNL**.

After running a backtest:

```python
b.run_backtest()
pnl = b.results()
print("PNL:", pnl)
```

---

## Example: SMA Cross Strategy backtest (custom implementation)

```python
import cauchybacktest as cb
from collections import deque

class ExampleSmaCross(cb.IStrategy):
    def __init__(self, ticker, short=10, long=30, qty=10):
        super().__init__()
        self.t, self.s, self.l, self.q = ticker, short, long, qty
        self.c = deque(maxlen=long)
        self.pos = False
        self.ps = self.pl = None  # previous short/long SMA values

    def onBar(self, bar: cb.Bar) -> cb.Decision:
        price = bar.close
        if bar.ticker != self.t:
            return cb.Decision(bar.ticker, 0, price)

        self.c.append(price)
        if len(self.c) < self.l:
            return cb.Decision(bar.ticker, 0, price)

        c = list(self.c)
        s = sum(c[-self.s:]) / self.s
        l = sum(c) / self.l

        qty = 0
        if self.ps is not None:
            up = self.ps <= self.pl and s > l
            dn = self.ps >= self.pl and s < l
            if up and not self.pos:
                qty, self.pos = +self.q, True
            elif dn and self.pos:
                qty, self.pos = -self.q, False

        self.ps, self.pl = s, l
        return cb.Decision(bar.ticker, qty, price)


b = cb.Backtester(starting_cash=100.0)

data_feed = cb.CsvDataFeed(
    filepath="../data/0005.HKHKD_Ticks_17.11.2025-17.11.2025.csv",
    ticker="0005.HKHKD",
)
b.set_feed(data_feed)

execution_handler = cb.SimulatedExecutionHandler()
b.set_execution_handler(execution_handler)

strat = ExampleSmaCross(ticker="0005.HKHKD")
b.set_strat(strat)

b.run_backtest()
pnl = b.results()
print("PNL:", round(pnl, 2))
```

---

## Testing

Build the project, then from the `build/` directory run:

```bash
ctest
```

---

## Code style

- Follow Google C++ Style Guide:  
  https://google.github.io/styleguide/cppguide.html

---

## Git conventions

- Conventional Commits:  
  https://www.conventionalcommits.org/en/v1.0.0/
- Branch off for every feature/fix.
- Open a PR to merge.
- PRs must be reviewed by another team member.
