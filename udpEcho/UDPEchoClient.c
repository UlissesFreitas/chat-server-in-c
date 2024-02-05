#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ECHOMAX 255

void DieWithError(char *erroMessage);

int main(int argc, char *argv[]){

	int sock;						// socket descriptor
	struct sockaddr_in echoServAddr;// endereco do servidor echo
	struct sockaddr_in fromAddr;		// endereco do echo

	unsigned short echoServPort;	// porta do servidor echo
	unsigned int fromSize;			// tamanho do in-out recvfrom()

	char *servIP;					// endereco IP do servidor
	char *echoString;				// string para enviar ao servidor
	char echoBuffer[ECHOMAX+1];		// buffer ppara receber a string

	int echoStringLen;				// tamanho do string enviada
	int respStringLen;				// tamanho da resposta recebida

	if((argc < 3) || (argc > 4)) {
		fprintf(stderr, "Uso: %s <Server IP> <Porta> <mensagem> \n", argv[0]);
		exit(1);
	}

	servIP = argv[1];
	echoServPort = atoi(argv[2]);
	echoString = argv[3];

	// verifica o tamanho da string de entrada que sera enviada ao servidor
	if((echoStringLen = strlen(echoString)) > ECHOMAX) {
		DieWithError("Echo e muiti longo");
	}

	// cira o socket UPD
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sock < 0){
		DieWithError("socket() failed");
	}


	// constroi a estrutura de endereco do servidor
	memset(&echoServAddr, 0, sizeof(echoServAddr));	// zera a estrutura
	echoServAddr.sin_family = AF_INET;					// endereco de internet IPV4
	echoServAddr.sin_addr.s_addr = inet_addr(servIP);	// endereco IP do servidor
	echoServAddr.sin_port = htons(echoServPort);		// Porta do servidor


	// envia a string ao servidor
	if(sendto(sock, echoString, echoStringLen, 0,
	(struct sockaddr*) &echoServAddr, sizeof(echoServAddr)) != echoStringLen) {
		DieWithError("Sendto() Error");
	}


	// recebe uma resposta
	fromSize = sizeof(fromAddr);
	respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr *) &fromAddr, &fromSize);

	if(respStringLen != echoStringLen){
		DieWithError("recvfrom() failed");
	}

	if(echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr){
		fprintf(stderr, "Erro: ");
		exit(1);
	}

	echoBuffer[respStringLen] = '\0';
	printf("Recebido: %s\n", echoBuffer);

	close(sock);

	return 0;
}


void DieWithError(char *erroMessage){
	exit(1);
}
