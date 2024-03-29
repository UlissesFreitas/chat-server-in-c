#include <stdio.h> 		// printf() e fprintf()
#include <sys/socket.h> // socket(), bind() e connect()
#include <arpa/inet.h> 	// sockaddr_in e inet_ntoa()
#include <stdlib.h>		// atoi()
#include <string.h>		// memset()
#include <unistd.h>		//  close()

#define MAXCONEX 5 		// maximo de conexoes
#define RCVBUFSIZE 32 	// maximo de dados transmitidos


void DieWithError(char *errorMensage);
void HandleTCPClient(int clntSocket);


int main(int argc, char *argv[]){

	int servSock;	// Descricao/status do socket do servidor
	int clntSock;	// Descricao/status do socekt do cliente

	struct sockaddr_in echoServAddr; // socket di servidor
	struct sockaddr_in echoClntAddr; // socket do cliente

	unsigned short echoServPort; // porta do servidor
	unsigned int clntLen;		// tamando da estrutura de endereco

	if( argc != 2) {
		fprintf(stderr, "Uso: %s <Porta>\n", argv[0]);
		exit(1);
	}
	// atoi(), conveerte string em int
	echoServPort = atoi(argv[1]);


	servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// caso socket() retorne -1 ocorreu um erro
	if(servSock < 0 ){
		DieWithError("O socket falhou");
	}

	memset(&echoServAddr, 0, sizeof(echoServAddr));		// Zera a estrutura

	echoServAddr.sin_family = AF_INET; 					// Endereco IPV4
	echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);	// Qualquer interface
	echoServAddr.sin_port = htons(echoServPort);			// Porta do servidor

	if( bind(servSock, (struct sockaddr*) &echoServAddr, sizeof(echoServAddr)) < 0 ){
		DieWithError("O Bind falhou\n");
	}

	if( listen(servSock, MAXCONEX) ){
		DieWithError("O listen falhou");
	}

	for (;;) {
		clntLen = sizeof(echoClntAddr);

		clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen);
		if( clntSock < 0){
			DieWithError("O accept falhou");
		}

		printf("Cliente conectado <%s>", inet_ntoa(echoClntAddr.sin_addr));

		HandleTCPClient(clntSock);
	}

	return 0;
}

void DieWithError(char *errorMessage){
	perror(errorMessage);
	exit(1);
}


void HandleTCPClient(int clntSocket){

	char echoBuffer[RCVBUFSIZE]; // Buffer que armazena as mensagens
	int recvMsgSize; // tamanho da mensagem recebida


	// recv() escreve dentro do echoBuffer a mensagem que esta contida no clntSocket
	// recv() retorna o tamanho da mensagem recebida ou -1 se ocorrer um erro
	recvMsgSize = 1;

	while(1){
	// recvMsgSize >0

		recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0);
		if(recvMsgSize <0){
			close(clntSocket);
			DieWithError("O recv falhou");
		}

		 if(send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize){
            close(clntSocket);
			DieWithError("O send falhou");
        }

	}

}



