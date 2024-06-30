from datetime import date
from datetime import datetime
import pandas as pd 
from jugaad_data.nse import stock_df
import sys



# Function to get daily data for the last x years and save it to different file formats
def get_and_save_stock_data(symbol,start_date,end_date):

    end_date=datetime.strptime(end_date,"%Y-%m-%d")
    start_date=datetime.strptime(start_date,"%Y-%m-%d")

    # Download stock data to pandas dataframe using jugaad-data library
    df=stock_df(symbol, start_date, end_date, series="EQ")
    df = df.iloc[::-1]
    print(df)

    # Save data to CSV
    csv_file = f"{symbol}.csv"
    df.to_csv(csv_file, index=False)

   
StockSymbol=str(input())
start_date=str(input())
end_date=str(input())

get_and_save_stock_data(StockSymbol,start_date,end_date)
