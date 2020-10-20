#include "header.h"
#include "xadrez.h"

void convHex(int *value, char *hexvalue)
{
    hexvalue[0] = 'A' + value[0];
    hexvalue[1] = '1' + value[1];
    hexvalue[2] = 0;
}

int main_game(char comando[6], peca tab[][8], int *pturno, int *pmove, int nroJogador)
{

    int turno = *pturno;
    int move = *pmove;
    int new_x = 0;
    int new_y = 0;
    int cap = 0;
    int validFlag = 0; //flag utilizada para validar comando do usuario no tratamento de ambiguidades
    int checkmate = 0;
    int flag = 0;
    checkmate = 1;

    printf("Move nbr. %d - ", move);
    if (turno)
    {
        puts("[1]Pretos: ");
        move++;
        *pmove = move;
    }
    else
    {
        puts("[0]Brancos: ");
    }

    if (comando[1] == 'x')
    {
        new_x = (int)comando[2] - (int)'a'; //a-h
        new_y = (int)comando[3] - (int)'1'; //1-8
        cap = 1;

        if (tab[new_x][new_y].jog == turno)
        {
            puts("Erro, local ocupado");
            return 0;
        }
    }
    else if (comando[1] >= 'a' && comando[1] <= 'h')
    {
        new_x = (int)comando[1] - (int)'a'; //a-h
        new_y = (int)comando[2] - (int)'1'; //1-8
        cap = 0;
        if (tab[new_x][new_y].jog != -1)
        {
            puts("Erro, local ocupado");
            return 0;
        }
    }

    //jogo procura se ha uma peca disponivel para ser movida e nao ha obstaculos
    switch (comando[0])
    {
    case 'p': //peao
        flag = movPeao(tab, new_x, new_y, turno, cap, pieceState);
        break;

    case 'r': //torre
        flag = movTorre(tab, new_x, new_y, turno, pieceState);
        break;

    case 'n': //cavalo
        flag = movCavalo(tab, new_x, new_y, turno, pieceState);
        break;

    case 'b': //bispo
        flag = movBispo(tab, new_x, new_y, turno, pieceState);
        break;

    case 'k': //rei
        flag = movRei(tab, new_x, new_y, turno, tab_danger);
        break;

    case 'q': //rainha
        flag = movRainha(tab, new_x, new_y, turno, pieceState);
        break;

    default:
        puts("\nErro, comando desconhecido");
        return 0;
        break;
    }
    //checa se o rei foi capturado
    for (int i = 0; i < 8 && checkmate == 1; i++)
    {
        for (int j = 0; j < 8 && checkmate == 1; j++)
        {
            if (tab[i][j].jog == (1 - turno) && tab[i][j].tipo == 'k')
            {
                checkmate = 0;
            }
        }
    }

    if (checkmate)
    {
        printf("\n\nParabens!! Jogador %d venceu!\n\n", turno);
        return 2;
    }
    
    if (!flag)
    {                                               
        puts("\nErro, peca nao pode ser movida\n"); //movimento invalido, repete o turno
        return 0;
    }
    else
    {
        update_danger(tab, tab_danger); //movimento valido, atualizar ataques
        turno = 1 - turno;
        *pturno = turno;
        return 1;
    }
    return 1;
}
