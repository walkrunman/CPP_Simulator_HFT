#ifndef DATA_LOADER
#define DATA_LOADER
class DataLoader{
public:
    DataLoader(string filename_lobs, string filename_trades) {
        filename_lobs = filename_lobs;
        lobs.open(filename_lobs);

        filename_trades = filename_trades;
        trades.open(filename_trades);

        getline(lobs, header);
        getline(trades, header);

    }
    ~DataLoader() {
        lobs.close();
        trades.close();
    }

    int8_t tick(){
        if(!snapshot_flag && !trade_flag)
        {
            if(receive_ts_trade == receive_ts_snapshot)
            {
                trade_flag    = tick_trade();
                snapshot_flag = tick_snapshot();

                receive_ts_trade = trade->get_receive_ts();
                receive_ts_snapshot = snapshot->get_receive_ts();

                return check_return(trade_flag, snapshot_flag, 0);       //return 0 means we return "snapshot" and "trade" simultaneously
            }
            else if(receive_ts_trade < receive_ts_snapshot)
            {
                trade_flag       = tick_trade();
                receive_ts_trade = trade->get_receive_ts();
                return check_return(trade_flag, snapshot_flag, 1);      //return 1 means we return "trade"
            }
            else
            {
                snapshot_flag = tick_snapshot();
                receive_ts_snapshot = snapshot->get_receive_ts();
                return check_return(trade_flag, snapshot_flag, 2);      //return 2 means we return "snapshot"
            }
        }
        else if(snapshot_flag && !trade_flag)
        {
            trade_flag       = tick_trade();
            receive_ts_trade = trade->get_receive_ts();
            return check_return(trade_flag, snapshot_flag, 1);          //return 1 means we return "trade"
        }
        else if(!snapshot_flag && trade_flag)
        {
            snapshot_flag = tick_snapshot();
            receive_ts_snapshot = snapshot->get_receive_ts();
            return check_return(trade_flag, snapshot_flag, 2);          //return 2 means we return "snapshot"
        }
        else
        {
            return -1;
        }
    }

    std::shared_ptr<Trade> get_trade(){
        return trade;
    }

    std::shared_ptr<Snapshot> get_snapshot(){
        return snapshot;
    }

private:
    string      header;
    string      line;

    string      filename_lobs;
    ifstream    lobs;

    string      filename_trades;
    ifstream    trades;

    int8_t check_return(int8_t flag1,
                        int8_t flag2,
                        int8_t normal_value){
        if(flag1*flag2){
            return int8_t(-1);
        }
        else{
            return normal_value;
        }
    }

    int8_t tick_trade(){
        if(!this->trades.eof()){
            getline(this->trades, line);
            if(line==""){return -1;}
            trade = make_shared<Trade>();
            trade->fill_data(line);
            return 0;
        }
        else{
            return -1;
        }
    }

    int8_t tick_snapshot(){
        if(!this->lobs.eof()){
            getline(this->lobs, line);
            if(line==""){return -1;}
            snapshot = make_shared<Snapshot>();
            snapshot -> fill_data(line);
            return 0;
        }
        else{
            return -1;
        }
    }

protected:
    int8_t   trade_flag     = 0;
    int8_t   snapshot_flag  = 0;

    std::shared_ptr<Trade>    trade;
    std::shared_ptr<Snapshot> snapshot;

    int64_t receive_ts_trade    = 0;
    int64_t receive_ts_snapshot = 0;
};

class Loader : public DataLoader
{
public:
    Loader(string filename_lobs, string filename_trades):
        DataLoader::DataLoader(filename_lobs, filename_trades){}

    int8_t tick(){
        tick_flag = DataLoader::tick();

        if(not_first_iteration){
            if(snapshot->get_receive_ts() > trade->get_receive_ts()){
                receive_time    = trade->get_receive_ts();
                exchange_time   = trade->get_exchange_ts();
                trade_actual    = trade;
            } else if(snapshot->get_receive_ts() < trade->get_receive_ts()){
                receive_time    = snapshot->get_receive_ts();
                exchange_time   = snapshot->get_exchange_ts();
                snapshot_actual = snapshot;
            } else{
                receive_time    = snapshot->get_receive_ts();
                exchange_time   = snapshot->get_exchange_ts();
                snapshot_actual = snapshot;
                trade_actual    = trade;
            }
        } else{
            receive_time    = min(snapshot->get_receive_ts(), trade->get_receive_ts());
            exchange_time   = min(snapshot->get_exchange_ts(), trade->get_exchange_ts());
            snapshot_actual = snapshot;
            trade_actual    = trade;
            not_first_iteration = true;
        }

        return tick_flag;
    }

    int64_t get_receive_time(){return receive_time;}
    int64_t get_exchange_time() {return exchange_time;}

    std::shared_ptr<Trade> get_trade() {return trade_actual;}
    std::shared_ptr<Snapshot> get_snapshot() {return snapshot;}

private:
    std::shared_ptr<Trade>    trade_actual;
    std::shared_ptr<Snapshot> snapshot_actual;

    int64_t receive_time;
    int64_t exchange_time;
    int8_t  tick_flag;
    bool    not_first_iteration = false;
};
#endif //DATA_LOADER



