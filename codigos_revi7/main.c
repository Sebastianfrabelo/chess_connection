#include "header.h"
#include "xadrez.h"

int main()
{
	printf("ver. 1.4.2\n");
	//int tab_danger[8][8]; //tabuleiro de ataques p/ xeques
	char pecas[] = {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'};
	//  char comando[6];    //comando dado pelo usuario
	//char ambig[2][4]; //guarda o codigo hexadecimal das casas aonde estao as pecas com ambiguidades
	//peca tab[8][8]; //tabuleiro, tab[a-f][1-8]
	//	int pieceState[2][2]; //guarda estado da peca, utilizado em ambiguidades
	int checkmate = 0;
	int flag = 0;
	int turno = 0; //var pra saber quem esta jogando
	int move = 1;

	

	while (1)
	{
		printf("\n -- Chess Connect -- \n");
		printf("Bem vindo, escolha uma acao:\n1) Host Game\n2) Join Game\n0) Exit\n\n>>");
		fflush(stdin);
		scanf("%d", &menu_select);
		switch (menu_select)
		{
		case 0:
			printf("Saindo do jogo...\n");
			exit(0);
			return 0;
			break;

		case 1:
			printf("Indique o numero da sala a ser criada (0 to 100): ");
			fflush(stdin);
			scanf("%d", &port_select);
			if (port_select < 0 || port_select > 100)
			{
				printf("Nro. Invalido\n");
				break;
			}
			host_game(port_select, &turno, &move,pecas);
			break;

		case 2:
			printf("Indique a sala que quer entrar (0 to 100): ");
			fflush(stdin);
			scanf("%d", &port_select);
			if (port_select < 0 || port_select > 100)
			{
				printf("Nro. Invalido ou sala cheia!\n");
				break;
			}
			join_game(port_select, &turno, &move,pecas);
			break;
		}
	}
	return 0;
}

//Funcao utilizada para simplificar verificacoes de erro
int verify(int ret, const char *msg)
{
	if (ret == -1)
	{
		perror(msg);
		exit(1);
	}
	return ret;
}