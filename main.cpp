#include "head.hpp"

int main()
{
    string BIN_LOBS = "C:\\cpp_projects\\Simulator\\data\\btcusdt_Binance_LinearPerpetual\\lobs.csv";
    string BIN_TRADES = "C:\\cpp_projects\\Simulator\\data\\btcusdt_Binance_LinearPerpetual\\trades.csv";
    string BIN_LOBS_TEST = "C:\\cpp_projects\\Simulator\\data\\btcusdt_Binance_LinearPerpetual\\lobs_test_generated.csv";
    string BIN_TRADES_TEST = "C:\\cpp_projects\\Simulator\\data\\btcusdt_Binance_LinearPerpetual\\trades_test_generated.csv";

    /*BaseStrategy strategy(BIN_LOBS_TEST,
                          BIN_TRADES_TEST,
                          0,              // md_latency
                          0,              // execution_latency
                          0,              // delay
                          5,              // hold_time
                          false);          // limit_switch_market*/

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
