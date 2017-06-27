//-----------------------------
//----- INCLUDE 		  -----
//-----------------------------
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <time.h>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "RTIMULib.h"

#include "TimeDif.h"
#include "Sensehat.h"
#include "Gps.h"
#include "Redis.h"

//-----------------------------
//----- Namespace 		  -----
//-----------------------------
using namespace std;
using namespace boost;

//-----------------------------
//----- Prototypes 		  -----
//-----------------------------
void dataSenseHat_thread();
void dataGps_thread();

//-----------------------------
//----- Fonction Main	  -----
//-----------------------------

int main() {
    // Lancement du Thread relatif au sensehat
    std::thread t1(dataSenseHat_thread);

    // Lancement du thread relatif au GPS
    std::thread t2(dataGps_thread);

    // On Attente la fin des Thread pour finir le programme
    t1.join();
    t2.join();

    return EXIT_SUCCESS;
}

//-----------------------------
//----- Thread SenseHat	  -----
//-----------------------------

void dataSenseHat_thread() {
    // SenseHat : On instancie l'objet 
    Sensehat objSenseHat;
    // SenseHat : On initialise les capteurs
    objSenseHat.initSenseHat();

    // Redis : On instancie l'objet (Connexion Redis)
    Redis objRedis;

    // Flag permettant de marquer l'enregistrement des offsets sous Redis
    bool booOffsetIsRecorded(false);

    // Différents offset stockés dans Redis
    double offsetX(0);
    double offsetY(0);
    double offsetZ(0);
    double offsetAltitude(0);

    while (true) {
        // SenseHat définit un interval d'interrogation, on va s'en servir pour calmer un peu le jeu
        usleep(objSenseHat.getSenseHatInterval());

        // Tableau amené à contenir l'ensemble des données propres au SenseHat
        vector<std::string> dataSenseHat;

        // Récupération des données SenseHat
        objSenseHat.getDataSensor();

        // On remplit le tableau avec les données du SenseHat
        // Données Gyro
        dataSenseHat.push_back(boost::lexical_cast<string>(objSenseHat.getX()));
        dataSenseHat.push_back(boost::lexical_cast<string>(objSenseHat.getY()));
        dataSenseHat.push_back(boost::lexical_cast<string>(objSenseHat.getZ()));
        // Données Altitude
        dataSenseHat.push_back(boost::lexical_cast<string>(objSenseHat.getAltitude()));
        // Données Température
        dataSenseHat.push_back(boost::lexical_cast<string>(objSenseHat.getTemperature()));

        // On enregistre dans Redis pour l'enregistreur de vol
        objRedis.setDataList("record_imu", dataSenseHat);

        // On enregistre les toutes premières données, on s'en servira pour établir les 0
        if (booOffsetIsRecorded == false) {
            objRedis.setDataSimple("offset_x", dataSenseHat[0]);
            objRedis.setDataSimple("offset_y", dataSenseHat[1]);
            objRedis.setDataSimple("offset_z", dataSenseHat[2]);
            objRedis.setDataSimple("offset_altitude", dataSenseHat[3]);

            booOffsetIsRecorded = true;
        }

        // On récupère les offset
        offsetX = boost::lexical_cast<double>(objRedis.getDataSimple("offset_x"));
        offsetY = boost::lexical_cast<double>(objRedis.getDataSimple("offset_y"));
        offsetZ = boost::lexical_cast<double>(objRedis.getDataSimple("offset_z"));
        offsetAltitude = boost::lexical_cast<double>(objRedis.getDataSimple("offset_altitude"));

        // On enregistre dans Redis les informations bruts
        objRedis.setDataSimple("current_raw_x", dataSenseHat[0]);
        objRedis.setDataSimple("current_raw_y", dataSenseHat[1]);
        objRedis.setDataSimple("current_raw_z", dataSenseHat[2]);
        objRedis.setDataSimple("current_raw_altitude", dataSenseHat[3]);
        objRedis.setDataSimple("current_raw_temperature", dataSenseHat[4]);

        // On enregistre dans Redis les informations compensées avec l'offset
        objRedis.setDataSimple("current_compensated_x", boost::lexical_cast<string>(boost::lexical_cast<double>(dataSenseHat[0]) - offsetX));
        objRedis.setDataSimple("current_compensated_y", boost::lexical_cast<string>(boost::lexical_cast<double>(dataSenseHat[1]) - offsetY));
        objRedis.setDataSimple("current_compensated_z", boost::lexical_cast<string>(boost::lexical_cast<double>(dataSenseHat[2]) - offsetZ));
        objRedis.setDataSimple("current_compensated_altitude", boost::lexical_cast<string>(boost::lexical_cast<double>(dataSenseHat[3]) - offsetAltitude));
        objRedis.setDataSimple("current_compensated_temperature", boost::lexical_cast<string>(boost::lexical_cast<double>(dataSenseHat[4])));

        // On enregistre le time actuel, on testera dans le diagnostic si cette partie est toujours en cours
        objRedis.setDataSimple("current_time_imu", boost::lexical_cast<string>(time(NULL)));
    }
}

//-----------------------------
//----- Thread GPS 		  -----
//-----------------------------

void dataGps_thread() {
    // Gps : On instancie l'objet 
    Gps objGps("/dev/ttyUSB0", 4800);

    // Redis : On instancie l'objet (Connexion Redis)
    Redis objRedis;

    // Flag permettant de marquer l'enregistrement de la base sous Redis
    bool booBaseIsRecorded(false);

    while (true) {
        // On va ralentir les interrogations à 1s, c'est largement suffisant pour notre usage
        usleep(1000000);

        // Tableau amené à contenir l'ensemble des données propres au GPS
        vector<std::string> dataGPS;

        // Récupération des données GPS
        objGps.getDataGps();

        // On remplit le tableau avec les données du GPS
        // Données Latitude
        dataGPS.push_back(boost::lexical_cast<string>(objGps.getLatitude()));
        dataGPS.push_back(boost::lexical_cast<string>(objGps.getLatitudeIndicator()));
        // Données Longitude
        dataGPS.push_back(boost::lexical_cast<string>(objGps.getLongitude()));
        dataGPS.push_back(boost::lexical_cast<string>(objGps.getLongitudeIndicator()));

        // On enregistre dans Redis pour l'enregistreur de vol
        objRedis.setDataList("record_gps", dataGPS);

        // On enregistre les toutes premières données, on s'en servira pour établir les 0
        if (booBaseIsRecorded == false) {
            objRedis.setDataSimple("base_Latitude", dataGPS[0]);
            objRedis.setDataSimple("base_Latitude_Indicator", dataGPS[1]);
            objRedis.setDataSimple("base_Longitude", dataGPS[2]);
            objRedis.setDataSimple("base_Longitude_Indicator", dataGPS[3]);
        }

        // On enregistre dans Redis les informations en cours
        objRedis.setDataSimple("current_Latitude", dataGPS[0]);
        objRedis.setDataSimple("current_Latitude_Indicator", dataGPS[1]);
        objRedis.setDataSimple("current_Longitude", dataGPS[2]);
        objRedis.setDataSimple("current_Longitude_Indicator", dataGPS[3]);

        //...et le status du GPS
        objRedis.setDataSimple("current_GPS_Status", boost::lexical_cast<string>(objGps.getStatusGps()));

        // On enregistre le time actuel, on testera dans le diagnostic si cette partie est toujours en cours
        objRedis.setDataSimple("current_time_gps", boost::lexical_cast<string>(time(NULL)));
    }
}

