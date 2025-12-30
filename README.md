# Cauchy In-House Backtesting Platform

A C++ backtesting platform, with python bindings, created for Cauchy Capital

## How To Build
navigate to the root of the project and run: 

`cmake -B build` </br>
`cmake --build build`

The executable should now be available in the /build/ directory.

## How To Use

Once built, the python package `cauchybacktest` lives inside the build/ directory. Create any python file in the build/ directory to run a backtest.

### Creating a Backtester
To run a backtest, first create a `cauchybacktest.Backtester`, specifying the `starting_cash` on creation.

`b = cauchybacktest.Backtester(starting_cash=100.0)`

A backtester requires a data feed and a strategy to run. 

### Creating a Data Feed
A backtester requires a data feed to run.

To create a datafeed, create a `cauchybacktest.CsvDataFeed`, specifying the `filepath` and `ticker` on creation.

`filepath`: a string, the filepath to a CSV, containing quotes in the following format:

Local Time (in DD.MM.YYYY hh:mm:ss.f), Ask, Bid, AskVolume, BidVolume

with comma delimiters.

`ticker`: the name of the ticker.

you can create a data feed as such:
`data_feed = cauchybacktest.CsvDataFeed(filepath=<filepath>, ticker=<ticker-for-data)`
## Creating a Strategy
A backtester requires a strategy to run.

a default strategy, `cauchybacktest.SmaCrossStrategy`, already exist within the module.

to implement your own strategy, implement the `cauchybacktest.IStrategy` interface with a class, with the `onBar(bar: cauchybacktest.Bar) -> cauchybacktest.Decision:` method. e.g:

```
Import cauchybacktest as cb

Class ExampleStrat(cb.IStrategy):
  def __init__(self, ticker):
    super().__init__()
    self.t = ticker

  def onBar(self, bar: cb.Bar) -> cb.Decision:
    price = bar.close
    qty = 100
    ...
    return cb.Decision(bar.ticker, qty, price) # buy ticker at price bar.close, for quantity of 100 units.
```

### Running a backtest
You can then set strategies and datafeeds as follows, using set_feed, and set_strat:

```
b = cb.Backtester(starting_cash=100.0)

data_feed = cb.CsvDataFeed(filepath="../data/0005.HKHKD_Ticks_17.11.2025-17.11.2025.csv", 
                           ticker="0005.HKHKD")
b.set_feed(data_feed)

strat = ExampleStrat(ticker="0005.HKHKD")
b.set_strat(strat)

b.run_backtest()

```

### Results
Currently, only PNL is available as a statistic. Once a backtest has been run, to view pnl:



```
b.run_backtest
b.results
```
### Example: SMA Cross Strategy backtest
```
import cauchybacktest as cb
from collections import deque

class ExampleSmaCross(cb.IStrategy):
    def __init__(self, ticker, short=10, long=30, qty=10):
        super().__init__()
        self.t, self.s, self.l, self.q = ticker, short, long, qty
        self.c = deque(maxlen=long)
        self.pos = False
        self.ps = self.pl = None  # previous short/long sma

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
            if up and not self.pos: qty, self.pos = +self.q, True
            elif dn and self.pos:  qty, self.pos = -self.q, False

        self.ps, self.pl = s, l
        return cb.Decision(bar.ticker, qty, price)


b = cb.Backtester(starting_cash=100.0)

data_feed = cb.CsvDataFeed(filepath="../data/0005.HKHKD_Ticks_17.11.2025-17.11.2025.csv", 
                           ticker="0005.HKHKD")
b.set_feed(data_feed)

strat = ExampleSmaCross(ticker="0005.HKHKD")
b.set_strat(strat)

b.run_backtest()
pnl = b.results()
print("PNL: ", round(pnl, 2))

```


## To Test
build the project, and then navigate to the build directory. run:

`ctest`


## Code Style
Google's code style: https://google.github.io/styleguide/cppguide.html


## Git Conventions
Conventional commits: https://www.conventionalcommits.org/en/v1.0.0/

Branch off for every feature/fix. PR to merge.
PR must be reviewed by other member.





