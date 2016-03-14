#include <iostream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <ostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <time.h>

using namespace std;

struct TradeDataStruct
{
	int shares; 
	int buyIndex;
	int sellIndex;
	double buyPrice;
	double sellPrice;
	double netBuyCost;
	double netSellCost;
	double netGainLoss;
	double stopLoss;
	string ticker;
	string buyDate;
	string sellDate;
};


int main()
{	

	ifstream dateFile("data/dates.txt");
	ifstream tickerFile("data/tickers.txt");
	ifstream pricesFile("data/prices.txt");
	
	int rows, cols;
	
	vector<string> dates;
	vector<string> tickers;
	
	// Populate dates vector
	for (rows = 0; !dateFile.eof(); rows++)
	{
		string tempString;
		dateFile >> tempString;
		dates.push_back(tempString);	
	}
	dateFile.close();
	
	// Populate dates vector	
	for (cols = 0; !tickerFile.eof(); cols++)
	{
		string tempString;
		tickerFile >> tempString;
		tickers.push_back(tempString);	
	}
	tickerFile.close();
	
	// create appropriately sized prices column
	double prices[rows][cols];
	
	// Populate prices array
	while (!pricesFile.eof())
	{
		for(int i = 0; i < rows; i++)
		{
			int lnLength = cols*15;
			char buffer[lnLength];
			stringstream varStream;
			pricesFile.getline(buffer, lnLength);
			varStream << buffer;
			for(int j = 0; j < cols; j++)
			{
				varStream >> prices[i][j];			
			}
		}
	}

	// Display & Test
	/*
	cout << setw(17) << "Date";
	for(int j = 0; j < cols; j++)
	{
		cout << setw(9) << tickers[j];
	}
	
	cout << endl;
	std::cout << std::fixed;
    std::cout << std::setprecision(2);

	for(int i = 0; i < rows; i++)
	{
		cout << setw (5) << i << setw(12) << dates[i];
		for(int j = 0; j < cols; j++)
		{
			cout << setw(9) << prices[i][j];
		}
		cout << endl;
	}
	*/
	
	if (remove("data/massTradeLog.txt")) perror("Error");
	ofstream massTradeLog("data/massTradeLog.txt", ios::app);
		
	if (remove("data/tradeLog.txt")) perror("Error");
	ofstream tradeLog("data/tradeLog.txt", ios::app);
	
	srand (time(NULL));
	
	double initialInvestment = 7000;
	double stopLossWeight = .75; // percent below stop loss a stock get's sold for;
	double stopLossMargin = .001;
	massTradeLog << "All simulations are averaged over 10000 trials on an 8 year virtual timeline." << endl << endl;
	
	for(stopLossMargin = .001; stopLossMargin < .030; stopLossMargin=stopLossMargin + .0005)
	{
		int simCount = 1000;
		double avgWorth;
		double sumWorth = 0;
		double maxWorth = 0;
		double minWorth = 1000000;
		int lossCount = 0;
		int millionaireCount = 0;
		for(int i = 0; i < simCount; i++)
		{
			TradeDataStruct currentTrade;
			
			int day = rand() % 100;
			double tradeFee = 9.00;		
			double invested = 0.00;						// set initial investment value at amount
			double liquid = initialInvestment;
			double netWorth = invested+liquid;
			int tradeCount = 0;							// start at 0 counting through the tradeData vector
			bool inTrade = false;
			int lastStock = cols; 						// ensures no stock will be bought twice in a row, and 
			int stockIndex = lastStock;				// no stock is excluded from the first trade
			int tradeIndex = 0;
			
			std::cout << std::fixed;
			std::cout << std::setprecision(2);
		
			for(int day = 0; day < rows; day++)
			{	
				if(!inTrade)	// buy max value of random stock
				{			
					while(stockIndex == lastStock)
					{
						stockIndex = rand() % (cols-1);		// pick a stock at random;
					} 
					
					lastStock = stockIndex;
									
					currentTrade.ticker = tickers[stockIndex];
					tradeIndex++;
					
					currentTrade.buyIndex = day;
					currentTrade.buyDate = dates[day];
					currentTrade.buyPrice = prices[day][stockIndex];
					currentTrade.shares = (liquid - tradeFee)/currentTrade.buyPrice;
					currentTrade.netBuyCost = (currentTrade.buyPrice * currentTrade.shares) + tradeFee;
					
					// make the transaction by adjusting balances
					liquid = liquid - currentTrade.netBuyCost;
					invested = currentTrade.netBuyCost;
					netWorth = liquid + invested;
					inTrade = true;

					// set Stop Loss
					currentTrade.stopLoss = currentTrade.buyPrice - (currentTrade.buyPrice * stopLossMargin);
					// increment day
				}
				
				if(inTrade)
				{	
					if(prices[day][stockIndex] > currentTrade.stopLoss)
					{
						double testStopLoss =  prices[day][stockIndex] - (prices[day][stockIndex] * stopLossMargin);
						if(testStopLoss > currentTrade.stopLoss)
						{
							currentTrade.stopLoss = testStopLoss;
						}
					}
					
					if(prices[day][stockIndex] <= currentTrade.stopLoss)
					{
						cout << "EOD Price:  " << prices[day][stockIndex] << endl;
						cout << "Net Change: " << prices[day][stockIndex]  - prices[day-1][stockIndex] << endl;
						cout << "% Change: " << (prices[day][stockIndex]  - prices[day-1][stockIndex])/prices[day-1][stockIndex] *100 << "%" << endl;
						cout << "StopLoss:     " << currentTrade.stopLoss << endl;
									
						currentTrade.sellDate = dates[day];
						currentTrade.sellPrice = prices[day][stockIndex];
						currentTrade.netSellCost = currentTrade.sellPrice * currentTrade.shares - tradeFee;
						liquid = liquid + currentTrade.netSellCost;
						invested = 0;
						netWorth = liquid + invested;
						currentTrade.netGainLoss = currentTrade.netSellCost - currentTrade.netBuyCost;
						inTrade = false;
					
						/*
						tradeLog 	<< "Trans #" << tradeIndex << ": " << setw(5) << currentTrade.ticker << endl;
						tradeLog	<< "  Bought: " << setw(10) << currentTrade.buyDate << ", " << currentTrade.shares 
										<< " shares at $" << currentTrade.buyPrice << " for $" << currentTrade.netBuyCost << endl;
						tradeLog	<< "  Sold:     " << setw(10) << currentTrade.sellDate << " at $" << currentTrade.sellPrice 
										<< " for $" << currentTrade.netSellCost << endl;
						tradeLog	<< "  Net Gain/Loss: $" << currentTrade.netGainLoss << endl << endl;
						*/
					}
				}
			}
			
			/*
			massTradeLog << "STATS, Run" << i + 1 << endl;
			massTradeLog << "Started on " << dates[0] << " with $" << initialInvestment << endl;
			massTradeLog << "Finished on " << dates[rows-1] << " with $" << netWorth << endl;
			massTradeLog << "Net Gain: " << netWorth - initialInvestment << endl;
			massTradeLog << "ROI: " << (netWorth - initialInvestment)/initialInvestment * 100 << "%" << endl << endl;	
			*/
			sumWorth = sumWorth + netWorth;
			
			if(netWorth > maxWorth)
			{
				maxWorth = netWorth;
			}
			
			if(netWorth < minWorth)
			{
				minWorth = netWorth;
			}
			
			if(netWorth < initialInvestment)
			{
				lossCount++;
			}
			
			if(netWorth > 1000000)
			{
				millionaireCount++;
			}
		}
		
		avgWorth = sumWorth/(simCount);
		
		massTradeLog << "Starting with " << initialInvestment << " and a fixed trailing stop loss margin of " << stopLossMargin <<  endl;
		massTradeLog << "Highest final Net Worth: " << maxWorth << endl;
		massTradeLog << "Highest ROI: " << (maxWorth - initialInvestment)/initialInvestment * 100 << "%" << endl;
		massTradeLog << "Lowest final Net Worth: " << minWorth << endl;
		massTradeLog << "Lowest ROI: " << (minWorth - initialInvestment)/initialInvestment * 100 << "%" << endl;
		massTradeLog << "Average final Net Worth: " << avgWorth << endl;
		massTradeLog << "Average ROI: " << (avgWorth - initialInvestment)/initialInvestment * 100 << "%" << endl;
		massTradeLog << "Number of simulations resulting in a net loss: " << lossCount << endl; 
		massTradeLog << "Number of simulations earning over a million dollars in 8 years: " << millionaireCount << endl;
		massTradeLog << "Probablility of becoming a millionaire in 8 years with this strategy: " << millionaireCount/simCount << endl << endl;
		
	}
	
	return 0;
}

