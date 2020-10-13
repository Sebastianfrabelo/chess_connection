#include "header.h"
#include "xadrez.h"

int join_game(int port, int *pturno, int *pmove, char pecas[])
{
	int enviados;
	char mensagem[6];
	int recebidos;
	char resposta[6];
	int result = 0;

	verify(client_socket = socket(AF_INET, SOCK_STREAM, 0), "Erro ao criar o socket!\n");

	addr.sin_family = AF_INET;
	addr.sin_port = htons(DEFAULT_PORT + port);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));

	printf("Tentando se conectar ao servidor...\n");

	if (connect(client_socket, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		printf("Erro ao se conectar!\n");
		return 1;
	}

	printf("Conectado!\n\n");
	// inicializa tabuleiro normal e de ataques
	for (int i = 0; i < 8; i++)
	{
		for (int j = 2; j <= 5; j++)
		{
			tab[i][j].jog = -1;
			tab[i][j].tipo = 'z'; // sem peca
		}
	}

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			tab_danger[i][j][0] = -1; //sem peca
			tab_danger[i][j][1] = -1; //sem peca
		}
	}

	for (int i = 0; i < 8; i++)
	{
		tab[i][0].jog = 0;
		tab[i][0].tipo = pecas[i];

		tab[i][1].jog = 0;
		tab[i][1].tipo = 'p';

		tab[i][6].jog = 1;
		tab[i][6].tipo = 'p';

		tab[i][7].jog = 1;
		tab[i][7].tipo = pecas[i];
	}
	showGame(tab, 0);

	do
	{
		while (1 && recebidos != -1)
		{
			printf("Envie seu movimento (ou exit para sair da sala): ");
			fflush(stdin);
			scanf("%s", mensagem);
			if (!strcmp(resposta, "exit") || !strcmp(mensagem, "exit"))
			{
				printf("Terminando conexao....\n");
				enviados = send(client_socket, mensagem, strlen(mensagem), 0);
				close(client_socket);
				printf("Conexao encerrada.\n");
				recebidos = -1;
				return 0;
			}
			result = main_game(mensagem, tab, pturno, pmove, 0);
			if (result == 2)
			{
				strcpy(mensagem, "exit");
				enviados = -1;
				break;
			}
			if (result)
			{
				break;
			}
			else
			{
				printf("Comando incorreto, tente novamente...\n");
			}
		}
		enviados = send(client_socket, mensagem, strlen(mensagem), 0);
		if (result == 2)
			break;
		//	main_game(mensagem, tab, pturno, pmove,0);

		memset(resposta, '0', sizeof(resposta));
		recebidos = recv(client_socket, resposta, 6, 0);
		resposta[recebidos] = '\0';
		if (!strcmp(resposta, "exit") || !strcmp(mensagem, "exit"))
		{
			printf("Terminando conexao....\n");
			close(client_socket);
			printf("Conexao encerrada.\n");
			recebidos = -1;
			return 0;
		}
		if (main_game(resposta, tab, pturno, pmove, 0) == 2)
			break;

		printf("Jogada do adversario: %s\n", resposta);
		//	main_game(resposta, tab, pturno, pmove, 0);

	} while (recebidos != -1 && enviados != -1);

	printf("Saindo da sala...");
	close(client_socket);
	printf("Voce saiu da sala.\n");
	return 0;
}