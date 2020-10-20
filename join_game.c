#include "header.h"
#include "xadrez.h"

/*********************************************************************************************************************************************/

	//Arquivo join_game.c
	//Responsavel pela conexao do socket pela perspectiva do jogador que nao eh host (dono da sala).

/*********************************************************************************************************************************************/

/*
Funcao int join_game(): Responsavel pela conexao do socket, pelo gerenciamento da conexao do jogo e interface do usuario.
Entrada:
    int port: Porta a ser utilizada na conexao.
    int *pturno: Qual jogador esta jogando agora?
    int *pmove: Quantas rodadas se passaram ate agora?
    char pecas[]: Array de pecas provindas da main.
Saida:
	Sempre 0.
*/
int join_game(int port, int *pturno, int *pmove, char pecas[])
{
	
	int enviados;
	char mensagem[6];
	int recebidos;
	char resposta[6];
	int result = 0;

	verify(client_socket = socket(AF_INET, SOCK_STREAM, 0), "Erro ao criar o socket!\n"); //Funcao declarada na main.

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

	// inicializa tabuleiro com -1
	for (int i = 0; i < 8; i++)
	{
		for (int j = 2; j <= 5; j++)
		{
			tab[i][j].jog = -1;
			tab[i][j].tipo = 'z'; // sem peca
		}
	}

	for (int i = 0; i < 8; i++) //inicializa o board em si
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
	showGame(tab, 0);

	do
	{
		while (recebidos != -1)
		{
			//ENVIA MOVIMENTO AO OPONENTE
			printf("Envie seu movimento (ou exit para sair da sala): ");
			fflush(stdin);
			scanf("%s", mensagem);
			if (!strcmp(resposta, "exit") || !strcmp(mensagem, "exit")) //check "exit"
			{
				printf("Terminando conexao....\n");							   //termina conexao com "exit"
				enviados = send(client_socket, mensagem, strlen(mensagem), 0); //manda "exit" pro oponente
				close(client_socket);
				printf("Conexao encerrada.\n");
				recebidos = -1;
				return 0;
			}
			result = main_game(mensagem, tab, pturno, pmove, 0); //faz movimento no board
			if (result == 2)												 //fim de jogo
			{
				strcpy(mensagem, "exit");
				enviados = -1;
				break;
			}
			if (result) //movimento valido
			{
				break;
			}
			else
			{
				printf("Tente novamente...\n");
			}
		}
		enviados = send(client_socket, mensagem, strlen(mensagem), 0);
		if (result == 2) //fim de jogo
			break;
		showGame(tab, 0);

		//ACEITA RESPOSTA DO OPONENTE
		recebidos = recv(client_socket, resposta, 6, 0);
		resposta[recebidos] = '\0';
		if (!strcmp(resposta, "exit") || !strcmp(mensagem, "exit")) //testa exit
		{
			printf("Terminando conexao....\n");
			close(client_socket);
			printf("Conexao encerrada.\n");
			recebidos = -1;
			return 0;
		}
		
		result = main_game(resposta, tab, pturno, pmove, 0);
		printf("Jogada do adversario: %s\n", resposta);
		showGame(tab, 0);

		if (result == 2) //fim de jogo?
			break;

	} while (recebidos != -1 && enviados != -1); //enquanto ha conexao, continuar o jogo
	return 0;
}

