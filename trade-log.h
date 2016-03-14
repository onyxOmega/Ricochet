/* Custom Class: TradeLog

	Contains and tracks data over a series of simulated trades
	
	Author: William Fischer
*/

#ifndef TRADELOG_H
#define TRADELOG_H

class TradeLogClass
{	
	int startIndex = rand() % 261;			// row (date) to start on, within first year of data
	double value = 5000.00;						// set initial investment value at amount
	double startStock = rand() % cols;		// starting stock index
	double tradeFee = 9.00;	
}; 


#endif