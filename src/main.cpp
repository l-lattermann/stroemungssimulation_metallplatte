#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>

#include "Variabeln.h"
#include "Funktionen.h"

using namespace std;


int main()
{
//========================BEGRÜßUNG=============================================
Hallo();

//========================BERECHNE FELDGRÖẞEN===================================
    Breite=0.05/Dl+2;
	Hohe=1/Dl+2;

//========================WERTE EINLESEN========================================
    werteEinlesen();

//========================FELDER ERSTELLEN======================================
	erstelleFeld(U);
	erstelleFeld(V);
	erstelleFeld(T);
	erstelleFeld(ResU);

//================================STARTWERTE EINRICHTIEN=========================
	for (int j=0; j<Hohe; j++)
	{
		for (int i=0; i<Breite; i++)
		{
			U[j][i]=u1_0;
			V[j][i]=u2_0;
			T[j][i]=t_raum;
			ResU[j][i]=1;
		}
	}

	for (int j=0; j<Hohe; j++)
	{
		T[j][0]=t_wand;
    }

//===============================OFSTREAM RESIDUUMSPLOT===================================
ofstream ResiduumsPlotWerte ("Plotwerte/Residuumsplotwerte.dat");


//===============================BERECHNUNGEN===========================================
int Durchlauf=0;
for (int j=1; j<Hohe-1; j++)
{
    do
    {
		for (int i=1; i<Breite-1; i++)
		{
            tempU=U[j][i];
            T[j][i]= (U[j][i]*T[j-1][i] -V[j][i]*T[j][i+1]
                    + a*(T[j][i-1]+T[j][i+1])/Dl)/ (U[j][i]-V[j][i]+ 2*a/Dl);

            U[j][i]= (U[j][i]*(U[j-1][i]/Dl) - V[j][i]*(U[j][i+1]/Dl)
                      + (nu*(U[j][i-1] + U[j][i+1])/(Dl*Dl)) + g*((T[j][i]-t_raum)/t_raum))
                      /((U[j][i]/Dl)-(V[j][i]/Dl)+ (2*nu/(Dl*Dl)));

            V[j][i+1]=V[j][i] + U[j-1][i] - U[j][i];

			ResU[j][i]= fabs(fabs(U[j][i])-fabs(tempU));
		}Durchlauf++;
	} while (ResUzuGross(ResU, j, Durchlauf, ResiduumsPlotWerte));


}
cout<<"Durchlauf: "<<Durchlauf<<endl<<endl<<endl;

//==========================Close ResiduumsplotOfstream================================================================
ResiduumsPlotWerte.close();

//==========================AUSGABE KONSOLE=============================================================================
cout<<"Drücke Enter zum fortfahren: "<<endl;
getchar();
AusgabeKonsole(U, strngU);

cout<<"Drücke Enter zum fortfahren: "<<endl;
getchar();
AusgabeKonsole(T, strngT);

cout<<"Drücke Enter zum fortfahren: "<<endl;
getchar();
AusgabeKonsole(ResU, strngResU);

cout<<"Drücke Enter zum fortfahren: "<<endl;
getchar();

//=============================ERSTELLE||ERSETZE DIE ERGEBNISS DATEI==================================================
remove ("Ergebnisse.txt");
ofstream Ergebnisse("Ergebnisse.txt", ofstream::app);
Ergebnisse<<"Eingelesene Konstanten: "<<endl<<"µ=  "<<nu<<"\n" <<"a=  "<< a<<"\n"<<"g=  "<<g<<"\n"<<"dL=  "<<Dl<<"\n"<<"t_Wand=  "<<t_wand;
Ergebnisse<<"\n"<<"t_Raum=  "<<t_raum<<"\n"<<"u1_0=  "<<u1_0<<"\n"<<"u1_0=  "<<u2_0<<"\n"<<endl<<endl;
Ergebnisse<<"Anzahl der Schleifendurchläufe: "<<Durchlauf<<endl<<endl<<endl;
TxtAusgabeErgebnisse(U, strngU, Ergebnisse);
TxtAusgabeErgebnisse(V, strngV, Ergebnisse);
TxtAusgabeErgebnisse(T, strngT, Ergebnisse);
Ergebnisse.close();

//========================================PlotDaten==========================================================
ofstream PlotwerteU;
ofstream PlotwerteV;
ofstream PlotwerteT;

PlotWerteAusgeben(U, strngU, PlotwerteU);
PlotWerteAusgeben(V, strngV, PlotwerteV);
PlotWerteAusgeben(T, strngT, PlotwerteT);


cout<<endl<<"Dateien mit Plotwerten wurden erstellt und sind im Ordner 'Plotwerte' zu finden!"<<endl;
cout<<"Eine Ergebnisstabelle ist im Programmordner als Ergebnisse.txt zu finden!"<<endl;



//=========================LÖSCHE DIE FELDER=================================================================
loescheFeld(U);
loescheFeld(V);
loescheFeld(T);
loescheFeld(ResU);


return 0;
}
