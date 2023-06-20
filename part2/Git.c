#include "Git.h"

int main(){
    char *quit = "quit";
    int cond = 1;
    // header *branch = cria_header();
    hash *branch = inicia_hash();
    guarda_info_hash(branch, "Master");
    // header *commits = cria_header();
    hash *commits = inicia_hash();
    guarda_info_commit_hash(commits, "First commit", "Master");
    int brach_atual = -1;

    while(cond == 1){
        char *comando = (char*) malloc(50 * sizeof(char));

        printf("User@PC:$ ");
        gets(comando);
        
        if(strcmp(comando, quit) == 0){
            break;
        }

        header *linha = separa_string(comando);

        verifica_comando(linha, branch, commits, &brach_atual);

        limpa(linha);

        printf("\n");
    }
    limpa_hash(branch);
    limpa_hash(commits);
    printf("Finalizando programa...\n");
    
    return 0;
}