#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int jog;   //0 = brancas | 1 =  pretas | -1 = sem peca
    char tipo; //qual peca eh
} peca;


char ambig[2][4];        //guarda o codigo hexadecimal das casas aonde estao as pecas com ambiguidades
int tab_danger[8][8][2]; //0 = branca ataca | 1 = preta ataca | -1 = ninguem ataca
int pieceState[2][2];    //guarda estado da peca, utilizado em ambiguidades
peca tab[8][8];          //tabuleiro, tab[a-f][1-8]


void moverPeca(peca tab[][8], int x0, int y0, int x1, int y1);
int movRBQ(peca tab[][8], int new_x, int new_y, int turno, char tipo, int sq[], int size, int pieceState[2][2]); 
int movTorre(peca tab[][8], int new_x, int new_y, int turno, int pieceState[2][2]);
int movBispo(peca tab[][8], int new_x, int new_y, int turno, int pieceState[2][2]);
int movCavalo(peca tab[][8], int new_x, int new_y, int turno, int pieceState[2][2]);
int movRainha(peca tab[][8], int new_x, int new_y, int turno, int pieceState[2][2]);
int movRei(peca tab[][8], int new_x, int new_y, int turno, int tab_danger[][8][2]);
int movPeao(peca tab[][8], int new_x, int new_y, int turno, int cap, int pieceState[2][2]);
void showGame(peca tab[][8], int nroJogador);
int update_danger(peca tab[][8], int tab_danger[][8][2]);

int main_game(char comando[6], peca tab[][8], int *turno, int *move, int nroJogador);
