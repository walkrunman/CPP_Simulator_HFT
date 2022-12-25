#ifndef DATA_LOADER
#define DATA_LOADER
class DataLoader{
public:
    DataLoader(string filename_lobs, string filename_trades) {
        this->filename_lobs = filename_lobs;
        this->lobs.open(filename_lobs);

        this->filename_trades = filename_trades;
        this->trades.open(filename_trades);

        getline(this->lobs, this->header);
        getline(this->trades, this->header);

    }
    ~DataLoader() {
        this->lobs.close();
        this->trades.close();
    }

    int8_t tick(){
        if(!snapshot_flag && !trade_flag)
        {
            if(receive_ts_trade == receive_ts_snapshot)
            {
                trade_flag    = tick_trade();
                snapshot_flag = tick_snapshot();

                receive_ts_trade = trade_strategy->get_receive_ts();
                receive_ts_snapshot = snapshot_strategy->get_receive_ts();

                return check_return(trade_flag, snapshot_flag, 0);       //return 0 means we return "snapshot" and "trade" simultaneously
            }
            else if(receive_ts_trade < receive_ts_snapshot)
            {
                trade_flag       = tick_trade();
                receive_ts_trade = trade_strategy->get_receive_ts();
                return check_return(trade_flag, snapshot_flag, 1);      //return 1 means we return "trade"
            }
            else
            {
                snapshot_flag = tick_snapshot();
                receive_ts_snapshot = snapshot_strategy->get_receive_ts();
                return check_return(trade_flag, snapshot_flag, 2);      //return 2 means we return "snapshot"
            }
        }
        else if(snapshot_flag && !trade_flag)
        {
            trade_flag       = tick_trade();
            receive_ts_trade = trade_strategy->get_receive_ts();
            return check_return(trade_flag, snapshot_flag, 1);          //return 1 means we return "trade"
        }
        else if(!snapshot_flag && trade_flag)
        {
            snapshot_flag = tick_snapshot();
            receive_ts_snapshot = snapshot_strategy->get_receive_ts();
            return check_return(trade_flag, snapshot_flag, 2);          //return 2 means we return "snapshot"
        }
        else
        {
            return -1;
        }
    }

    std::shared_ptr<Trade> get_trade(){
        return trade_strategy;
    }

    std::shared_ptr<Snapshot> get_snapshot(){
        return snapshot_strategy;
    }

private:
    string      header;
    string      line;

    string      filename_lobs;
    ifstream    lobs;

    string      filename_trades;
    ifstream    trades;

    int8_t   trade_flag     = 0;
    int8_t   snapshot_flag  = 0;

    std::shared_ptr<Trade>    trade_strategy;
    std::shared_ptr<Snapshot> snapshot_strategy;

    int64_t receive_ts_trade    = 0;
    int64_t receive_ts_snapshot = 0;

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
            trade_strategy = make_shared<Trade>();
            trade_strategy->fill_data(line);
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
            snapshot_strategy = make_shared<Snapshot>();
            snapshot_strategy -> fill_data(line);
            return 0;
        }
        else{
            return -1;
        }
    }
};
#endif //DATA_LOADER

