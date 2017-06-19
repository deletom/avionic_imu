/**
 * Retrieving informations from GPS
 */

#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "Gps.h"

using namespace std;
using namespace boost;

/**
 * Constructor
 * port : Information about the port used. Ex: /dev/ttyUSB0 (Serial port)
 * baud_rate : Information about the baud rate. Ex : 4800
 */
Gps::Gps(std::string port, unsigned int baud_rate) : io(), serial(io, port) {
    serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));

    m_latitude = "";
    m_longitude = "";

    m_statusGps = false;

    m_indicatorLatitude = "";
    m_indicatorLongitude = "";
}

void Gps::getDataGps() {
    std::string result;
    vector<string> strs;

    result = Gps::readLine();

    boost::split(strs, result, boost::is_any_of(","));

    if (strs.size() >= 12) {
        if (strs[2] == "A") {

            //m_latitude = Gps::getConvertDegreeToDecimal(strs[3], strs[4]);
            m_latitude = strs[3];
            m_indicatorLatitude = strs[4];

            //m_longitude = Gps::getConvertDegreeToDecimal(strs[5], strs[6]);
            m_latitude = strs[5];
            m_indicatorLongitude = strs[6];

            m_statusGps = true;
        } else {
            m_statusGps = false;
        }
    }
}

/**
 * Reading of data GPS with port and baud rate
 * @return string result => Line Data of the current reading
 */
std::string Gps::readLine() {
    using namespace boost;
    char c;
    std::string result;

    for (;;) {
        asio::read(serial, asio::buffer(&c, 1));
        switch (c) {
            case '\r':
                break;
            case '\n':
                return result;
            default:
                result += c;
        }
    }
}

std::string Gps::getConvertDegreeToDecimal(std::string degree, std::string hemisphere) {
    vector<string> element;
    std::string returnDecimalDataGps;

    boost::split(element, degree, boost::is_any_of("."));

    if (element.size() >= 2) {
        returnDecimalDataGps = boost::lexical_cast<string>((std::stof(element[0]) / 100) + ((std::stof(element[1]) / 100) / 60));
        if (hemisphere == "N" || hemisphere == "E") {
            return returnDecimalDataGps;
        }
        if (hemisphere == "S" || hemisphere == "W") {
            return "-" + returnDecimalDataGps;
        }
    }
    return "";
}

/**
 * Accessors
 */

void Gps::setStatusGps(bool status) {
    m_statusGps = status;
}

bool Gps::getStatusGps() {
    return m_statusGps;
}

void Gps::setLatitude(std::string latitude) {
    m_latitude = latitude;
}

std::string Gps::getLatitude() {
    return m_latitude;
}

void Gps::setLongitude(std::string longitude) {
    m_longitude = longitude;
}

std::string Gps::getLongitude() {
    return m_longitude;
}

void Gps::setLatitudeIndicator(std::string latitudeIndicator) {
    m_indicatorLatitude = latitudeIndicator;
}

std::string Gps::getLatitudeIndicator() {
    return m_indicatorLatitude;
}

void Gps::setLongitudeIndicator(std::string longitudeIndicator) {
    m_indicatorLongitude = longitudeIndicator;
}

std::string Gps::getLongitudeIndicator() {
    return m_indicatorLongitude;
}
