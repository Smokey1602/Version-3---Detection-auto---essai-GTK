// g++ -Wall -o test1 camera.c `pkg-config --cflags --libs opencv`

#include "opencv2/core/core.hpp" //bibliotheque générale d'opencv
#include "opencv2/highgui/highgui.hpp" //bibilotheque auxilliaire(traitement d'image)
#include "opencv2/imgproc/imgproc.hpp" //bibliotheque auxilliaire(affochage des images)
#include "opencv2/opencv.hpp" // Root des bibilotheques
#include <stdlib.h>
#include <stdio.h>
#include <iostream> //bibliotheque de gestion des entrées video
#include <string.h>
#include <sys/time.h> //bibliotheque interne a la raspberry (permet de recupere la date et l'heure de la raspberry


using namespace cv;
using namespace std;


char Minute[20]={0};
string oldminute="\0";
double loop=0;
static int seconds_last = 99;
Mat source;
int minuteS=1;
int compteurS=0;


VideoCapture capture(0);
	


void get_time()//fonction nous permettant de recuperer la date et l heure de la raspberry
{
/*
	Présentation: Ceci est une fonction générique et modifiée permettant d'acceder a la date et l'heure de la raspberry
	Explications:
	1- nous récupérons la date actuelle
	2- on test voir si nous sommes a une nouvelle date (ici 1seconde plus tard)
	3- on met a jour notre flag de detection de nouvelle data
	4- nous récuperons et formatons toutes les odnénes de dates comme nous en avons besoin
	Précisions : Cette fonction est GENERIQUE elle marche sur tout les raspberry par defaut aucun paquet n est nécessaire
*/
	timeval curTime;
	gettimeofday(&curTime, NULL);
	if (seconds_last == curTime.tv_sec)
	return;
	
	seconds_last = curTime.tv_sec;
	
	strftime(Minute, 80, "%M:", localtime(&curTime.tv_sec));

}

int main (int argc, char **argv)
{
	while(1)
{

	capture >> source ;
	get_time();
	loop++;

	if(oldminute!=Minute)
	{
		compteurS++;
		oldminute=Minute;

	}
	if(minuteS!=0)
	{
		if(compteurS>=minuteS)
		{

			compteurS=0;
			loop=loop/60;
			printf("%.2f fps \n",loop);
			loop=0;
		
		}
	}
	
	imshow("flux", source);
	
}
return 0;
}