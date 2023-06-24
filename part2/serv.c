#include "Git.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main(void){

    printf("Abrindo socket do servidor...\n");

    /* Estruturas de soquete do cliente e do servidor */
    struct sockaddr_in client, server;

    /* Descritores de arquivo de cliente e servidor */
    int serverfd, clientfd;

    char buffer[BUFFER_LENGTH];

    while (1)
    {

        fprintf(stdout, "Iniciando o Servidor\n");

        /* Cria um soquete IPv4 TCP */
        serverfd = socket(AF_INET, SOCK_STREAM, 0);
        if (serverfd == -1)
        {
            perror("Não foi possível criar o soquete do servidor:");
            return EXIT_FAILURE;
        }
        fprintf(stdout, "Soquete do servidor criado com fd: %d\n", serverfd);

        /* Define as propriedades do soquete do servidor */
        server.sin_family = AF_INET;
        server.sin_port = htons(PORT);
        memset(server.sin_zero, 0x0, 8);

        /* Manipula o erro de porta já em uso */
        int yes = 1;
        if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR,
                       &yes, sizeof(int)) == -1)
        {
            perror("Erro nas opções do soquete:");
            return EXIT_FAILURE;
        }

        /* Associa o soquete a uma porta */
        if (bind(serverfd, (struct sockaddr *)&server, sizeof(server)) == -1)
        {
            perror("Erro ao associar o soquete:");
            return EXIT_FAILURE;
        }

        /* Começa a esperar conexões de clientes */
        if (listen(serverfd, 1) == -1)
        {
            perror("Erro na espera de conexões:");
            return EXIT_FAILURE;
        }
        fprintf(stdout, "Ouvindo na porta %d\n", PORT);

        socklen_t client_len = sizeof(client);
        if ((clientfd = accept(serverfd,
                               (struct sockaddr *)&client, &client_len)) == -1)
        {
            perror("Erro ao aceitar conexão do cliente:");
            return EXIT_FAILURE;
        }

        /* Copia para o buffer a mensagem de boas-vindas */
        strcpy(buffer, "Olá, cliente!\n\0");

        /* Envia a mensagem para o cliente */
        if (send(clientfd, buffer, strlen(buffer), 0))
        {
            fprintf(stdout, "Cliente conectado.\nAguardando mensagem do cliente ...\n");

            /* Comunica com o cliente até receber a mensagem "bye" */
            do
            {

                /* Zera o buffer */
                memset(buffer, 0x0, BUFFER_LENGTH);

                // Receber os dados do cliente
                int received_bytes = recv(clientfd, buffer, BUFFER_LENGTH, 0);
                if (received_bytes == -1)
                {
                    perror("Falha ao receber dados do cliente");
                    break;
                }

                if (received_bytes == 0)
                {
                    // O cliente encerrou a conexão inesperadamente
                    fprintf(stdout, "Cliente desconectado.\n");
                    break;
                }
                else if (strcmp(buffer, "bye") == 0) {
                    // Mensagem "bye" recebida
                    send(clientfd, "bye", 3, 0);
                }
                else
                {

                    // Tentar interpretar os dados recebidos como uma estrutura Aluno
                    hash *commits = inicia_hash();
                    if (received_bytes == sizeof(hash))
                    {
                        memcpy(commits, buffer, sizeof(hash));
                        // Imprimir os dados do aluno
                        printf("Dados do commit recebido:\n");
                        imprime_commits(commits);

                        fprintf(stdout, "Commit recebido.\n");
                    }
                    else
                    {
                        // Os dados recebidos não correspondem a uma estrutura Aluno
                        fprintf(stdout, "Dados recebidos não correspondem a uma estrutura commit: %s\n", buffer);
                    }

                    // Responder o cliente com a lista de alunos armazenada no servidor
                    memset(buffer, 0x0, BUFFER_LENGTH);
                    strcpy(buffer, "Commits atuais:\n\n");
                    imprime_commits(commits);

                    // Enviar a resposta para o cliente
                    if (send(clientfd, buffer, strlen(buffer), 0) == -1)
                    {
                        perror("Falha ao enviar resposta para o cliente");
                        break;
                    }
                }

            } while (strcmp(buffer, "bye"));
        }

        /* Fecha a conexão do cliente */
        close(clientfd);

        /* Feche o soquete local */
        close(serverfd);

        printf("Conexão fechada\n\n");
    }
    return EXIT_SUCCESS;
}
