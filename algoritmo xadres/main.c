#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "xadres.h"

int main()
{
    char pecas[] = {'r','n','b','q','k','b','n','r'};
    char comando[10];
    peca tab[8][8];//tabuleiro, tab[a-f][1-8]
    int turno = 0;

 // inicializa tabuleiro normal e de ataques
    for(int i = 0; i < 8; i++){
        for(int j = 2; j<= 5 ; j++){
            tab[i][j].jog = -1;
            tab[i][j].tipo = 'z';// sem peca
        }
    }
    
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8 ; j++){
            tab_danger[i][j] = -1;//sem peca
        }
    }
    
    
    for(int i = 0; i<8;i++){
        tab[i][0].jog = 0;
        tab[i][0].tipo = pecas[i];

        tab[i][1].jog = 0;
        tab[i][1].tipo = 'p';

        tab[i][6].jog = 1;
        tab[i][6].tipo = 'p';

        tab[i][7].jog = 1;
        tab[i][7].tipo = pecas[i];
    }
    showGame(tab);
    int checkmate = -1;
    int flag = 0;//flag = 1 : sucesso
    int new_x = 0;
    int new_y = 0;
    int cap = 0;
    peca aux;
    while(checkmate == -1){
        printf("\n Jogador %d:\n",turno);
        //scanf("%s",comando);
        fgets(comando,5,stdin);
;       if(comando[1]=='x'){
            new_x = (int)comando[2] - (int)'a';//a-h
            new_y = (int)comando[3] - (int)'1';//1-8
            cap = 1;

            if (tab[new_x][new_y].jog == turno) {
                puts("Erro, local ocupado");
                continue;
            }
        }
        else if(comando[1] >= 'a' && comando[1] <= 'h') {
            new_x = (int)comando[1] - (int)'a';//a-h
            new_y = (int)comando[2] - (int)'1';//1-8
            cap = 0;
            if (tab[new_x][new_y].jog != -1) {
                puts("Erro, local ocupado");
                continue;
            }
        }
        else if (comando[1] == '-') {//o-o | o-o-o

        }
        //jogo procura se ha uma peca disponivel para ser movida e nao ha obstaculos
        switch(comando[0]){
            case 'p'://peao
                flag = movPeao(tab, new_x, new_y, turno,cap);
                break;

            case 'r'://torre
                flag = movTorre(tab, new_x, new_y, turno);
                break;

            case 'n'://cavalo
                flag = movCavalo(tab, new_x, new_y, turno);
                break;

            case 'b'://bispo
                flag = movBispo(tab, new_x, new_y, turno);
                break;

            case 'k'://rei
                flag = movRei(tab, new_x, new_y, turno);
                break;

            case 'q'://rainha
                flag = movRainha(tab, new_x, new_y, turno);
                break;

            case 'o':
                //o-o e o-o-o sao trocas
                break;
            default:
                puts("\nErro, comando desconhecido");
                break;
        }
        system("cls");
        if (flag == 0) puts("\nErro, peca nao pode ser movida\n");
        else {
            update_danger(tab,**tab_danger);
            turno = 1 - turno;
            flag = 0;
        }
        showGame(tab);
       
    }
    return 0;
}
