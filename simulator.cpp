// Simulator provides actual data with functions get_snapshot and get_trade
// Information about snapshots and trades refreshed by function tick()
// The function tick also executes orders and do all rest job
// In order to place order use the function place_order
// In order to cancel order use the function cancel_order
// You can receive information about asset position with function get_pos()
// Also get_preplaced_own_trade_size() will return information about executed orders

#ifndef SIMULATOR
#define SIMULATOR
class Simulator{
public:
    Simulator(string lobs, string trades){
        loader = new DataLoader(lobs, trades);
    }

    short int tick(){;
        update_md(); // flag_update is updated inside the function

        prepare_orders();
        execute_orders();
        return flag_update;
    }

    void place_order(int id, bool aggro_side,
                     double price, double vol,
                     long long int ts){
        shared_ptr<Order> order_bid = make_shared<Order>(id, aggro_side, price, vol, ts);
        preplaced_orders.push_back(order_bid);
    }

    void place_order(std::shared_ptr<Order> order){
        preplaced_orders.push_back(order);
    }

    void cancel_order(int id, long long int strategy_time){
        shared_ptr<CancelOrder> cancel = make_shared<CancelOrder>(id, strategy_time);
        precanceled_orders.push_back(cancel);
    }

    void cancel_order(shared_ptr<CancelOrder> cancel){
        precanceled_orders.push_back(cancel);
    }

    std::shared_ptr<Trade> get_trade() {return trade_strategy;}
    std::shared_ptr<Snapshot> get_snapshot() {return snapshot_strategy;}
    list<std::shared_ptr<OwnTrade>> get_own_trades() {return own_trades;}
    double get_pos() {return pos;}
    double get_money() {return money;}


private:
    long long int md_latency        = 10000;    // latency in sending information from the simulator to the strategy
    long long int execution_latency = 10000;    // latency in sending information from the strategy to the simulator
                                                // in fact it is latency in execution orders and so on
    long long int sim_time          = 0;        // actual time for simulator
    double        money             = 0;        // how much money do we have
    double        pos               = 0;        // how much asset do we have
    short int flag_update;                      // flag_update = 0 if both "snapshot" and "trade" were received from
                                                // the function update_md(), 1 if only "trade", 2 if only "snapshot

    DataLoader *loader;

    std::shared_ptr<Trade>      trade_strategy;            // "trade" which available and actual for strategy
    std::shared_ptr<Snapshot>   snapshot_strategy;         // "snapshot" which available and actual for strategy

    std::shared_ptr<Trade>      trade_sim;                 // "trade" which available and actual for simulator
    std::shared_ptr<Snapshot>   snapshot_sim;              // "snapshot" which available and actual for simulator


    list<std::shared_ptr<Order>> preplaced_orders;         // orders which were placed by strategy
    list<std::shared_ptr<Order>> placed_orders;            // orders which were placed by strategy and actual for simulator

    list<std::shared_ptr<CancelOrder>> precanceled_orders; // 2 queues with cancelation requests
    list<std::shared_ptr<CancelOrder>> canceled_orders;    // similarly with preplaced_orders and placed_orders

    list<std::shared_ptr<OwnTrade>> preplaced_own_trades;  // similarly with 2 pairs above but own trades creates in simulator
    list<std::shared_ptr<OwnTrade>> own_trades;

    void prepare_orders(){
        move_placed_orders();
        move_canceled_orders();
        execute_order_cancelation();
        move_own_trades();
    }

    void execute_orders(){
        execute_limit_orders();
        // execute_market_orders(); // Not realized yet
    }

    void update_md();
    void move_placed_orders();
    void move_canceled_orders();
    void execute_order_cancelation();
    void move_own_trades();

    void execute_limit_orders();
};

void Simulator::update_md(){
    flag_update = loader->tick();
    trade_strategy = loader->get_trade();
    snapshot_strategy = loader->get_snapshot();
    sim_time = max(trade_strategy->get_exchange_ts(), snapshot_strategy->get_exchange_ts());

    trade_sim = trade_strategy;
    snapshot_sim = snapshot_strategy;
}

void Simulator::move_placed_orders(){
    for(int i = 0; i<preplaced_orders.size(); ++i){
        if(preplaced_orders.front()->get_time()+execution_latency <= sim_time){
            placed_orders.push_back(preplaced_orders.front());
            preplaced_orders.pop_front();
        }
        else{
            break;
        }
    }
}

void Simulator::move_canceled_orders(){
    for(int i = 0; i<precanceled_orders.size(); ++i){
        if(precanceled_orders.front()->get_time()+execution_latency <= sim_time){
            canceled_orders.push_back(precanceled_orders.front());
            precanceled_orders.pop_front();
        }
        else{
            break;
        }
    }
}

void Simulator::execute_order_cancelation(){
    for(auto co = canceled_orders.begin(); co != canceled_orders.end(); ++co){
        for(auto po = placed_orders.begin(); po != placed_orders.end(); ++po){
            if( (*(*co)).get_id()==(*(*po)).get_id()){
                co = canceled_orders.erase(co);
                po = placed_orders.erase(po);
                break;
            }
        }
    }
}

void Simulator::execute_limit_orders(){
    for(auto po = placed_orders.begin(); po != placed_orders.end(); ++po){
        if(trade_sim->get_side() == (*(*po)).get_side()){
            continue;
        }
        if( !trade_sim->get_side() && (*(*po)).get_side()) //trade = bid, order = ask
        {
            if( trade_sim->get_price() >= (*(*po)).get_price() &&
                trade_sim->get_vol()   >= (*(*po)).get_vol())
            {
                    shared_ptr<OwnTrade> own = make_shared<OwnTrade>((*(*po)).get_side(), (*(*po)).get_price(),
                                                     (*(*po)).get_vol(), sim_time);
                    preplaced_own_trades.push_back(own);
                    trade_sim->set_vol( trade_sim->get_vol() - (*(*po)).get_vol() );
                    po = placed_orders.erase(po);
                    continue;
            }
        }

        if( trade_sim->get_side() && !(*(*po)).get_side()) //trade = ask, order = bid
        {
            if( trade_sim->get_price() <= (*(*po)).get_price() &&
                trade_sim->get_vol()   >= (*(*po)).get_vol())
            {
                    shared_ptr<OwnTrade> own = make_shared<OwnTrade>((*(*po)).get_side(), (*(*po)).get_price(),
                                                     (*(*po)).get_vol(), sim_time);
                    preplaced_own_trades.push_back(own);
                    trade_sim->set_vol( trade_sim->get_vol() - (*(*po)).get_vol() );
                    po = placed_orders.erase(po);
                    continue;
            }
        }
        if(trade_sim->get_vol() < 0.01){
            break;
        }
    }
}

void Simulator::move_own_trades(){
    for(int i = 0; i<preplaced_own_trades.size(); ++i){
        if( preplaced_own_trades.front()->get_time()+md_latency <= sim_time ){
            own_trades.push_back(preplaced_own_trades.front());
            if(preplaced_own_trades.front()->get_side() == false){
                pos = pos + preplaced_own_trades.front()->get_vol();
                money = money - preplaced_own_trades.front()->get_vol() * preplaced_own_trades.front()->get_price();
            }
            else{
                pos = pos - preplaced_own_trades.front()->get_vol();
                money = money + preplaced_own_trades.front()->get_vol() * preplaced_own_trades.front()->get_price();
            }
            preplaced_own_trades.pop_front();
        }
        else{
            break;
        }
    }
    if(flag_update<0){
        for(int i = 0; i<preplaced_own_trades.size(); ++i){
            own_trades.push_back(preplaced_own_trades.front());
            preplaced_own_trades.pop_front();
        }
    }
}

#endif //SIMULATOR
