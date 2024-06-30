#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>


using namespace std;

struct StockData {
    string date;
    double closePrice;
};

vector<StockData> readCSV(const string& filename,const string& startDate, const string& endDate) {
    vector<StockData> data;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return data;
    }

    string line;
    getline(file, line); // Skip header line
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        StockData stock;
        for (int i = 0; getline(ss, token, ','); ++i) {
            if (i == 0) stock.date = token;
            else if (i == 7) stock.closePrice = stod(token);
        }
        // Check if the date is within the desired range
        if (stock.date >= startDate && stock.date <= endDate) {
            data.push_back(stock);
        }
        // Break the loop if we reached the end date
        if (stock.date > endDate) {
            break;
        }
    }
    file.close();
    return data;
}


//Formatting date
string formatDate(const string& inputDate) {
    istringstream iss(inputDate);
    int year, month, day;
    char dash1, dash2;
    iss >> year >> dash1 >> month >> dash2 >> day;

    ostringstream oss;
    oss << setw(2) << setfill('0') << day << '/' << setw(2) << setfill('0') << month << '/' << year;
    return oss.str();
}

int main() {
    string filename1, filename2;
    cout << "Enter CSV file-1 name: ";
    cin >> filename1;
    cout << "Enter CSV file-2 name: ";
    cin >> filename2;

     double x;
    cout<<"X :";
    cin >> x;


    double n;
    cout<<"n :";
    cin >> n;

    

    double threshold;
    cout<<"Threshold :";
    cin >> threshold;

    string startDate, endDate;
    cout << "Enter start date (YYYY-MM-DD): ";
    cin >> startDate;
    cout << "Enter end date (YYYY-MM-DD): ";
    cin >> endDate;
    vector<StockData> file1Data = readCSV(filename1+".csv",startDate, endDate);
    vector<StockData> file2Data = readCSV(filename2+".csv",startDate, endDate);

    // Assuming both files have the same number of rows
    if (file1Data.size() != file2Data.size()) {
        cerr << "Error: Unequal number of rows in the files." << endl;
        return 1;
    }

    ofstream file1("order_statistics1.csv");
    ofstream file2("order_statistics2.csv");
    ofstream file3("daily_pnl.csv");

    double size = file1Data.size();
    // cout << "\nSize :"<< size << endl;

    
    double total_shares1=0;
    double total_shares2=0;
    double net_profit=0;
    for(int i=0;i<size;i++){
            double current_spread = file1Data[i].closePrice - file2Data[i].closePrice;
            //cout <<"Spread: " << current_spread << endl;

            double count = 0;

            for (int j= 0; j < n; j++) {
                double spread = file1Data[j].closePrice - file2Data[j].closePrice;
                count = count+spread;
            }

            double mean = count/(n);
            //cout<<"Rolling Mean: " << mean << endl;

            double sumOfSquares = 0;

            for (int j = 0; j < n; j++) {
                double spread = file1Data[j].closePrice - file2Data[j].closePrice;
                double deviation = spread - mean;
                sumOfSquares = sumOfSquares + (deviation*deviation);
            }

            double std_dev = sqrt(sumOfSquares/(n));
            //cout<<"Rolling Standard Deviation: " << std_dev << endl;

            double z_score = (current_spread - mean)/(std_dev);
            //cout<<"z_score: " << z_score << endl;


            double close1=file1Data[i].closePrice;
            double close2=file2Data[i].closePrice;

            
            if (z_score>threshold){
                if (total_shares1<=x && total_shares1>(x*(-1))){
                    file1<<formatDate(file1Data[i].date)<<","<<"SELL"<<","<<1<<","<<close1<<"\n";
                    net_profit=net_profit+close1;
                    total_shares1-=1;
                }
                if (total_shares2<x && total_shares2>=(x*(-1))){
                    file2<<formatDate(file2Data[i].date)<<","<<"BUY"<<","<<1<<","<<close2<<"\n";
                    net_profit=net_profit-close2;
                    total_shares2+=1;
                }
            }else if (z_score<-threshold){
                if (total_shares1<x && total_shares1>=(x*(-1))){
                    file1<<formatDate(file1Data[i].date)<<","<<"BUY"<<","<<1<<","<<close1<<"\n";
                    net_profit=net_profit-close1;
                    total_shares1+=1;
                }
                if (total_shares2<=x && total_shares2>(x*(-1))){
                    file2<<formatDate(file2Data[i].date)<<","<<"SELL"<<","<<1<<","<<close2<<"\n";
                    net_profit=net_profit+close2;
                    total_shares2-=1;
                }
            }
            file3<<formatDate(file1Data[i].date)<<","<<net_profit<<"\n"; 
            
        
    }
    file1.close();
    file2.close();
    file3.close();

    //Writing final_pnl.txt
    ofstream file4("final_pnl.txt");
    net_profit+=(total_shares1)*file1Data[size-1].closePrice;
    net_profit+=(total_shares2)*file2Data[size-1].closePrice;
    file4<<net_profit;
    file4.close();
    return 0;
}