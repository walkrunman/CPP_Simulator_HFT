// Simulator provides actual data with functions get_snapshot and get_trade
// Information about snapshots and trades refreshed by function tick()
// The function tick also executes orders and do all rest job
// In order to place order use the function place_order
// In order to cancel order use the function cancel_order
// You can receive information about asset position with function get_pos()

#ifndef SIMULATOR
#define SIMULATOR
class Simulator{
public:
    Simulator(string lobs, string trades,
              int64_t md_latency_           = 10000,
              int64_t execution_latency_    = 10000,
              bool    limit_switch_market_  = true){
        loader = new DataLoader(lobs, trades);
        md_latency = md_latency_;
        execution_latency = execution_latency_;
        limit_switch_market = limit_switch_market_;
    }

    int8_t tick(){;
        update_md(); // flag_update is updated inside the function

        prepare_orders();
        execute_orders();
        return flag_update;
    }

    void place_order(int id, bool aggro_side,
                     double price, double vol, int64_t ts){
        shared_ptr<Order> order_bid = make_shared<Order>(id, aggro_side, price, vol, ts);
        preplaced_orders.push_back(order_bid);
    }

    void place_order(std::shared_ptr<Order> order){
        preplaced_orders.push_back(order);
    }

    void cancel_order(int id, int64_t strategy_time){
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
    int64_t md_latency;                     // latency in sending information from the simulator to the strategy
    int64_t execution_latency;              // latency in sending information from the strategy to the simulator
                                            // in fact it is latency in execution orders and so on
    int64_t sim_time            = 0;        // actual time for simulator
    double  money               = 0;        // how much money do we have
    double  pos                 = 0;        // how much asset do we have
    int8_t  flag_update;                    // flag_update = 0 if both "snapshot" and "trade" were received from
                                            // the function update_md(), 1 if only "trade", 2 if only "snapshot
    bool limit_switch_market;               // If true, orders which intersect current snapshot in opposite side
                                            // will be executed as market orders. I.e. for example if price of a bid order > best
                                            // ask price of the snapshot, it will be executed as market order with commission

    DataLoader *loader;

    std::shared_ptr<Trade>             trade_strategy;    // "trade" which available and actual for strategy
    std::shared_ptr<Snapshot>          snapshot_strategy; // "snapshot" which available and actual for strategy

    std::shared_ptr<Trade>             trade_sim;         // "trade" which available and actual for simulator
    std::shared_ptr<Snapshot>          snapshot_sim;      // "snapshot" which available and actual for simulator


    list<std::shared_ptr<Order>>       preplaced_orders;  // orders which were placed by strategy
    list<std::shared_ptr<Order>>       placed_orders;     // orders which were placed by strategy and actual for simulator

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

    void execute_one_market_order(Order order);
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
    for (auto co = canceled_orders.begin(); co != canceled_orders.end();)
    {
        for (auto po = placed_orders.begin(); po != placed_orders.end();){
            if ((*(*co)).get_id()==(*(*po)).get_id())
            {
                po = placed_orders.erase(po);
                break;
            }
            else ++po;
        }
        ++co;
    }
    canceled_orders.clear();
}

void Simulator::execute_limit_orders(){
    for(auto po = placed_orders.begin(); po != placed_orders.end();){

        if( (*(*po)).get_time()+execution_latency <  snapshot_sim->get_exchange_ts() )
        {
            // if limit_switch_market==true i.e. limit orders become market_orders, when they intersect opposite side of
            // the snapshot and if an order is bid order and if best ask price less or equal than the bid order price
            // then we execute the order as market order
            if(limit_switch_market && !(*(*po)).get_side() && snapshot_sim->get_ask_price()[0] <= (*(*po)).get_price())
            {
                execute_one_market_order((*(*po)));
                po = placed_orders.erase(po);
                continue;
            }
            // analogically. limit_switch_market==true, order is an ask order and best bid price more or equal than
            // the ask order price
            if(limit_switch_market && (*(*po)).get_side() && snapshot_sim->get_bid_price()[0] >= (*(*po)).get_price()){
                execute_one_market_order((*(*po)));
                po = placed_orders.erase(po);
                continue;
            }
        }

        // for limit orders
        if( (*(*po)).get_time()+execution_latency <  trade_sim->get_exchange_ts() )
        {
            if(trade_sim->get_side() == (*(*po)).get_side()){
                ++po;
                continue;
            }
            if( !trade_sim->get_side() && (*(*po)).get_side()) //trade = bid, order = ask
            {
                if( trade_sim->get_price() >= (*(*po)).get_price() &&
                    trade_sim->get_vol()   >= (*(*po)).get_vol())
                {
                        shared_ptr<OwnTrade> own = make_shared<OwnTrade>((*(*po)).get_side(), (*(*po)).get_price(),
                                                         (*(*po)).get_vol(), sim_time, 0);
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
                        cout << "2 time: " << sim_time << endl;
                        shared_ptr<OwnTrade> own = make_shared<OwnTrade>((*(*po)).get_side(), (*(*po)).get_price(),
                                                         (*(*po)).get_vol(), sim_time, 0);
                        preplaced_own_trades.push_back(own);
                        trade_sim->set_vol( trade_sim->get_vol() - (*(*po)).get_vol() );
                        po = placed_orders.erase(po);
                        continue;
                }
            }
            if(trade_sim->get_vol() < 0.00001){
                po = placed_orders.erase(po);
                break;
            }
        }
        ++po;
    }
}

void Simulator::execute_one_market_order(Order order){
    // This function calculates how much money we earn or lose in case of execution a market order
    // The expediency of calling a function must be decided outside of this function.
    // This function will generate OwnTrade in any case
    double saved_vol = order.get_vol();

    if(!order.get_side()) //if we have bid order
    {
        for(int level=0; level<10; level++){
            if(order.get_price()>=snapshot_sim->get_ask_price()[level])
            {
                if(saved_vol <= 0){
                    break;
                }
                if(saved_vol >= snapshot_sim->get_ask_vol()[level])
                {
                    shared_ptr<OwnTrade> own = make_shared<OwnTrade>(order.get_side(),
                                                                     snapshot_sim->get_ask_price()[level],
                                                                     snapshot_sim->get_ask_vol()[level],
                                                                     sim_time, 1);
                    preplaced_own_trades.push_back(own);
                    saved_vol -= snapshot_sim->get_ask_vol()[level];
                    continue;
                }
                if(saved_vol < snapshot_sim->get_ask_vol()[level])
                {
                    shared_ptr<OwnTrade> own = make_shared<OwnTrade>(order.get_side(),
                                                                     snapshot_sim->get_ask_price()[level],
                                                                     saved_vol, sim_time, 1);
                    preplaced_own_trades.push_back(own);
                    saved_vol = 0;
                }
            }
            else{
                break;
            }
        }
    }

    if(order.get_side()) //if we have ask order
    {
        for(int level=0; level<10; level++){
            if(order.get_price()<=snapshot_sim->get_bid_price()[level])
            {
                if(saved_vol <= 0){
                    break;
                }
                if(saved_vol >= snapshot_sim->get_bid_vol()[level])
                {
                    shared_ptr<OwnTrade> own = make_shared<OwnTrade>(order.get_side(),
                                                                     snapshot_sim->get_bid_price()[level],
                                                                     snapshot_sim->get_bid_vol()[level],
                                                                     sim_time, 1);
                    preplaced_own_trades.push_back(own);
                    saved_vol -= snapshot_sim->get_bid_vol()[level];
                    continue;
                }
                if(saved_vol < snapshot_sim->get_bid_vol()[level])
                {
                    shared_ptr<OwnTrade> own = make_shared<OwnTrade>(order.get_side(),
                                                                     snapshot_sim->get_bid_price()[level],
                                                                     saved_vol, sim_time, 1);
                    preplaced_own_trades.push_back(own);
                    saved_vol = 0;
                }
            }
            else{
                break;
            }
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
