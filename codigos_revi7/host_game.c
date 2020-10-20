#include "header.h"
#include "xadrez.h"

int host_game(int port, int *pturno, int *pmove, char pecas[])
{
	char continua;

	int server_socket, client_socket, size;
	STRUCT_IN server_addr, client_addr;
	verify((server_socket = socket(AF_INET, SOCK_STREAM, 0)),
		   "Erro em criar socket!");
	//SOCK_STREAM,0 eh utilizado em conexoes tipo TCP, que garantem o envio do pacote em stream

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(DEFAULT_PORT + port); //codigo da sala

	verify(bind(server_socket, (SA *)&server_addr, sizeof(server_addr)),
		   "Erro em criar bind!");
	verify(listen(server_socket, MAX_CONNECTIONS),
		   "Erro na funcao listen!");
	//Linka e escuta a conexao

	while (1)
	{
		printf("Aguardando conexao...\n\n");
		client_socket = accept(server_socket, 0, 0);
		verify(client_socket,
			   "Erro na conexao");
		printf("Conectado!!!\n");

		// inicializa tabuleiro com -1
		for (int i = 0; i < 8; i++)
		{
			for (int j = 2; j <= 5; j++)
			{
				tab[i][j].jog = -1;
				tab[i][j].tipo = 'z'; // sem peca
			}
		}

		// inicializa tabuleiro
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
		update_danger(tab, tab_danger);
		showGame(tab, 1);

		conectado(client_socket, pturno, pmove);
		printf("Continuar na sala? S/N: ");
		fflush(stdin);
		scanf("%c", &continua);
		if (continua == 'S' || continua == 's')
		{
			printf("Codigo atual da sala: %d\n", port);
		}
		else
		{
			client_socket = 0;
			printf("Sala excluida!\n");
			return 0;
		}
	}
	return 0;
}

int conectado(int client_socket, int *pturno, int *pmove)
{

	char buffer[BUFFERSIZE];
	size_t bytes_rd;
	int msgsize = 0;
	int enviados;
	char mensagem[6];
	int recebidos;
	char resposta[6];
	int result = 0;

	do
	{

		//RECEBE MOVIMENTO
		memset(resposta, '0', sizeof(resposta));
		recebidos = recv(client_socket, resposta, 6, 0);
		resposta[recebidos] = '\0';
		verify(recebidos, "Erro no recebimento da jogada!!\n");
		if (!strcmp(resposta, "exit") || !strcmp(mensagem, "exit")) //testa "exit"
		{
			printf("Terminando conexao....\n"); //caso exit game
			close(client_socket);
			printf("Conexao encerrada.\n");
			recebidos = -1;
			break;
		}
		result = main_game(resposta, tab, pturno, pmove, 0); //faz o movimento
		printf("Jogada do adversario: %s\n", resposta);
		if (result == 2) //fim de jogo?
			break;

		result = 0;
		showGame(tab, 1);
		
		//ENVIA MOVIMENTO
		while (recebidos != -1)
		{
			printf("Envie seu movimento (ou exit para sair da sala): ");
			fflush(stdin);
			scanf("%s", mensagem);
			if (!strcmp(resposta, "exit") || !strcmp(mensagem, "exit"))
			{
				enviados = send(client_socket, mensagem, strlen(mensagem), 0);
				printf("Terminando conexao....\n");
				close(client_socket);
				printf("Conexao encerrada.\n");
				recebidos = -1;
				break;
			}
			result = main_game(mensagem, tab, pturno, pmove, 1); //faz o movimento
			if (result == 2)												 //fim de jogo
			{
				//strcpy(mensagem, "exit"); //fim de jogo
				recebidos = -1;
				break;
			}
			if (result) //jogada valida
			{
				break;
			}
				printf("Comando incorreto, tente novamente...\n");
		}
		enviados = send(client_socket, mensagem, strlen(mensagem), 0);
		if (result == 2) //fim de jogo?
			break;
		showGame(tab, 1);

	} while (recebidos != -1 && enviados != -1); //continua o jogo

	printf("Saindo da sala...\n"); //fim de jogo ou desconexao
	close(client_socket);
	printf("Conexao encerrada.\n");
	return 0;
}
