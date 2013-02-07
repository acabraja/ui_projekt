#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "../../algorithms/minmax.h"

void ispis_Ploce(char ploca[][WIDTH])
{
	int i, j;
	printf("\t0\t1\t2\t3\t4\t5\t6\n");
	printf("      ----------------------------------------------------- \n");
	for (i = 0; i < HEIGHT; i++)
	{
		printf("\t");
		for(j = 0; j < WIDTH; j++)
			printf("%c\t", ploca[i][j]);
		printf("\n\n");
	}
}

int primiPoruku(int sock)
{
	char buffer[1024];
	memset(buffer, 0 ,1024);
	if(recv(sock, buffer, 1023, 0) <= 0){
		close(sock);
		exit(EXIT_FAILURE);
	}

	char value = buffer[0];
	int newMove = atoi(&value);
	return newMove;
}

void posaljiPoruku(int sock, int poruka)
{
	char buffer[1024];
	memset(buffer, 0, 1024);
	buffer[0] = poruka + '0';
	if(send(sock, buffer, strlen(buffer), 0) <= 0)
	{
		close(sock);
		exit(EXIT_FAILURE);
	}
}

int povuci_potez(int sock, char ploca[6][7])
{
	int stupac, i, odgovor;
	char Close[1042];
	memset(Close, 0, 1024);
	Close[0] = 'C';
	while(1)
	{
		printf("Upisite redni broj stupca (broj u rasponu 0,..,6) \n");
		scanf("%d", &stupac);

		if(stupac < 0 || stupac > 7)
		{
			printf("Nemoguc unos. pokusajte ponovo \n");
			continue;
		}

		posaljiPoruku(sock, stupac); //posalji indeks stupca za potez koji ces odigrati
		break;
	}

	for (i = 5; i >= 0; i--)
	{
		if (ploca[i][stupac] == '_') 
		{
			ploca[i][stupac] = 'O';
			break;
		}
	}

	odgovor = primiPoruku(sock);
	//printf("\n Odgovor: %d",odgovor);

	if(odgovor < 0 )
	{
		printf( "Doslo je do pogreske! Server pokusao nemoguc potez sa serverom! \n" );
		close(sock);
		exit(EXIT_FAILURE);
	}
	else{
		if( odgovor == AGENT_WIN ){
			printf("Aplikacija je pobjedila\n");
			//send(sock, Close, sizeof(Close), 0);
			return 1;
		}
		if( odgovor == PLAYER_WIN ){
			printf("Igrac je pobjedila\n");
			//send(sock, Close, sizeof(Close), 0);
			return 1;
		}
		if(odgovor == DRAW){
			printf("Nerijeseno\n");
			//send(sock, Close, sizeof(Close), 0);
			return 1;
		}
		for (i = 5; i >= 0; i--)
		{
			if (ploca[i][odgovor] == '_') 
			{
				ploca[i][odgovor] = 'X';
				break;
			}
		}
	}
	return 0;
}
int main( int argc, char **argv )
{
	char ploca[6][7];
	int i,j;
	
	for (i = 0 ;i < 6; i++)
		for(j = 0; j < 7; j++)
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
			case 2: gotovo = povuci_potez(mojSocket,ploca); break;
			case 3: gotovo = 1; printf("Odlucili ste zavrsiti igru\n"); break;
			default: printf( "Pogresna opcija \n" ); break;
		}
	}
	
	close(mojSocket);

	return 0;
}
