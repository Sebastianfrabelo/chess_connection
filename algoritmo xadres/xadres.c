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

int movTorre(peca tab[][8], int new_x, int new_y, int turno, char p) {
    for (int i = new_x + 1; i < 8; i++) {
        if (tab[i][new_y].jog == turno && tab[i][new_y].tipo == p) {
            moverPeca(tab, i, new_y, new_x, new_y);
            return 1;
        }
        else if (tab[i][new_y].jog != -1) break;

    }

    for (int i = new_x - 1; i >= 0; i--) {
        if (tab[i][new_y].jog == turno && tab[i][new_y].tipo == p) {
            moverPeca(tab, i, new_y, new_x, new_y);
            return 1;
        }
        else if (tab[i][new_y].jog != -1) break;
    }

    for (int j = new_y + 1; j < 8; j++) {
        if (tab[new_x][j].jog == turno && tab[new_x][j].tipo == p) {
            moverPeca(tab, new_x, j, new_x, new_y);
            return 1;
        }
        else if (tab[new_x][j].jog != -1) break;
    }

    for (int j = new_y - 1; j >= 0; j--) {
        if (tab[new_x][j].jog == turno && tab[new_x][j].tipo == p) {
            moverPeca(tab, new_x, j, new_x, new_y);
            return 1;
        }
        else if (tab[new_x][j].jog != -1) break;
    }
    return 0;
}

int movBispo(peca tab[][8], int new_x, int new_y, int turno, char p) {
    for (int i = 1; new_x + i < 8 && new_y + i < 8; i++) {
        if (tab[new_x + i][new_y + i].jog == turno && tab[new_x + i][new_y + i].tipo == p) {
            moverPeca(tab, new_x + i, new_y + i, new_x, new_y);
            return 1;
        }
        else if (tab[new_x + i][new_y + i].jog != -1) break;
    }

    for (int i = 1; new_x - i >= 0 && new_y - i >= 0; i++) {
        if (tab[new_x - i][new_y - i].jog == turno && tab[new_x - i][new_y - i].tipo == p) {
            moverPeca(tab, new_x - i, new_y - i, new_x, new_y);
            return 1;
        }
        else if (tab[new_x - i][new_y - i].jog != -1) break;
    }

    for (int i = 1; new_x + i < 8 && new_y - i >= 0; i++) {
        if (tab[new_x + i][new_y - i].jog == turno && tab[new_x + i][new_y - i].tipo == p) {
            moverPeca(tab, new_x + i, new_y - i, new_x, new_y);
            return 1;
        }
        else if (tab[new_x + i][new_y - i].jog != -1) break;
    }

    for (int i = 1; new_y + i < 8 && new_x - i >= 0; i++) {
        if (tab[new_x - i][new_y + i].jog == turno && tab[new_x - i][new_y + i].tipo == p) {
            moverPeca(tab, new_x - i, new_y + i, new_x, new_y);
            return 1;
        }
        else if (tab[new_x - i][new_y + i].jog != -1) break;
    }
    return 0;
}

int movCavalo(peca tab[][8], int new_x, int new_y, int turno) {
    int cav[4] = { -2,-1,1,2 };
    int t;
    for (int i = 0; i < 4; i++) {
        t = 3 - abs(cav[i]);
        if (new_x + cav[i] > 7 || new_x + cav[i] < 0) continue;

        if (new_y + t < 7 && new_y + t >= 0) {
            if (tab[new_x + cav[i]][new_y + t].jog == turno && tab[new_x + cav[i]][new_y + t].tipo == 'n') {
                moverPeca(tab, new_x + cav[i], new_y + t, new_x, new_y);
                return 1;
            }
        }
        if (new_y - t < 7 && new_y - t >= 0) {
            if (tab[new_x + cav[i]][new_y - t].jog == turno && tab[new_x + cav[i]][new_y - t].tipo == 'n') {
                moverPeca(tab, new_x + cav[i], new_y - t, new_x, new_y);
                return 1;
            }
        }
    }
    return 0;
}

int movRainha(peca tab[][8], int new_x, int new_y, int turno) {
    int f;
    f = movTorre(tab, new_x, new_y, turno, 'q');
    if (f == 0) movBispo(tab, new_x, new_y, turno, 'q');
    return f;
}

int movRei(peca tab[][8], int new_x, int new_y, int turno) {
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if ((tab[new_x + i][new_y + j].jog == turno && tab[new_x + i][new_y + j].tipo == 'k') && (tab_danger[new_x][new_y] == turno) || (tab_danger[new_x][new_y] == -1) ) {
                moverPeca(tab, new_x + i, new_y + j, new_x, new_y);
                return 1;
                break;
            }
        }
    }
    return 0;

}

int movPeao(peca tab[][8], int new_x, int new_y, int turno, int cap) {
    int t = 2 * turno - 1;
    if (cap == 0) {
        if (tab[new_x][new_y + t].jog == turno && tab[new_x][new_y + t].tipo == 'p') {
            moverPeca(tab, new_x, new_y + t, new_x, new_y);
            return 1;
        }
    }else{
        if (new_x < 7) {
            if (tab[new_x + 1][new_y + t].jog == turno && tab[new_x + 1][new_y + t].tipo == 'p') {
                moverPeca(tab, new_x - 1 , new_y + t, new_x, new_y);
                return 1;
            }
        }
        if (new_x > 0) {
            if (tab[new_x - 1][new_y + t].jog == turno && tab[new_x - 1][new_y + t].tipo == 'p') {
                moverPeca(tab, new_x - 1, new_y + t, new_x, new_y);
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
