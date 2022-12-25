#ifndef OWN_TRADE
#define OWN_TRADE
class OwnTrade{
public:
    OwnTrade(bool side_, double price_, double vol_, int64_t time_,
             bool trading_way_){
        side        = side_;
        price       = price_;
        vol         = vol_;
        time        = time_;
        trading_way = trading_way_;
    }

    int64_t get_time()        {return time;}
    double  get_price()       {return price;}
    double  get_vol()         {return vol;}
    bool    get_side()        {return side;}
    bool    get_trading_way() {return trading_way;}
private:
    int64_t time;                  // time in the simulator, i.e. on an stock exchange
    double price;                  // price
    double vol;                    // volume
    bool side;                     // 0 means bid, 1 means ask;
    bool trading_way;              // 0 means limit, 1 means market
};
#endif //OWN_TRADE
