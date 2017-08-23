# avionic_imu
  
__Avionique sur Raspberry Pi - Partie IMU__

Développement d'un système avionique complet pour raspberry PI.  
  
Objectif :  
- Récupération des informations provenant de la SenseHat
- Récupération des informations provenant du GPS
- Enregistrement de l'ensemble des données sous Redis
  
Articles sur le sujet :  
- [Présentation du projet](https://medium.com/@deletom/projet-avionique-pr%C3%A9sentation-f9af9e9d1aae)
- [Le modèle](https://medium.com/@deletom/projet-avionique-le-mod%C3%A8le-c2984ce23c3e)
- [Etat d'avancement](https://medium.com/@deletom/projet-avionique-etat-davancement-ac80b06f38f0)
- [Finalisation de la version v1.0](https://medium.com/@deletom/projet-avionique-finalisation-de-la-v1-0-17cbbb5a1abd)
  
Pré-requis : 
- [RTIMULib](https://github.com/RTIMULib/RTIMULib2) (pour les données IMU)
- [boost](http://www.boost.org/) (Parce que ça simplifie la vie :p)
- [Redis3m](https://github.com/luca3m/redis3m) (pour les accès redis)