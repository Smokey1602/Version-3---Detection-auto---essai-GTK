#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

void OnDestroy(GtkWidget *pWidget, gpointer pData);

void Minutep(GtkWidget *pWidget, GtkLabel *pLabel);
void Minutem(GtkWidget *pWidget, GtkLabel *pLabel);

void FlagModifier( GtkWidget *pWidget, GtkButton *pButton);


int main (int argc, char **argv)
{
	int i=0;
	char remplissage[6];

	//Déclaration des widgets
	GtkWidget *pWindow; //fenetre
	GtkWidget *pVBox;//box verticale.
	GtkWidget *pHBox;//box horizontale.
	GtkWidget *pButton[7];//tout les boutons
	GtkWidget *pLabel;
	gtk_init(&argc,&argv);


	

	//déclaration de la fenêtre
	pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	//changement du titre
	gtk_window_set_title(GTK_WINDOW(pWindow),"OneBee");
	//changement de la taille de la fenetre
	gtk_window_set_default_size(GTK_WINDOW(pWindow),300,300);
	//connection avec le signal destroy
	g_signal_connect(G_OBJECT(pWindow),"destroy",G_CALLBACK(OnDestroy),NULL);
	
	//Creation de la box verticale
	pVBox = gtk_vbox_new(GTK_ORIENTATION_VERTICAL,5);
	//ajout de la box dans la fenetre
	gtk_container_add(GTK_CONTAINER(pWindow),pVBox);
	//Creation de la box verticale
	pHBox = gtk_hbox_new(GTK_ORIENTATION_HORIZONTAL,5);

	//on nomme notre label
	pLabel = gtk_label_new("0");
	
	
	//Creation des boutons
	pButton[0]=gtk_button_new_with_label("1 - Calibration");
	pButton[1]=gtk_button_new_with_label("2 - Aucun Affichage");
	pButton[2]=gtk_button_new_with_label("3 - Affichage de la camera");
	pButton[3]=gtk_button_new_with_label("4 - Affichage du compteur seul");
	pButton[4]=gtk_button_new_with_label("5 - Affichage du graphique");
	pButton[5]=gtk_button_new_with_label("-");
	pButton[6]=gtk_button_new_with_label("+");
	pButton[7]=gtk_button_new_with_label("Quitter");

	//ajout des bouttons dans la box
	gtk_box_pack_start(GTK_BOX(pVBox),pButton[0],TRUE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(pVBox),pButton[1],TRUE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(pVBox),pButton[2],TRUE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(pVBox),pButton[3],TRUE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(pVBox),pButton[4],TRUE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(pVBox),pHBox,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(pHBox),pButton[5],TRUE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(pHBox),pLabel,TRUE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(pHBox),pButton[6],TRUE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(pVBox),pButton[7],TRUE,FALSE,0);


	g_signal_connect(G_OBJECT(pButton[0]),"clicked",G_CALLBACK(FlagModifier), pButton[0]);
	g_signal_connect(G_OBJECT(pButton[1]),"clicked",G_CALLBACK(FlagModifier), pButton[1]);
	g_signal_connect(G_OBJECT(pButton[2]),"clicked",G_CALLBACK(FlagModifier), pButton[2]);
	g_signal_connect(G_OBJECT(pButton[3]),"clicked",G_CALLBACK(FlagModifier), pButton[3]);
	g_signal_connect(G_OBJECT(pButton[4]),"clicked",G_CALLBACK(FlagModifier), pButton[4]);

	g_signal_connect(G_OBJECT(pButton[5]),"clicked",G_CALLBACK(Minutem), pLabel);
	g_signal_connect(G_OBJECT(pButton[6]),"clicked",G_CALLBACK(Minutep), pLabel);

	g_signal_connect(G_OBJECT(pButton[7]),"clicked",G_CALLBACK(OnDestroy),NULL);

	//Affichage de la fenêtre
	gtk_widget_show_all(pWindow);	
	//Démarrage de la boucle evenementielle
	gtk_main();
		
	return EXIT_SUCCESS;
}

void OnDestroy(GtkWidget *pWidget, gpointer pData)
{
	//arret de la boucle
	gtk_main_quit();
}
void Minutep(GtkWidget *pWidget, GtkLabel *pLabel)
{
	const gchar *texte;
	gchar valeur=0;
	gchar data [5]={0};
	texte = gtk_label_get_text(GTK_LABEL(pLabel));
	valeur = (gchar) *texte - 48+1; // !!!!!!
	printf("%d",valeur);
	snprintf(data,sizeof(data),"%d",valeur);
	gtk_label_set_text (GTK_LABEL(pLabel),data);
	
}
void Minutem(GtkWidget *pWidget, GtkLabel *pLabel)
{
	gtk_label_set_text (GTK_LABEL(pLabel),"lili");	
	
}
void FlagModifier( GtkWidget *pWidget, GtkButton *pButton)
{
	switch(gtk_button_get_label(pButton)[0])
	{
		case('1'):
		printf("1");
		break;
		case('2'):
		printf("11");
		break;
		case('3'):
		printf("111");
		break;
		case('4'):
		printf("1111");
		break;
		case('5'):
		printf("11111");
		break;
		case('6'):
		printf("111111");
		break;
	}

}