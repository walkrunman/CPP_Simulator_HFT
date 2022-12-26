#ifndef UTILS
#define UTILS
void print_results(list<std::shared_ptr<OwnTrade>> own_trade,
                   string path_results = "results.csv"){
    ofstream trades;
    trades.open(path_results);
    trades << "time;side;price;vol;mid_price;trading_way"<< endl;
    for(auto it=own_trade.begin(); it!=own_trade.end(); ++it){
        trades <<
        (*(*it)).get_time()         << ";" <<
        (*(*it)).get_side()         << ";" <<
        (*(*it)).get_price()        << ";" <<
        (*(*it)).get_vol()          << ";" <<
        (*(*it)).get_mid_price()    << ";" <<
        (*(*it)).get_trading_way()  << endl;
    }
    trades.close();
}

template <typename T>
void print_id(list<std::shared_ptr<T>> container){
    for(auto it=container.begin(); it!=container.end(); ++it){
        cout << (*(*it)).get_id() << " ";
    }
    cout << endl;
}

template <typename T>
void print_time(list<std::shared_ptr<T>> container){
    for(auto it=container.begin(); it!=container.end(); ++it){
        cout << (*(*it)).get_time() << " ";
    }
    cout << endl;
}

template <typename T>
void print_price(list<std::shared_ptr<T>> container){
    for(auto it=container.begin(); it!=container.end(); ++it){
        cout << (*(*it)).get_price() << " ";
    }
    cout << endl;
}
#endif //UTILS
