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
    double vwap;
    double low;
    double high;
    double noOfTrades;
    double open;
};

vector<StockData> readCSV(const string& filename, const string& startDate, const string& endDate) {
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
            else if(i == 2) stock.open = stod(token);
            else if(i == 3) stock.high = stod(token);
            else if(i == 4) stock.low = stod(token);
            else if(i == 7) stock.closePrice = stod(token);
            else if(i == 8) stock.vwap = stod(token);
            else if(i == 13) stock.noOfTrades = stod(token);
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



// Function to print a matrix
void printMatrix(const vector<vector<double>>& matrix) {
    for (const auto& row : matrix) {
        for (double elem : row) {
            cout << elem << "\t";
        }
        cout << endl;
    }
}

// Function to find the transpose of a matrix
vector<vector<double>> transpose(const vector<vector<double>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    
    // Create a matrix to store the transpose
    vector<vector<double>> transposed(cols, vector<double>(rows));
    
    // Populate the transposed matrix
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            transposed[j][i] = matrix[i][j];
        }
    }
    
    return transposed;
}

// Function to multiply two matrices
vector<vector<double>> matrixMultiply(const vector<vector<double>>& mat1, const vector<vector<double>>& mat2) {
    int rows1 = mat1.size();
    int cols1 = mat1[0].size();
    int cols2 = mat2[0].size();
    
    // Create a intermediate matrix
    vector<vector<double>> intermediate(rows1, vector<double>(cols2, 0.0));
    
    // Perform matrix multiplication
    for (int i = 0; i < rows1; ++i) {
        for (int j = 0; j < cols2; ++j) {
            for (int k = 0; k < cols1; ++k) {
                intermediate[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
    
    return intermediate;
}

// Function to find the determinant of a square matrix
double determinant(const vector<vector<double>>& matrix);

// Function to find the inverse of a square matrix
vector<vector<double>> inverse(const vector<vector<double>>& matrix) {
    int n = matrix.size();
    
    // Create a matrix to store the inverse
    vector<vector<double>> inverseMatrix(n, vector<double>(n));
    
    // Find determinant of the matrix
    double det = determinant(matrix);
    if (det == 0) {
        cout << "Inverse does not exist for this matrix." << endl;
        return inverseMatrix;
    }
    
    // Find adjoint of the matrix
    vector<vector<double>> adjoint(n, vector<double>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            // Get the cofactor of element (i, j)
            vector<vector<double>> cofactor(n - 1, vector<double>(n - 1));
            int x = 0, y = 0;
            for (int row = 0; row < n; ++row) {
                for (int col = 0; col < n; ++col) {
                    if (row != i && col != j) {
                        cofactor[x][y++] = matrix[row][col];
                        if (y == n - 1) {
                            y = 0;
                            ++x;
                        }
                    }
                }
            }
            // Sign of adjoint matrix positive if sum of indices is even.
            // Otherwise negative.
            double sign = ((i + j) % 2 == 0) ? 1 : -1;
            adjoint[j][i] = sign * determinant(cofactor);
        }
    }
    
    // Find inverse of the matrix by dividing adjoint by determinant
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            inverseMatrix[i][j] = adjoint[i][j] / det;
        }
    }
    
    return inverseMatrix;
}



// Function to find the determinant of a square matrix using recursion
double determinant(const vector<vector<double>>& matrix) {
    int n = matrix.size();
    if (n == 1) {
        return matrix[0][0];
    }
    if (n == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
    double det = 0;
    for (int i = 0; i < n; ++i) {
        vector<vector<double>> cofactor(n - 1, vector<double>(n - 1));
        int x = 0, y = 0;
        for (int row = 1; row < n; ++row) {
            for (int col = 0; col < n; ++col) {
                if (col != i) {
                    cofactor[x][y++] = matrix[row][col];
                    if (y == n - 1) {
                        y = 0;
                        ++x;
                    }
                }
            }
        }
        det += (i % 2 == 0 ? 1 : -1) * matrix[0][i] * determinant(cofactor);
    }
    return det;
}




int main() {
    string filename;
    cout << "Enter CSV file name: ";
    cin >> filename;

    double x;
    cout<<"X :";
    cin >> x;

    double p;
    cout<<"P :";
    cin >> p;

    string train_startDate, train_endDate;
    cout << "Enter train start date (YYYY-MM-DD): ";
    cin >> train_startDate;
    cout << "Enter train end date (YYYY-MM-DD): ";
    cin >> train_endDate;

    vector<StockData> trainData = readCSV(filename+".csv",train_startDate,train_endDate);

    

    double days = trainData.size();
    //cout<<"\nDays: "<< days<<endl;


    vector<vector<double>>y_matrix(days-1,vector<double>(1));
    int count1 = days-1;
    for(int i =0;i<days-1;i++){
        if(count1==0){
            break;
        }else{
            y_matrix[i][0] = trainData[count1].closePrice;
            count1--;
        }
        
    }

    // cout<< "\ny_matrix: " << endl;
    // printMatrix(y_matrix);


    
    vector<vector<double>>x_matrix(days-1,vector<double>(8));
    int count = days-1;

    for(int i=0;i<=days;i++){
        if(count==0){
            break;
        }else{
            x_matrix[i][0] = 1;
            x_matrix[i][1] = trainData[count-1].closePrice;
            x_matrix[i][2] = trainData[count-1].open;
            x_matrix[i][3] = trainData[count-1].vwap;
            x_matrix[i][4] = trainData[count-1].low;
            x_matrix[i][5] = trainData[count-1].high;
            x_matrix[i][6] = trainData[count-1].noOfTrades;
            x_matrix[i][7] = trainData[count].open;
            count--;
        }
        
    }
    // cout<< "\nx_matrix: " << endl;
    // printMatrix(x_matrix);


    vector<vector<double>> transposed = transpose(x_matrix);
    // cout<< "\ntrans_matrix: " << endl;
    // printMatrix(transposed);

    
    vector<vector<double>> intermediate = matrixMultiply(transposed, x_matrix);
    // cout<< "\nmulti_matrix: " << endl;
    // printMatrix(intermediate);

    
    vector<vector<double>> invMatrix = inverse(intermediate);
    // cout<< "\ninv_matrix: " << endl;
    // printMatrix(invMatrix);

    
    vector<vector<double>> final = matrixMultiply(invMatrix,transposed);
    // cout<< "\nfinal: " << endl;
    // printMatrix(final);


    vector<vector<double>> result = matrixMultiply(final,y_matrix);
    // cout<< "\nResult: " << endl;
    // printMatrix(result);


    string startDate, endDate;
    cout << "Enter start date (YYYY-MM-DD): ";
    cin >> startDate;
    cout << "Enter end date (YYYY-MM-DD): ";
    cin >> endDate;

    

    vector<StockData> fileData = readCSV(filename+".csv",startDate,endDate);
    double size = fileData.size();
    // cout<<"\nSize: "<< days<<endl;


    ofstream file1("order_statistics.csv");
    ofstream file2("daily_pnl.csv");

    double total_shares=0;
    double net_profit=0;
    for(int i=1;i<size;i++){
        
            vector<vector<double>>new_x_matrix(1,vector<double>(8));
            new_x_matrix[0][0] = 1;
            new_x_matrix[0][1] = trainData[i].closePrice;
            new_x_matrix[0][2] = trainData[i].open;
            new_x_matrix[0][3] = trainData[i].vwap;
            new_x_matrix[0][4] = trainData[i].low;
            new_x_matrix[0][5] = trainData[i].high;
            new_x_matrix[0][6] = trainData[i].noOfTrades;
            new_x_matrix[0][7] = trainData[i-1].open;
            

            // cout<< "\nnew_x_matrix: " << endl;
            // printMatrix(new_x_matrix);


            vector<vector<double>> estimated_close = matrixMultiply(new_x_matrix,result);
            //cout << "\n Answer:" << endl;
            //printMatrix(estimated_close);

            double percentage = ((estimated_close[0][0] - fileData[i-1].closePrice)/(fileData[i-1].closePrice))*100;
            //cout<<"\nPercentage: "<<percentage<<endl;
            double close=fileData[i-1].closePrice;
            
            if (percentage>=p){
                if (total_shares<x && total_shares>=(x*(-1))){
                    file1<<formatDate(fileData[i-1].date)<<","<<"BUY"<<","<<1<<","<<close<<"\n";
                    net_profit-=close;
                    total_shares+=1;
                }
            }else if (percentage<=-p){
                if (total_shares<=x && total_shares>(x*(-1))){
                    file1<<formatDate(fileData[i-1].date)<<","<<"SELL"<<","<<1<<","<<close<<"\n";
                    net_profit+=close;
                    total_shares-=1;
                }
            }
            file2<<formatDate(fileData[i-1].date)<<","<<net_profit<<"\n"; 
    }
    file1.close();
    file2.close();

    //Writing final_pnl.txt
    ofstream file3("final_pnl.txt");
    net_profit+=(total_shares)*fileData[size-1].closePrice;
    file3<<net_profit;
    file3.close();
        
    return 0;
}