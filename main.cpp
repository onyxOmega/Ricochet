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
#include <boost/lexical_cast.hpp>

using namespace std;

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
	
	// Seed random from fixed number (for testing)
	srand (5);

    return 0;
}

