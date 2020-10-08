#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>

#define THE_PORT 1234
#define BUFFERSIZE 4096

typedef struct sockaddr_in STRUCT_IN;
typedef struct sockaddr SA;

void handle_connection(int socket);
int verify(int ret,const char *msg);

pthread_mutex_t mutexsum = PTHREAD_MUTEX_INITIALIZER;  
void *sendmessage();
void *listener();
int    sock_cliente;
struct sockaddr_in addr;
int done=1;

void *status;
pthread_attr_t attr;


int main(int argc, char *argv[]) {
	
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
		client_socket = accept(server_socket,0,0); // (SA*)&client_addr, (socklen_t*)sizeof(STRUCT_IN));
		verify(client_socket, 
			"Erro na conexao");
			
		printf("Conectado!\n");
		
		//codigo de pos - conexao	
		
		
			pthread_t threads[2];
			pthread_attr_init(&attr);
			pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

			pthread_create(&threads[0], &attr, sendmessage, NULL);
			pthread_create(&threads[1], &attr, listener, NULL);

			while(done){}
		
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



void *sendmessage(){
	int  enviados;
	char mensagem[256];

	do{  
		printf("Server: ");
		fflush(stdin);
		//fgets(mensagem,256,stdin);
		scanf("%s",mensagem);
		mensagem[strlen(mensagem)-1] = '\0';
		enviados = send(sock_cliente,mensagem,strlen(mensagem),0);
	}while(strcmp(mensagem,"exit")!=0);
	return NULL;
}

void *listener(){
	int recebidos;
	char resposta[256];
	
	do{
		recebidos = recv(sock_cliente,resposta,256,0);              /* Recebe mensagem do cliente */
		resposta[recebidos] = '\0';
		printf("\n Cliente: %s\n",resposta);
	}while(strcmp(resposta,"exit")!=0); 
	
	pthread_mutex_destroy(&mutexsum);
	pthread_exit(NULL);
	done=0;
	return NULL;
}