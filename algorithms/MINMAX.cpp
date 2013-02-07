#include"MINMAX.h"

//heuristika
int heuristika(int ploca[6][7],int r,int s,int igrac)
{
	int procjena = 0;
	int suma = 0;
	ploca[r][s] = igrac;
	int suprotnaFigura,granicaLijevo,granicaDesno,granicaGore,granicaDolje,prvaStrana,drugaStrana;
	for(int i = 0; i < 6;i++)
	{
		if(i < 3)
		{
			granicaGore = 3;
			granicaDolje = i;
		}
		else
		{
			granicaGore = 5 - i;
			granicaDolje = 3;
		}
		for(int j = 0;j < 7;j++)
		{
			if(ploca[i][j] != 0)
			{
				suprotnaFigura = (-1)*ploca[i][j];
				//os za 0 stupnjeva
				if (j < 3)
                {
                       granicaLijevo = j;
                       granicaDesno = 3;
                }
                else if (j > 3)
                {
                       granicaLijevo = 3;
                       granicaDesno = 6 - j;
                }
                else granicaLijevo = granicaDesno = 3;
                prvaStrana = 0;
                for (int k = 1; k <= granicaLijevo; k++)
                {
					if (ploca[5-i][j - k] == suprotnaFigura) break;
                    prvaStrana++;
                }
                drugaStrana = 0;
                for (int k = 1; k <= granicaDesno; k++)
                {
					if (ploca[5-i][j + k] == suprotnaFigura) break;
                    drugaStrana++;
                }
                if (prvaStrana == 3 && drugaStrana == 3) suma += 4;
                else if (prvaStrana == 3) suma += 1 + drugaStrana;
                else if (drugaStrana == 3) suma += 1 + prvaStrana;
                else if (prvaStrana < drugaStrana) suma += prvaStrana;
                else suma += drugaStrana;
                //os na 90 stupnjeva
                prvaStrana = 0;
                for (int k = 1; k <= granicaGore; k++)
                {
					if (ploca[5 - i - k][j] == suprotnaFigura) break;
                    prvaStrana++;
                }
                drugaStrana = 0;
                for (int k = 1; k <= granicaDolje; k++)
                {
					if (ploca[5 - i + k][j] == suprotnaFigura) break;
                    drugaStrana++;
                }
                if (prvaStrana == 3 && drugaStrana == 3) suma += 4;
                else if (prvaStrana == 3) suma += 1 + drugaStrana;
                else if (drugaStrana == 3) suma += 1 + prvaStrana;
                else if (prvaStrana < drugaStrana) suma += prvaStrana;
                else suma += drugaStrana;
                //os na 45 stupnjeva
                prvaStrana = 0;
                for (int k = 1; (k <= granicaDesno) && (k <= granicaGore); k++)
                {
					if (ploca[5 - i - k][j + k] == suprotnaFigura) break;
                    prvaStrana++;
                }
                drugaStrana = 0;
                for (int k = 1; (k <= granicaLijevo) && (k <= granicaDolje); k++)
                {
					if (ploca[5 - i + k][j - k] == suprotnaFigura) break;
                    drugaStrana++;
                }
                if (prvaStrana == 3 && drugaStrana == 3) suma += 4;
                else if (prvaStrana == 3) suma += 1 + drugaStrana;
                else if (drugaStrana == 3) suma += 1 + prvaStrana;
                else if (prvaStrana < drugaStrana) suma += prvaStrana;
                else suma += drugaStrana;
                //os na 135 stupnjeva
                prvaStrana = 0;
                for (int k = 1; (k <= granicaLijevo) && (k <= granicaGore); k++)
                {
					if (ploca[5 - i - k][j - k] == suprotnaFigura) break;
                    prvaStrana++;
                }
                drugaStrana = 0;
                for (int k = 1; (k <= granicaDesno) && (k <= granicaDolje); k++)
                {
					if (ploca[5 - i + k][j + k] == suprotnaFigura) break;
                    drugaStrana++;
                }
                if (prvaStrana == 3 && drugaStrana == 3) suma += 4;
                else if (prvaStrana == 3) suma += 1 + drugaStrana;
                else if (drugaStrana == 3) suma += 1 + prvaStrana;
                else if (prvaStrana < drugaStrana) suma += prvaStrana;
                else suma += drugaStrana;
                procjena += ploca[i][j] * suma;
                 
                }
            }
        }
		ploca[r][s] = 0;
        return procjena;
}

bool ispitajPobjedu(int ploca[6][7],int figura)
{
	int brojFigura;
	for(int i = 0;i < 6;i++)
		for(int j = 0;j < 7;j++)
		{
            //os na 0 stupnjeva
            brojFigura = 0;
            for (int k = 0; k < 4; k++)
            {
				if ((j - k) < 0 || ploca[i][j - k] != figura) break;
                brojFigura++;
            }
            if(brojFigura == 4) return true;
			brojFigura = 0;
            for (int k = 0; k < 4; k++)
            {
				if ((j + k) > 6 || ploca[i][j + k] != figura) break;
                brojFigura++;
            }
			if(brojFigura == 4) return true;
			brojFigura = 0;
            //os na 90 stupnjeva
            for (int k = 0; k < 4; k++)
            {
				if ((i + k) > 5 || ploca[i + k][j] != figura) break;
				brojFigura++;
            }
			if(brojFigura == 4) return true;
			brojFigura = 0;
            for (int k = 0; k < 4; k++)
            {
				if ((i - k) < 0 || ploca[i - k][j] != figura) break;
                    brojFigura++;
            }
            if(brojFigura == 4) return true;
			brojFigura = 0;
            //os na 45 stupnjeva
            for (int k = 0; k < 4; k++)
            {
				if ((i + k) > 5 || (j + k) > 6 || ploca[k + i][j + k] != figura) break;
                    brojFigura++;
            }
            if(brojFigura == 4) return true;
			brojFigura = 0;
            for (int k = 0; k < 4; k++)
            {
				if ((i + k) > 5 || (j - k) < 0 || ploca[i + k][j - k] != figura) break;
				brojFigura++;
            }
			if(brojFigura == 4) return true;
		}
         return false;
}

vector< pair<int,int> > pronadiPoteze(int ploca[6][7])
{
	vector<pair<int,int> > potezi;
	for(int j = 0;j < 7;j++)
	{
		for(int i = 0;i < 6;i++)
		{
			if(ploca[i][j] == 0)
			{
				pair<int,int> p(i,j);
				potezi.push_back(p);
				break;
			}
		}
	}
	return potezi;
}



 int MaxCvor(int ploca[6][7],int r,int s,int* alfa, int* beta, int dubina)
{
	ploca[r][s] = -1;
	if(ispitajPobjedu(ploca,-1))
	{
		ploca[r][s] = 0;
		return (-673) * (dubina + 1);
	}
	if (dubina == 0)
	{
		ploca[r][s] = 0;
		return heuristika(ploca,r,s,-1);
	}
	vector<pair<int,int> > potezi = pronadiPoteze(ploca);
	if (potezi.size() == 0)
	{
		ploca[r][s] = 0;
		return heuristika(ploca,r,s,-1);
	}
	dubina--;
	int cvorVrijednost, temp;
    cvorVrijednost = *alfa;
    for(int i = 0; i < potezi.size(); i++)
    {
		temp = MinCvor(ploca,potezi[i].first,potezi[i].second,&cvorVrijednost, beta, dubina);
		ploca[r][s] = 0;
		if (temp > cvorVrijednost) cvorVrijednost = temp;  //minimax
		if (cvorVrijednost >= *beta)
		{
			ploca[r][s] = 0;
			return cvorVrijednost; //beta podrezivanje
		}
	}
	ploca[r][s] = 0;
	return cvorVrijednost;
}

int MinCvor(int ploca[6][7],int r,int s, int* alfa, int* beta, int dubina)
{
	ploca[r][s] = 1;
	if (ispitajPobjedu(ploca,1))
	{
		ploca[r][s] = 0;
		return (673) * (dubina + 1);
	}
	if (dubina == 0)
	{
		ploca[r][s] = 0;
		return heuristika(ploca,r,s,1);
	}
	vector<pair<int,int> > potezi = pronadiPoteze(ploca);
	if (potezi.size() == 0)
	{
		ploca[r][s] = 0;
		return heuristika(ploca,r,s,1);
	}
	dubina--;
	int cvorVrijednost, temp;
	cvorVrijednost = *beta;
	int i;
	for (i = 0; i < potezi.size(); i++)
	{
		temp = MaxCvor(ploca,potezi[i].first,potezi[i].second, alfa, &cvorVrijednost, dubina);
		if (temp < cvorVrijednost) cvorVrijednost = temp;   //minimax
		if (cvorVrijednost <= *alfa)
		{
			ploca[r][s] = 0;
			return cvorVrijednost;  //alfa podrezivanje
		}
	}
	ploca[r][s] = 0;
	return cvorVrijednost;
}


//odredivanje sljedeceg poteza
int potez(int ploca[6][7],int dubina)
{
	dubina--;
	int korijenVrijednost,temp;
	int cvor[2] = {0,9};
	korijenVrijednost = INT_MIN;
	vector<pair<int,int> > potezi = pronadiPoteze(ploca);
	//for(int j = 0;j < potezi.size(); j++) cout<<potezi[j].second<<endl;
	int i;
	int alfa = INT_MIN;
	int beta = INT_MAX;
	for(i = 0;i < potezi.size(); i++)
	{
		ploca[potezi[i].first][potezi[i].second] = -1;
		if(ispitajPobjedu(ploca,-1)) return potezi[i].second;
		ploca[potezi[i].first][potezi[i].second] = 0;
	}
	for(i = 0;i < potezi.size(); i++)
	{
		temp = MinCvor(ploca,potezi[i].first,potezi[i].second,&alfa,&beta,dubina);
		if(temp > korijenVrijednost)
		{
			korijenVrijednost = temp;
			cvor[0] = potezi[i].first;
			cvor[1] = potezi[i].second;
		}
	}
	if(cvor[1] != 9)
	{
		ploca[cvor[0]][cvor[1]] = 1;
		if(ispitajPobjedu(ploca,1)) return 8;
	}
	return cvor[1];
}


//funkcija koju poziva server,moze se zadati dubina
int MinMaxAlfaBeta(int ploca[6][7])
{
	bool prviSam = 1;
	int dubina = 5;
	for(int j = 0;j < 7;j++)
	{
			if(ploca[0][j] != 0)
			{
				prviSam = 0;
				break;
			}
	}
	//cout<<prviSam<<endl;
	if(prviSam) return 3;
	if(ispitajPobjedu(ploca,-1)) return 7;
	return potez(ploca,dubina);
}


