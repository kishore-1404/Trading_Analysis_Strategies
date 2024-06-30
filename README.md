# Strategies-for-conducting-trading-analysis

### STRATEGY: "BASIC"

**Implementation Overview:**

- The code reads stock price data from a CSV file and processes it to determine trading signals.
- It calculates the monotonic trend of the price series over the specified number of past days (n).
- Based on the trend, buy or sell orders are generated for each day within the specified date range.
- The code tracks positions and computes net profits over the trading period.

**Implementation:**
To run this strategy, we will run the command:

`make strategy=BASIC symbol=SBIN n=5 x=2 start_date="b" end_date="a"`

The configurable parameters in this are - symbol, n, x, start date and end date. Running this command should create 2 csv files (daily cashflow.csv and order statistics.csv).

**Conclusion:** This code provides a basic framework for implementing a momentum-based trading strategy, allowing users to generate buy or sell signals based on short-term price trends.



### STRATEGY: "DMA"

**Implementation Overview:**

- The code reads stock price data from a CSV file and computes the DMA and standard deviation for each day within the specified date range.
- Based on the DMA and standard deviation, buy or sell orders are generated.
- Position sizes and net profits are tracked throughout the trading period.

**Implementation:**
To run this strategy, we will run the command:

`make strategy=DMA symbol=SBIN n=50 x=3 p=2 start_date="a" end_date="b"`

The configurable parameters in this are - symbol, n, x, p, start date and end date. Running this command should create 2 csv files (daily cashflow.csv and order statistics.csv).

**Conclusion:** This code offers a straightforward implementation of the n-Day Moving Average (DMA) strategy, providing traders with a tool to identify and capitalize on price trends in the market. 

### STRATEGY: "DMA++"

**Implementation Overview:**

- The code reads stock price data from a CSV file and calculates the Efficiency Ratio (ER) and AMA based on the specified parameters.
- It incorporates stop-loss functionality by closing positions forcefully after the maximum holding period if not closed otherwise.
- Trading signals are generated based on the comparison between the current price and the AMA adjusted by the percent change threshold.

**Implementation:**
To run this strategy, we will run the command:

`make strategy="DMA++" symbol=SBIN x=4 p=5 n=14 max_hold_days=28 c1=2 c2=0.2 start_date="a" end_date="b"
`
The configurable parameters in this are - symbol, x, p, n, max hold days, c1, c2, start date and end date. Running this command should create 2 csv files (daily cashflow.csv and order statistics.csv).



**Conclusion:** This code offers an improved DMA strategy with added features to enhance trading performance. By incorporating stop-loss functionality and a smoothing factor for AMA calculation, the strategy becomes more robust and adaptable to dynamic market conditions.

### STRATEGY: "MACD"

**Implementation Overview:**

- The code reads stock price data from a CSV file and calculates the Short EWM, Long EWM, MACD, and Signal Line based on the specified parameters.
- Trading signals are generated based on the comparison between MACD and the Signal Line.
- Positions are opened or closed accordingly, and trading actions (buy/sell) along with quantities and prices are recorded in CSV files.

**Conclusion:** The MACD strategy code offers traders a tool for trend analysis and signal generation based on exponential weighted means of stock prices.

**Implementation:**
To run this strategy, we will run the command:

`make strategy=MACD symbol=SBIN x=3 start_date="a" end_date="b"`

The configurable parameters are x, start date and end date. Running this command should create 2 csv files (daily cashflow.csv and order statistics.csv)


### STRATEGY: "RSI"

**Implementation Overview:**

- The code reads stock price data from a CSV file and calculates the average gain, average loss, RS, and RSI based on the specified parameters.
- Trading signals are generated based on the comparison between RSI and the oversold and overbought thresholds.
- Positions are opened or closed accordingly, and trading actions (buy/sell) along with quantities and prices are recorded in CSV files.

**Implementation:**
To run this strategy, we will run the command:

`make strategy=RSI symbol=SBIN x=3 n=14 oversold_threshold=30 overbought_threshold=70 start_date="a" end_date="b"`

The configurable parameters are x, n, oversold threshold, overbought threshold, start date and end date. Running this command should create 2 csv files (daily cashflow.csv and order statistics.csv).

**Conclusion:** The RSI strategy code offers traders a tool for trend analysis and signal generation based on the Relative Strength Index indicator.

### STRATEGY: "ADX"

**Implementation Overview:**

- The code reads stock price data from a CSV file and calculates TR, +DM, -DM, ATR, DX, and ADX based on the specified parameters.
- Trading signals are generated based on the comparison between ADX and the specified threshold.
- Positions are opened or closed accordingly, and trading actions (buy/sell) along with quantities and prices are recorded in CSV files.

**Implementation:**
To run this strategy, we will run the command:

`make strategy=ADX symbol=SBIN x=3 n=14 adx_threshold=25 start_date="a" end_date="b"`

The configurable parameters are symbol, x, n, ADX threshold, start date, end date. Running this command should create 2 csv files (daily cashflow.csv and order statistics.csv).
  
  **Conclusion:** The ADX strategy code offers traders a tool for trend analysis and signal generation based on the Average Directional Index indicator.




### STRATEGY: "LINEAR_REGRESSION"

**Implementation Overview:**

1. **Data Preparation:**
    - The code reads historical stock data from a CSV file, extracting relevant features for each day.
    - It constructs matrices for the independent variables (x_matrix) and dependent variable (y_matrix) based on the historical data.
2. **Parameter Learning:**
    - The parameters of the linear regression model are learned using the training data.
    - Matrix operations are performed to obtain the coefficients of the regression model.
3. **Prediction and Trading Strategy:**
    - The learned model is applied to predict the close price for each day in the prediction period.
    - Based on the predicted and actual prices, the code decides whether to buy, sell, or skip the stock.
4. **Output Generation:**
    - The code outputs trading decisions and relevant statistics in CSV files (daily_cashflow.csv and order_statistics.csv).

**Implementation:**
To run this strategy, we will run the command:

`make strategy="LINEAR_REGRESSION" symbol=SBIN x=3 p=2 train_start_date="a" train_end_date="b" start_date="c" end_date="d"`

The configurable parameters are - symbol, x, p, train start date, train end date, start date, end date. Running this command should create 2 csv files (daily cashflow.csv and order statistics.csv).

**Conclusion:** This code provides a framework for stock price prediction using linear regression, allowing users to configure parameters and analyze trading decisions based on predicted price movements.

### STRATEGY: "BEST_OF_ALL"

**Implementation Overview:**

- The code runs all strategies concurrently, with each strategy utilizing the specified parameters.
- Each strategy independently reads the stock price data from a CSV file and generates trading signals based on its predefined logic.
- Trading actions, such as buying or selling, along with quantities and prices, are recorded in CSV files for each strategy.
- Net profit or loss is tracked for each strategy over the specified trading period.
- After all strategies have completed execution, the code identifies the strategy with the highest overall PnL and selects it as the best-of-all strategy.
- The selected strategy's trading actions and cash flow information are written to output CSV files.

**Implementation:**
To run this strategy, we will run the command:

`make strategy="BEST_OF_ALL" symbol=SBIN start_date="a" end_date="b"`

The configurable parameters are - symbol, start date and end date. Running this command should create 2 csv files (daily cashflow.csv and order statistics.csv).
  
  **Conclusion:** The best-of-all strategy code offers traders a method to systematically compare and select the most profitable trading strategy among multiple options.This approach allows for efficient strategy comparison and selection, potentially leading to improved trading outcomes.

### STRATEGY: "PAIRS"

**Implementation Overview:**

- The code reads stock price data for two stocks from CSV files and processes them to calculate trading signals.
- It computes the spread, rolling mean, standard deviation, and z-score for each day within the specified date range.
- Based on the z-score, buy or sell signals are generated for each stock in the pair.
- The code tracks positions for both stocks and computes net profits over the trading period.
- It also implements a stop-loss mechanism based on a user-defined threshold to close positions if the spread deviates significantly.

**Implementation:**
To run this strategy, we will run the command:

`make strategy=PAIRS symbol1=SBIN symbol2=ADANIENT x=5 n=20 threshold=2 start_date="a" end_date="b"`

The configurable parameters are - symbol1, symbol2, x, n, threshold, start date and end date. Running this command should create 3 csv files (daily cashflow.csv, order statistics 1.csv, and order statistics 2.csv).

**Conclusion:** This code provides a framework for implementing a mean-reverting pairs trading strategy, allowing users to exploit potential inefficiencies in the relationship between pairs of stocks. 


### STRATEGY: "PAIRS" (stop_loss_threshold)

**Instructions Overview:**

- The stop-loss mechanism is incorporated into a pairs trading strategy, where positions are opened based on the z-score crossing a threshold.
- If the z-score moves in the unexpected direction and crosses the stop-loss threshold, the strategy closes the position to limit potential losses.
- The stop_loss_threshold parameter determines the level at which positions are closed.
- The strategy follows the same intuition, constraints, and assumptions as the basic pairs trading strategy, with an additional parameter for the stop-loss threshold.

**Implementation:**
To run this strategy, we will run the command

`make strategy=PAIRS symbol1=SBIN symbol2=ADANIENT x=5 n=20 threshold=2 stop_loss_threshold=4 start_date="a" end_date="b"`

The configurable parameters are - symbol1, symbol2, x, n, threshold, stop loss threshold, start date and end date. Running this command should create 3 csv files (daily cashflow.csv, order statistics 1.csv, and order statistics 2.csv).

**Conclusion:** The addition of a stop-loss mechanism enhances the pairs trading strategy by introducing a risk management component. This mechanism helps mitigate potential losses by automatically closing positions when the z-score moves unexpectedly beyond a specified threshold.

