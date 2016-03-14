#include "trade-log.h"

void TradeLogClass::TradeLogClass()
{
	startIndex = rand() % 261;			// row (date) to start on, within first year of data
	value = 5000.00;						// set initial investment value at amount
	startStock = rand() % cols;		// starting stock index
	tradeFee = 9.00;
}