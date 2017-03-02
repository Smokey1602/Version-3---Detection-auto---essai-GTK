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


// -------PROTOTYPAGE ----------

void sauvegarde_graphique();// Fonction de sauvegarde du graphique en temps réel

void calib_auto();//foncion de calibration automatique

void sauvegarde_calib(); //fonction de sauvegarde de la calibration ( obsolete depuis le calibrage automatique

void calibration(); // fonction permettant de modifier la position des portes pendant l'execution du programme (visuel uniquement)

void get_time(); //fonction de récupération de l'heure et de la date (gere aussi l'initialisation des variables correspondantes (Heure,Minute,Day ... etc)

void window_create(); //fonctiond d'initialisation des fenetres (aps obligatoire mais permet de travailler dans une memoire que nous connaissons et qui est sure)

void passage(int i); //fonction permettant de compter le passage des abeilles (le i correspond a l'indice de l'image que nous passons)

void sauvegarde_automatique(); //fonction de sauvegarde (tout les x minutes (choisi apr l'utilisateur)) uniquement les ficher .csv

void menu(); //fonction d affichage du menu principal. ce contente d'afficher la fenetre menu et prouve le bon lancement du programme

void suppressbruit(Mat Pic); //fonction générique qui permet de supprimer le bruit dans une image

void recall(); //rappel des données de calibration (obslolete depuis le calibrage automatique)

void sauvegarde_graphique(); //fonction de sauvegarde du graphique



//--------DECLARATION DES VARIABLES -------------------------------------------------------

//initialisation par defaut(obsolete mais présent au cas ou un sous nombre de porte serait utilisé
int X[40]={450,460,470,480,490,500,510,520,530,540,550,560,570,580,590,600,610,620,630,640};
int Y[2]={300,330};

int Quitter=0; //varibales flag permettant de quitter le programme sur demande (debug only)
int C=0; //variable flag permettant d'entrer dans la calibration des portes (debugo only)
int video=1;// variable permettant de determiner l'affichage que nous voulons( 0=rien, 1=video, 2=image fixe, 3=graphique)
int quitterC=0;//variable flag permettant de quitter la calibration automatique
int T_variable[100]={0}; //(dans fonction recall) permet de recupere dans un tableau les valeurs de fichier "Sauvegarde.txt" qui est le fichier de sauvegarde de la calibration
int T_raw[24000]={0}; //(dans fonction sauvegare_graphique) tableau tres lourd, permettant de mettre en memoire la totalitée de la sauvegarde journaliere pour créer le graphique au format une case pour un chiffre
int T_data[7500]={0}; //(dans sauvegarde_graphique) tableau plus leger contenant les données des mesures de la journée au format une case pour un valeur (
int nombreporte=0; //variable qui porte le nombre de porte cette variable est initialisée dans calib_auto

//---------Creation de toutes les images--------------------

Mat hsvcrop[20]; //tableau contenant toutes les petites images apres leur traitement HSV
Mat source,hsvsource,masquesource,src; //declaration des matrices d'images principales (pour le debug)
Mat masquecrop[20]; //tableau contenant toutes les petites images apres le traitement du filtre de couleur
Mat cropedImage[20]; //tableau contenant toutes les originaux des petites images
Mat im_menu; //declaration d'une matrice pour l'image principale
Mat Graph; // matrice de sauvegarde de l'image du graphique (permet aussi d'afficher ce dernier


VideoCapture capture(0); //initialisation du flux(on le met ici car toutes les fonctions profiterons du flux video sans redéclaration

//////////////////////////////////////////////////////////////

//------------Creation des variables de traitement d'image----------------

int LowH =0,HighH =178,LowS =0,HighS=255,LowV=0,HighV=80; // declaration  des variables permettan la detection du noir
Mat Lignes = Mat::zeros( source.size(), CV_8UC3 ); //permet le dessin de lignes (pr le debug)

//////////////////////////////////////////////////////////////

// ---------- declaration de toutes les variables de chaque image crées nous permet de creer les vecteurs de déplacement des abeilles --------

int LastX[40]={-1}, LastY[40]={-1}, posX[40],posY[40]; //tableau de variables (1case /image) permettant de tracer le mouvement d'une abeille dans un canal en temps réel sur un ecran <=> position actuelle de l'abeille
int flag[40]={0}; //flag de passage evite de compter 1000000 sorties alors que c'etait juste une abeille qui attendais dans la mauvaise zone .... sa****

//////////////////////////////////////////////////////////////

int bisY[40]={0}; //sauvegarde différée de la position de l'abeille. Pour detecter correctement le mouvement position a T+1
int deplacement[40]={0}; //variable etant plus simplement un vecteur en 1D (son signe etantla seule chose qui nous importe)

//-------------------------variables communes au programme-----------------------

int entree=0,sortie=0; //variables comptant les entrée sorties des abeilles
char name[30]; //variable permettant de créer les affichages des entrées sorties

//////////////////////////////////////////////////////////////

// --------- variables pour recupere l'heure permettant la sauvegarde -----------

static int seconds_last = 99; //variable permettant de determiner le changement de seconde(chargé avec 99 aléatoirement pour entrer une premeire fois dans la boucle)
char DateString[20],Jour[20],Minute[20],HeureMinute[20],Time[20]; //variables dont nous allons nous servir dans tout le programme et nous permettant de mettre l'heure et la date dans des variables lisibles
string oldday="\0",oldminute="\0"; //variables de flag permettant de determiner si nous changeons de jour ou non.

//////////////////////////////////////////////////////////////


FILE *file; //fichier de sortie des detections
FILE *file2; //fichier de sauvegarde de secours
FILE *variables; //fichier de sauvegarde des données de calibration
FILE *graph; //fichier pour creer le graphique
char nom[100]; //tableau sauvegardant le nom du ficher de facon dynamique(le nom est changant a hauteur d'une fois par jour)[sauvegarde serveur]
char nom2[100];//tableau sauvegardant le nom du ficher de facon dynamique(le nom est changant a hauteur d'une fois par jour)[sauvegarde interne]
char image[100];//tableau sauvegardant le nom du graphique de facon dynamique(le nom est changant a hauteur d'une fois par jour)[sauvegarde interne]
char image2[100];//tableau sauvegardant le nom du grahpique de facon dynamique(le nom est changant a hauteur d'une fois par jour)[sauvegarde serveur]

int minuteS=1,compteurS=0;//variables permettant de faire variere le temps de sauvegarde des sauvegardes
int PassMinut=0; //flag permettant de signaler le changement de minute
float loop=0; //essai de fps

//-----------------FIN DE LA DECLARATION DES VARIABLES ----------------------------------------------------//

void calib_auto()
/*
	Présentation :
	Cette fonction ne prenant aucun de parametres et ne retournant rien nous permet d effectuer une calibration automatique
	des portes d'entrées sortie. 
	Explication : 
	1-Nous prennons l'image sortant du flux video(qui est normalement l'entrée de la ruche vue du dessus)
	2-Nous traitons cette image pour ne garder que la couleur "rouge"
	3-Nous prenons une ligne de l'image et nous sucrutons la totalitée de cette ligne
	4-Nous scrutons les données au fur et a mesure qu'elle arrivent et les rangeons dans un tableau.
	Précisions:
	etape 4: -> en toute logique, nous avons dans le tableau tout les moments où la ligne de pixel change de couleur
	cad que lorsque que l'on a decouvert le pixel 0 nosu enregistrons l'endroit ou nous sommes dans la ligne
	et ensuite nous ne fesons rien mais des que nous trouvons un pixel a 255 nous reenregistrons cette position qui
	marquera la fin de la detection de la porte "1".
*/

{
	int flag0=0,flag255=0,ecart=0,matj=0,nbporte=0,i=0,tmp=0;
	int calibauto[25]={0};


	cvtColor(source,hsvsource,CV_BGR2HSV);
	inRange(hsvsource,Scalar(90,100,100,0),Scalar(130,255,255,0),masquesource);
	suppressbruit(masquesource);
	//imshow("masksource",masquesource);
	//printf("%d %d",masquesource.cols,masquesource.rows);
	
		for(matj=0;matj<masquesource.rows;matj++)
		{	
			switch(masquesource.at<uchar>(matj,610))
			{
				case 0:
				if(flag0==0 && ecart >10)
				{
					//printf("%d %d \n",matj,nbporte);
					flag0=1;flag255=0;ecart=0;
					calibauto[nbporte]=matj;nbporte++;
				}break;
				
				case 255:
				if(flag255==0 && ecart >10)
				{
					//printf("%d %d \n",matj,nbporte);
					flag0=0;flag255=1;ecart=0;
					calibauto[nbporte]=matj;nbporte++;
				}break;
				
			}
		ecart++;
			
		}
	Y[0]=calibauto[2];
	Y[1]=calibauto[3];
	
	ecart=0;flag0=0;flag255=0;nbporte=0,tmp=calibauto[2]+30;
	//printf("%d \n",tmp);
	
	cvtColor(source,hsvsource,CV_BGR2HSV);
	inRange(hsvsource,Scalar(0,50,50,0),Scalar(20,255,255,0),masquesource);
	suppressbruit(masquesource);
	//imshow("masksource",masquesource);
	
		for(matj=0;matj<masquesource.cols;matj++)
		{	
			switch(masquesource.at<uchar>(tmp,matj))
			{
				case 0:
				if(flag0==0 && ecart >7)
				{
					//printf("%d %d \n",matj,nbporte);
					flag0=1;flag255=0;ecart=0;
					calibauto[nbporte]=matj;nbporte++;
				}break;
				
				case 255:
				if(flag255==0 && ecart >7)
				{
					//printf("%d %d \n",matj,nbporte);
					flag0=0;flag255=1;ecart=0;
					calibauto[nbporte]=matj;nbporte++;
				}break;
				
			}
		ecart++;
			
		}
		nombreporte=(nbporte-3)/2;
		//printf("%d",nombreporte);
		for(i=0;i<nombreporte*2;i++)
		{
			X[i]=calibauto[i+2];
		}
		for(i=nombreporte*2;i<40;i++)
		{
			X[i]=0;
		}




}
void sauvegarde_calib()
{/*
	Présentation : Fonction rapide nous permettant de sauvegarder les données de notre calibration.
	Explication: 
	1-Nous ouvrons un fichier ou le creeons s'il n'existe pas
	2-Nous ecrivons dans ce dernier toutes les positions que nous avons utilisé
	3-Nous fermons le fichier
	Précisions: fprintf nous permet d'ecrire directement dans un fichier e configurer en "w+"
*/
	int i=0;
	variables = fopen("sauvegarde.txt","w+");
	
	for(i=0;i<20;i++)
	{
		fprintf(variables,"%d ",X[i]);
	}
		fprintf(variables,"%d ",Y[0]);
		fprintf(variables,"%d ",Y[1]);
	fclose(variables);
}

void calibration(void)
{
/*
	Présentation : Cette fonction nous permet de modifier les tailles de nos portes.
	Explications :
	1-Nous creeons deja des nouvelles feneters temporaire et utilisée uniquement dans cette fonction 
	2-Nous lancons un while pour gere dynamiquement les parametres de fenetres
	3-Nous creeons des barres de selections fesant bouger en meme temps les lignes 
	4-Nous avons en direct le rendu de la calibration
	5-Nous attendons la demande d'extinction pour quitter la fonction et nous detruisons les fenetres temporaire
	en meme temps
	Précisions : createTrackbar -> permet d'implémenter les curseurs de sélection
		     line -> permet de dessiner les lignes sur l'image principale

*/

	C=0;int trackbar=0;
	char ligne[10]={0};
	int coulLigne=0;
	calib_auto();
	namedWindow("Calibration1", CV_WINDOW_AUTOSIZE);
	namedWindow("Calibration2", CV_WINDOW_AUTOSIZE);	
	namedWindow("Calibration3", CV_WINDOW_AUTOSIZE);
	namedWindow("Calibration4", CV_WINDOW_AUTOSIZE);
	
	while(waitKey(1)!=1048675)
	{
	capture >> source;	
		for(trackbar=0;trackbar<nombreporte*2;trackbar++)
		{
			if(trackbar<=10)
			{
				sprintf(ligne,"ligne %d",trackbar);
				createTrackbar(ligne,"Calibration1",&X[trackbar], 640);
			}
			if(trackbar>10 && trackbar<=20)
			{
				sprintf(ligne,"ligne %d",trackbar);
				createTrackbar(ligne,"Calibration2",&X[trackbar], 640);
			}
			if(trackbar>20 && trackbar<=30)
			{
				sprintf(ligne,"ligne %d",trackbar);
				createTrackbar(ligne,"Calibration3",&X[trackbar], 640);
			}
			if(trackbar>30 && trackbar<=40)
			{
				sprintf(ligne,"ligne %d",trackbar);
				createTrackbar(ligne,"Calibration4",&X[trackbar], 640);
			}

			if(coulLigne>=2)
			{
				line(source, Point(X[trackbar], 0), Point(X[trackbar], 640), Scalar(0,255,255), 2);
				coulLigne=0;trackbar++;
				line(source, Point(X[trackbar], 0), Point(X[trackbar], 640), Scalar(0,255,255), 2);
			}
			else
			{
				line(source, Point(X[trackbar], 0), Point(X[trackbar], 640), Scalar(0,255,0), 2);
				coulLigne++;
			}
		
		}
		createTrackbar("colonne sup","Calibration2",&Y[0], 480);
		createTrackbar("colonne inf","Calibration2",&Y[1], 480);
		createTrackbar("Quitter","Calibration2",&quitterC, 1);
	

		line(source, Point(0, Y[0]), Point(720, Y[0]), Scalar(0,0,0), 2);
		line(source, Point(0, Y[1]), Point(720, Y[1]), Scalar(0,0,0), 2);
		imshow("flux_video", source); //show the original image
		if(quitterC==1)
		{
			sauvegarde_calib();
			quitterC=0;
			setTrackbarPos("Calibration","Menu", C);
			destroyWindow("Calibration1");
			destroyWindow("Calibration2");
			destroyWindow("Calibration3");
			destroyWindow("Calibration4");
			break;	
		}
	}
	destroyWindow("Calibration1");
	destroyWindow("Calibration2");
	setTrackbarPos("Calibration","Menu", C);
}
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
	
	strftime(DateString, 80, "%d-%m-%Y", localtime(&curTime.tv_sec));
	strftime(Jour, 80, "%d", localtime(&curTime.tv_sec));
	strftime(Minute, 80, "%M:", localtime(&curTime.tv_sec));
	strftime(HeureMinute, 80, "%H:%M", localtime(&curTime.tv_sec));
	strftime(Time,20,"%X",localtime(&curTime.tv_sec));
}
void window_create(void)// fonction creant les fenetres principales
{
	// ais-je vraiment besoin de l'expliquer?

	namedWindow("flux_video", CV_WINDOW_AUTOSIZE);
	namedWindow("Menu",CV_WINDOW_NORMAL);
	
}
void passage(int i)
{
/*
	Présentation : Cette fonction nous permet de compter le nombre de passage d'une abeille dans une porte. Les variables
	etant communes au programme la valeur"entree' et "sortie" compte les entrées sorties de toutes les abeilles de toutes
	les portes
	Explications:
	1- Nous determinons le sens de déplacement des abeilles 
	2- Nous regardons dans quelles zones elles sont et distingons 3 cas (dans la zone "entrée", dans la zone"sortie" et 
	dans la zone "rien"
	3- Esuite si le mouvement respecte la postition, nous determinons le cas dans lequel nous sommes.
	4- Enfin on detecte que l'abeille quitte bien la zone de détection pour eviter un comptage inutile
	5- Pour finir, nous enregistrons la derniere position de l'abeille pour determiner ensuite son nouveau mouvement
	Précisions: 
	C'est comme se servir d'un vecteur ou nous cherchons a detecter sa direction, son amplitude n'ayant aucun effet. 
*/
	deplacement[i] = (bisY[i]-LastY[i]);
	//printf("deplacement :%d; bisY:%d; LastX:%d;\n",deplacement,bisY,LastX);

		if(deplacement[i]>0 && LastY[i]<Y[0]+15 && flag[i]==0)
		{
			sortie=sortie+1;
			flag[i]=1;			
		}
		if(deplacement[i]<0 && LastY[i]>Y[1]-15 && flag[i]==0)
		{	
			entree=entree+1;
			flag[i]=1;
		}
	if(LastY[i]>Y[0]+30 && LastY[i]<Y[1]-30)
	{
		flag[i]=0;
	}	
	bisY[i]=LastY[i];
}


void dessinligne(int i)//dessine les lignes pour suivi d objet
{
/*
	Présentation: on pourrait croire cette fonction inutile vu son nom.. mais en fait elle est le coeur de la detection
	En effet elle permet de determiner le centre de l'abeille lors de son passage dans la porte.
	Explications:
	1-On défini un moment et nous nous ne servons pour determiner une position relative du point dans l'image
	2-On se sert de ces moments pour recuperer la coordonnée du point que nous enregistrons dans une variable
	3-On fait un test improbable de sécuritée pour eviter d'avoir des données n'existant pas(negatives)
	4-On met en tampon la position de l'abeille.
	Précisions: l'affichage de la ligne rouge n'est PAS obligatoire. elle est la pour présentation de la detection et
	ne consomme aucune ressource processeur (ou tellement infime qu'elle est négligeable...
*/
	Moments Moments = moments(masquecrop[i]);

  	double M01 = Moments.m01;
 	double M10 = Moments.m10;
 	double Area = Moments.m00;

   // si area <= 10000, cela signifie que l'objet est trop petit pour etre detecté 
	if (Area > 400)
 	{
	//calculate the position of the ball
   	posX[i] = (M10 / Area)+X[i*2];
   	posY[i] = (M01 / Area)+Y[0];        
        
		if (LastX[i] >= 0 && LastY[i] >= 0 && posX[i] >= 0 && posY[i] >= 0)
   		{
    		//Draw a red line from the previous point to the current point
    		line(source, Point(posX[i], posY[i]), Point(LastX[i], LastY[i]), Scalar(0,0,255), 2);
   		}

    	LastX[i] = posX[i];
   	LastY[i] = posY[i];
  	}
  	//imshow("flux_video", source); //show the original image
	
}



void sauvegarde_automatique() //contient aussi la sauvegarde de secours
{
/*
	Présentation: Voici la fonction de sauvegarde automatique des données. Cette sauvegarde s'occupe uniquement des
	données .csv pour une utilisation dans exel ou tout autre logiciel similaire.
	Explications:
	1-Si nous sommes un nouveau jour, nous recréeons un fichier vierge qui contiendra les données de la journée
	2-Ensuite nous fesons un test pour voir si nous sommes a une nouvelle minute de l'heure
	3-On teste notre compteur de minute pour voir s'il n est pas different de zero (si c est le cas aucune sauvgarde
	n est faite
	4-On teste ensuite notre compteur pour voir si nous sauvegarde a l'interval demandé par l'utilisateur
	5-Nous sauvegardons dans le fichier sous un format Heure:Minute entree sortie
	6-On reset les compteurs pour les sauvegardes ulterieures
	Précisions: Dans l absolu nous sauvegardons a deux endroits : dans le dossier du programme et dans la dossier pour
	la communication avec l exterieur

*/
	if(oldday!=Jour)
	{
		
		snprintf(image,sizeof(image),"Sauvegarde/Graphiques/Graphique_du_%s.jpg",DateString);
		//snprintf(image2,sizeof(image),"/var/www/onebee.fr/public_html/Données/Graphiques/Graphique_du_%s.jpg",DateString);
		Graph = imread("fond_graphique.jpg");
		imwrite(image,Graph);

		//snprintf(nom,sizeof(nom),"/var/www/onebee.fr/public_html/Données/Fichiers_csv/%s.csv",DateString);///var/www/html/
		snprintf(nom2,sizeof(nom2),"Sauvegarde/Fichiers_csv/%s.csv",DateString);//on enregistre avec le fichier
		
		//file=fopen(nom,"a+");	
		file2=fopen(nom2,"a+");
		//fprintf(file,"heure,entree,sortie\n");
		//fprintf(file2,"heure,entree,sortie\n");
		oldday=Jour;
		calib_auto();
	}

	if(oldminute!=Minute || waitKey(1)==1048673)
	{
		compteurS++;
		oldminute=Minute;

	}
	if(minuteS!=0)
	{
		if(compteurS>=minuteS)
		{
			sauvegarde_graphique();

			putText(source,"Sauvegarde", Point(250,240) , FONT_HERSHEY_SIMPLEX, 2, Scalar(0,0,255),2,false );
			//file=fopen(nom,"a+");	
			file2=fopen(nom2,"a+");			
			//fprintf(file,"%s;%d;%d\n",HeureMinute,entree,sortie);
			fprintf(file2,"%s;%d;%d\n",HeureMinute,entree,sortie);
			//fclose(file);
			fclose(file2);
			
			entree=0;sortie=0;
			compteurS=0;
			loop=loop/60;
			printf("%.2f fps \n",loop);
			loop=0;
			PassMinut=1;
		
		}
	}

}
void menu()
{
// il y a t'il vraiment besoin de l expliquer?...

	im_menu = imread( "logo.jpg");	
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("                  #########################################\n");
	printf("                  #########################################\n");
	printf("                  ##                MENU                 ##\n");
	printf("                  #########################################\n");
	printf("                  ##Une fenetre avec des interrupteurs   ##\n");
	printf("                  ##est a votre disposition modifiez son ##\n");
	printf("                  ##etat pour entrer dans le menu desiré ##\n");
	printf("                  #########################################\n");	
	printf("                  ###########   CLEAN INSTALL   ###########\n");
	printf("                  #########################################\n");

	createTrackbar("Calibration","Menu",&C, 1);
	createTrackbar("Video","Menu",&video, 3);
	createTrackbar("Minutes entre sauvegardes","Menu",&minuteS, 60);
	createTrackbar("Quitter","Menu",&Quitter, 1);
	imshow("Menu",im_menu);
	moveWindow("Menu",0,0);
	
}
		
void suppressbruit(Mat Pic)
{
/*
	Présentation : Ceci est une fonction donnée par OpenCV. Ellle permet de réduire le bruit des images que nous
	traitons. En quelques sortes en regarde dans un cercle quel pixel est le plus présent et fait de cercle un cercle 
	de la couleur qui est la plus présente.
	Explications : N/A
	Précisions : N/A

*/
	//---------------------------------------------------------------------------------------//
	// ce paquet permettent d'enlever tout les bruits résiduels qui pourrait etre interprétés comme une autre couleur que le noir
	//morphological opening (removes small objects from the foreground) 
  		erode(Pic, Pic, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  		dilate(Pic, Pic, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

  	 //morphological closing (removes small holes from the foreground)
  		dilate(Pic, Pic, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
  		erode(Pic, Pic, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	//---------------------------------------------------------------------------------------//
}

void recall()

{	
/*
	Présentation:Cette fonction permet de récuperer les données de calibration qui ont ete sauvegardée avant l'extinction
	du programme.
	Explications:	
	1-Tout d'abord on ouvre le fichier de sauvegarde
	1.1-On test voir si le fichier c'est bien ouvert
	2-On récupere toutes les données du fichier que nous avons ouvert et les stockons dans un tableau
	3-Nous scrutons ce tableau jusqu'a sa fin 
	3.1-Pendant cette boucle on attends la detection d'un caractere spécial et pendant l'attente nous incrémentons un compteur
	3.2-Quand le caractere spécial est la on effectue un calcul pour passer des 2 (ou3) unitée a une dizaine(centaine)
	3.3-On somme et on sauvegarde dans une variable
	3.4-On sauvegarde cette variable dans sa variable propre ( X[] ou Y[])
	Précisions: On doit faire le calcul test=T_variable.... car on peut lire les caracteres que un a un et non comme
	une chaine de caractere. De ce fait nous devons attribuer le bon poid a chaque variable que nous lisons.

*/
	int c=0,i=0,j=0,compteur_recall=0,test=0,indice=0;

	variables=fopen("sauvegarde.txt","r");
	if (variables == NULL)
	{
		printf("Chargement des données impossible, valeurs par defaut... recall\n");
		return;
	}
	do{
		c=(int)getc(variables);
		T_variable[i]=c-48;
		i++;
	}while (c!=EOF);
	for(j=0;j<i;j++)
	{
		
		if(T_variable[j]==-16)
		{
			if(compteur_recall == 2)
			{
				test = T_variable[j-compteur_recall]*10 + T_variable[j-compteur_recall+1];
				j++;
			}
			if(compteur_recall == 3)
			{
				test = T_variable[j-compteur_recall]*100 + T_variable[j-compteur_recall+1]*10 + T_variable[j-compteur_recall+2];
				j++;
			}
			compteur_recall=0;
			if(indice<20)
			{
				X[indice] = test;
			}
			else
			{
				Y[indice-20]= test;
			}
			indice++;
		}	
		
		compteur_recall++;
	}
	fclose(variables);
}

void sauvegarde_graphique()
{
/*
	Présentation:Cette fonction permet de récuperer les données de calibration qui ont ete sauvegardée avant l'extinction
	du programme.
	Explications:	
	1-Tout d'abord on ouvre le fichier de sauvegarde
	1.1-On test voir si le fichier c'est bien ouvert
	2-On récupere toutes les données du fichier que nous avons ouvert et les stockons dans un tableau
	3-Nous scrutons ce tableau jusqu'a sa fin 
	3.1-Pendant cette boucle on attends la detection d'un caractere spécial et pendant l'attente nous incrémentons un compteur
	3.2-Quand le caractere spécial est la on effectue un calcul pour passer des 2 (ou3) unitée a une dizaine(centaine)
	3.3-On somme et on sauvegarde dans une variable
	3.4-On sauvegarde cette variable dans sa variable propre ( T_data[])
	4-Ensuite avec ce tableau (T_data) nous effectuons une boucle
	4.1-Cette boucle scrute les parametre du tableau 3 par 3 (le tableau est rempli sous ce format (heure,entee,sorite,heur,entrée,sortie...)
	4.2-Grace aux fonctions lignes nous crées un support graphique et nous le remplissons aussi avec des barres
	de données.
	4.3-Enfin on ecrit sur le graphque l'heure par pas d'une heure
	Précisions: On doit faire le calcul test=T_variable.... car on peut lire les caracteres que un a un et non comme
	une chaine de caractere. De ce fait nous devons attribuer le bon poids a chaque variable que nous lisons.	
	On crée un graphique pour les deux parties, soit les entrées et les sorites.
*/	
	int time=0,i;
	char Heure[5];

		
		Graph = imread("fond_graphique.jpg");
		//printf("%s",image);
		time=(HeureMinute[0]-48)*600 + (HeureMinute[1]-48)*60 + (HeureMinute[3]-48)*10 + (HeureMinute[4]-48);
		
			line(Graph,Point(time+50,180),Point(time+50,180-entree),Scalar(0,0,255),1);
			line(Graph,Point(time+50,400),Point(time+50,400-sortie),Scalar(0,255,0),1);

			line(Graph,Point(50,180),Point(1490,180),Scalar(0,0,0),2);//horizontal entree
			line(Graph,Point(50,180),Point(50,25),Scalar(0,0,0),2);//vertical entree

			line(Graph,Point(50,400),Point(1490,400),Scalar(0,0,0),2);//horizontal sortie
			line(Graph,Point(50,400),Point(50,250),Scalar(0,0,0),2);//vertical sortie
			
			//les deux prochaines lignes sont pour l'affichage du texte sur le graphique
			putText(Graph,"entree", Point(5,15) , FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0,0,0), 1,false );
			putText(Graph,"sortie", Point(5,240) , FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0,0,0), 1,false );
			//les prochaines lignes sont la pour l'affichage de l'heure sur l axe des ordonnées

			for(i=0;i<24;i++)
			{	
				sprintf(Heure,"%d:00",i);
				putText(Graph,Heure, Point(50+60*i,190) , FONT_HERSHEY_SIMPLEX, 0.2, Scalar(0,0,0), 1,false );
				putText(Graph,Heure, Point(50+60*i,410) , FONT_HERSHEY_SIMPLEX, 0.2, Scalar(0,0,0), 1,false );
			}
			
		imwrite(image,Graph);
		//snprintf(image2,sizeof(image2),"/var/www/onebee.fr/public_html/Données/Graphiques/Graphique_du_%s.jpeg",DateString);
		//imwrite(image2,Graph);

	
	//line(src,Point(10,10),Point(10,100),Scalar(0,0,255),1);
	


}

int main(int argc, char **argv)
{	
	int i=0;
	recall();


	//initialisation de la video pour la camera IP
	//VideoCapture capture("http://10.2.2.29/mjpg/video.mjpg");
	
	
	if(!capture.isOpened()){
	printf("impossible d'initialiser le flux video\n verifiez les branchements");
	return -1;
	}
	
	window_create();
	menu();
	while(1)
{	
	capture >> source ;
	//calib_auto();
	get_time();
	loop++;
	//on decoupe l'image principale pour ne garder que les parties qui nous interessent

	//calib_auto();

	for(i=0;i<nombreporte;i++)
	{

			//on converti les images en HSV
		cvtColor(source(Rect(X[i*2],Y[0],X[i*2+1]-X[i*2],Y[1]-Y[0])),hsvcrop[i],CV_BGR2HSV);
			//on supprime les couleurs que nous voulons pas et affichons celle que l'on veut en blanc
		inRange(hsvcrop[i],Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquecrop[i]);
			// NOTE : parametres du noir : (lowH=0 lowS=0 lowV=0, HighH=178 highS=255 highV=84 ! pour la detection d'abeille
			//on supprime le bruit de la video (fonction)
		suppressbruit(masquecrop[i]);
			//fonction de comptage d entrée sortie
		passage(i);
			//on trace les vecteurs
		dessinligne(i);
	}
	
	
	/*
	//on affiche ces images rognées (bebug)
	imshow("cropedImage1", masquecrop[0]);
	imshow("cropedImage2", masquecrop[1]);
	imshow("cropedImage3", masquecrop[2]);
	imshow("cropedImage4", masquecrop[3]);
	imshow("cropedImage5", masquecrop[4]);
	imshow("cropedImage6", masquecrop[5]);
	imshow("cropedImage7", masquecrop[6]);
	imshow("cropedImage8", masquecrop[7]);
	imshow("cropedImage9", masquecrop[8]);
	imshow("cropedImage10", masquecrop[9]);
	
	
	
	
	//affiche le masque de l'image d'origine
	cvtColor(source,hsvsource,CV_BGR2HSV);
	inRange(hsvsource,Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquesource);
	suppressbruit(masquesource);
	imshow("masque",masquesource);*/


	sauvegarde_automatique();
	

	switch(video)
	{
	case 0:
		
		destroyWindow("flux_video");	
		break;
	case 1 :
			
		namedWindow("flux_video", CV_WINDOW_AUTOSIZE);
		putText(source,Time, Point(10,440) , FONT_HERSHEY_SIMPLEX, .3, Scalar(0,255,255),1,false );
		sprintf(name,"Entrees :%d",entree);
    		putText(source,name, Point(10,130) , FONT_HERSHEY_SIMPLEX, .7, Scalar(0,255,255), 2,8,false );
		sprintf(name,"Sorties :%d",sortie);
    		putText(source,name, Point(10,190) , FONT_HERSHEY_SIMPLEX, .7, Scalar(0,255,255), 2,8,false );
		imshow("flux_video", source);
		break;
	case 2 :
		src = imread( "image1.jpg");
		namedWindow("flux_video", CV_WINDOW_AUTOSIZE);
		sprintf(name,"Entrees :%d",entree);
    		putText(src,name, Point(20,200) , FONT_HERSHEY_SIMPLEX, 3, Scalar(255,0,0), 2,8,false );
		sprintf(name,"Sorties :%d",sortie);
    		putText(src,name, Point(20,380) , FONT_HERSHEY_SIMPLEX, 3, Scalar(255,0,0), 2,8,false );
		imshow("flux_video", src);
		break;
	case 3 :
		
		imshow("flux_video", Graph);
		break;	
	}
	
	if(C==1)
	{
		calibration();
	}
	if(Quitter==1)
	{	
	
		printf("Fin du programme par demande de l'utilisateur\n");
    	    	destroyAllWindows();
	    	fclose(file2);  // faire attention ici 
	    	return 0;
	}
				
}
	

}