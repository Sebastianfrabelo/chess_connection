#include <stdio.h> 
#include <string.h> 
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main()
{ 
int    meu_socket;
int    sock_cliente;
struct sockaddr_in addr;

meu_socket = socket(AF_INET,SOCK_STREAM,0);

if(meu_socket == -1)
  {
  printf("\nErro ao criar o socket!\n");
  return 1;
  }

addr.sin_family      = AF_INET;
addr.sin_port        = htons(1234);
addr.sin_addr.s_addr = INADDR_ANY;
memset(&addr.sin_zero,0,sizeof(addr.sin_zero));

if(bind(meu_socket,(struct sockaddr*)&addr,sizeof(addr)) == -1)
  {
  printf("\nErro na funcao bind()\n");
  return 1;
  }

if(listen(meu_socket,1) == -1)
  {
  printf("\nErro na funcao listen()\n");
  return 1;
  }

printf("\nAguardando cliente...\n");

sock_cliente = accept(meu_socket,0,0);

if(sock_cliente == -1)
  {
  printf("\nErro na funcao accept()\n");
  return 1;
  }

printf("\nCliente conectado!\n");

int recebidos, enviados; /* Controle de bytes enviados e recebidos */
char mensagem[256];      /* Buffer para envio de mensagens */ 
char resposta[256];      /* Buffer para receber mensagens  */

do  /* Executa as instruções abaixo ... */
  { 

  recebidos = recv(sock_cliente,resposta,256,0);              /* Recebe mensagem do cliente */
  resposta[recebidos] = '\0';                                 /* Finaliza a string com o caractere NULO */
  printf("Cliente: %s\n",resposta); 	       	              /* Mostra a mensagem do cliente */

  printf("Servidor: ");                                       /* Simplesmente informa que deve-se preencher uma mensagem */
  fgets(mensagem,256,stdin);                                  /* Obtém uma mensagem digitada */
  mensagem[strlen(mensagem)-1] = '\0';                        /* Finaliza a string */
  enviados = send(sock_cliente,mensagem,strlen(mensagem),0);  /* Envia a string */
  
  }while(enviados != -1 ); /* ... enquanto as funções send() e recv() não retornarem -1 = ERRO */

close(sock_cliente);         
close(meu_socket);         
return 0;
}
