#include "Git.h"

int main(void) {
    printf("Abrindo socket do servidor...\n");

    /* Estruturas de soquete do cliente e do servidor */
    struct sockaddr_in client, server;

    /* Descritores de arquivo de cliente e servidor */
    int serverfd, clientfd;

    char buffer[BUFFER_LENGTH];
    char comi[BUFFER_LENGTH];
    header *dados_salvos;
    char palavra[BUFFER_LENGTH];

    while (1) {
        printf("-------------------------------------------------------------------------------------------\n");
        fprintf(stdout, "Iniciando o Servidor\n");

        /* Cria um soquete IPv4 TCP */
        serverfd = socket(AF_INET, SOCK_STREAM, 0);
        if (serverfd == -1) {
            perror("Não foi possível criar o soquete do servidor:");
            return EXIT_FAILURE;
        }
        fprintf(stdout, "Soquete do servidor criado com fd: %d\n", serverfd);

        /* Define as propriedades do soquete do servidor */
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons(PORT);
        memset(server.sin_zero, 0x0, 8);

        /* Manipula o erro de porta já em uso */
        int yes = 1;
        if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("Erro nas opções do soquete:");
            return EXIT_FAILURE;
        }

        /* Associa o soquete a uma porta */
        if (bind(serverfd, (struct sockaddr *)&server, sizeof(server)) == -1) {
            perror("Erro ao associar o soquete:");
            close(serverfd);
            return EXIT_FAILURE;
        }

        /* Começa a esperar conexões de clientes */
        if (listen(serverfd, 1) == -1) {
            perror("Erro na espera de conexões:");
            close(serverfd);
            return EXIT_FAILURE;
        }
        fprintf(stdout, "Ouvindo na porta %d\n", PORT);

        socklen_t client_len = sizeof(client);
        if ((clientfd = accept(serverfd, (struct sockaddr *)&client, &client_len)) == -1) {
            perror("Erro ao aceitar conexão do cliente:");
            close(serverfd);
            return EXIT_FAILURE;
        }

        /* Copia para o buffer a mensagem de boas-vindas */
        strcpy(buffer, "Repositório remoto iniciado!\n\0");

        /* Envia a mensagem para o cliente */
        if (send(clientfd, buffer, strlen(buffer), 0)) {
            fprintf(stdout, "\nCliente conectado.\nAguardando acao do cliente ...\n\n");

            /* Comunica com o cliente até receber a mensagem "bye" */
            do {
                /* Zera o buffer */
                memset(buffer, 0x0, BUFFER_LENGTH);

                // Receber os dados do cliente
                int received_bytes = recv(clientfd, buffer, BUFFER_LENGTH, 0);
                if (received_bytes == -1) {
                    perror("Falha ao receber dados do cliente");
                    break;
                }

                if (received_bytes == 0) {
                    // O cliente encerrou a conexão inesperadamente
                    fprintf(stdout, "Cliente desconectado.\n");
                    break;
                } else if (strcmp(buffer, "Commit recebido!") == 0) {
                    // Mensagem "bye" recebida
                    send(clientfd, "Commit recebido!", BUFFER_LENGTH, 0);

                }else if (strcmp(buffer, "pull") == 0) {
                    // memset(buffer, 0x0, BUFFER_LENGTH);
                    // imprime_commits2(dados_salvos);
                    // printf("\n");
                    // send(clientfd, dados_salvos, sizeof(header), 0);
                    for(com *p = dados_salvos -> prim; p != NULL; p = p -> proximo){
                        *palavra = (char*) malloc(BUFFER_LENGTH * sizeof(char));
                        sprintf(palavra, "%d %s %s", p -> chave, p -> info, p -> branch_do_commit);
                        printf("DADOS DO COMMIT ENVIADO: %s\n", palavra);
                        send(clientfd, palavra, BUFFER_LENGTH, 0);
                    }
                    send(clientfd, "Commit recebido!", BUFFER_LENGTH, 0);
                
                }else {
                    strncpy(comi, buffer, received_bytes);
                    comi[received_bytes] = '\0';

                    // Imprimir os dados do aluno
                    printf("DADOS DO COMMIT RECEBIDO: ");
                    printf("%s\n", comi);
                    header *comando = separa_string(comi);
                    dados_salvos = guarda_server(comando);
                    limpa(comando);
                    send(clientfd, "Commit recebido!", BUFFER_LENGTH, 0);

                    // Enviar a resposta para o cliente
                    if (send(clientfd, buffer, strlen(buffer), 0) == -1) {
                        perror("Falha ao enviar resposta para o cliente");
                        break;
                    }
                }
            } while (1);
        }

        /* Fecha a conexão do cliente */
        close(clientfd);

        /* Feche o soquete local */
        close(serverfd);

        printf("Conexão fechada\n\n");
        printf("-------------------------------------------------------------------------------------------\n");
    }
    return EXIT_SUCCESS;
}
