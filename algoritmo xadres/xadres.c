#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "xadres.h"

void moverPeca(peca tab[][8], int x0, int y0, int x1, int y1) {
    tab[x1][y1].tipo = tab[x0][y0].tipo;
    tab[x1][y1].jog = tab[x0][y0].jog;

    tab[x0][y0].tipo = 'z';
    tab[x0][y0].jog = -1;
}


int movCavalo(peca tab[][8], int new_x, int new_y, int turno) {
    //procura por um cavalo
    int cav[4] = { -2,-1,1,2 };
    int x, y;
    for (int i = 0; i < 4; i++) {
        x = new_x + cav[i];
        y = new_y + 3 - abs(cav[i]);
        if (x > 7 || x < 0) continue;
        if (y < 7 && y >= 0) {
            if (tab[x][y].jog == turno && tab[x][y].tipo == 'n') {
                moverPeca(tab, x, y, new_x, new_y);
                return 1;
            }
        }
        y = new_y - 3 + abs(cav[i]);
        if (y < 7 && y >= 0) {
            if (tab[x][y].jog == turno && tab[x][y].tipo == 'n') {
                moverPeca(tab, x, y, new_x, new_y);
                return 1;
            }
        }
    }
    return 0;
}

// nao usar no main
int movRBQ(peca tab[][8], int new_x, int new_y, int turno, char tipo,int sq[], int size) { 
    int i, j, x, y, t;
    for (int k = 0; k < size; k++) {
        t = 1;
        x = i = sq[k];
        y = j = sq[(k + size/4) % size];
        while (x < 8 && x > -1 && y < 8 && y > -1) {
            if (tab[x][y].jog == turno && tab[x][y].tipo == tipo) {
                moverPeca(tab, x, y, new_x, new_y);
                return 1;
            }
            else if (tab[x][y].jog != -1) break;
            t++;
            x = new_x + (t * i);
            y = new_y + (t * j);
        }
    }
    return 0;
}
int movTorre(peca tab[][8], int new_x, int new_y, int turno) {
    // procura por uma torre na horizontal e vertical do local escolhido 
    int sq[] = {1, 0, -1, 0};
    return movRBQ(tab, new_x, new_y, turno, 'r', sq, 4);
}

int movBispo(peca tab[][8], int new_x, int new_y, int turno) {
    // procura por um bispo nas diagonais do local escolhido 
    int sq[] = { 1, 1, -1, -1 };
    return movRBQ(tab, new_x, new_y, turno, 'b', sq, 4);
}


int movRainha(peca tab[][8], int new_x, int new_y, int turno) {
    // procura pela rainha nas diagonais, horizontal e vertical do local escolhido 
    int sq[] = {0,1,1,1,0,-1,-1,-1};
    return movRBQ(tab, new_x, new_y, turno, 'q', sq, 8);
}

int movRei(peca tab[][8], int new_x, int new_y, int turno) {
    int x, y;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            x = new_x + i;
            y = new_y + j;
            if (x == -1 || x == 8 || y == -1 || y == 8) break;
            if (tab[x][y].jog == turno && tab[x][y].tipo == 'k') {
                moverPeca(tab, x, y, new_x, new_y);
                return 1;
                break;
            }
        }
    }
    return 0;
}


int movPeao(peca tab[][8], int new_x, int new_y, int turno, int cap) {
    int y = new_y + 2 * turno - 1;
    int x;
    if (cap == 0) {
        x = new_x;
        if (tab[x][y].jog == turno && tab[x][y].tipo == 'p') {
            moverPeca(tab, x, y, new_x, new_y);
            return 1;
        }
    }else{
        if (new_x < 7) {
            x = new_x + 1;
            if (tab[x][y].jog == turno && tab[x][y].tipo == 'p') {
                moverPeca(tab, x , y, new_x, new_y);
                return 1;
            }
        }
        if (new_x > 0) {
            x = new_x - 1;
            if (tab[x][y].jog == turno && tab[x][y].tipo == 'p') {
                moverPeca(tab, x, y, new_x, new_y);
                return 1;
            }
        }
    }
    return 0;
}

void showGame(peca tab[][8]) {
    char* line = "   -------------------------------------------------";
    puts(line);
    for (int j = 7; j > -1; j--) {
        printf(" %d ", j+1);
        for (int i = 0; i < 8; i++) {
            if (tab[i][j].tipo != 'z') {
                printf("| %d %c ",tab[i][j].jog,tab[i][j].tipo + ('A' - 'a'));
            }
            else {
                printf("|     ");
            }
        }
        puts("|");
        puts(line);
    }
    puts("      A     B     C     D     E     F     G     H");
    puts("\n\n");
}


int update_danger(peca tab[][8],int *tab_danger_temp[][8]){
    
    int tab_danger[8][8];
    
    int tempi = 0;
    int tempj = 0;
    
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8 ; j++){
            tab_danger[i][j] = -1;
        }
    }
    
    for(int i = 0;i < 8;i++){
        for(int j = 0;j < 8;j++){
            
            if(tab[i][j].tipo == 'p'){ //pawns attack
                if(tab[i][j].jog == 0){
                    if(i<7 && j<7){
                        tab_danger[i+1][j+1] = tab[i][j].jog;
                    }
                    if(i<7 && j>0){
                        tab_danger[i+1][j-1] = tab[i][j].jog;
                    }
                }else{
                    if(i>0 && j<7){
                         tab_danger[i-1][j+1] = tab[i][j].jog;
                    }
                    if(i>0 && j>0){
                        tab_danger[i-1][j-1] = tab[i][j].jog; 
                    }
                }
            }
            
            if(tab[i][j].tipo == 'r'){ //rooks attack
                //check i axis +
                for(tempi = 1;tempi + i < 8;tempi++){
                    tab_danger[i+tempi][j] = tab[i][j].jog;
                    if(tab[i+tempi][j].jog != -1){
                        break;
                    }
                }
                //check i axis -
                for(tempi = -1;tempi + i > 0;tempi--){
                    tab_danger[i+tempi][j] = tab[i][j].jog;
                    if(tab[i+tempi][j].jog != -1){
                        break;
                    }
                }
                //check j axis +
                for(tempj = 1;tempj + j < 8;tempj++){
                    tab_danger[i][j+tempj] = tab[i][j].jog;
                    if(tab[i][j+tempj].jog != -1){
                        break;
                    }
                }
                //check j axis -
                for(tempj = -1;tempj + i > 0;tempi--){
                    tab_danger[i][j+tempj] = tab[i][j].jog;
                    if(tab[i][j+tempj].jog != -1){
                        break;
                    }
                }
            }
            
            if(tab[i][j].tipo == 'b'){ //bishops attack
                //check i j axis + +
                tempi = 1;
                tempj = 1;
                for(;tempi + i < 8 && tempj < 8  ;){
                    tab_danger[i+tempi][j+tempj] = tab[i][j].jog;
                    if(tab[i+tempi][j+tempj].jog != -1){
                        break;
                    }
                    tempi++;
                    tempj++;
                }
                
                //check i j axis + -
                tempi = 1;
                tempj = -1;
                for(;tempi + i < 8 && tempj > 0;){
                    tab_danger[i+tempi][j+tempj] = tab[i][j].jog;
                    if(tab[i+tempi][j+tempj].jog != -1){
                        break;
                    }
                    tempi++;
                    tempj--;
                }
                
                //check i j axis - -
                tempi = -1;
                tempj = -1;
                for(;tempi + i > 0 && tempj > 0;){
                    tab_danger[i+tempi][j+tempj] = tab[i][j].jog;
                    if(tab[i+tempi][j+tempj].jog != -1){
                        break;
                    }
                    tempi--;
                    tempj--;
                }
                
                //check i j axis - +
                tempi = -1;
                tempj = 1;
                for(;tempi + i > 0 && tempj < 8;){
                    tab_danger[i+tempi][j+tempj] = tab[i][j].jog;
                    if(tab[i+tempi][j+tempj].jog != -1){
                        break;
                    }
                    tempi--;
                    tempj++;
                }
            }
        }
    }
    tab_danger_temp = *tab_danger;
    return 0;
}
