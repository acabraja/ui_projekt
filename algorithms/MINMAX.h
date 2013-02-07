//#include <iostream>
#include<vector>
#include<utility>
#include<climits>

using namespace std;

//heuristika
int heuristika(int ploca[6][7]);

bool ispitajPobjedu(int ploca[6][7]);

vector< pair<int,int> > pronadiPoteze(int ploca[6][7]);

int MaxCvor(int ploca[6][7],int r,int s,int* alfa,int* beta, int dubina);

int MinCvor(int ploca[6][7],int r,int s,int* alfa,int* beta, int dubina);

int MinMaxAlfaBeta(int ploca[6][7]);

int MinMaxAlfaBeta(int ploca[6][7]);
