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

//Coisas od outro header "xadrez.h"
/*
typedef struct
{
    int jog; //0 = brancas | 1 =  pretas | -1 = sem peca
    char tipo;
} peca;

void moverPeca(peca tab[][8], int x0, int y0, int x1, int y1);
int movRBQ(peca tab[][8], int new_x, int new_y, int turno, char tipo, int sq[], int size, int pieceState[2][2]); // nao usar no main
int movTorre(peca tab[][8], int new_x, int new_y, int turno, int pieceState[2][2]);
int movBispo(peca tab[][8], int new_x, int new_y, int turno, int pieceState[2][2]);
int movCavalo(peca tab[][8], int new_x, int new_y, int turno, int pieceState[2][2]);
int movRainha(peca tab[][8], int new_x, int new_y, int turno, int pieceState[2][2]);
int movRei(peca tab[][8], int new_x, int new_y, int turno);
int movPeao(peca tab[][8], int new_x, int new_y, int turno, int cap, int pieceState[2][2]);
void showGame(peca tab[][8]);
int update_danger(peca tab[][8], int tab_danger_temp[][8]);

int main_game(char comando[6], peca tab[][8], int move, int turno);
*/