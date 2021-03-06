#include "header.h"
#include "xadrez.h"

/*********************************************************************************************************************************************/

	//Arquivo main.c

/*********************************************************************************************************************************************/

int main()
{
	printf("ver. 1.4.2\n");
	char pecas[] = {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'};
	int checkmate = 0;
	int flag = 0;
	int turno = 0; //var pra saber quem esta jogando
	int move = 1;
	char hash[4];

	
	while (1)
	{
		printf("\n -- Chess Connect -- \n");
		printf("Bem vindo, escolha uma acao:\n1) Host Game (as Player 1)\n2) Join Game (as Player 0)\n0) Exit\n\n>>");
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
			printf("Indique o numero da sala a ser criada (AAA to ZZZ): ");
			fflush(stdin);
			scanf("%s", hash);
			hash[3] = '\0';
			port_select = hash[0] + (3 * hash[1]) + (7 * hash[2]) % 1000;
			if (port_select < 0 || port_select > 1001)
			{
				printf("Nro. Invalido\n");
				break;
			}
			host_game(port_select, &turno, &move,pecas);
			break;

		case 2:
			printf("Indique a sala que quer entrar (AAA to ZZZ): ");
			fflush(stdin);
			scanf("%s", hash);
			hash[3] = '\0';
			port_select = hash[0] + (3 * hash[1]) + (7 * hash[2]) % 1001;
			if (port_select < 0 || port_select > 1001)
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

//Funcao utilizada para simplificar verificacoes de erro relacionadas a socket. Utilizado nos arquivos host_game.c e join_game.c!
int verify(int ret, const char *msg)
{
	if (ret == -1)
	{
		perror(msg);
		exit(1);
	}
	return ret;
}