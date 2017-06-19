#include <string>
#include "RTIMULib.h"
#include "Sensehat.h"

using namespace std;

/**
 * Intialisation des différents capteurs
 */
bool Sensehat::initSenseHat() {
    RTIMUSettings *settings = new RTIMUSettings("RTIMULib");

    // Instantiation de l'objet IMU (Centrale inertielle)
    m_imu = RTIMU::createIMU(settings);
    // Instantiation de l'objet pressure (Capteur de pression)
    m_pressure = RTPressure::createPressure(settings);

    try {
        if ((m_imu == NULL) || (m_imu->IMUType() == RTIMU_TYPE_NULL)) {
            throw string("No IMU object found \n");
        }

        if (m_pressure == NULL) {
            throw string("No PRESSURE object found \n");
        }

        // Initilisation de la centrale inertielle
        m_imu->IMUInit();

        m_imu->setSlerpPower(0.02);
        // Activation du Gyro
        m_imu->setGyroEnable(true);
        // Activation de l'Accelerometre
        m_imu->setAccelEnable(true);
        // Activation du Compas
        m_imu->setCompassEnable(true);

        // Initialisation du capteur de pression
        m_pressure->pressureInit();

    } catch (string const &chaine) {
        printf("ERROR: \n");
        return false;
    }

    return true;
}

/**
 * Récupération des données provenant :
 *  - de la centrale intertielle
 *  - du capteur de pression
 */
void Sensehat::getDataSensor() {
    // On définit l'intervalle recommandé par la centrale
    usleep(m_imu->IMUGetPollInterval() * 1000);

    while (m_imu->IMURead()) {

        // Recuperation des donnees de la centrale inertielle
        RTIMU_DATA imuData = m_imu->getIMUData();
        m_pressure->pressureRead(imuData);

        // Données provenant de la centrale
        m_x = imuData.fusionPose.x() * RTMATH_RAD_TO_DEGREE;
        m_y = imuData.fusionPose.y() * RTMATH_RAD_TO_DEGREE;
        m_z = imuData.fusionPose.z() * RTMATH_RAD_TO_DEGREE;

        // Données d'altitude
        m_altitude = RTMath::convertPressureToHeight(imuData.pressure);

        // Données de température;
        m_temperature = imuData.temperature;

        fflush(stdout);
    }
}

/**
 * Accessors
 */
int Sensehat::getSenseHatInterval() {
    return m_imu->IMUGetPollInterval() * 1000;
}

void Sensehat::setX(double x) {
    m_x = x;
}

double Sensehat::getX() {
    return m_x;
}

void Sensehat::setY(double y) {
    m_y = y;
}

double Sensehat::getY() {
    return m_y;
}

void Sensehat::setZ(double z) {
    m_z = z;
}

double Sensehat::getZ() {
    return m_z;
}

void Sensehat::setAltitude(double altitude) {
    m_altitude = altitude;
}

double Sensehat::getAltitude() {
    return m_altitude;
}

void Sensehat::setTemperature(double temperature) {
    m_temperature = temperature;
}

double Sensehat::getTemperature() {
    return m_temperature;
}

void Sensehat::setAcceleration(double acceleration) {
    m_acceleration = acceleration;
}

double Sensehat::getAcceleration() {
    return m_acceleration;
}
