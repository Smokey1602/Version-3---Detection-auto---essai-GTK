// g++ -Wall -o test1 camera.c `pkg-config --cflags --libs opencv`

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sys/time.h>

using namespace cv;
using namespace std;

//--------DECLARATION DES VARIABLES -------------------------------------------------------

char kalib=0; //variable flag pour la calibration
int X1=0,X2=0,X3=0,X4=0,X5=0,X6=0,X7=0,X8=0,X9=0,X10=0,X11=0,X12=0,X13=0,X14=0,X15=0,X16=0,X17=0,X18=0,X19=0,X20=0;
int Y1=0,Y2;

//---------Creation de toutes les images--------------------
Mat hsvcrop1,hsvcrop2,hsvcrop3,hsvcrop4,hsvcrop5,hsvcrop6,hsvcrop7,hsvcrop8,hsvcrop9,hsvcrop10;
Mat source,hsvsource,masquesource;
Mat masquecrop1,masquecrop2,masquecrop3,masquecrop4,masquecrop5,masquecrop6,masquecrop7,masquecrop8,masquecrop9,masquecrop10;
Mat cropedImage1,cropedImage2,cropedImage3,cropedImage4,cropedImage5,cropedImage6,cropedImage7,cropedImage8,cropedImage9,cropedImage10;

//////////////////////////////////////////////////////////////:
//------------Creation de toutes les variables-----------------
int LowH =0,HighH =178,LowS =0,HighS=255,LowV=0,HighV=80; // permet la detection du noir
Mat Lignes = Mat::zeros( source.size(), CV_8UC3 ); //permet le dessin de lignes (pr le debug)
// ---------- declaration de toutes les variables de chaque image crées --------
int LastX1=-1,LastY1 = -1,posX1,posY1;
int LastX2=-1,LastY2 = -1,posX2,posY2;
int LastX3=-1,LastY3 = -1,posX3,posY3;
int LastX4=-1,LastY4 = -1,posX4,posY4;
int LastX5=-1,LastY5 = -1,posX5,posY5;
int LastX6=-1,LastY6 = -1,posX6,posY6;
int LastX7=-1,LastY7 = -1,posX7,posY7;
int LastX8=-1,LastY8 = -1,posX8,posY8;
int LastX9=-1,LastY9 = -1,posX9,posY9;
int LastX10=-1,LastY10 = -1,posX10,posY10;
int flag1=0,flag2=0,flag3=0,flag4=0,flag5=0,flag6=0,flag7=0,flag8=0,flag9=0,flag10=0;
int numsauv=0;
/////////////////////////////////////////
int bisY1=0,bisY2=0,bisY3=0,bisY4=0,bisY5=0,bisY6=0,bisY7=0,bisY8=0,bisY9=0,bisY10=0;
///////////////variables communes au programme///////////
int entree=0,sortie=0;
char name[30];

// --------- variables pour recupere l'heure permettant la sauvegarde -------
static int seconds_last = 99;
char DateString[20],HeureString[20];
char sauvegarde=0; // regle la sauvegarde en manuel defaut (voir fonction "sauvegarde(void)")
char day=0,oldday=0,minute,oldminute,heure;
FILE *file; //fichier de sortie des detections


//-----------------FIN DE LA DECLARATION DES VARIABLES ---------------------------------------------------------


void calibration(void)
{	
		createTrackbar("ligne 1","calibration1",&X1, 640);
		createTrackbar("ligne 2","calibration1",&X2, 640);
		createTrackbar("ligne 3","calibration1",&X3, 640);
		createTrackbar("ligne 4","calibration1",&X4, 640);
		createTrackbar("ligne 5","calibration1",&X5, 640);
		createTrackbar("ligne 6","calibration1",&X6, 640);
		createTrackbar("ligne 7","calibration1",&X7, 640);
		createTrackbar("ligne 8","calibration1",&X8, 640);
		createTrackbar("ligne 9","calibration1",&X9, 640);
		createTrackbar("ligne 10","calibration1",&X10, 640);
		createTrackbar("ligne 11","calibration1",&X11, 640);
		createTrackbar("ligne 12","calibration1",&X12, 640);
		createTrackbar("ligne 13","calibration2",&X13, 640);
		createTrackbar("ligne 14","calibration2",&X14, 640);
		createTrackbar("ligne 15","calibration2",&X15, 640);
		createTrackbar("ligne 16","calibration2",&X16, 640);
		createTrackbar("ligne 17","calibration2",&X17, 640);
		createTrackbar("ligne 18","calibration2",&X18, 640);
		createTrackbar("ligne 19","calibration2",&X19, 640);
		createTrackbar("ligne 20","calibration2",&X20, 640);
		createTrackbar("colonne sup","calibration2",&Y1, 640);
		createTrackbar("colonne inf","calibration2",&Y2, 640);
	

		line(source, Point(X1, 0), Point(X1, 640), Scalar(255,0,0), 2);
		line(source, Point(X2, 0), Point(X2, 640), Scalar(255,128,0), 2);
		line(source, Point(X3, 0), Point(X3, 640), Scalar(255,255,0), 2);
		line(source, Point(X4, 0), Point(X4, 640), Scalar(128,255,0), 2);
		line(source, Point(X5, 0), Point(X5, 640), Scalar(0,255,0), 2);
		line(source, Point(X6, 0), Point(X6, 640), Scalar(0,255,128), 2);
		line(source, Point(X7, 0), Point(X7, 640), Scalar(0,255,255), 2);
		line(source, Point(X8, 0), Point(X8, 640), Scalar(0,128,255), 2);
		line(source, Point(X9, 0), Point(X9, 640), Scalar(127,0,255), 2);
		line(source, Point(X10, 0), Point(X10, 640), Scalar(255,0,255), 2);
		line(source, Point(X11, 0), Point(X11, 640), Scalar(255,0,127), 2);
		line(source, Point(X12, 0), Point(X12, 640), Scalar(255,0,0), 2);
		line(source, Point(X13, 0), Point(X13, 640), Scalar(255,128,0), 2);
		line(source, Point(X14, 0), Point(X14, 640), Scalar(255,255,0), 2);
		line(source, Point(X15, 0), Point(X15, 640), Scalar(128,255,0), 2);
		line(source, Point(X16, 0), Point(X16, 640), Scalar(0,255,0), 2);
		line(source, Point(X17, 0), Point(X17, 640), Scalar(0,255,128), 2);
		line(source, Point(X18, 0), Point(X18, 640), Scalar(0,255,255), 2);
		line(source, Point(X19, 0), Point(X19, 640), Scalar(0,128,255), 2);
		line(source, Point(X20, 0), Point(X20, 640), Scalar(127,0,255), 2);
		line(source, Point(0, Y1), Point(720, Y1), Scalar(255,0,255), 2);
		line(source, Point(0, Y2), Point(720, Y2), Scalar(255,0,127), 2);

		imshow("flux_video", source); //show the original image
}
void get_time()//fonction nous permettant de recuperer la date et l heure de la raspberry
{
	timeval curTime;
	gettimeofday(&curTime, NULL);
	if (seconds_last == curTime.tv_sec)
		return;
	
	seconds_last = curTime.tv_sec;
	
	strftime(DateString, 80, "%Y-%m-%d", localtime(&curTime.tv_sec));
	strftime(HeureString, 80, "%H:%M:%S", localtime(&curTime.tv_sec));
	heure=10*HeureString[0]+HeureString[1];
	minute = 10*HeureString[3]+HeureString[4];
	day=10*DateString[8]+DateString[9];
}
void window_create(void)// fonction creant l affichage le flux video
{
	namedWindow("flux_video", CV_WINDOW_AUTOSIZE);
	namedWindow("calibration1", CV_WINDOW_AUTOSIZE);
	namedWindow("calibration2", CV_WINDOW_AUTOSIZE);
}
void passage1(void)
{
	int deplacement1=0;
	
	deplacement1 = (bisY1-LastY1);
	//printf("deplacement :%d; bisY:%d; LastX:%d;\n",deplacement,bisY,LastX);

		if(deplacement1>0 && LastY1<260 && flag1==0)
		{
			entree=entree+1;
			flag1=1;			
		}
		if(deplacement1<0 && LastY1>340 && flag1==0)
		{	
			sortie=sortie+1;
			flag1=1;
		}
	if(LastY1>50 && LastY1<430)
	{
		flag1=0;
	}	
	bisY1=LastY1;
}
void passage2(void)
{
	int deplacement2=0;
	
	deplacement2 = (bisY2-LastY2);
	//printf("deplacement :%d; bisY:%d; LastX:%d;\n",deplacement,bisY,LastX);

		if(deplacement2>0 && LastY2<260 && flag2==0)
		{
			entree=entree+1;
			flag2=1;			
		}
		if(deplacement2<0 && LastY2>340 && flag2==0)
		{	
			sortie=sortie+1;
			flag2=1;
		}
	if(LastY2>50 && LastY2<430)
	{
		flag2=0;
	}	
	bisY2=LastY2;
}
void passage3(void)
{
	int deplacement3=0;
	
	deplacement3 = (bisY3-LastY3);
	//printf("deplacement :%d; bisY:%d; LastX:%d;\n",deplacement,bisY,LastX);

		if(deplacement3>0 && LastY3<260 && flag3==0)
		{
			entree=entree+1;
			flag3=1;			
		}
		if(deplacement3<0 && LastY3>340 && flag3==0)
		{	
			sortie=sortie+1;
			flag3=1;
		}
	if(LastY3>50 && LastY3<430)
	{
		flag3=0;
	}	
	bisY3=LastY3;
}
void passage4(void)
{
	int deplacement4=0;
	
	deplacement4 = (bisY4-LastY4);
	//printf("deplacement :%d; bisY:%d; LastX:%d;\n",deplacement,bisY,LastX);

		if(deplacement4>0 && LastY4<260 && flag4==0)
		{
			entree=entree+1;
			flag4=1;			
		}
		if(deplacement4<0 && LastY4>340 && flag4==0)
		{	
			sortie=sortie+1;
			flag4=1;
		}
	if(LastY4>50 && LastY4<430)
	{
		flag4=0;
	}	
	bisY4=LastY4;
}
void passage5(void)
{
	int deplacement5=0;
	
	deplacement5 = (bisY5-LastY5);
	//printf("deplacement5 :%d; bisY5:%d; LastX5:%d;\n",deplacement5,bisY5,LastX5);

		if(deplacement5>0 && LastY5<280 && flag5==0)
		{
			entree=entree+1;
			flag5=1;			
		}
		if(deplacement5<0 && LastY5>320 && flag5==0)
		{	
			sortie=sortie+1;
			flag5=1;
		}
	if(LastY5>50 && LastY5<430)
	{
		flag5=0;
	}	
	bisY5=LastY5;
}
void passage6(void)
{
	int deplacement6=0;
	
	deplacement6 = (bisY6-LastY6);
	//printf("deplacement :%d; bisY:%d; LastX:%d;\n",deplacement,bisY,LastX);

		if(deplacement6>0 && LastY6<260 && flag6==0)
		{
			entree=entree+1;
			flag6=1;			
		}
		if(deplacement6<0 && LastY6>340 && flag6==0)
		{	
			sortie=sortie+1;
			flag6=1;
		}
	if(LastY6>50 && LastY6<430)
	{
		flag6=0;
	}	
	bisY6=LastY6;
}
void passage7(void)
{
	int deplacement7=0;
	
	deplacement7 = (bisY7-LastY7);
	//printf("deplacement :%d; bisY:%d; LastX:%d;\n",deplacement,bisY,LastX);

		if(deplacement7>0 && LastY7<260 && flag7==0)
		{
			entree=entree+1;
			flag7=1;			
		}
		if(deplacement7<0 && LastY7>340 && flag7==0)
		{	
			sortie=sortie+1;
			flag7=1;
		}
	if(LastY7>50 && LastY7<430)
	{
		flag7=0;
	}	
	bisY7=LastY7;
}
void passage8(void)
{
	int deplacement8=0;
	
	deplacement8 = (bisY8-LastY8);
	//printf("deplacement :%d; bisY:%d; LastX:%d;\n",deplacement,bisY,LastX);

		if(deplacement8>0 && LastY8<260 && flag8==0)
		{
			entree=entree+1;
			flag8=1;			
		}
		if(deplacement8<0 && LastY8>340 && flag8==0)
		{	
			sortie=sortie+1;
			flag8=1;
		}
	if(LastY8>50 && LastY8<430)
	{
		flag8=0;
	}	
	bisY8=LastY8;
}
void passage9(void)
{
	int deplacement9=0;
	
	deplacement9 = (bisY9-LastY9);
	//printf("deplacement :%d; bisY:%d; LastX:%d;\n",deplacement,bisY,LastX);

		if(deplacement9>0 && LastY9<260 && flag9==0)
		{
			entree=entree+1;
			flag9=1;			
		}
		if(deplacement9<0 && LastY9>340 && flag9==0)
		{	
			sortie=sortie+1;
			flag9=1;
		}
	if(LastY9>50 && LastY9<430)
	{
		flag9=0;
	}	
	bisY9=LastY9;
}
void passage10(void)
{
	int deplacement10=0;
	
	deplacement10 = (bisY10-LastY10);
	//printf("deplacement :%d; bisY:%d; LastX:%d;\n",deplacement,bisY,LastX);

		if(deplacement10>0 && LastY10<260 && flag10==0)
		{
			entree=entree+1;
			flag10=1;			
		}
		if(deplacement10<0 && LastY10>340 && flag10==0)
		{	
			sortie=sortie+1;
			flag10=1;
		}
	if(LastY10>50 && LastY10<430)
	{
		flag10=0;
	}	
	bisY10=LastY10;
}

void dessinligne1(void)//dessine les lignes pour suivi d objet
{
	Moments Moments = moments(masquecrop1);

  	double M01 = Moments.m01;
 	double M10 = Moments.m10;
 	double Area = Moments.m00;

   // si area <= 10000, cela signifie que l'objet est trop petit pour etre detecté 
	if (Area > 400)
 	{
	//calculate the position of the ball
   	posX1 = M10 / Area;
   	posY1 = (M01 / Area)+250;        
        
		if (LastX1 >= 0 && LastY1 >= 0 && posX1 >= 0 && posY1 >= 0)
   		{
    		//Draw a red line from the previous point to the current point
    		line(source, Point(posX1, posY1), Point(LastX1, LastY1), Scalar(0,0,255), 2);
   		}

    	LastX1 = posX1;
   	LastY1 = posY1;
  	}
  	//imshow("flux_video", source); //show the original image
	
}
void dessinligne2(void)//dessine les lignes pour suivi d objet
{
	Moments Moments = moments(masquecrop2);

  	double M01 = Moments.m01;
 	double M10 = Moments.m10;
 	double Area = Moments.m00;

   // si area <= 10000, cela signifie que l'objet est trop petit pour etre detecté 
	if (Area > 400)
 	{
	//calculate the position of the ball
   	posX2 = (M10 / Area) +64;
   	posY2 = (M01 / Area)+250;        
        
		if (LastX2 >= 0 && LastY2 >= 0 && posX2 >= 0 && posY2 >= 0)
   		{
    		//Draw a red line from the previous point to the current point
    		line(source, Point(posX2, posY2), Point(LastX2, LastY2), Scalar(0,0,255), 2);
   		}

    	LastX2 = posX2;
   	LastY2 = posY2;
  	}
  	//imshow("flux_video", source); //show the original image
	
}
void dessinligne3(void)//dessine les lignes pour suivi d objet
{
	Moments Moments = moments(masquecrop3);

  	double M01 = Moments.m01;
 	double M10 = Moments.m10;
 	double Area = Moments.m00;

   // si area <= 10000, cela signifie que l'objet est trop petit pour etre detecté 
	if (Area > 400)
 	{
	//calculate the position of the ball
   	posX3 = (M10 / Area) +128;
   	posY3 = (M01 / Area)+250;        
        
		if (LastX3 >= 0 && LastY3 >= 0 && posX3 >= 0 && posY3 >= 0)
   		{
    		//Draw a red line from the previous point to the current point
    		line(source, Point(posX3, posY3), Point(LastX3, LastY3), Scalar(0,0,255), 2);
   		}

    	LastX3 = posX3;
   	LastY3 = posY3;
  	}
  	//imshow("flux_video", source); //show the original image
	
}
void dessinligne4(void)//dessine les lignes pour suivi d objet
{
	Moments Moments = moments(masquecrop4);

  	double M01 = Moments.m01;
 	double M10 = Moments.m10;
 	double Area = Moments.m00;

   // si area <= 10000, cela signifie que l'objet est trop petit pour etre detecté 
	if (Area > 400)
 	{
	//calculate the position of the ball
   	posX4 = (M10 / Area) +192;
   	posY4 = (M01 / Area)+250;        
        
		if (LastX4 >= 0 && LastY4 >= 0 && posX4 >= 0 && posY4 >= 0)
   		{
    		//Draw a red line from the previous point to the current point
    		line(source, Point(posX4, posY4), Point(LastX4, LastY4), Scalar(0,0,255), 2);
   		}

    	LastX4 = posX4;
   	LastY4 = posY4;
  	}
  	//imshow("flux_video", source); //show the original image
	
}
void dessinligne5(void)//dessine les lignes pour suivi d objet
{
	Moments Moments = moments(masquecrop5);

  	double M01 = Moments.m01;
 	double M10 = Moments.m10;
 	double Area = Moments.m00;

   // si area <= 10000, cela signifie que l'objet est trop petit pour etre detecté 
	if (Area > 400)
 	{
	//calculate the position of the ball
   	posX5 = (M10 / Area) +256;
   	posY5 = (M01 / Area)+250;        
        
		if (LastX5 >= 0 && LastY5 >= 0 && posX5 >= 0 && posY5 >= 0)
   		{
    		//Draw a red line from the previous point to the current point
    		line(source, Point(posX5, posY5), Point(LastX5, LastY5), Scalar(0,0,255), 2);
   		}

    	LastX5 = posX5;
   	LastY5 = posY5;
  	}
  	//imshow("flux_video", source); //show the original image
	
}
void dessinligne6(void)//dessine les lignes pour suivi d objet
{
	Moments Moments = moments(masquecrop6);

  	double M01 = Moments.m01;
 	double M10 = Moments.m10;
 	double Area = Moments.m00;

   // si area <= 10000, cela signifie que l'objet est trop petit pour etre detecté 
	if (Area > 400)
 	{
	//calculate the position of the ball
   	posX6 = (M10 / Area) +320;
   	posY6 = (M01 / Area)+250;        
        
		if (LastX6 >= 0 && LastY6 >= 0 && posX6 >= 0 && posY6 >= 0)
   		{
    		//Draw a red line from the previous point to the current point
    		line(source, Point(posX6, posY6), Point(LastX6, LastY6), Scalar(0,0,255), 2);
   		}

    	LastX6 = posX6;
   	LastY6 = posY6;
  	}
  	//imshow("flux_video", source); //show the original image
	
}
void dessinligne7(void)//dessine les lignes pour suivi d objet
{
	Moments Moments = moments(masquecrop7);

  	double M01 = Moments.m01;
 	double M10 = Moments.m10;
 	double Area = Moments.m00;

   // si area <= 10000, cela signifie que l'objet est trop petit pour etre detecté 
	if (Area > 400)
 	{
	//calculate the position of the ball
   	posX7 = (M10 / Area) +384;
   	posY7 = (M01 / Area)+250;        
        
		if (LastX7 >= 0 && LastY7 >= 0 && posX7 >= 0 && posY7 >= 0)
   		{
    		//Draw a red line from the previous point to the current point
    		line(source, Point(posX7, posY7), Point(LastX7, LastY7), Scalar(0,0,255), 2);
   		}

    	LastX7 = posX7;
   	LastY7 = posY7;
  	}
  	//imshow("flux_video", source); //show the original image
	
}
void dessinligne8(void)//dessine les lignes pour suivi d objet
{
	Moments Moments = moments(masquecrop8);

  	double M01 = Moments.m01;
 	double M10 = Moments.m10;
 	double Area = Moments.m00;

   // si area <= 10000, cela signifie que l'objet est trop petit pour etre detecté 
	if (Area > 400)
 	{
	//calculate the position of the ball
   	posX8 = (M10 / Area) +448;
   	posY8 = (M01 / Area)+250;        
        
		if (LastX8 >= 0 && LastY8 >= 0 && posX8 >= 0 && posY8 >= 0)
   		{
    		//Draw a red line from the previous point to the current point
    		line(source, Point(posX8, posY8), Point(LastX8, LastY8), Scalar(0,0,255), 2);
   		}

    	LastX8 = posX8;
   	LastY8 = posY8;
  	}
  	//imshow("flux_video", source); //show the original image
	
}
void dessinligne9(void)//dessine les lignes pour suivi d objet
{
	Moments Moments = moments(masquecrop9);

  	double M01 = Moments.m01;
 	double M10 = Moments.m10;
 	double Area = Moments.m00;

   // si area <= 10000, cela signifie que l'objet est trop petit pour etre detecté 
	if (Area > 400)
 	{
	//calculate the position of the ball
   	posX9 = (M10 / Area) +512;
   	posY9 = (M01 / Area)+250;        
        
		if (LastX9 >= 0 && LastY9 >= 0 && posX9 >= 0 && posY9 >= 0)
   		{
    		//Draw a red line from the previous point to the current point
    		line(source, Point(posX9, posY9), Point(LastX9, LastY9), Scalar(0,0,255), 2);
   		}

    	LastX9 = posX9;
   	LastY9 = posY9;
  	}
  	imshow("flux_video", source); //show the original image
	
}
void dessinligne10(void)//dessine les lignes pour suivi d objet
{
	Moments Moments = moments(masquecrop10);

  	double M01 = Moments.m01;
 	double M10 = Moments.m10;
 	double Area = Moments.m00;

   // si area <= 10000, cela signifie que l'objet est trop petit pour etre detecté 
	if (Area > 400)
 	{
	//calculate the position of the ball
   	posX10 = (M10 / Area) +576;
   	posY10 = (M01 / Area)+250;        
        
		if (LastX10 >= 0 && LastY10 >= 0 && posX10 >= 0 && posY10 >= 0)
   		{
    		//Draw a red line from the previous point to the current point
    		line(source, Point(posX10, posY10), Point(LastX10, LastY10), Scalar(0,0,255), 2);
   		}

    	LastX10 = posX10;
   	LastY10 = posY10;
  	}
  	imshow("flux_video", source); //show the original image
	
}

void sauvegarde_automatique()
{
	if(oldday!=day)
	{
		char nom[20];
		snprintf(nom,sizeof(nom),"%s.txt",DateString);
		fprintf(file,"heure	entree	sortie");

		if(oldminute!=minute || waitKey(30)==1048673)
		{
							
			printf("sauvegarde ... \n");
			file=fopen(nom,"a");
			fprintf(file,"%d:%d	%d	%d \n",heure,minute,entree,sortie);
			entree=0;sortie=0;
			fclose(file);
			oldminute=minute;
		}
	
	}

}
void menu()
{
	printf("#########################################\n");
	printf("##                MENU                 ##\n");
	printf("#########################################\n");
	printf("##La lettre correspond a la touche a   ##\n");
	printf("##appuyer pour changer la configuration##\n");
	printf("#########################################\n");
	printf("##a: sauvegarde manuelle               ##\n");
	printf("##c: calibration	               ##\n");
	printf("#########################################\n");
	printf("##b: reprendre le programme       ##\n");
	printf("#########################################\n");
	printf("#########################################\n");
	printf("##echap pour quitter                   ##\n");
	printf("#########################################\n");
	while(waitKey(30)!=1048674)
	{
		if (waitKey(30) == 1048603) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       		{
           	cout << "Quitter par demande de l'utilisateur" << endl;
    	    	destroyAllWindows();
			fclose(file);
	    	break;
	    
       		}
		if (waitKey(30) == 1048675) //calibration sur la touche "c"
       		{
	    		kalib=1;
			printf("calibration OK");
			return;
       		}
	}
	
	
}
void initialisation()
{
	printf("########################################\n");
	printf("##           Projet  OneBee           ##\n");
	printf("##    Universite Savoie Mont Blanc    ##\n");
	printf("########################################\n");
	printf("##         Proposé par Mind           ##\n");
	printf("########################################\n");
	printf("## Realisé par: Jean-Camille Lebreton ##\n");
	printf("##                                    ##\n");	
	printf("########################################\n");
	printf("##          Groupe OneBee  2          ##\n");
	printf("########################################\n");
	printf("##          Annee  2015-2016          ##\n");
	printf("########################################\n");
	printf("########################################\n");
	printf("########################################\n");
	printf("######################  Version 0.5   ##\n");
	printf("########################################\n");
	printf("\n\n\n");
	
	printf("Touches utiles :\n");
	printf("echap pour quitter le programme \n ");
	printf("appuyez a tout moment sur 'm' pour le menu\n");
	menu();
}
		
void suppressbruit(Mat Pic)
{
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
int main(int argc, char **argv)
{	
	//initialisation de la video
	VideoCapture capture(0);
	if(!capture.isOpened()){
	printf("impossible d'initialiser le flux video\n verifiez les branchements");
	return -1;
	}

	window_create();	
	initialisation();
	while(1)
{	
	get_time();
	//on recupere les images au fur et a mesure
	capture >> source;
	
	if(kalib==1)
	{
		calibration();
	}
	
	//image rognée nous permettant pour le moment de couper l image generale en 2
	cropedImage1 = source(Rect(0,250,34,100));
	cropedImage2 = source(Rect(61,250,24,100));
	cropedImage3 = source(Rect(109,250,34,100));
	cropedImage4 = source(Rect(167,250,37,100));
	cropedImage5 = source(Rect(224,250,41,100));
	cropedImage6 = source(Rect(282,250,41,100));
	cropedImage7 = source(Rect(337,250,51,100));
	cropedImage8 = source(Rect(402,250,44,100));
	cropedImage9 = source(Rect(460,250,41,100));
	cropedImage10 = source(Rect(515,250,31,100));

	//on affiche ces images rognées (bebug)
	imshow("croped1", cropedImage1);
	imshow("croped2", cropedImage2);
	imshow("croped3", cropedImage3);
	imshow("croped4", cropedImage4);
	imshow("croped5", cropedImage5);
	imshow("croped6", cropedImage6);
	imshow("croped7", cropedImage7);
	imshow("croped8", cropedImage8);
	imshow("croped9", cropedImage9);
	imshow("croped10", cropedImage10);		

	//on converti en hsv
	cvtColor(cropedImage1,hsvcrop1,CV_BGR2HSV);
	cvtColor(cropedImage2,hsvcrop2,CV_BGR2HSV);
	cvtColor(cropedImage3,hsvcrop3,CV_BGR2HSV);
	cvtColor(cropedImage4,hsvcrop4,CV_BGR2HSV);
	cvtColor(cropedImage5,hsvcrop5,CV_BGR2HSV);
	cvtColor(cropedImage6,hsvcrop6,CV_BGR2HSV);
	cvtColor(cropedImage7,hsvcrop7,CV_BGR2HSV);
	cvtColor(cropedImage8,hsvcrop8,CV_BGR2HSV);
	cvtColor(cropedImage9,hsvcrop9,CV_BGR2HSV);
	cvtColor(cropedImage10,hsvcrop10,CV_BGR2HSV);

	//on convertit l'image source en hsv (debug)
	cvtColor(source,hsvsource,CV_BGR2HSV);
	inRange(hsvsource,Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquesource);
	imshow("hsv source",masquesource);


	//on supprime les couleurs que nous voulons pas et affichons celle que l'on veut en blanc
	inRange(hsvcrop1,Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquecrop1);
	inRange(hsvcrop2,Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquecrop2);
	inRange(hsvcrop3,Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquecrop3);
	inRange(hsvcrop4,Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquecrop4);
	inRange(hsvcrop5,Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquecrop5);
	inRange(hsvcrop6,Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquecrop6);
	inRange(hsvcrop7,Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquecrop7);
	inRange(hsvcrop8,Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquecrop8);
	inRange(hsvcrop9,Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquecrop9);
	inRange(hsvcrop10,Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquecrop10);

	// NOTE : couleurs du noir : (lowH=0 lowS=0 lowV=0, HighH=178 highS=255 highV=84 ! pas pour la detection d'abeille
	
	suppressbruit(masquecrop1);
	suppressbruit(masquecrop2);
	suppressbruit(masquecrop3);
	suppressbruit(masquecrop4);
	suppressbruit(masquecrop5);
	suppressbruit(masquecrop6);
	suppressbruit(masquecrop7);
	suppressbruit(masquecrop8);
	suppressbruit(masquecrop9);
	suppressbruit(masquecrop10);

	//on affiche l image finale
	//imshow("maskcroped1",masquecrop1);
	//imshow("maskcroped2",masquecrop2);

	
	dessinligne1();
	dessinligne2();
	dessinligne3();
	dessinligne4();
	dessinligne5();
	dessinligne6();
	dessinligne7();
	dessinligne8();
	dessinligne9();
	dessinligne10();



	passage1(); //fonction de comptage des entrées sorties
	passage2();
	passage3();
	passage4();
	passage5();
	passage6();
	passage7();
	passage8();
	passage9();
	passage10();


	printf("entree:%d; sortie:%d \n",entree,sortie);
	//printf("%d",waitKey(30));

	
    	sprintf(name,"entrees :%d",entree);
    	putText(source,name, Point(10,130) , FONT_HERSHEY_SIMPLEX, .7, Scalar(0,255,255), 2,8,false );
	sprintf(name,"sorties :%d",sortie);
    	putText(source,name, Point(10,190) , FONT_HERSHEY_SIMPLEX, .7, Scalar(0,255,255), 2,8,false );
	imshow("flux_video", source);
	
		//sauvegarde_automatique();
	
	 if (waitKey(30) == 1048603) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "Quitter par demande de l'utilisateur" << endl;
    	    destroyAllWindows();
			fclose(file);
	    break;
	    
       }
	if (waitKey(30) == 1048685)
	{
		menu();
	}
}
	

}