/**
 * Get Object Redis connexion
 */
#include "Redis.h"
#include <vector>
#include <iostream>
#include <boost/algorithm/string/join.hpp>


using namespace std;
using namespace redis3m;

Redis::Redis() {
    m_conn = connection::create();
    std::cout << "Redis OK" << std::endl;
}

bool Redis::setDataList(std::string key, vector<std::string> data) {
    m_conn->run(command("LPUSH") << key << boost::algorithm::join(data, "|"));

    return true;
}

bool Redis::setDataSimple(std::string key, std::string data) {
    m_conn->run(command("SET") << key << data);

    return true;
}

std::string Redis::getDataSimple(std::string key) {
    reply r = m_conn->run(command("GET") << key);
    return r.str();
}

