//-----------------------------
//----- INCLUDE 		  -----
//-----------------------------
#include <iostream>
#include <string>
#include <thread>
#include <vector>

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
int main()
{		
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
void dataSenseHat_thread() 
{
	// SenseHat : On instancie l'objet 
	Sensehat objSenseHat;
	// SenseHat : On initialise les capteurs
    objSenseHat.initSenseHat();    
    
    // Redis : On instancie l'objet (Connexion Redis)
    Redis objRedis;
    
    // Flag permettant de marquer l'enregistrement des offsets sous Redis
    bool booOffsetIsRecorded(false); 
	
	while (true)
	{
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
		objRedis.setDataList("record_sensehat", dataSenseHat);
		
		// On enregistre les toutes premières données, on s'en servira pour établir les 0
		if (booOffsetIsRecorded == false) 
		{
			objRedis.setDataSimple("offset_X", dataSenseHat[0]);
			objRedis.setDataSimple("offset_Y", dataSenseHat[1]);
			objRedis.setDataSimple("offset_Z", dataSenseHat[2]);
			objRedis.setDataSimple("offset_Altitude", dataSenseHat[3]);
			objRedis.setDataSimple("offset_Temperature", dataSenseHat[4]);
		}
		
		// On enregistre dans Redis les informations en cours
		objRedis.setDataSimple("current_X", dataSenseHat[0]);
		objRedis.setDataSimple("current_Y", dataSenseHat[1]);
		objRedis.setDataSimple("current_Z", dataSenseHat[2]);
		objRedis.setDataSimple("current_Altitude", dataSenseHat[3]);
		objRedis.setDataSimple("current_Temperature", dataSenseHat[4]);
		
	}
}

//-----------------------------
//----- Thread GPS 		  -----
//-----------------------------
void dataGps_thread() 
{
	// Gps : On instancie l'objet 
	Gps objGps("/dev/ttyUSB0", 4800);
	
    // Redis : On instancie l'objet (Connexion Redis)
    Redis objRedis;
    
    // Flag permettant de marquer l'enregistrement de la base sous Redis
    bool booBaseIsRecorded(false); 
	
	while (true)
	{
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
		if (booBaseIsRecorded == false) 
		{
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
	}
}

