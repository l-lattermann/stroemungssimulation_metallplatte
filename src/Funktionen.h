#ifndef FUNKTIONEN_INCLUDED
#define FUNKTIONEN_INCLUDED

#include <fstream>
#include <iostream>
#include <limits>
#include <math.h>



using namespace std;

void Hallo()
{
    cout <<"==============================================================================="<<endl;
    cout <<"|"<<setw(50)<<"Numerik-Projekt"<<"                                            |"<<endl;
    cout <<"==============================================================================="<<endl;
    cout <<"|                                                                             |"<<endl;
    cout <<"| Authoren:  Laurenz Lattermann                                               |"<<endl;
    cout <<"==============================================================================="<<endl;
    cout <<endl<<endl;
    cout <<"Diese Programm berechnet die Strömungsgeschwindigkeiten von Luft an einer vertikalen heissen Platte."<<endl;
    cout <<"Im Programmordner liegt eine Datei Startwerte.txt. Dort können Sie problemrelevante Konstanten eingeben."<<endl;
    cout <<"Die Ergebnisse werden ebenfalls im Programmordner in der Datei Ergebnisse.txt gespeichert."<<endl<<endl<<endl<<endl<<endl;
    cout <<"Im Folgenden können Sie den Verlauf der Residuen im Rechenfeld beobachten. "<<endl<<endl;
    cout <<"Drücken Sie eine beliebige Taste, um fortzufahren:"<<endl;

    getchar();
    cout<<endl<<endl<<endl;

}
//==========================================LIEẞT WERTE EIN UND GIBT SIE AUF DER KONSOLE AUS=================================
istream & skipline (istream& in)
{

    return in.ignore(numeric_limits<streamsize>::max(), '\n'); //gibt den String zurück,
                                                                    //bis ein Absatz kommt (das gibt der parameter "\n" an)
}

int werteEinlesen ()
{
    ifstream input ("Startwerte.txt"); //hier liegt meine Textdatei

    //prüfen, ob die Datei vorhanden ist
   if (!input){
    cerr << "Fehler beim Oeffnen der Datei!" <<endl; // cerr könnte auch cout heißen, kommt aufs Gleiche raus
    return -1;
    }


    // prüfen ob die Parameter vorhanden sind,  dabei müssen sie jeweils in einer Zeile stehen (skipline)
    if (input >> nu >> skipline >> a >> skipline >> g >> skipline >> Dl >> skipline >> t_wand >> skipline >> t_raum >> skipline >> u1_0 >> skipline >> u2_0 >> skipline >> Genauigkeit)
    {
        //Parameter ausgeben, da könnten anstatt auch rechenoperationen stehen
        cout<<"Eingelesene Werte: "<<endl;

        cout<<"µ=  "<<nu<<"\n" <<"a=  "<< a<<"\n"<<"g=  "<<g<<"\n"<<"t_Wand=  "<<t_wand;
        cout<<"\n"<<"t_Raum=  "<<t_raum<<"\n"<<"u1_0=  "<<u1_0<<"\n"<<"u2_0=  "<<u2_0<<"\n"<<"DeltaL= "<<Dl<<"\n"<<"Rechengenauigkeit= "<<Genauigkeit<<endl<<endl;

    }
    cout<<"Drücke Enter zum berechnen "<<endl;
    getchar();
    return 0;
}

void PlotWerteAusgeben(double **FeldPtr, string Name, ofstream &Plotwerte)
{
    string ofstrmName;
    ofstrmName= "Plotwerte/"+Name+"Plotwerte.dat"; // Erstelle String für den jeweiligen Namen der outputfile
    Plotwerte.open(ofstrmName.c_str()); // öffne Datei mit dem StringNamen. .c_str() wandelt in für Funktion verwenbaren String um
    if(Plotwerte.is_open())
    {
        Plotwerte<<"Plotwerte für: "<<Name<<endl<<endl;


        for (int j=0; j<Hohe; j+=5)
        {
            for (int i=0; i<Breite; i++)
            {
                Plotwerte<<j*Dl<<"      "<<i*Dl<<"      "<< FeldPtr[j][i]<<endl;
            }
        }
    } else
        {
            cout<<ofstrmName<<" konnte nicht geoeffnet werden!!"<<endl;
        }

    Plotwerte.close();
}

//========================================ERSTELLE FELD=====================================================================
void erstelleFeld(double **&FeldPtr)
{
    FeldPtr= new double *[Hohe];
	for (int j=0; j<Hohe; j++)
	{
		FeldPtr[j]= new double [Breite];
	}
}

//=========================================LÖSCHE FELD=========================================================================
 void loescheFeld(double **FeldPtr)
{
    for (int j=0; j<Hohe; j++)
	{
		delete[] FeldPtr [j];
	}
	delete [] FeldPtr;
	FeldPtr= NULL;
}
//=============================================================================================================================




//========================================PRÜFT OB RESIDUUM AN ALLEN STELLEN IN ORDNUNG================================
bool ResUzuGross(double **FeldPt, int j, int Durchlauf, ofstream &ResiduumsPlotWerte)
{
    double compVar=Genauigkeit;

    for(int i=1; i<Breite-2; i++)
    {
        if(ResU[j][i]>compVar)
        {
            compVar=ResU[j][i];     //Suche das größte Residuum im Feld
        }
    }

    if (compVar>Genauigkeit)
    {
        cout<<"Residuum:   "<<compVar<<endl;
        if(j==0.1/Dl||j==0.2/Dl||j==0.3/Dl||j==0.7/Dl)
        {
            ResiduumsPlotWerte<<j<<"  "<<compVar<<endl; //Speicher das größte Residuum im Log
        }
        if(j==1400)
        {
            ResiduumsPlotWerte<<j<<"  "<<compVar<<endl; //Speicher das größte Residuum im Log
        }
        return true;
    }else{return false;}
}
//=============================================================================================================================




//==========================================ERGEBNISSAUSGABE IN TXT DATEI====================================
void TxtAusgabeErgebnisse(double **U, string strngU, ofstream &Ergebnisse)
{
    Ergebnisse<<"Die approximierten Werte für "<<strngU<<":"<<endl;
    Ergebnisse<<"          ";
    for (int i=0; i<6; i++)
    {
        Ergebnisse<<setw(21)<<(HoheArr[i]+1)*Dl<<"m";
    }
    Ergebnisse<<endl<<"________________________________________________________________________________________________________________________________________________"<<endl;

    {int tHohe, tBreite;
    Ergebnisse<<setw(5)<<BreiteArr[0]<<setw(3)<<"m  |";
    for (int i=0; i<6; i++)
        {
            tHohe=HoheArr[i];
            Ergebnisse<<setw(22)<<U[tHohe][0];
        }Ergebnisse<<endl;

    for (int j=1; j<19; j++)
    {
        Ergebnisse<<setw(5)<<(BreiteArr[j]+1)*Dl<<setw(3)<<"m  |";
        for (int i=0; i<6; i++)
        {
            tBreite=BreiteArr[j];
            tHohe=HoheArr[i];
            Ergebnisse<<setw(22)<<U[tHohe][tBreite];
        }
        Ergebnisse<<endl;
    }Ergebnisse<<endl<<endl<<endl<<endl;
    }

     HoheArr[0]=0.01/Dl-1;
     HoheArr[1]=0.12/Dl-1;
     HoheArr[2]=0.2/Dl-1;
     HoheArr[3]=0.3/Dl-1;
     HoheArr[4]=0.7/Dl-1;
     HoheArr[5]=1/Dl-1;

     BreiteArr[0]=0;
     if(Dl==0.0005)
     {
        BreiteArr[1]=0.0005/Dl;
     }
     else {BreiteArr[1]=0.0005/Dl-1;}

     if(Dl==0.0005)
     {
        BreiteArr[2]=0.001/Dl;
     }
     else {BreiteArr[2]=0.001/Dl-1;}
     BreiteArr[3]=0.002/Dl-1;
     BreiteArr[4]=0.003/Dl-1;
     BreiteArr[5]=0.004/Dl-1;
     BreiteArr[6]=0.005/Dl-1;
     BreiteArr[7]=0.006/Dl-1;
     BreiteArr[8]=0.007/Dl-1;
     BreiteArr[9]=0.008/Dl-1;
     BreiteArr[10]=0.009/Dl-1;
     BreiteArr[11]=0.01/Dl-1;
     BreiteArr[12]=0.015/Dl-1;
     BreiteArr[13]=0.02/Dl-1;
     BreiteArr[14]=0.025/Dl-1;
     BreiteArr[15]=0.03/Dl-1;
     BreiteArr[16]=0.035/Dl-1;
     BreiteArr[17]=0.04/Dl-1;
     BreiteArr[18]=0.05/Dl-1;

}
//=============================================================================================================================




//===========================================ERGEBNISSAUSGABE AUF KONSOLE======================================================
void AusgabeKonsole(double **FeldPtr, string FeldName)
{
    cout<<"Werte für "<<FeldName<<": "<<endl;
	for (int j=0; j<19; j++)
	{
		for (int i=0; i<6; i++)
		{
            int tBreite, tHohe;
            tBreite= BreiteArr[j];
            tHohe= HoheArr[i];
			cout<<setw(12)<<FeldPtr[tHohe][tBreite]<<" ";
        }
		cout<<endl;
	}cout<<endl;


	HoheArr[0]=0.01/Dl-1;
     HoheArr[1]=0.12/Dl-1;
     HoheArr[2]=0.2/Dl-1;
     HoheArr[3]=0.3/Dl-1;
     HoheArr[4]=0.7/Dl-1;
     HoheArr[5]=1/Dl-1;

     BreiteArr[0]=0;
     if(Dl==0.0005)
     {
        BreiteArr[1]=0.0005/Dl;
     }
     else {BreiteArr[1]=0.0005/Dl-1;}

     if(Dl==0.0005)
     {
        BreiteArr[2]=0.001/Dl;
     }
     else {BreiteArr[2]=0.001/Dl-1;}

     BreiteArr[3]=0.002/Dl-1;
     BreiteArr[4]=0.003/Dl-1;
     BreiteArr[5]=0.004/Dl-1;
     BreiteArr[6]=0.005/Dl-1;
     BreiteArr[7]=0.006/Dl-1;
     BreiteArr[8]=0.007/Dl-1;
     BreiteArr[9]=0.008/Dl-1;
     BreiteArr[10]=0.009/Dl-1;
     BreiteArr[11]=0.01/Dl-1;
     BreiteArr[12]=0.015/Dl-1;
     BreiteArr[13]=0.02/Dl-1;
     BreiteArr[14]=0.025/Dl-1;
     BreiteArr[15]=0.03/Dl-1;
     BreiteArr[16]=0.035/Dl-1;
     BreiteArr[17]=0.04/Dl-1;
     BreiteArr[18]=0.05/Dl-1;
}
//=============================================================================================================================



#endif // FUNKTIONEN_INCLUDED
