#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define THE_PORT 8989
#define BUFFERSIZE 4096

typedef struct sockaddr_in STRUCT_IN;
typedef struct sockaddr SA;
void handle_connection(int socket);

int main(int argc, char *argv[]) {
int verify(int ret,const char *msg);
	
	int server_socket, client_socket, size;
	STRUCT_IN server_addr, client_addr;
	
	verify((server_socket = socket(AF_INET, SOCK_STREAM,0)),
			"Erro em criar socket!");
				//SOCK_STREAM,0 eh utilizado em conexoes tipo TCP, que garantem o envio do pacote em stream
			
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(THE_PORT);
	
	verify(bind(server_socket,(SA*) &server_addr,sizeof(server_addr)),
			"Erro em criar bind!");	
	
	verify(listen(server_socket,1),
			"Erro na funcao listen!");
				//Linka e escuta a conexao
			
	while(1){
		
		printf("Aguardando conexao...\n\n");

		verify(client_socket = accept(server_socket, (SA*)&client_addr, (socklen_t*)sizeof(STRUCT_IN)), 
			"Erro na conexao");
		printf("Conectado!\n");
		
	
		//codigo de pos - conexao	
		
	}	
						
	return 0;
}

//Funcao utilizada para simplificar verificacoes de erro
int verify(int ret,const char *msg){
	if(ret == -1){
		perror(msg);
		exit(1);
	}
	return ret;
}