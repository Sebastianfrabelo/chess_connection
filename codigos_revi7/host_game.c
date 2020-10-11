#include "header.h"

int host_game(int port)
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

		conectado(client_socket);
		printf("Continuar na sala? S/N: ");
		fflush(stdin);
		scanf("%c", &continua);
		if (continua == 'N' || continua == 'n')
		{
			printf("Sala excluida!\n");
			return 0;
		}
		printf("Codigo atual da sala: %d\n", port);
	}
	return 0;
}

int conectado(int client_socket)
{
	char buffer[BUFFERSIZE];
	size_t bytes_rd;
	int msgsize = 0;
	int enviados;
	char mensagem[256];
	int recebidos;
	char resposta[256];

	do
	{
		recebidos = recv(client_socket, resposta, 256, 0);
		resposta[recebidos] = '\0';
		verify(recebidos, "Erro no recebimento da jogada!!\n");
		if (!strcmp(resposta, "exit") || !strcmp(mensagem, "exit"))
		{
			printf("Terminando conexao....\n");
			close(client_socket);
			printf("Conexao encerrada.\n");
			recebidos = -1;
		}
		printf("Jogada do oponente: %s\n", resposta);

		printf("Indique sua jogada: ");
		fflush(stdin);
		scanf("%s", mensagem);
		if (!strcmp(resposta, "exit") || !strcmp(mensagem, "exit"))
		{
			printf("Terminando conexao....\n");
			close(client_socket);
			printf("Conexao encerrada.\n");
			recebidos = -1;
		}
		enviados = send(client_socket, mensagem, strlen(mensagem), 0);
	} while (recebidos != -1 && enviados != -1);
	printf("Saindo da sala...\n");
	close(client_socket);
	printf("Conexao encerrada.\n");
	return 0;
}
