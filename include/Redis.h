#ifndef DEF_REDIS
#define DEF_REDIS

#include <vector>
#include <redis3m/redis3m.hpp>

using namespace std;
using namespace redis3m;

class Redis
{
	public:
		Redis();
		bool setDataList(std::string key, vector<std::string> data);
		bool setDataSimple(std::string key, std::string data);
		
		std::string getDataSimple(std::string key);

	private:
		connection::ptr_t m_conn;
};

#endif // DEF_REDIS

