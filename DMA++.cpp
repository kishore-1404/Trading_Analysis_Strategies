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

// Calculating the ER(efficiency ratio)
double get_ER(vector<vector<string>> data ,double n,double index){
    double start=index-n+1;
    double change_over_n_days=abs(std::stod(data[index][7])-std::stod(data[start][7])); //change over ndays including today
    double sum_of_changes=0;
    for (double row_index=start;row_index<index;row_index++){
        sum_of_changes+=abs(std::stod(data[row_index][7])-std::stod(data[row_index+1][7]));
    }
    double ER=change_over_n_days/sum_of_changes;
    return ER;
}

//Calculation SF(smooothing factor)
double get_SF(double t,double c1,double c2,double ER){
    if (t==0){
        return 0.5;
    }else{
        double temp=((2*ER)/(1+c2));
        double SF= ((1-c1)*get_SF(t-1,c1,c2,ER))+(c1*((temp-1)/temp+1));
        return SF;
    }
}

double get_AMA(vector<vector<string>> data,double t,double start_index,double c1,double c2,double ER){
    if (t==0){
        return stod(data[start_index][7]);
    }else{
        double SF=get_SF(t,c1,c2,ER);
        double Pt=stod(data[start_index+t][7]);
        double AMA=(get_AMA(data,t-1,start_index,c1,c2,ER)*(1-SF))+(SF*(Pt));
        return AMA;
    }
}


void DMA2(string input_file,string output_file1,string output_file2,double n,double x,double p,double c1,double c2,double max_hold_days,string start_date){
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
    double start_index=1;
    for (double row_index=1;row_index<data.size();row_index++){
        if (data[row_index][0]==start_date){
            start_index=row_index;
            break;
        }
    }
    double total_shares=0;
    double net_profit=0;
    double count=0;

    for (double row_index=start_index;row_index<data.size();row_index++){

        double ER=get_ER(data,n,start_index);
        double SF=get_SF(row_index-start_index,c1,c2,ER);
        double AMA=get_AMA(data,row_index-start_index,start_index,c1,c2,ER);
        double close=std::stod(data[row_index][7]);

        if (total_shares!=0){
            count++;
        }

        if (count==max_hold_days){
            if (total_shares>0){
                file1<<formatDate(data[row_index][0])<<","<<"SELL"<<","<<abs(total_shares)<<","<<close<<"\n";
                net_profit+=total_shares*close;
                total_shares=0;
                count=0;
            }
            if (total_shares<0){
                file1<<formatDate(data[row_index][0])<<","<<"BUY"<<","<<abs(total_shares)<<","<<close<<"\n";
                net_profit+=total_shares*close;
                total_shares=0;
                count=0;
            }
        }

        if (close>=AMA+p){
            if (total_shares<x && total_shares>=(x*(-1))){
                file1<<formatDate(data[row_index][0])<<","<<"BUY"<<","<<1<<","<<close<<"\n";
                net_profit-=close;
                total_shares+=1;
            }
        }else if (AMA>=close+p){
            if (total_shares<=x && total_shares>(x*(-1))){
                file1<<formatDate(data[row_index][0])<<","<<"SELL"<<","<<1<<","<<close<<"\n";
                net_profit+=close;
                total_shares-=1;
            }
        }
        file2<<formatDate(data[row_index][0])<<","<<net_profit<<"\n";

    }
       
    //Writing final_pnl.txt
    ofstream file3("final_pnl.txt");
    net_profit+=(total_shares)*(stod(data[data.size()-1][7]));
    file3<<net_profit;
    file3.close();
   
        
    file1.close();
    file2.close();

    }
}

int main(){
//taking the required inputs for Basic Strategy
    string end_date,start_date,strategy,symbol;
    double n,x,p,c1,c2,max_hold_days;
    cin>>strategy>>symbol>>n>>x>>p>>c1>>c2>>max_hold_days>>start_date>>end_date;
    if (start_date<=end_date){
        //writing order_statisics
        string output_file1="order_statistics.csv";
        string output_file2="daily_pnl.csv";
        string input_file=symbol+".csv";
        DMA2(input_file,output_file1,output_file2,n,x,p,c1,c2,max_hold_days,start_date);
        
    return 0;
    }

}
