#ifndef OWN_TRADE
#define OWN_TRADE
class OwnTrade{
public:
    OwnTrade(bool side_, double price_, double vol_, long long int time_){
        side = side_;
        price = price_;
        vol = vol_;
        time = time_;
    }

    bool get_side() {return side;}
    double get_price(){return price;}
    double get_vol(){return vol;}
    long long int get_time(){return time;}
private:
    bool side;
    double price;
    double vol;
    long long int time;
};
#endif //OWN_TRADE
