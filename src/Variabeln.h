#ifndef VARIABELN_H_INCLUDED
#define VARIABELN_H_INCLUDED

#include <string>
using namespace std;

long int Hohe;
long int Breite;

double **U;
double **V;
double **T;
double **ResU;

string strngU="U";
string strngV="V";
string strngT="T";
string strngResU="ResU";


double a;
double nu;
double g;

double Genauigkeit;
double Dl=0.0005;
double tempU;
double t_raum;
double t_wand;
double u1_0;
double u2_0;


int BreiteArr[19];
int HoheArr[6];



#endif // VARIABELN_H_INCLUDED
