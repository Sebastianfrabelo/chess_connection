#include <stdio.h>
#include <stdlib.h>


typedef struct{
    int jog;//0: brancas, 1: pretas, -1: sem peca
    char tipo;
}peca;//

int main()
{
    char pecas[] = {'r','n','b','q','k','b','n','r'};
    char comando[10];
    peca tab[8][8];//tabuleiro, tab[1-8][a-f]
    int turno = 0;

 // inicializa tabuleiro
    for(int i = 0; i < 8; i++){
        for(int j = 2; j<= 5 ; j++){
            tab[i][j].jog = -1;
            tab[i][j].tipo = 'z';// sem peca
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

    int k = 1;
    int pos[2];
    while(k){
        scanf("%s",comando);
        if(comando[1]=='x'){
            pos[1] = (int)comando[2] - 'a';//a-h
            pos[0] = (int)comando[3] - 1;//1-8
        }else{
            pos[1] = (int)comando[1] - 'a';//a-h
            pos[0] = (int)comando[2] - 1;//1-8
        }

        switch(comando[0]){
            int t;
            case 'p'://peao
                t = 2*turno-1;
                //checa se há um peao na posicao anterior
                if(tab[pos[0] + t][pos[1]].jog == turno && tab[pos[0] + t][pos[1]].tipo == 'p'){
                    //peao se move
                    tab[pos[0]][pos[1]].tipo = 'p';
                    tab[pos[0]][pos[1]].jog = turno;

                    tab[pos[0]+t][pos[1]].tipo = 'z';
                    tab[pos[0]+t][pos[1]].jog = -1;
                }
                else{
                    //erro, nao pode se mover
                }

                break;
            case 'r'://torre

                break;
            case 'n'://cavalo

                break;
            case 'b'://bispo

                break;
            case 'k'://rei

                break;
            case 'q'://rainha

                break;
            case 'o':

                break;
            default:
                break;

        }

    }


    return 0;
}
