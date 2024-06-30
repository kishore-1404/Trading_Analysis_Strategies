from datetime import date
from datetime import datetime,timedelta
import pandas as pd 
from jugaad_data.nse import stock_df
import sys
import subprocess

def compile_and_run_cpp(args):
    # Compile test.cpp using g++
    compile_process = subprocess.Popen(["g++", args[1]+".cpp", "-o", "a.out"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    compile_output, compile_error = compile_process.communicate()

    # Check if compilation was successful
    if compile_process.returncode == 0:
        print("Compilation successful.")
        # Run the compiled executable with command-line arguments
        run_process = subprocess.Popen(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        run_output, run_error = run_process.communicate()
        print("Output of the program:")
        print(run_output.decode())
        if run_error:
            print("Error occurred while running the program:")
            print(run_error.decode())
    else:
        print("Compilation failed:")
        print(compile_error.decode())

def reformat_date(input_date):
    # Parse input date string
    date_obj = datetime.strptime(input_date, '%d/%m/%Y')
    # Format date as "YYYY-MM-DD"
    output_date = date_obj.strftime('%Y-%m-%d')
    return output_date

def subtract_days(input_date, n_days):
    # Parse input date string
    date_obj = datetime.strptime(input_date, '%d/%m/%Y')
    # Subtract n days from the input date
    new_date = date_obj - timedelta(days=n_days)
    # Format the resulting date as "DD/MM/YYYY"
    output_date = new_date.strftime('%d/%m/%Y')
    return output_date

# Function to get daily data for the last x years and save it to different file formats
def get_and_save_stock_data(symbol,start_date,end_date):

    end_date=reformat_date(end_date)
    start_date=reformat_date(start_date)
    end_date=datetime.strptime(end_date,"%Y-%m-%d")
    start_date=datetime.strptime(start_date,"%Y-%m-%d")

    # Download stock data to pandas dataframe using jugaad-data library
    df=stock_df(symbol, start_date, end_date, series="EQ")
    df = df.iloc[::-1]
    print(df)

    # Save data to CSV
    csv_file = f"{symbol}.csv"
    df.to_csv(csv_file, index=False)

   
strategy=str(sys.argv[1])
if strategy=="BASIC":
    symbol=str(sys.argv[2])
    n=str(sys.argv[3])
    x=str(sys.argv[4])
    start_date=str(sys.argv[5])
    end_date=str(sys.argv[6])
    args = ["./a.out", "BASIC", symbol, n, x, start_date, end_date]
    get_and_save_stock_data(symbol,subtract_days(start_date,3*n),end_date)
    compile_and_run_cpp(args)


if srategy=="DMA":
    symbol=str(sys.argv[2])
    n=(sys.argv[3])
    x=(sys.argv[4])
    p=(sys.argv[5])
    start_date=str(sys.argv[6])
    end_date=str(sys.argv[7])
    args = ["./a.out", "DMA", symbol, n, x, p, start_date, end_date]
    get_and_save_stock_data(symbol,subtract_days(start_date,3*n),end_date)
    compile_and_run_cpp(args)
    

if strategy=="DMA++":
    symbol=str(sys.argv[2])
    x=(sys.argv[3])
    p=(sys.argv[4])
    n=(sys.argv[5])
    max_hold_days=(sys.argv[6])
    c1=(sys.argv[7])
    c2=(sys.argv[8])
    start_date=(sys.argv[9])
    end_date=(sys.argv[10])
    args = ["./a.out", "DMA++", symbol, x, p, n, max_hold_days, c1, c2, start_date, end_date]
    get_and_save_stock_data(symbol,subtract_days(start_date,3*n),end_date)
    compile_and_run_cpp(args)


if strategy=="MACD":
    symbol=str(sys.argv[2])
    x=(sys.argv[3])
    start_date=str(sys.argv[4])
    end_date=str(sys.argv[5])
    args = ["./a.out", "MACD", symbol, x, start_date, end_date]
    get_and_save_stock_data(symbol,subtract_days(start_date,10),end_date)
    compile_and_run_cpp(args)


if strategy=="RSI":
    symbol=str(sys.argv[2])
    x=(sys.argv[3])
    n=(sys.argv[4])
    oversold_threshold=(sys.argv[5])
    overbought_threshold=(sys.argv[6])
    start_date=str(sys.argv[7])
    end_date=str(sys.argv[8])
    args = ["./a.out", "RSI", symbol, x, n, oversold_threshold, overbought_threshold, start_date, end_date]
    get_and_save_stock_data(symbol,subtract_days(start_date,3*n),end_date)
    compile_and_run_cpp(args)


if strategy=="ADX":
    symbol=str(sys.argv[2])
    x=(sys.argv[3])
    p=(sys.argv[4])
    oversold_threshold=(sys.argv[5])
    start_date=str(sys.argv[6])
    end_date=str(sys.argv[7])
    args = ["./a.out", "ADX", symbol, x, p, oversold_threshold, start_date, end_date]
    get_and_save_stock_data(symbol,subtract_days(start_date,10),end_date)
    compile_and_run_cpp(args)


if strategy="LINEAR_REGRESSION":
    symbol=str(sys.argv[2])
    x=(sys.argv[3])
    p=(sys.argv[4])
    train_start_date=str(sys.argv[5])
    train_end_date=str(sys.argv[6])
    start_date=str(sys.argv[7])
    end_date=str(sys.argv[8])
    args = ["./a.out", "LINEAR_REGRESSION", symbol, x, p, train_start_date, train_end_date, start_date, end_date]
    get_and_save_stock_data(symbol,subtract_days(start_date,10),end_date)
    compile_and_run_cpp(args)

if strategy="BEST_OF_ALL":
    symbol=str(sys.argv[2])
    start_date=str(sys.argv[3])
    end_date=str(sys.argv[4])
    args = ["./a.out", "BEST_OF_ALL", symbol, start_date, end_date]
    get_and_save_stock_data(symbol,subtract_days(start_date,40),end_date)
    compile_and_run_cpp(args)

if strategy="PAIRS":
    symbol1=str(sys.argv[2])
    symbol2=str(sys.argv[3])
    x=(sys.argv[4])
    n=(sys.argv[5])
    threshold=(sys.argv[6])
    stop_loss_threshold=(sys.argv[7])
    start_date=""
    end_date=""
    if len(stop_loss_threshold)<10:
        start_date=(sys.argv[8])
        end_date=(sys.argv[9])
    else:
        start_date=stop_loss_threshold
        stop_loss_threshold=str(214748364)
        end_date=sys.argv[8]
    get_and_save_stock_data(symbol1,subtract_days(start_date,2*n),end_date)
    get_and_save_stock_data(symbol2,subtract_days(start_date,2*n),end_date)
    args = ["./a.out", "PAIRS", symbol1, symbol2, x, n, threshold, stop_loss_threshold, start_date, end_date]
    compile_and_run_cpp(args)


