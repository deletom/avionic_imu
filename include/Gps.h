#ifndef DEF_GPS
#define DEF_GPS

#include <string>
#include <boost/asio.hpp>

class Gps
{
	public:
		Gps(std::string port, unsigned int baud_rate);
		void getDataGps();
		
		void setStatusGps(bool status);
		bool getStatusGps();
		
		void setLatitude(std::string latitude);
		std::string getLatitude();
		
		void setLongitude(std::string longitude);
		std::string getLongitude();
		
		void setLatitudeIndicator(std::string latitudeIndicator);
		std::string getLatitudeIndicator();
		
		void setLongitudeIndicator(std::string longitudeIndicator);
		std::string getLongitudeIndicator();

	private:
		boost::asio::io_service io;
		boost::asio::serial_port serial;
		std::string readLine();
		std::string getConvertDegreeToDecimal(std::string degree, std::string hemisphere);
		
		bool m_statusGps;
		
		std::string m_indicatorLatitude;
		std::string m_indicatorLongitude;
		
		std::string m_latitude;
		std::string m_longitude;
};

#endif // GPS_H

