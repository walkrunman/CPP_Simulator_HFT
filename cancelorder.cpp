#ifndef CANCAL_ORDER
#define CANCAL_ORDER
class CancelOrder{
public:
    CancelOrder(int id_, int64_t time_){
        id = id_;
        time = time_;
    }
    int get_id(){return id;}
    int64_t get_time(){return time;}

    void set_time(int64_t time) {time = time;}
private:
    int id;
    int64_t time;
};
#endif //CANCAL_ORDER
