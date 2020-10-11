#include "header.h"

int main()
{
	int menu_select;
	int port_select;

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
			host_game(port_select);
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
			join_game(port_select);
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