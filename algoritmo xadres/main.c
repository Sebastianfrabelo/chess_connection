#include "xadres.h"

void convHex(int *value, char *hexvalue){
    hexvalue[0] = 'A' + value[0];
    hexvalue[1] = '1' + value[1];
    hexvalue[2] = 0;
}

int main()
{
    char pecas[] = {'r','n','b','q','k','b','n','r'};
    char comando[6];    //comando dado pelo usuario
    char ambig[2][4]; //guarda o codigo hexadecimal das casas aonde estao as pecas com ambiguidades
    peca tab[8][8]; //tabuleiro, tab[a-f][1-8]
    int turno = 0;
    int pieceState[2][2]; //guarda estado da peca, utilizado em ambiguidades
    int validFlag = 0; //flag utilizada para validar comando do usuario no tratamento de ambiguidades

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
    int checkmate, flag, new_x, new_y, cap, move;
    checkmate = flag = new_x = new_y = cap = 0;
    move = 1;
    while(!checkmate){
        printf("%d - ", move);
        if (turno) {
            puts("[1]Pretos: ");
            move++;
        }
        else puts("[0]Brancos: ");
        fgets(comando,6,stdin);
        if(comando[1]=='x'){
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
        
        //jogo procura se ha uma peca disponivel para ser movida e nao ha obstaculos
        switch(comando[0]){
            case 'p'://peao
                flag = movPeao(tab, new_x, new_y, turno,cap, pieceState);
                break;

            case 'r'://torre
                flag = movTorre(tab, new_x, new_y, turno, pieceState);
                break;

            case 'n'://cavalo
                flag = movCavalo(tab, new_x, new_y, turno, pieceState);
                break;

            case 'b'://bispo
                flag = movBispo(tab, new_x, new_y, turno, pieceState);
                break;

            case 'k'://rei
                flag = movRei(tab, new_x, new_y, turno);
                break;

            case 'q'://rainha
                flag = movRainha(tab, new_x, new_y, turno, pieceState);
                break;

            default:
                puts("\nErro, comando desconhecido");
                break;
        }
        //checa se o rei foi capturado
        checkmate = 1;
        for (int i = 0; i < 8 && checkmate == 1; i++) {
            for (int j = 0; j < 8 && checkmate == 1; j++) {
                if (tab[i][j].jog == (1 - turno) && tab[i][j].tipo == 'k') checkmate = 0;
            }
        }
        system("cls");
        showGame(tab);
        
        if (checkmate) printf("\n\nParabens!! Jogador %d venceu!\n\n", turno);
        
        else switch(flag){ //verifica valor da flag de retorno do movimento da peca, 0 = movimento invalido, 1 = movimento valido, 2 = ambiguidade
            case 0:
                puts("\nErro, peca nao pode ser movida\n"); //movimento invalido, repete o turno
                break;
            case 1:
                update_danger(tab,tab_danger); //movimento valido, atualizar ataques
                turno = 1 - turno;
                flag = 0;
                break;
            case 2: //tratamento de ambiguidades
                convHex(pieceState[0], ambig[0]); //converte posicao das pecas em hexadecimal para impressao
                convHex(pieceState[1], ambig[1]);
                validFlag = 0;
                printf("\nAmbiguidade no movimento, escolha entre 1. %s e 2. %s (Digite 1 ou 2): \n", ambig[0], ambig[1]);
                while(!validFlag){
                    fgets(comando, 2, stdin);
                    switch(comando[0]){
                        case '1':
                            moverPeca(tab, pieceState[0][0], pieceState[0][1], new_x, new_y);
                            update_danger(tab,tab_danger);
                            turno = 1 - turno;
                            flag = 0;
                            validFlag = 1;
                            break;
                        case '2':
                            moverPeca(tab, pieceState[1][0], pieceState[1][1], new_x, new_y);
                            update_danger(tab,tab_danger);
                            turno = 1 - turno;
                            flag = 0;
                            validFlag = 1;
                            break;
                        default: 
                            puts("\nComando invalido! Insira o comando novamente (Digite 1 ou 2): \n");
                            break;
                    }
                }
                break;
        }
    }
    return 0;
}
