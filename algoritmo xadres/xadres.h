#pragma once

typedef struct {
    int jog;//0 = brancas | 1 =  pretas | -1 = sem peca
    char tipo;
}peca;

void moverPeca(peca** tb, int x0, int y0, int x1, int y1);
int movTorre(peca** tab, int new_x, int new_y, int turno, char p);
int movBispo(peca** tab, int new_x, int new_y, int turno, char p);
int movCavalo(peca** tab, int new_x, int new_y, int turno);
int movRainha(peca** tab, int new_x, int new_y, int turno);
int movRei(peca** tab, int new_x, int new_y, int turno);
int movPeao(peca** tab, int new_x, int new_y, int turno,int cap);