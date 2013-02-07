#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

//#include "minmax.h"

#define POTEZ 10
#define PREKID 11

void prekini_Komunikaciju(int sock);

int posaljiPoruku( int sock, int vrstaPoruke, char *poruka )
{
  int duljinaPoruke = strlen( poruka );

	int poslano, poslanoZadnje;
	
	int duljinaPoruke_n = htonl( duljinaPoruke );
	poslanoZadnje = send( sock, &duljinaPoruke_n, sizeof( duljinaPoruke_n ), 0 );
	if( poslanoZadnje != sizeof( duljinaPoruke_n ) ) return 0;

	int vrstaPoruke_n = htonl( vrstaPoruke );
	poslanoZadnje = send( sock, &vrstaPoruke_n, sizeof( vrstaPoruke_n ), 0 );
	if( poslanoZadnje != sizeof( vrstaPoruke_n ) ) return 0;

	poslano = 0;
	while( poslano != duljinaPoruke )
	{
		poslanoZadnje = send( sock, poruka + poslano, duljinaPoruke - poslano, 0 );

		if( poslanoZadnje == -1 || poslanoZadnje == 0 ) return 0;
		poslano += poslanoZadnje;
	}

	return 1;
}


int primiPoruku( int sock, int *vrstaPoruke, char **poruka )
{
	int primljeno, primljenoZadnje;
	
	int duljinaPoruke_n, duljinaPoruke;
	primljenoZadnje = recv( sock, &duljinaPoruke_n, sizeof( duljinaPoruke_n ), 0 );
	if( primljenoZadnje != sizeof( duljinaPoruke_n ) ) return 0;
	duljinaPoruke = ntohl( duljinaPoruke_n );

	int vrstaPoruke_n;
	primljenoZadnje = recv( sock, &vrstaPoruke_n, sizeof( vrstaPoruke_n ), 0 );
	if( primljenoZadnje != sizeof( primljenoZadnje ) ) return 0;
	*vrstaPoruke = ntohl( vrstaPoruke_n );

	*poruka = (char *) malloc( ( duljinaPoruke + 1 ) * sizeof( char ) );

	primljeno = 0;
	while( primljeno != duljinaPoruke )
	{
		primljenoZadnje = recv( sock, *poruka + primljeno, duljinaPoruke - primljeno, 0 );

		if( primljenoZadnje == -1 || primljenoZadnje == 0 ) return 0;
		primljeno += primljenoZadnje;
	}

	(*poruka)[ duljinaPoruke ] = '\0';

	return 1;
}

void ispis_Ploce(char ploca[6][7])
{
	int i,j;
	printf("\t0\t1\t2\t3\t4\t5\t6\n");
	printf("      ----------------------------------------------------- \n");
	for (i=0;i<6;i++)
	{
		printf("\t");
		for(j=0;j<7;j++)
			printf("%c\t", ploca[i][j]);
		printf("\n\n");
	}
}

int posaljiChar(int sock, int VRSTA_PORUKE, char c)
{
	
}

int primiChar(int sock, int *VRSTA_PORUKE, char *c)
{
	
}

void povuci_potez(int sock, char ploca[6][7])
{
	int stupac;
	printf("Upisite redni broj stupca (broj u rasponu 0,..,7) \n");
	scanf("%d", &stupac);
	char c = (char)stupac;
	posaljiChar(sock, POTEZ, c); //posalji indeks stupca za potez koji ces odigrati
	
	int i;
	for (i=5;i>=0;i--)
	{
		if (ploca[i][stupac] == '_') 
		{
			ploca[i][stupac] = 'O';
			break;
		}
	}

	int vrstaPoruke;
	char odgovor;
	int int_odgovor;
	if( primiChar( sock, &vrstaPoruke, &odgovor ) != 1 ) //primi indeks stupca za potez koji ce server odigrati
	{
		printf( "Doslo je do pogreske u komunikaciji sa serverom! \n" );
		exit(1);
	}
	
	int_odgovor = (int)odgovor;

	switch(vrstaPoruke)
	{
		case POTEZ: 
			if(int_odgovor < 0 || int_odgovor >= 7)
			{
				printf( "Doslo je do pogreske - nemam dobar indeks za potez! \n" );
				exit(1);
			}
			
			int i;
			for (i=5;i>=0;i--)
			{
				if (ploca[i][int_odgovor] == '_') 
				{
					ploca[i][int_odgovor] = 'O';
					break;
				}
			}
		case -2: printf("Računalo je pobijedilo čovjeka! \n"); prekini_Komunikaciju(sock); return;
		case -1: printf("Čovjek je pobijedio! \n"); prekini_Komunikaciju(sock); return;
		case -3: printf("Remi! \n"); prekini_Komunikaciju(sock); return;
	}
}

void prekini_Komunikaciju(int sock)
{
	posaljiPoruku( sock, PREKID, "close" );

	int vrstaPoruke;
	char *odgovor;
	if( primiPoruku( sock, &vrstaPoruke, &odgovor ) != 1 )
	{
		printf( "Doslo je do pogreske u komunikaciji sa serverom\n" );
		exit(1);
	}

	if( vrstaPoruke != PREKID )
	{
		printf( "Doslo je do pogreske u komunikaciji sa serverom - prekid komunikacije! \n" );
		exit(1);
	}
	
	if( strcmp( odgovor, "close" ) != 0 )
		printf( "Greska: %s\n", odgovor );
	else
	    printf( "OK\n" );
}

int main( int argc, char **argv )
{
	char ploca[6][7];
	int i,j;
	
	for (i=0;i<6;i++)
		for(j=0;j<7;j++)
			ploca[i][j] = '_';
	
	if( argc != 3 )
	{
		printf( "Upotreba: %s IP-adresa port\n", argv[0] );
		exit(1);
	}

	char dekadskiIP[20]; 
	strcpy( dekadskiIP, argv[1] );
	int port;          
	sscanf( argv[2], "%d", &port );

	int mojSocket = socket( PF_INET, SOCK_STREAM, 0 );
	if( mojSocket == -1 )
	{
		printf("Greska prilikom kreiranja socketa!\n");
			exit(1);
	}
	/*
	struct sockaddr_in adresaServera;
	
	adresaServera.sin_family = AF_INET;
	adresaServera.sin_port = htons( port );
	if( inet_aton( dekadskiIP, &adresaServera.sin_addr ) == 0 )
	{
		printf( "%s nije dobra adresa!\n", dekadskiIP );
		exit(1);
	}
	memset( adresaServera.sin_zero, '\0', 8 );
	
	if( connect( mojSocket,
		        (struct sockaddr *) &adresaServera,
		        sizeof( adresaServera ) ) == -1 )
		{printf( "connect\n" ); exit(1); }
		*/
	int gotovo = 0;
	while( !gotovo )
	{
		printf( "\n\nOdaberi opciju:\n"
				"   1. ispis trenutnog stanja ploče\n"
				"   2. povuci potez\n"
				"   3. prekini igru\n"
				"   \n: " );
				
		int opcija;
		scanf("%d", &opcija);
		
		switch(opcija)
		{
			case 1: ispis_Ploce(ploca); break;
			case 2: povuci_potez(mojSocket,ploca); break;
			case 3: prekini_Komunikaciju(mojSocket); gotovo = 1; break;
			default: printf( "Pogresna opcija \n" ); break;
		}
	}

	close(mojSocket);

	return 0;
}
