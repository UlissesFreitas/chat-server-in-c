#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ECHOMAX 255

void DieWithError(char *errorMessage);

int main(int argc, char *argv[]){

	int sock;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in echoClntAddr;
	unsigned int cliAddrLen;
	char echoBuffer[ECHOMAX];
	unsigned short echoServPort;
	int recvMsgSize;

	if(argc != 2){
		fprintf(stderr, "Uso: %s <porta>", argv[0]);
	}

	echoServPort = atoi(argv[1]);

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if(sock < 0){
		DieWithError("socket() falhou");
	}


	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family = AF_INET;
	echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	echoServAddr.sin_port = htons(echoServPort);

	if(bind(sock, (struct  sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0){
		DieWithError("Bind() falhou");
	}

	for(;;){
		cliAddrLen = sizeof(echoClntAddr);

		recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0,
					(struct sockaddr*) &echoClntAddr, &cliAddrLen);
		if(recvMsgSize < 0){
			DieWithError("recvFrom() falhou");
		}

		printf("Cliente %s\n", inet_ntoa(echoClntAddr.sin_addr));

		if(sendto(sock, echoBuffer, recvMsgSize, 0,
			(struct sockaddr *) &echoClntAddr, sizeof(echoClntAddr)) != recvMsgSize){
			DieWithError("Numero de bytes enviados e direfente dos receidos\n");
		}
	}

	return 0;
}


void DieWithError(char *errorMessage){
	perror(errorMessage);
	exit(1);
}



