#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Git.h"

int main(){
    char *quit = "quit";
    int cond = 1;
    header *branch = cria_header();
    guarda_info(branch, "Master");
    header *commits = cria_header();
    guarda_info_commit(commits, "First commit", branch -> primeiro -> info);

    while(cond == 1){
        char *comando = (char*) malloc(50 * sizeof(char));
        printf("User@PC:$ ");
        gets(comando);
        if(strcmp(comando, quit) == 0){
            break;
        }

        header *linha = separa_string(comando);

        verifica_comando(linha, branch, commits);

        limpa(linha);
    }

    printf("Finalizando programa...\n");
    return 0;
}