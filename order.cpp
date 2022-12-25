#ifndef ORDER
#define ORDER
class Order{
public:
    Order(int id_, bool aggro_side_,
          double price_, double vol_, int64_t ts_){
            id = id_;
            aggro_side  = aggro_side_;
            price       = price_;
            vol         = vol_;
            ts          = ts_;
    }
    ~Order(){}

    int64_t       get_time()    {return ts;}
    double        get_vol()     {return vol;}
    double        get_price()   {return price;}
    bool          get_side()    {return aggro_side;}
    int           get_id()      {return id;}

    void set_vol(double vol_)   {vol = vol_;}

private:
    int64_t         ts;
    double          vol;
    double          price;
    int             id;
    bool            aggro_side; //false for bid, true for ask
};
#endif //ORDER


