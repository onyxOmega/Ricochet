#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <sstream>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/lexical_cast.hpp>



using namespace std;
using namespace boost::gregorian;
int const ROWS = 730;
int const COLS = 4;

int main()
{
    char const* dataFileName = "data/data.txt";
    string tickers[COLS];
    double prices[ROWS][COLS];
    date dates[ROWS];

    ifstream dataFile;
    dataFile.open(dataFileName);

    char bufChar[40];
    stringstream varStream;
    dataFile.getline(bufChar, 40);
    varStream << bufChar;

    string bufString;
    varStream >> bufString;
    cout << bufString << "  ";
    for(int col = 0; varStream >> bufString; col++)
    {
        tickers[col] = bufString;
        cout << tickers[col] << "  ";
    }
    cout << endl;

    for(int row = 0; !dataFile.eof(); row++)
    {
        dataFile.getline(bufChar, 40);
        varStream << bufChar;
        string bufString;
        varStream >> bufString;
        date tempDate(from_string(bufString));
        dates[row] = tempDate;
		for(int col = 0; varStream >> bufString; col++)
        {
            prices[row][col] = boost::lexical_cast<double>(bufString);
            cout << prices[row][col] << "  ";
        }
    }
    cout << "Hello world!" << endl;
    return 0;
}
