#ifndef STRATEGY
#define STRATEGY
class BaseStrategy{
public:
    BaseStrategy(string lobs, string trades,
                 int64_t md_latency,
                 int64_t execution_latency,
                 int64_t delay_,
                 int64_t hold_time_,
                 bool limit_switch_market){
        sim = new Simulator(lobs,
                            trades,
                            md_latency,
                            execution_latency,
                            limit_switch_market);
        delay = delay_;
        hold_time = hold_time_;
    }
    void run(){
        int cnt = 0;  // counter of iterations (optional)

        while(data_type>=0){
            data_type = sim->tick();                                         // update_data, executing previous orders and so on
            auto trade     = sim->get_trade();                               // get trade data
            auto snapshot  = sim->get_snapshot();                            // get snapshot data
            time = max(trade->get_receive_ts(), snapshot->get_receive_ts()); // define strategy time

            // A condition below(data_type==2 || data_type==0) is optional.
            // If we decide place orders if and only if snapshot was changed
            // then we should skip trade changes. But if we decide to place orders
            // depends on other changes, we should comment out this condition
            if((data_type==2 || data_type==0) && time >= prev_time+delay){
                call_place_order(1, snapshot->get_ask_price()[0], 0.001);
                prev_time = time;
            }
            cancel_old_orders();

            /*if(cnt>1e4){
                break;
            }*/
            ++cnt;
        }
        cout << "Count of iterations: " << cnt << endl;
        cout << "Count of placed orders: " << order_counter << endl;
        cout << "Count of assets: " << sim->get_pos() << endl;
        cout << "How much money do we have: " << sim->get_money() << endl;

        print_results(sim->get_own_trades());
    }

private:
    Simulator *sim;                                // Simulator
    short int data_type           = 0;             // value are returned from the function tick() from simulator
                                                   // 0 when we get both "snapshot" and "trade", 1 if when we get only trade
                                                   // 2 - snapshot, -1 if we get an empty line from files (i.e. data was ended)
    unsigned int order_counter    = 0;             // count orders which were tried to place
    int64_t time                  = 0;             // time actual for strategy
    int64_t prev_time             = 0;             // the time we last requested to place an order

    int64_t delay                 = 1e4;           // Delay between placing orders (we don't place orders frequently than
                                                   // 1 time per "delay" nanoseconds)
    int64_t hold_time             = 1e4;           // Orders older than "hold_time" are removed

    list<shared_ptr<Order>> placed_orders;         // A container for memorization orders we placed

    void cancel_old_orders();
    void call_place_order(bool side, double price, double vol);
};

void BaseStrategy::call_place_order(bool side, double price, double vol){
    // Function for placing orders
    // You can use the function place_order from simulator but
    // memorization of placed orders will be up to you
    shared_ptr<Order> order = make_shared<Order>(order_counter, side, price, vol, time);
    sim->place_order(order);
    placed_orders.push_back(order);
    order_counter++;
}

void BaseStrategy::cancel_old_orders(){
    // This function automatically removes all orders older than hold_time
    // You can use the function cancel_order from the simulator instead
    // Don't forget remove an order from the list of placed orders
    for(int i = 0; i<placed_orders.size(); i++){
        if( placed_orders.front()->get_time() + hold_time <= time){
            sim->cancel_order(placed_orders.front()->get_id(), time);
            placed_orders.pop_front();
        }
        else{
            break;
        }
    }
}

#endif //STRATEGY

