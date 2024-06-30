#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

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

// Calculating the average gain and average loss
double get_avg_gain(vector<vector<string>> data , int start_index ,int end_index ,int n){
    double sum=0;
    for (int row_index=start_index-1;row_index<end_index;row_index++){
        sum+=std::max((stod(data[row_index+1][7])-stod(data[row_index][7])),0.0);
    }
    return sum/n;
}

double get_avg_loss(vector<vector<string>> data , int start_index ,int end_index ,int n){
    double sum=0;
    for (int row_index=start_index-1;row_index<end_index;row_index++){
        sum+=abs(min((stod(data[row_index+1][7])-stod(data[row_index][7])),0.0));
    }
    return sum/n;
}


void RSI(string input_file,string output_file1,string output_file2,int n,int x,double overbought_threshold,double oversold_threshold,string start_date){
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

            // Initiating start and end for calculating avg gain and loss 
            int start=row_index-n+1;
            int end=row_index;
            double close=std::stod(data[row_index][7]);
            double avg_gain=get_avg_gain(data,start,end,n);
            double avg_loss=get_avg_loss(data,start,end,n);

            //Calculating RS and RSI
            double RS=avg_gain/avg_loss;
            double RSI=100-(100/(1+RS));

            //Writing to files accordingly
            if (RSI<oversold_threshold){
                if (total_shares<x && total_shares>=(x*(-1))){
                    file1<<formatDate(data[row_index][0])<<","<<"BUY"<<","<<1<<","<<close<<"\n";
                    net_profit-=close;
                    total_shares+=1;
                }
            }else if (RSI>overbought_threshold){
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
    int n,x,p;
    double overbought_threshold,oversold_threshold;
    cin>>strategy>>symbol>>n>>x>>overbought_threshold>>oversold_threshold>>start_date>>end_date;
    if (start_date<=end_date){
        //writing order_statisics
        string output_file1="order_statistics.csv";
        string output_file2="daily_pnl.csv";
        string input_file=symbol+".csv";
        RSI(input_file,output_file1,output_file2,n,x,overbought_threshold,oversold_threshold,start_date);   
    return 0;
    }

}
