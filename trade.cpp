#ifndef TRADES
#define TRADES

// Trade Class Container

class Trade: public Ticker{
public:
    Trade(){}
    ~Trade(){}

    int64_t get_exchange_ts() {return exchange_ts;}
    int64_t get_receive_ts() {return receive_ts;}
    double get_price() {return price;}
    double get_vol() {return vol;}
    bool get_side() {return aggro_side;}

    void set_vol(double vol_) {vol = vol_;}

private:
    int64_t exchange_ts;
    int64_t receive_ts;
    double price;
    double vol;
    bool aggro_side; //false for bid, true for ask

    void fill_by_token(string token, int k){
        if (k == 0){
            receive_ts = stoll(token);
        } else if (k == 1){
            exchange_ts = stoll(token);
        } else if (k == 2){

            if (token == "BID"){
                aggro_side = false;
            }
            else{
                aggro_side = true;
            }

        } else if (k == 3) {
            price = stod(token);
        } else if (k == 4) {
            vol = stod(token);
        }
    }

};
#endif //TRADES
