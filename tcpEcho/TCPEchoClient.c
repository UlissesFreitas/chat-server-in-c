#include <stdio.h> 		// printf() e fprintf()
#include <sys/socket.h> // socket(), bind() e connect()
#include <arpa/inet.h> 	// sockaddr_in e inet_ntoa()
#include <stdlib.h>		// atoi()
#include <string.h>		// memset()
#include <unistd.h>		//  close()

#define RCVBUFSIZE 32 		// maximo de conexoes


void DieWithError(char *errorMensage);


int main(int argc, char *argv[]){

	int sock;	// Descricao/status do socket do cliente

	struct sockaddr_in echoServAddr; // socket do servidor

	unsigned short echoServPort; // porta do servidor

	char *servIP;
	char *echoString;
	char echoBuffer[RCVBUFSIZE];

	unsigned int echoStringLen;
	int bytesRcvd, totalBytesRcvd;


	if( (argc < 3) || (argc >4) ) {
		fprintf(stderr, "Uso: %s <Porta>\n", argv[0]);
		exit(1);
	}

	servIP = argv[1];
	echoServPort = atoi(argv[2]);
	echoString = argv[3];



	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// caso socket() retorne -1 ocorreu um erro
	if(sock < 0 ){
		DieWithError("O socket falhou");
	}

	memset(&echoServAddr, 0, sizeof(echoServAddr));		// Zera a estrutura

	echoServAddr.sin_family = AF_INET; 					// Endereco IPV4
	echoServAddr.sin_addr.s_addr = inet_addr(servIP);		// IP do servidor
	echoServAddr.sin_port = htons(echoServPort);		// Porta do servidor

	if( connect(sock,(struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0 ){
		DieWithError("O connect falhou");
	}

	echoStringLen = strlen(echoString);

	if(send(sock, echoString, echoStringLen, 0) != echoStringLen){
		DieWithError("O send falhou");
	}

	totalBytesRcvd = 0;
	printf("Received: ");

	while(totalBytesRcvd < echoStringLen){

		bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE -1, 0);
		if(bytesRcvd <=0 ){
			DieWithError("O recv falhou");
		}

		totalBytesRcvd += bytesRcvd;
		echoBuffer[bytesRcvd] = '\0';
		printf("%s", echoBuffer);
	}

	printf("\n");
	close(sock);

	return 0;
}

void DieWithError(char *errorMessage){
	perror(errorMessage);
	exit(1);
}


