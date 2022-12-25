#include <iostream>
#include <string>
#include <fstream>
#include <list>

#include <algorithm>
#include <memory>

using namespace std;

#include "ticker.cpp"       // parent class for classes snapshot and trade
#include "snapshot.cpp"     // contains snapshot information
#include "trade.cpp"        // contains trade information
#include "order.cpp"        // orders which we make
#include "cancelorder.cpp"  // requests for orders cancelation
#include "owntrade.cpp"     // information about completed transactions

#include "utils.cpp"
#include "dataloader.cpp"   // class for load information
#include "simulator.cpp"    // look info inside the file
#include "strategy.cpp"     // look info inside the file


