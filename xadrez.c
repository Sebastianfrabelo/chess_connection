#include "header.h"
#include "xadrez.h"

/*********************************************************************************************************************************************/

	//Arquivo xadrez.c
	//Funcoes relacionadas ao jogo de xadrez em si.

/*********************************************************************************************************************************************/

/*
Funcao void moverPeca(): Movimento propriamente dito, simplesmente coloca a peca no local final de seu movimento
	e esvazia a casa aonde estava anteriormente.
Entradas:
	peca tab[][8]: Matriz do Tabuleiro (localizacao das pecas)
	int x0: Posicao horizontal da peca anterior ao movimento
	int y0: Posicao vertical da peca anterior ao movimento
	int x1: Posicao horizontal da peca posterior ao movimento
	int y1: Posicao vertical da peca posterior ao movimento
*/
void moverPeca(peca tab[][8], int x0, int y0, int x1, int y1)
{
    tab[x1][y1].tipo = tab[x0][y0].tipo;
    tab[x1][y1].jog = tab[x0][y0].jog;

    tab[x0][y0].tipo = 'z';
    tab[x0][y0].jog = -1;
}

/*********************************************************************************************************************************************/

/*
Funcao int movCavalo(): Verifica um movimento atribuido ao cavalo. A partir da posicao que o usuario especificou, verifica se ha um cavalo que
	pode se mover para ela.
Entradas:
	peca tab[][8]: Matriz do Tabuleiro (localizacao das pecas)
	int new_x: Posicao horizontal aonde o usuario deseja posicionar o cavalo
	int new_y: Posicao vertical aonde o usuario deseja posicionar o cavalo
	int turno: Rodada aonde o jogo se encontra (quem esta jogando?).
	int pieceState[2][2]: Matriz para salvar a posicao do cavalo encontrado.
Saida:
	0 se o movimento for invalido e 1 se for valido.
*/
int movCavalo(peca tab[][8], int new_x, int new_y, int turno, int pieceState[2][2])
{
    int cav[4] = {-2, -1, 1, 2}; //posiçoes relativas a new_x e new_y
    int achouCav = 0;            //numero de cavalos encontrados: 0 = movimento invalido, 1 = movimento valido
    int x, y;                    //coordenadas no tabuleiro

    for (int i = 0; i < 4; i++)
    { // |x - new_x| + |y - new_y| = 3,
        //essa equacao utilizando os valores do array cav[4] acima
        //	da todas as possiveis posicoes do cavalo
        x = new_x + cav[i];
        y = new_y + 3 - abs(cav[i]);
        if (x > 7 || x < 0)
            continue; //fora do campo
        if (y < 8 && y >= 0)
        {
            if (tab[x][y].jog == turno && tab[x][y].tipo == 'n')
            { //cavalo encontrado
                pieceState[0][0] = x;
                pieceState[0][1] = y;
                achouCav = 1;
            }
        }
        y = new_y - 3 + abs(cav[i]);
        if (y < 8 && y >= 0)
        {
            if (tab[x][y].jog == turno && tab[x][y].tipo == 'n')
            { //cavalo encontrado
                pieceState[0][0] = x;
                pieceState[0][1] = y;
                achouCav = 1;
            }
        }
    }

    if (achouCav) //achou peca
    {
        moverPeca(tab, pieceState[0][0], pieceState[0][1], new_x, new_y);
        return 1;
    }
    return 0;
}

/*********************************************************************************************************************************************/


/*
Funcao int movRBQ(): Verifica um movimento atribuido a uma torre (R), um bispo (B) ou rainha (Q). A partir da posicao que o usuario especificou, 
	verifica se ha um(a) torre/bispo/rainha que pode se mover para ela. Divide o campo de busca em um "relogio" com o centro na nova posicao:
	quatro bracos de busca para torres e bispos e oito para rainha. Cada braco de busca inicia-se em uma posicao adjacente e checa ate o fim do
	tabuleiro, ate encontrar um obstaculo ou ate encontrar a peca desejada.
Entradas:
	peca tab[][8]: Matriz do Tabuleiro (localizacao das pecas)
	int new_x: Posicao horizontal aonde o usuario deseja posicionar a peca.
	int new_y: Posicao vertical aonde o usuario deseja posicionar a peca.
	int turno: Rodada aonde se encontra o jogo (quem esta jogando?).
	char tipo: Tipo de peca que deseja se movimentar
	int sq: Sequencia que eh passada para o relogio.
	int size: Tamanho da sequencia.
	int pieceState[2][2]: Matriz para salvar a posicao da peca encontrada.
Saida:
	0 se o movimento for invalido e 1 se for valido.
*/
int movRBQ(peca tab[][8], int new_x, int new_y, int turno, char tipo, int sq[], int size, int pieceState[2][2])
{
    // sq eh a sequencia para o "relogio", passa para o proximo "braco"
    // size = tamanho de sq
    int numFound = 0; //numero de pecas do tipo requerido encontradas: 0 = movimento invalido, 1 = movimento valido
    int i, j, x, y, t;
    for (int k = 0; k < size && numFound != 1; k++)
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
                pieceState[0][0] = x;
                pieceState[0][1] = y;
                numFound = 1;
                break; //Peca encontrada
            }
            else if (tab[x][y].jog != -1)
                break; //caminho bloqueado por outra peca
            //proxima posicao
            t++;
            x = new_x + (t * i);
            y = new_y + (t * j);
        }
    }
    if (numFound) //achou peca
    {
        moverPeca(tab, pieceState[0][0], pieceState[0][1], new_x, new_y);
        return 1;
    }
    return 0;
}

/*********************************************************************************************************************************************/

//As proximas funcoes sao intimamente ligadas com a ultima funcao movRBQ(), e simplesmente passarao para essa a sequencia da peca.

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

/*********************************************************************************************************************************************/

/*
Funcao int movRei(): Verifica um movimento atribuido ao rei. A partir da posicao que o usuario especificou, verifica se ha um rei que pode se 
	mover para ela. Verifica as casas adjacentes e tambem verifica se a posicao que o usuario indicou colocara o rei em perigo.
Entradas:
	peca tab[][8]: Matriz do Tabuleiro (localizacao das pecas)
	int new_x: Posicao horizontal aonde o usuario deseja posicionar a peca.
	int new_y: Posicao vertical aonde o usuario deseja posicionar a peca.
	int turno: Rodada aonde se encontra o jogo (quem esta jogando?).
	int tab_danger[][8][2]: Tabuleiro de ataques, as posicoes preenchidas no tabuleiro sao inseguras.
Saida:
	0 se o movimento for invalido e 1 se for valido.
*/
int movRei(peca tab[][8], int new_x, int new_y, int turno, int tab_danger[][8][2])
{
    int x, y;
    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            x = new_x + i;
            y = new_y + j;
            if (x == -1 || x == 8 || y == -1 || y == 8)
                break;                                                                                                  //fora do tabuleiro
            if ((tab[x][y].jog == turno && tab[x][y].tipo == 'k') && (tab_danger[new_x][new_y][!(tab[x][y].jog)] != 1)) //rei nao suicida
            {
                moverPeca(tab, x, y, new_x, new_y);
                return 1;
                break;
            }
        }
    }
    return 0;
}

/*********************************************************************************************************************************************/

/*
Funcao int movPeao(): Verifica um movimento atribuido a um peao. A partir da posicao que o usuario especificou, verifica se ha um peao que 
	pode se mover para ela. Verifica as casas anteriores caso seja um movimento comum e as diagonais anteriores se for um movimento de captura.
	Movimentos iniciais sao levado em consideracao.
Entradas:
	peca tab[][8]: Matriz do Tabuleiro (localizacao das pecas)
	int new_x: Posicao horizontal aonde o usuario deseja posicionar o cavalo
	int new_y: Posicao vertical aonde o usuario deseja posicionar o cavalo
	int turno: Rodada aonde se encontra o jogo (quem esta jogando?).
	int cap: Valor booleano. Indica se o movimento eh de captura.
	int pieceState[2][2]: Matriz para salvar a posicao da peca encontrada.
Saida:
	0 se o movimento for invalido e 1 se for valido.
*/
int movPeao(peca tab[][8], int new_x, int new_y, int turno, int cap, int pieceState[2][2])
{
    int y = new_y + 2 * turno - 1;
    int x;
    int achouPawn = 0; //numero de peoes encontrados (apenas captura!!)
                       //0 = movimento invalido, 1 = movimento valido

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
                pieceState[0][0] = x;
                pieceState[0][1] = y;
                achouPawn = 1;
            }
        }
        if (new_x > 0)
        {
            x = new_x - 1;
            if (tab[x][y].jog == turno && tab[x][y].tipo == 'p')
            {
                pieceState[0][0] = x;
                pieceState[0][1] = y;
                achouPawn = 1;
            }
        }
    }

    if (achouPawn) //achou peca
    {
        moverPeca(tab, pieceState[0][0], pieceState[0][1], new_x, new_y);
        return 1;
    }
    return 0;
}

/*********************************************************************************************************************************************/

/*
Funcao void showGame(): Imprime o jogo na tela. Tipografia de http://patorjk.com/software/taag/#p=display&f=Graffiti&t=Type%20Something%20
Entradas:
	peca tab[][8]: Matriz do Tabuleiro (localizacao das pecas)
	int NroJogador: Valor booleano. Qual jogador esta jogando?
*/
void showGame(peca tab[][8], int NroJogador)
{
    update_danger(tab, tab_danger);
    NroJogador = !NroJogador;

    //  http://patorjk.com/software/taag/#p=display&f=Doh&t=CHESS%0A   - modificado
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
    puts(blankspace);
    puts(line);
    for (int j = 7; j > -1; j--)
    {
        puts(blankspace);
        if (NroJogador)
        {
            printf("%d ", j + 1);
            for (int i = 0; i < 8; i++)
            {
                if (tab[i][j].tipo != 'z')
                {
                    printf("| %d %c ", tab[i][j].jog, tab[i][j].tipo + ('A' - 'a'));
                }
                else
                {
                    printf("|  %c%c ", tab_danger[i][j][0] == 1 ? '#' : ' ', tab_danger[i][j][1] == 1 ? '%' : ' ');
                }
            }
        }
        else
        {
            printf("%d ", 8 - j);
            for (int i = 1; i < 9; i++)
            {
                if (tab[8 - i][7 - j].tipo != 'z')
                {
                    printf("| %d %c ", tab[8 - i][7 - j].jog, tab[8 - i][7 - j].tipo + ('A' - 'a'));
                }
                else
                {
                    printf("|     ");
                }
            }
        }

        puts("|");
        puts(blankspace);
        puts(line);
    }
    puts(blankspace);
    if (NroJogador)
        puts("     A     B     C     D     E     F     G     H");
    else
        puts("     H     G     F     E     D     C     B     A");
    puts("\n\n");
}

/*********************************************************************************************************************************************/

/*
Funcao int update_danger(): Atualiza o tabuleiro de ataque (posicoes na qual o rei nao pode se movimentar).
Entradas:
	peca tab[][8]: Matriz do Tabuleiro (localizacao das pecas).
	int tab_danger[][8][2]: Tabuleiro de ataques, as posicoes preenchidas no tabuleiro sao inseguras.
Saida: Sempre 0.
*/
int update_danger(peca tab[][8], int tab_danger[][8][2])
{

    int tempi = 0;
    int tempj = 0;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            tab_danger[i][j][0] = -1;
            tab_danger[i][j][1] = -1;
        }
    }

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {

            if (tab[i][j].tipo == 'p')
            { //Ataque dos peoes
                if (tab[i][j].jog == 0)
                {
                    if (i > 0 && j < 7)
                    {
                        tab_danger[i - 1][j + 1][0] = 1;
                    }
                    if (i < 7 && j < 7)
                    {
                        tab_danger[i + 1][j + 1][0] = 1;
                    }
                }
                else
                {
                    if (i > 0 && j > 0)
                    {
                        tab_danger[i - 1][j - 1][1] = 1;
                    }
                    if (i < 7 && j > 0)
                    {
                        tab_danger[i + 1][j - 1][1] = 1;
                    }
                }
            }

            //Ataque da rainha = ataque das torres + ataque dos bispos.
            if (tab[i][j].tipo == 'r' || tab[i][j].tipo == 'q')
            { //Ataque das torres
                //Posicoes a direita da peca
                for (tempi = 1; tempi + i < 8; tempi++)
                {
                    tab_danger[i + tempi][j][tab[i][j].jog] = 1;
                    if (tab[i + tempi][j].jog != -1)
                    {
                        break;
                    }
                }
                //Posicoes a esquerda da peca
                for (tempi = -1; tempi + i > 0; tempi--)
                {
                    tab_danger[i + tempi][j][tab[i][j].jog] = 1;
                    if (tab[i + tempi][j].jog != -1)
                    {
                        break;
                    }
                }
                //Posicoes acima da peca
                for (tempj = 1; tempj + j < 8; tempj++)
                {
                    tab_danger[i][j + tempj][tab[i][j].jog] = 1;
                    if (tab[i][j + tempj].jog != -1)
                    {
                        break;
                    }
                }
                //Posicoes embaixo da peca
                for (tempj = -1; tempj + j > 0; tempj--)
                {
                    tab_danger[i][j + tempj][tab[i][j].jog] = 1;
                    if (tab[i][j + tempj].jog != -1)
                    {
                        break;
                    }
                }
            }

            if (tab[i][j].tipo == 'b' || tab[i][j].tipo == 'q')
            { //Ataque de bispos
                //Posicoes diagonal direita superior.
                tempi = 1;
                tempj = 1;
                for (; tempi + i < 8 && tempj + j < 8;)
                {
                    tab_danger[i + tempi][j + tempj][tab[i][j].jog] = 1;
                    if (tab[i + tempi][j + tempj].jog != -1)
                    {
                        break;
                    }
                    tempi++;
                    tempj++;
                }

                //Posicoes diagonal direita inferior
                tempi = 1;
                tempj = -1;
                for (; tempi + i < 8 && tempj + j >= 0;)
                {
                    tab_danger[i + tempi][j + tempj][tab[i][j].jog] = 1;
                    if (tab[i + tempi][j + tempj].jog != -1)
                    {
                        break;
                    }
                    tempi++;
                    tempj--;
                }

                //Posicoes diagonal esquerda inferior
                tempi = -1;
                tempj = -1;
                for (; tempi + i >= 0 && tempj + j >= 0;)
                {
                    tab_danger[i + tempi][j + tempj][tab[i][j].jog] = 1;
                    if (tab[i + tempi][j + tempj].jog != -1)
                    {
                        break;
                    }
                    tempi--;
                    tempj--;
                }

                //Posicoes diagonal esquerda superior
                tempi = -1;
                tempj = 1;
                for (; tempi + i >= 0 && tempj + j < 8;)
                {
                    tab_danger[i + tempi][j + tempj][tab[i][j].jog] = 1;
                    if (tab[i + tempi][j + tempj].jog != -1)
                    {
                        break;
                    }
                    tempi--;
                    tempj++;
                }
            }

            if (tab[i][j].tipo == 'n')
            { //Ataque dos cavalos.
                if (i + 1 < 8 && j + 2 < 8)
                    tab_danger[i + 1][j + 2][tab[i][j].jog] = 1;
                if (i + 1 < 8 && j - 2 >= 0)
                    tab_danger[i + 1][j - 2][tab[i][j].jog] = 1;
                if (i - 1 >= 0 && j + 2 < 8)
                    tab_danger[i - 1][j + 2][tab[i][j].jog] = 1;
                if (i - 1 >= 0 && j - 2 >= 0)
                    tab_danger[i - 1][j - 2][tab[i][j].jog] = 1;

                if (j + 1 < 8 && i + 2 < 8)
                    tab_danger[i + 2][j + 1][tab[i][j].jog] = 1;
                if (j + 1 < 8 && i - 2 >= 0)
                    tab_danger[i - 2][j + 1][tab[i][j].jog] = 1;
                if (j - 1 >= 0 && i + 2 < 8)
                    tab_danger[i + 2][j - 1][tab[i][j].jog] = 1;
                if (j - 1 >= 0 && i - 2 >= 0)
                    tab_danger[i - 2][j - 1][tab[i][j].jog] = 1;
            }

            if (tab[i][j].tipo == 'k')
            {
                //Ataque do rei.
                for (int tempi = -1; tempi < 2; tempi++)
                {
                    for (int tempj = -1; tempj < 2; tempj++)
                    {
                        if ((i + tempi) >= 0 && (i + tempi) < 8 && (j + tempj >= 0) && (j + tempj) < 8)
                            tab_danger[i + tempi][j + tempj][tab[i][j].jog] = 1;
                    }
                }
            }
        }
    }
    return 0;
}
