#include "head.hpp"

int main()
{
    string BIN_LOBS = "./lobs.csv";
    string BIN_TRADES = "./trades.csv";

    BaseStrategy strategy(BIN_LOBS_TEST,
                          BIN_TRADES_TEST,
                          1e4,              // md_latency
                          1e4,              // execution_latency
                          1e5,              // delay
                          1e5,              // hold_time
                          true);            // limit_switch_market
    strategy.run();

    return 0;
}
