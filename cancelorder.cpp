#ifndef CANCAL_ORDER
#define CANCAL_ORDER
class CancelOrder{
public:
    CancelOrder(int id_, long long int time_){
        id = id_;
        time = time_;
    }
    int get_id(){return id;}
    long long int get_time(){return time;}

    void set_time(long long int time) {time = time;}
private:
    int id;
    long long int time;
};
#endif //CANCAL_ORDER
