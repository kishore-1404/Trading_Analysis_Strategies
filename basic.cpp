#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

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


//checking monotonicity
int check_monotonic(vector<vector<string>> data,int index,int n){
    int start=index-n+1;
    int end=index;
    int monotone=0;
    if (std::stod(data[start][7])>std::stod(data[start+1][7])){
        monotone=-1;
    }else{
        monotone=1;
    }
    for (int row_index=start+1;row_index<index;row_index++){
        if (monotone==1){
            if (std::stod(data[row_index][7])>=std::stod(data[row_index+1][7])){
                monotone=0;
                break;
            }
        }
        if (monotone==-1){
            if (std::stod(data[row_index][7])<=std::stod(data[row_index+1][7])){
                monotone=0;
                break;
            }
        }

    }
    return monotone;
}

void Basic(string input_file,string output_file1,string output_file2,int n,int x,string start_date){
    vector<vector<string>> data;
    readCSV(input_file,data);
        // for (int i=0;i<data.size();i++) {
        // // for (const auto& cell : row) {
        //     std::cout << data[i][7]<< ",";
        // // }
        // std::cout << std::endl;
        // }

    int start_index=1;
    for (int row_index=1;row_index<data.size();row_index++){
        if (data[row_index][0]==start_date){
            start_index=row_index;
            break;
        }
    }
    ofstream file1(output_file1);
    ofstream file2(output_file2);
    if (file1.is_open() && file2.is_open()){
        file1<<"Date,Order_dir,Quantity,Price"<<"\n";
        file2<<"Date,Cashflow"<<"\n";
        double total_shares=0;
        double net_profit=0;
        for (int row_index=start_index;row_index<data.size();row_index++){
            int monotone=check_monotonic(data,row_index,n);
            double close=std::stod(data[row_index][7]);
            if(monotone==1){
                if (total_shares<x && total_shares>=(x*(-1))){
                    file1<<formatDate(data[row_index][0])<<","<<"BUY"<<","<<1<<","<<close<<"\n";
                    net_profit-=close;
                    total_shares+=1;
                }
            }
            if (monotone==-1){
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
    cin>>strategy>>symbol>>n>>x>>start_date>>end_date;
    if (start_date<=end_date){
        //writing order_statisics
        string output_file1="order_statistics.csv";
        string output_file2="daily_pnl.csv";
        string input_file=symbol+".csv";
        Basic(input_file,output_file1,output_file2,n,x,start_date);
        
    return 0;
    }

}
