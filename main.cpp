#include "head.hpp"

int main()
{
    string BIN_LOBS = "./lobs.csv";
    string BIN_TRADES = "./trades.csv";

    BaseStrategy strategy(BIN_LOBS,
                          BIN_TRADES,
                          1e4,              // md_latency
                          1e4,              // execution_latency
                          1e6,              // delay
                          1e6,              // hold_time
                          false);          // limit_switch_market
    strategy.run();

    return 0;
}
