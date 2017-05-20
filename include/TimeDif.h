#ifndef DEF_TIMEDIF
#define DEF_TIMEDIF

#include <time.h>

class TimeDif
{
	public:
		void setStartDate();
		void setEndDate();
		float getDiffDate();
		
	private:
		clock_t m_startDate;
		clock_t m_endDate;
};

#endif // DEF_TIMEDIF

