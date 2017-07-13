#ifndef SENSEHAT_H
#define SENSEHAT_H

class Sensehat
{
    public:
        bool initSenseHat();
        void getDataSensor();
        
        void setX(double x);
        double getX();
        
        void setY(double y);
        double getY();
        
        void setZ(double z);
        double getZ();
        
        void setAccelX(double accel_x);
        double getAccelX();
        
        void setAccelY(double accel_y);
        double getAccelY();
        
        void setAccelZ(double accel_z);
        double getAccelZ();

        void setAltitude(double altitude);
        double getAltitude();
        
        void setTemperature(double temperature);
        double getTemperature();
        
        void setAcceleration(double acceleration);
        double getAcceleration();
        
        int getSenseHatInterval();
                
    private:
		RTIMU *m_imu;
		RTPressure *m_pressure;
		
		double m_x;
		double m_y;
		double m_z;
                
		double m_accel_x;
		double m_accel_y;
		double m_accel_z;

		double m_altitude;
		double m_temperature;
		double m_acceleration;
		
};

#endif // SENSEHAT_H
