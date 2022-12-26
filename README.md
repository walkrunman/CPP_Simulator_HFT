<h1 align="center">HFT Simulator</a>
<h2 align="left">Data Requirements</h2>
In order to use this simulator you have to have 2 files: lobs.csv and trades.csv. <br>
lobs.csv should consist data about order books changes. <br>
<br>
Example: <br>
![lobs.csv](https://github.com/walkrunman/CPP_Simulator_HFT/blob/main/images/order_book_example.png?raw=true) 
<br>
<br>
10 prices and 10 volumes for ask and 10 prices and 10 volumes for bid <br>
<br>
trades.csv should consist data about anonymous trades 
<br>
![trades.csv](https://github.com/walkrunman/CPP_Simulator_HFT/blob/main/images/trades_example.png?raw=true) 
<br>
<h2 align="left">Usage</h2>
Simulator parameters:
- string lobs, string trades:   paths to files with order books and anonymous trades
- int64_t md_latency:           latency with which market data are updated (simulator send
- int64_t execution_latency:    orders are executed after this latency
- int64_t delay:                delay between placing orders
- int64_t hold_time:            we will cancel orders older than this time
- bool limit_switch_market:     if true than orders which cross opposite side of the order book will be executed as market orders
<br>
Simulator methods:
<br>
- The function tick() in the simulator updates data, executes orders and does everything it's work.
- The function get_trade() returns last anonymous trade
- Analogically get_snapshot returns actual snapshot
- get_strategy_time() defines current_time for strategy.
- get_pos() return current assets position
- get_money() returns current money position
- placed_orders(id, side, price, vol, time) place limit orders. Parameters: id; side = 0 if bid, 1 if ask; price; vol = volume, time = current_time in strategy. It is possible to place orders with another time, but it can lead to unexpectable behaviour
- cancel_order(id, timw) cancels orders. Parameters: id of an order, time when we decide to cancel an order (Not time of an order, it's current time for strategy)
<br>
In the file utils a function print_results was released. It print results into file with name results.csv
