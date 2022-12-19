#ifndef TICKER
#define TICKER

// This class is class-container for market data or trades data

class Ticker{
public:
    Ticker(){}
    virtual ~Ticker(){}

    void fill_data(string s){
        size_t pos = 0;
        string token;
        int k = 0;
        while ((pos = s.find(delimiter)) != string::npos) {
            token = s.substr(0, pos);
            s.erase(0, pos + delimiter.length());
            fill_by_token(token, k);
            k++;
        }
        token = s.substr(0, pos);
        fill_by_token(token, k);
    }
protected:
    const string delimiter = ",";
private:
    virtual void fill_by_token(string token, int k){}
};
#endif //SNAPSHOT
