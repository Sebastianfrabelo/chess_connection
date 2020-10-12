#include "xadrez.h"

//Movimento propriamente dito, simplesmente coloca a peca no local final de seu movimento
//e esvazia a casa aonde estava anteriormente.
void moverPeca(peca tab[][8], int x0, int y0, int x1, int y1)
{
    tab[x1][y1].tipo = tab[x0][y0].tipo;
    tab[x1][y1].jog = tab[x0][y0].jog;

    tab[x0][y0].tipo = 'z';
    tab[x0][y0].jog = -1;
}

//Movimento do cavalo
int movCavalo(peca tab[][8], int new_x, int new_y, int turno, int pieceState[2][2])
{
    int cav[4] = {-2, -1, 1, 2}; //posiçoes relativas a new_x e new_y
    int numCav = 0;              //numero de cavalos encontrados: 0 = movimento invalido, 1 = movimento valido, 2 = ambiguidade
    int x, y;                    //coordenadas no tabuleiro

    for (int i = 0; i < 4; i++)
    {   // |x - new_x| + |y - new_y| = 3,
        //essa equacao utilizando os valores da string acima
        //da todas as possiveis posicoes do cavalo
        x = new_x + cav[i];
        y = new_y + 3 - abs(cav[i]);
        if (x > 7 || x < 0)
            continue; //fora do campo
        if (y < 8 && y >= 0)
        {
            if (tab[x][y].jog == turno && tab[x][y].tipo == 'n')
            { //cavalo encontrado
                pieceState[numCav][0] = x;
                pieceState[numCav][1] = y;
                numCav++;
            }
        }
        y = new_y - 3 + abs(cav[i]);
        if (y < 8 && y >= 0)
        {
            if (tab[x][y].jog == turno && tab[x][y].tipo == 'n')
            { //cavalo encontrado
                pieceState[numCav][0] = x;
                pieceState[numCav][1] = y;
                numCav++;
            }
        }
    }

    switch (numCav)
    { //quantos cavalos foram encontrados?
    case 0:
        return 0;
        break;
    case 1:
        moverPeca(tab, pieceState[0][0], pieceState[0][1], new_x, new_y); //se o movimento for valido simplesmente mova a peca
        return 1;
        break;
    case 2:
        return 2;
        break;
    }

    return 0;
}

//movimento da torre , bispo e rainha. nao usar no main.
//divide o campo de busca em um "relógio" com o centro na nova posicao. 4 bracos de busca para torres e bispos, 8 para rainha
//cada braco de busca inicia-se em uma posicao adjacente e checa ate o fim do tabuleiro, até encontrar um obstaculo ou até encontrar a peca desejada
//se encontrar a peca desejada, a peca é movida e retorna 1. se nao encontrar nenhuma peca, retorna 0, se houver ambiguidade retorna 2
int movRBQ(peca tab[][8], int new_x, int new_y, int turno, char tipo, int sq[], int size, int pieceState[2][2])
{
    // sq é a sequancia para o "relógio", passa para o proximo "braco"
    // size = tamanho de sq
    int numFound = 0; //numero de pecas do tipo requerido encontradas: 0 = movimento invalido, 1 = movimento valido, 2 = ambiguidade
    int i, j, x, y, t;
    for (int k = 0; k < size; k++)
    {
        t = 1;
        i = sq[k];
        x = new_x + i;
        j = sq[(k + size / 4) % size];
        y = new_y + j;
        while (x < 8 && x > -1 && y < 8 && y > -1)
        { //fora do tabuleiro
            if (tab[x][y].jog == turno && tab[x][y].tipo == tipo)
            { // encontrou a peca
                pieceState[numFound][0] = x;
                pieceState[numFound][1] = y;
                numFound++;
                break; //Peca encontrada, se ha outras, elas estao bloqueadas.
            }
            else if (tab[x][y].jog != -1)
                break; //caminho bloqueado por outra peca
            //proxima posiçao
            t++;
            x = new_x + (t * i);
            y = new_y + (t * j);
        }
    }

    switch (numFound)
    { //quantas pecas foram encontradas?
    case 0:
        return 0;
        break;
    case 1:
        moverPeca(tab, pieceState[0][0], pieceState[0][1], new_x, new_y);
        return 1;
        break;
    case 2:
        return 2;
        break;
    }
}

int movTorre(peca tab[][8], int new_x, int new_y, int turno, int pieceState[2][2])
{
    // procura por uma torre na horizontal e vertical do local escolhido
    int sq[] = {1, 0, -1, 0};
    return movRBQ(tab, new_x, new_y, turno, 'r', sq, 4, pieceState);
}

int movBispo(peca tab[][8], int new_x, int new_y, int turno, int pieceState[2][2])
{
    // procura por um bispo nas diagonais do local escolhido
    int sq[] = {1, 1, -1, -1};
    return movRBQ(tab, new_x, new_y, turno, 'b', sq, 4, pieceState);
}

int movRainha(peca tab[][8], int new_x, int new_y, int turno, int pieceState[2][2])
{
    // procura pela rainha nas diagonais, horizontal e vertical do local escolhido
    int sq[] = {0, 1, 1, 1, 0, -1, -1, -1};
    return movRBQ(tab, new_x, new_y, turno, 'q', sq, 8, pieceState);
}

//procura o rei nas casas adjacentes
int movRei(peca tab[][8], int new_x, int new_y, int turno)
{
    int x, y;
    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            x = new_x + i;
            y = new_y + j;
            if (x == -1 || x == 8 || y == -1 || y == 8)
                break; //fora do tabuleiro
            if (tab[x][y].jog == turno && tab[x][y].tipo == 'k')
            {
                moverPeca(tab, x, y, new_x, new_y);
                return 1;
                break;
            }
        }
    }
    return 0;
}

// procura o peão na posiçao anterior
int movPeao(peca tab[][8], int new_x, int new_y, int turno, int cap, int pieceState[2][2])
{
    int y = new_y + 2 * turno - 1;
    int x;
    int pawnNum = 0; //numero de peoes encontrados (apenas captura!!)
                     //0 = movimento invalido, 1 = movimento valido, 2 = ambiguidade

    if (cap == 0)
    { // nao e captura
        x = new_x;
        if (tab[x][y].jog == turno && tab[x][y].tipo == 'p')
        {
            moverPeca(tab, x, y, new_x, new_y);
            return 1;
        }
        else if ((turno == 0 && new_y == 3) || (turno == 1 && new_y == 4))
        { // movimento inicial de 2
            y = new_y + 2 * (2 * turno - 1);
            if (tab[x][y].jog == turno && tab[x][y].tipo == 'p')
            {
                moverPeca(tab, x, y, new_x, new_y);
                return 1;
            }
        }
    }
    else
    { // captura
        if (new_x < 7)
        {
            x = new_x + 1;
            if (tab[x][y].jog == turno && tab[x][y].tipo == 'p')
            {
                pieceState[pawnNum][0] = x;
                pieceState[pawnNum][1] = y;
                pawnNum++;
            }
        }
        if (new_x > 0)
        {
            x = new_x - 1;
            if (tab[x][y].jog == turno && tab[x][y].tipo == 'p')
            {
                pieceState[pawnNum][0] = x;
                pieceState[pawnNum][1] = y;
                pawnNum++;
            }
        }
    }

    switch (pawnNum)
    { //quantos peoes foram encontrados?
    case 0:
        return 0;
        break;
    case 1:
        moverPeca(tab, pieceState[0][0], pieceState[0][1], new_x, new_y);
        return 1;
        break;
    case 2:
        return 2;
        break;
    }
}
//imprime jogo (http://patorjk.com/software/taag/#p=display&f=Graffiti&t=Type%20Something%20)
void showGame(peca tab[][8])
{

    //  http://patorjk.com/software/taag/#p=display&f=Doh&t=CHESS%0A   - moificado
    puts(" \n\n\n\n"
         "               CCCCCCCCCCCCC    HHHHHHHHH     HHHHHHHHH    EEEEEEEEEEEEEEEEEEEEEE       SSSSSSSSSSSSSSS        SSSSSSSSSSSSSSS             \n"
         "            CCC::::::::::::C    H:::::::H     H:::::::H    E::::::::::::::::::::E     SS:::::::::::::::S     SS:::::::::::::::S            \n"
         "          CC:::::::::::::::C    H:::::::H     H:::::::H    E::::::::::::::::::::E    S:::::SSSSSS::::::S    S:::::SSSSSS::::::S            \n"
         "         C:::::CCCCCCCC::::C    HH::::::H     H::::::HH    EE::::::EEEEEEEEE::::E    S:::::S     SSSSSSS    S:::::S     SSSSSSS            \n"
         "        C:::::C       CCCCCC      H:::::H     H:::::H        E:::::E       EEEEEE    S:::::S                S:::::S                        \n"
         "       C:::::C                    H:::::H     H:::::H        E:::::E                 S:::::S                S:::::S                        \n"
         "       C:::::C                    H::::::HHHHH::::::H        E::::::EEEEEEEEEE        S::::SSSS              S::::SSSS                     \n"
         "       C:::::C                    H:::::::::::::::::H        E:::::::::::::::E         SS::::::SSSSS          SS::::::SSSSS                \n"
         "       C:::::C                    H:::::::::::::::::H        E:::::::::::::::E           SSS::::::::SS          SSS::::::::SS              \n"
         "       C:::::C                    H::::::HHHHH::::::H        E::::::EEEEEEEEEE              SSSSSS::::S            SSSSSS::::S             \n"
         "       C:::::C                    H:::::H     H:::::H        E:::::E                             S:::::S                S:::::S            \n"
         "        C:::::C       CCCCCC      H:::::H     H:::::H        E:::::E       EEEEEE                S:::::S                S:::::S            \n"
         "         C:::::CCCCCCCC::::C    HH::::::H     H::::::HH    EE::::::EEEEEEEE:::::E    SSSSSSS     S:::::S    SSSSSSS     S:::::S            \n"
         "          CC:::::::::::::::C    H:::::::H     H:::::::H    E::::::::::::::::::::E    S::::::SSSSSS:::::S    S::::::SSSSSS:::::S            \n"
         "            CCC::::::::::::C    H:::::::H     H:::::::H    E::::::::::::::::::::E    S:::::::::::::::SS     S:::::::::::::::SS             \n"
         "               CCCCCCCCCCCCC    HHHHHHHHH     HHHHHHHHH    EEEEEEEEEEEEEEEEEEEEEE     SSSSSSSSSSSSSSS        SSSSSSSSSSSSSSS               \n\n\n");

    char *blankspace = "                                           ";
    char *line = "  -------------------------------------------------";
    printf(blankspace);
    puts(line);
    for (int j = 7; j > -1; j--)
    {
        puts(blankspace);
        printf("%d ", j + 1);
        for (int i = 0; i < 8; i++)
        {
            if (tab[i][j].tipo != 'z')
            {
                printf("| %d %c ", tab[i][j].jog, tab[i][j].tipo + ('A' - 'a'));
            }
            else
            {
                printf("|     ");
            }
        }
        puts("|");
        printf(blankspace);
        puts(line);
    }
    printf(blankspace);
    puts("     A     B     C     D     E     F     G     H");
    puts("\n\n");
}

int update_danger(peca tab[][8], int tab_danger[][8])
{

    int tempi = 0;
    int tempj = 0;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            tab_danger[i][j] = -1;
        }
    }

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {

            if (tab[i][j].tipo == 'p')
            { //pawns attack
                if (tab[i][j].jog == 0)
                {
                    if (i < 7 && j < 7)
                    {
                        tab_danger[i + 1][j + 1] = tab[i][j].jog;
                    }
                    if (i < 7 && j > 0)
                    {
                        tab_danger[i + 1][j - 1] = tab[i][j].jog;
                    }
                }
                else
                {
                    if (i > 0 && j < 7)
                    {
                        tab_danger[i - 1][j + 1] = tab[i][j].jog;
                    }
                    if (i > 0 && j > 0)
                    {
                        tab_danger[i - 1][j - 1] = tab[i][j].jog;
                    }
                }
            }

            if (tab[i][j].tipo == 'r')
            { //rooks attack
                //check i axis +
                for (tempi = 1; tempi + i < 8; tempi++)
                {
                    tab_danger[i + tempi][j] = tab[i][j].jog;
                    if (tab[i + tempi][j].jog != -1)
                    {
                        break;
                    }
                }
                //check i axis -
                for (tempi = -1; tempi + i > 0; tempi--)
                {
                    tab_danger[i + tempi][j] = tab[i][j].jog;
                    if (tab[i + tempi][j].jog != -1)
                    {
                        break;
                    }
                }
                //check j axis +
                for (tempj = 1; tempj + j < 8; tempj++)
                {
                    tab_danger[i][j + tempj] = tab[i][j].jog;
                    if (tab[i][j + tempj].jog != -1)
                    {
                        break;
                    }
                }
                //check j axis -
                for (tempj = -1; tempj + i > 0; tempi--)
                {
                    tab_danger[i][j + tempj] = tab[i][j].jog;
                    if (tab[i][j + tempj].jog != -1)
                    {
                        break;
                    }
                }
            }

            if (tab[i][j].tipo == 'b')
            { //bishops attack
                //check i j axis + +
                tempi = 1;
                tempj = 1;
                for (; tempi + i < 8 && tempj < 8;)
                {
                    tab_danger[i + tempi][j + tempj] = tab[i][j].jog;
                    if (tab[i + tempi][j + tempj].jog != -1)
                    {
                        break;
                    }
                    tempi++;
                    tempj++;
                }

                //check i j axis + -
                tempi = 1;
                tempj = -1;
                for (; tempi + i < 8 && tempj > 0;)
                {
                    tab_danger[i + tempi][j + tempj] = tab[i][j].jog;
                    if (tab[i + tempi][j + tempj].jog != -1)
                    {
                        break;
                    }
                    tempi++;
                    tempj--;
                }

                //check i j axis - -
                tempi = -1;
                tempj = -1;
                for (; tempi + i > 0 && tempj > 0;)
                {
                    tab_danger[i + tempi][j + tempj] = tab[i][j].jog;
                    if (tab[i + tempi][j + tempj].jog != -1)
                    {
                        break;
                    }
                    tempi--;
                    tempj--;
                }

                //check i j axis - +
                tempi = -1;
                tempj = 1;
                for (; tempi + i > 0 && tempj < 8;)
                {
                    tab_danger[i + tempi][j + tempj] = tab[i][j].jog;
                    if (tab[i + tempi][j + tempj].jog != -1)
                    {
                        break;
                    }
                    tempi--;
                    tempj++;
                }
            }
        }
    }
    return 0;
}
