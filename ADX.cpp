#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>


using namespace std;

int ADX=0;
/*Indexes
DATE    0
SERIES  1
OPEN    2
HIGH    3
LOW   4
PREV. CLOSE  5  
LTP     6
CLOSE   7
VWAP    8
52W H   9
52W L   10
VOLUME  11
VALUE   12
NO OF TRADES  13  
SYMBOL   14*/

void readCSV(const string& filename,vector<vector<string>>& data) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename <<endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        vector<string> row;
        istringstream iss(line);
        string cell;

        while (getline(iss, cell, ',')) {
            row.push_back(cell);
        }

        data.push_back(row);
    }

    file.close();
}

string formatDate(const string& inputDate) {
    istringstream iss(inputDate);
    int year, month, day;
    char dash1, dash2;
    iss >> year >> dash1 >> month >> dash2 >> day;

    ostringstream oss;
    oss << setw(2) << setfill('0') << day << '/' << setw(2) << setfill('0') << month << '/' << year;
    return oss.str();
}


double get_TR(vector<vector<string>>data,int t,int start_index){
    double high=stod(data[start_index+t][3]);
    double low=stod(data[start_index+t][4]);
    double prevClose=stod(data[start_index+t][5]);

    return max(high-low,max(high-prevClose,low-prevClose));
    
}

double get_DMhigh(vector<vector<string>>data,int t,int start_index){
    double high=stod(data[start_index+t][3]);
    double prevhigh=stod(data[start_index+t-1][3]);

    return max(high,prevhigh);
}

double get_DMlow(vector<vector<string>>data,int t,int start_index){
    double low=stod(data[start_index+t][4]);
    double prevlow=stod(data[start_index+t-1][4]);

    return max(low,prevlow);
}

double get_ATR(vector<vector<string>>data,int t,int start_index,int n){
    double a=2/static_cast<double>(n+1);
    if (t==0){
        return get_TR(data,t,start_index);
    }else{
        double ATR=((1-a)*get_ATR(data,t-1,start_index,n))+(a*get_TR(data,t,start_index));
        return ATR;
    }
}

double get_DX(vector<vector<string>> data,int t,int start_index,int n){
    double DMhigh=get_DMhigh(data,t,start_index);
    double DMlow=get_DMlow(data,t,start_index);
    double ATR=get_ATR(data,t,start_index,n);
    double DIplus=DMhigh/ATR;
    double DIminus=DMlow/ATR;

    double DX=100*((DIplus-DIminus)/(DIplus+DIminus));
    return DX;
}

double get_ADX(vector<vector<string>>data,int t,int start_index,int n){
    double a=2/static_cast<double>(n+1);
    if (t==0){
        ADX=get_DX(data,t,start_index,n);
    }else{
        double ADX=((1-a)*ADX)+(a*get_DX(data,t,start_index,n));
    }
    return ADX;
}


void ADXfunc(string input_file,string output_file1,string output_file2,int n,int x,double adx_threshold,string start_date){
    //reading the csv file
    vector<vector<string>> data;
    readCSV(input_file,data);


    //Writing output csv files as given    
    ofstream file1(output_file1);
    ofstream file2(output_file2);
   
    if (file1.is_open() && file2.is_open()){
        file1<<"Date,Order_dir,Quantity,Price"<<"\n";
        file2<<"Date,Cashflow"<<"\n";

    //Getting the index of start_date 
    int start_index=1;
    for (int row_index=1;row_index<data.size();row_index++){
        if (data[row_index][0]==start_date){
            start_index=row_index;
            break;
        }
    }

    //Getting signals
        double total_shares=0;
        double net_profit=0;
        for (int row_index=start_index;row_index<data.size();row_index++){
            

            double close=std::stod(data[row_index][7]);
            double ADX=get_ADX(data,row_index-start_index,start_index,n);


            if (ADX>adx_threshold){
                if (total_shares<x && total_shares>=(x*(-1))){
                    file1<<formatDate(data[row_index][0])<<","<<"BUY"<<","<<1<<","<<close<<"\n";
                    net_profit-=close;
                    total_shares+=1;
                }
            }else if(ADX<adx_threshold){
                if (total_shares<=x && total_shares>(x*(-1))){
                    file1<<formatDate(data[row_index][0])<<","<<"SELL"<<","<<1<<","<<close<<"\n";
                    net_profit+=close;
                    total_shares-=1;
                }
            }
            file2<<formatDate(data[row_index][0])<<","<<net_profit<<"\n";
         }
    file1.close();
    file2.close();

    //Writing final_pnl.txt
    ofstream file3("final_pnl.txt");
    net_profit+=(total_shares)*(stod(data[data.size()-1][7]));
    file3<<net_profit;
    file3.close();

    }
}

int main(){
//taking the required inputs for Basic Strategy
    string end_date,start_date,strategy,symbol;
    int n,x;
    double adx_threshold;
    cin>>strategy>>symbol>>n>>x>>adx_threshold>>start_date>>end_date;
    if (start_date<=end_date){
        //writing order_statisics
        string output_file1="order_statistics.csv";
        string output_file2="daily_pnl.csv";
        string input_file=symbol+".csv";
        ADXfunc(input_file,output_file1,output_file2,n,x,adx_threshold,start_date);
        
    return 0;
    }

}
