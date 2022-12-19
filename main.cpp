#include "head.hpp"

int main()
{
    string BIN_LOBS = "./lobs.csv";
    string BIN_TRADES = "./trades.csv";
    Strategy strategy(BIN_LOBS, BIN_TRADES);
    strategy.run();
}
