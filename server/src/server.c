#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "../../algorithms/minmax.h"
#include "../../algorithms/MINMAX.h"
// the default server port
#define	DEFAULT_PORT	64201

short int newInstanceAlgorithm(int sock);


//server Instance
typedef struct{
	int sock;
}Data;

void *serverInstance(Data *const data)
{
	newInstanceAlgorithm(data->sock);
}

// startServer()
// 	returns the server socket, or -1 if we couldn't bind the socket
int startServer(int port) {
	int sockfd;
	struct sockaddr_in address;
	int yes = 1;

	// open the server socket
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr,"Unable to open socket!\n");
		return -1;
	}

	// setup bind stuffs
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	//address.sin_port = htons(port);
	address.sin_port = htons((unsigned short)port);

	// bind the socket
	if (bind(sockfd, (struct sockaddr *)&address, sizeof(struct sockaddr)) == -1) {
		fprintf(stderr,"Unable to bind socket!\n");
		return -1;
	}

	// allow socket rebinding of local ports
	if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
		fprintf(stderr,"Unable to set reuseable socket binding property...\n");
	}

	// listen for connections
	if (listen(sockfd,10) != 0) {
		fprintf(stderr,"Unable to set socket for listen mode!\n");
		return -1;
	}

	// return the socket
	return sockfd;
}

// newInstanceAlgorithm()
// 	returns 1 for normal messages, and -1 if we received a shutdown message
short int newInstanceAlgorithm(int sockfd) {
	char buffer[1024];
	short int message = 1;
	char returnValue = 0;
	int newMove;
	int board[HEIGHT][WIDTH]; // TODO get define value 
  int in_col[WIDTH];	
	// init empty board
	memset(board, 0, sizeof(board)); 
	memset(in_col, 0, sizeof(in_col));
	
	while(1){
		// erase the buffer
		memset(buffer, 0, 1024);
		
		// Recive: client's new move 
		returnValue = recv(sockfd, buffer, 1023, 0);
		if (returnValue<=0){
			close(sockfd);
			break;
		}

		// check for shutdown message
		// we'll need a more secure shutdown method in the future...
		if (strcmp(buffer,"close") == 0) {
			printf("\nReceiving closing message...\nClosing forked subprocess %d\n\n",getpid());
			message = -1;
			close(sockfd);
			break;
		}

		// Call algorithm
		char value = buffer[0];	
		newMove = atoi(&value);
		board[5 - in_col[newMove]++][newMove] = YELLOW;
	int i, j;
	printf("\n\t0\t1\t2\t3\t4\t5\t6\n");
	printf("      ----------------------------------------------------- \n");
	for (i = 0; i < HEIGHT; i++)
	{
		printf("\t");
		for(j = 0; j < WIDTH; j++)
			printf("%d\t", board[i][j]);
		printf("\n\n");
	}

		//printf("\nPrimio sam %d",newMove);
		//newMove =MinMaxAlfaBeta(board);
		newMove = minMax(board);
		/* saljem plocu board sa trenutnim stanjem i ocekujem povratno gdje ubaciti novi potez*/
		//printf("\n Saljem %d",newMove);
		value = newMove + '0';
		buffer[0] = value;
		
		board[5 - in_col[newMove]++][newMove] = ORANGE;

		// Send: server's new move
		returnValue = send(sockfd, buffer, strlen(buffer), 0);
		if (returnValue<=0){
			close(sockfd);
			break;
		}
	}

	// return -1 on shutdown, 1 otherwise
	return message;
}

// waitForConnections()
// 	waits in an infinite loop for sockect connections, then passes them to 
void waitForConnections(int sockfd) {
	pthread_t *pth;
	struct sockaddr_in remoteAddress;
	int remoteAddressSize = sizeof(struct sockaddr_in);
	int remoteSocket;
	
	int newpid = 0;
	
	// listen for connections for...ever (or until we get a shutdown message)
	for (;;) {
		// accept the remote connection
		remoteSocket = accept(sockfd, (struct sockaddr *)&remoteAddress, (socklen_t*)&remoteAddressSize);
		
		// preparetion for thread
		pth = (pthread_t*)malloc(sizeof(pthread_t));
		Data data;
		data.sock = remoteSocket;
		
		newpid = 0;
		newpid = fork();
		
		if (newpid > 0){
			printf("\nConnection request arrived. Process %d forked on process %d", getpid(), newpid);
			fflush(stdout);
		}
		
		if (newpid==0)
			pthread_create(pth, NULL, (void*(*)(void*))serverInstance, &data);
	}
}

int main(int argc, char **argv) {
	int sockfd = 0, port;

	if (argc > 1) {
		// user specified a port
		port = atoi(argv[1]);
	} else {
		// otherwise, use the default port
		port = DEFAULT_PORT;
	}

	if ((sockfd = startServer(port)) != -1) {
		// wait for incoming connections
		waitForConnections(sockfd);

		// close the server socket
		close(sockfd);
	}

	return EXIT_SUCCESS;
}
