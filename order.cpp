#ifndef ORDER
#define ORDER
class Order{
public:
    Order(int id_, bool aggro_side_, double price_,
          double vol_, long long int ts_){
            id = id_;
            aggro_side  = aggro_side_;
            price       = price_;
            vol         = vol_;
            ts          = ts_;
    }
    ~Order(){}

    long long int get_time()    {return ts;}
    double        get_vol()     {return vol;}
    double        get_price()   {return price;}
    bool          get_side()    {return aggro_side;}
    int           get_id()      {return id;}

    void set_time(long long int time) {ts = time;}

private:
    long long int   ts;
    double          vol;
    double          price;
    int             id;
    bool            aggro_side; //false for bid, true for ask
};
#endif //ORDER


