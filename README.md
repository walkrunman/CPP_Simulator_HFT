<h1 align="center">HFT Simulator</a>
<h2 align="left">Data Requirements</h2>
In order to use this simulator you have to have 2 files: lobs.csv and trades.csv. <br>
lobs.csv should consist data about order books changes. <br>
<br>
Example: <br>
![lobs.csv](/CPP_Simulator_HFT/images/order_book_example.png) 
<br>
<br>
10 prices and 10 volumes for ask and 10 prices and 10 volumes for bid <br>
<br>
trades.csv should consist data about anonymous trades 
<br>
![trades.csv]
(https://github.com/walkrunman/CPP_Simulator_HFT/raw/main/images/trades_example.png) 
<br>
<h2 align="left">Usage</h2>
Simulator parameters: <br>
- string lobs, string trades:   paths to files with order books and anonymous trades <br>
- int64_t md_latency:           latency with which market data are updated (simulator send data to strategy after this latency) <br>
- int64_t execution_latency:    orders are executed after this latency <br>
- int64_t delay:                delay between placing orders<br>
- int64_t hold_time:            we will cancel orders older than this time<br>
- bool limit_switch_market:     if true than orders which cross opposite side of the order book will be executed as market orders<br>
<br>
Simulator methods:
<br>
- The function tick() in the simulator updates data, executes orders and does everything it's work.<br>
- The function get_trade() returns last anonymous trade<br>
- Analogically get_snapshot returns actual snapshot<br>
- get_strategy_time() defines current_time for strategy.<br>
- get_pos() return current assets position <br>
- get_money() returns current money position <br>
- placed_orders(id, side, price, vol, time) place limit orders. Parameters: id; side = 0 if bid, 1 if ask; price; vol = volume, time = current_time in strategy. It is possible to place orders with another time, but it can lead to unexpectable behaviour <br>
- cancel_order(id, timw) cancels orders. Parameters: id of an order, time when we decide to cancel an order (Not time of an order, it's current time for strategy) <br>
<br>
In the file utils a function print_results was released. It print results into file with name results.csv <br>

<h2 align="left">Tests</h2>
I've generated 2 files for test strategy: lobs.csv and trades.csv: <br>
![test_lobs.csv]
(https://github.com/walkrunman/CPP_Simulator_HFT/blob/main/images/test_lobs.png)
<br>
![test_trades.csv]
(https://github.com/walkrunman/CPP_Simulator_HFT/blob/main/images/test_trades.png)
<br>
It means mid_price grow up linearly. <br>
And here's how best-bid-best-ask strategy works with these data. Here's graphic of PnL in time. <br>
![bbo_test.csv]
(https://github.com/walkrunman/CPP_Simulator_HFT/blob/main/images/bbo_test.png) <br>
<br>
Another example based on real data, but with optimistic parameters in simulation: <br>
![bbo.csv]
(https://github.com/walkrunman/CPP_Simulator_HFT/blob/main/images/bbo.png) <br>

<h2 align="left">Contacts</h2>
Feel free to contact me: antonlyulin93@gmail.com
