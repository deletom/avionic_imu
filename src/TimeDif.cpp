/**
 * Calculate Execution time
 */
#include <ctime>
#include "TimeDif.h"

using namespace std;

void TimeDif::setStartDate()
{
	m_startDate = clock();
}

void TimeDif::setEndDate()
{
	m_endDate = clock();
}

float TimeDif::getDiffDate()
{
	
	return ((float)(m_endDate - m_startDate))/CLOCKS_PER_SEC;
}
