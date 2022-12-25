#ifndef SNAPSHOT
#define SNAPSHOT

// Snapshot Class Container

class Snapshot: public Ticker{
public:
    Snapshot(){}
    ~Snapshot(){}

    int64_t get_receive_ts()  {return receive_ts;}
    int64_t get_exchange_ts() {return exchange_ts;}

    double *get_ask_price() {return ask_price;}
    double *get_ask_vol()   {return ask_vol;}

    double *get_bid_price() {return bid_price;}
    double *get_bid_vol()   {return bid_vol;}

private:
    int64_t receive_ts;
    int64_t exchange_ts;

    double ask_price[10];
    double ask_vol[10];

    double bid_price[10];
    double bid_vol[10];

    void fill_by_token(string token, int k){
        if (k == 0){
                receive_ts = stoll(token);
            } else if (k == 1){
                exchange_ts = stoll(token);
            } else if ((k-2)%4 == 0){
                ask_price[(k-2)/4] = stod(token);
            } else if ((k-2)%4 == 1){
                ask_vol[(k-2)/4] = stod(token);
            } else if ((k-2)%4 == 2){
                bid_price[(k-2)/4] = stod(token);
            } else {
                bid_vol[(k-2)/4] = stod(token);
            }
    }
};
#endif //SNAPSHOT
