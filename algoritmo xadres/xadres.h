#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int jog;//0 = brancas | 1 =  pretas | -1 = sem peca
    char tipo;
}peca;

int tab_danger[8][8];//tabuleiro de ataques p/ xeques

void moverPeca(peca tab[][8], int x0, int y0, int x1, int y1);
int movTorre(peca tab[][8], int new_x, int new_y, int turno, char p);
int movBispo(peca tab[][8], int new_x, int new_y, int turno, char p);
int movCavalo(peca tab[][8], int new_x, int new_y, int turno);
int movRainha(peca tab[][8], int new_x, int new_y, int turno);
int movRei(peca tab[][8], int new_x, int new_y, int turno);
int movPeao(peca tab[][8], int new_x, int new_y, int turno,int cap);
void showGame(peca tab[][8]);
int update_danger(peca tab[][8],int *tab_danger_temp[][8]);
