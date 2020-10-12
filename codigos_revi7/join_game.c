#include "header.h"
#include "xadrez.h"

int join_game(int port)
{
	int enviados;
	char mensagem[256];
	int recebidos;
	char resposta[256];

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
	do
	{
		printf("Envie seu movimento (ou exit para sair da sala): ");
		fflush(stdin);
		scanf("%s", mensagem);
		enviados = send(client_socket, mensagem, strlen(mensagem), 0);

		if (!strcmp(resposta, "exit") || !strcmp(mensagem, "exit"))
		{
			printf("Terminando conexao....\n");
			close(client_socket);
			printf("Conexao encerrada.\n");
			recebidos = -1;
		}

		recebidos = recv(client_socket, resposta, 256, 0);
		resposta[recebidos] = '\0';
		printf("Jogada do adversario: %s\n", resposta);

	} while (recebidos != -1 && enviados != -1);

	printf("Saindo da sala...");
	close(client_socket);
	printf("Voce saiu da sala.\n");
	return 0;
}