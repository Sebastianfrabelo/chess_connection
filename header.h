#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_PORT 2000 //acima das portas reservadas pelo OS
#define BUFFERSIZE 4096   //tamanho max da mensagem
#define MAX_CONNECTIONS 1 //nro de adversarios
typedef struct sockaddr_in STRUCT_IN;
typedef struct sockaddr SA;

int client_socket;
struct sockaddr_in addr;
int sock_cliente;

void handle_connection(int socket);
int verify(int ret, const char *msg);
int conectado(int client_socket, int *pturno, int *pmove);
int host_game(int port, int *pturno, int *pmove,char pecas[]);
int join_game(int port, int *pturno, int *pmove,char pecas[]);

int menu_select;
int port_select;