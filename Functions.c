#include "Git.h"

hash *inicia_hash(void){
    hash *h = (hash*) malloc(sizeof(hash) * tam);

    for(int i = 0; tam > i; i++){
        (h + i) -> encad = cria_header();
        (h + i) -> hist_commits = cria_header();
    }

    return h;
}

int funcao_hash(void){
    int valor = gerarNumeroAleatorio();
    
    return valor % tam;

}

int gerarNumeroAleatorio(){
    srand(time(NULL)); 

    int numero = rand();

    return numero;
}

header *cria_header(void){
    header *x = (header*) malloc(sizeof(header));

    x -> primeiro = NULL;
    x -> ultimo = NULL;
    x -> prim = NULL;
    x -> ult = NULL;

    return x;
}

header *separa_string(char *comando){
    int inicio = 0, fim = 0;
    int elementos = 0;
    header *h = cria_header();

    for(int i = 0; strlen(comando) > i; i++){
        if(comando == NULL){
            continue;
        }
        while(comando[i] == ' ')
            i++;  
        
        if(comando[i] == '\0'){
            break;
        } 
        inicio = i;

        while(comando[i] != ' ' && i < comando[i] != '\0')
            i++;
        fim = i;

        char *buffer = (char*) malloc(30 * sizeof(char));
        strncpy(buffer, comando + inicio, fim - inicio);
        buffer[fim - inicio] = '\0';
        guarda_info(h, buffer);
    }
    
    return h;

}

void guarda_info(header *h, char *info){
    encad *x = (encad*) malloc(sizeof(encad));
    
    if(h -> primeiro == NULL){
        h -> primeiro = x;
        h -> ultimo = x;
        x -> proximo = NULL;
        x -> anterior = NULL;
        x -> info = info;
    }else{
        h -> ultimo -> proximo = x;
        x -> anterior = h -> ultimo;
        x -> proximo = NULL;
        h -> ultimo = x;
        x -> info = info;
    }
}

void guarda_info_commit(header *h, char *info, char *branch_commit){
    com *x = (com*) malloc(sizeof(com));
    
    if(h -> prim == NULL){
        h -> prim = x;
        h -> ult = x;
        x -> proximo = NULL;
        x -> anterior = NULL;
        x -> info = info;
        x -> branch_do_commit = branch_commit;
    }else{
        h -> ult -> proximo = x;
        x -> anterior = h -> ult;
        x -> proximo = NULL;
        h -> ult = x;
        x -> info = info;
        x -> branch_do_commit = branch_commit;
    }

}

void verifica_comando(header *comando, header *branch, header *commits){
    char *git = "git";
    char *branch_cod = "branch";
    char *commit = "commit";
    char *checkout = "checkout";
    char *merge = "merge";
    char *log = "log";
    char *clear = "clear";

    if(strcmp(comando -> primeiro -> info, clear) == 0 && strcmp(comando -> ultimo -> info, clear) == 0){
        f_clear();
        return;
    }

    if(strcmp(comando -> primeiro -> info, git) != 0 || comando -> ultimo == comando -> primeiro){
        printf("Comando nao reconhecido!\n");
        return;

    }else{
        if(strcmp(comando -> primeiro -> proximo -> info, branch_cod) == 0){
            f_branch(comando, branch, commits);

        }else if(strcmp(comando -> primeiro -> proximo -> info, commit) == 0){
            f_commit(comando, commits, branch -> primeiro -> info);

        }else if(strcmp(comando -> primeiro -> proximo -> info, checkout) == 0){
            f_checkout(comando, branch);

        }else if(strcmp(comando -> primeiro -> proximo -> info, merge) == 0){
            f_merge(commits, branch -> primeiro -> info, comando -> ultimo -> info, branch);

        }else if(strcmp(comando -> primeiro -> proximo -> info, log) == 0){
            f_log(comando, commits, branch);

        }else{
            printf("Comando nao reconhecido!\n");
            return;

        }
    }
}

void f_clear(void){

    system("clear");

}

void f_commit(header *h, header *commits, char *branch_atual){
    char *err = "-m";
    char *err2 = "commit";

    if(strcmp(h -> ultimo -> info, err) == 0 || strcmp(h -> ultimo -> info, err2) == 0 || strcmp(h -> ultimo -> anterior -> info, err) != 0){
        printf("Comando invalido!\n");

    }else{
        for(com *p = commits -> prim; p != NULL; p = p -> proximo){
            if(strcmp(p -> info, h -> ultimo -> info) == 0){
                printf("Commit '%s' ja feito, mude o nome.\n", p -> info);
                return;
            }
        }

        guarda_info_commit(commits, h -> ultimo -> info, branch_atual);
        printf("Commit '%s' criado com sucesso!\n", commits -> ult -> info);
    }

}

void f_log(header *h, header *commits, header *branch){
    char *log = "log";

    if(strcmp(h -> ultimo -> info, log) == 0){
        printf("Branch: %s\n", branch -> primeiro -> info);
        imprime_commits(commits, branch -> primeiro -> info);
    }else{
        printf("Comando invalido!\n");
    }
}

void f_branch(header *h, header *branch, header *commits){
    char *branch_cod = "branch";
    char *master = "Master";

    if(strcmp(h -> ultimo -> info, branch_cod) == 0){
        for(encad *p = branch -> primeiro; p != NULL; p = p -> proximo){
            printf("%p %s\n", p, p -> info);
        }

    }else{
        for(encad *p = branch -> primeiro; p != NULL; p = p -> proximo){
            if(strcmp(p -> info, h -> ultimo -> info) == 0){
                printf("Branch '%s' ja existe.\n", h -> ultimo -> info);
                return;
            }
        }

        guarda_info(branch, h -> ultimo -> info);
        printf("Branch '%s' criada com sucesso!\n", branch -> ultimo -> info);

        for(com *p = commits -> prim; p != NULL; p = p -> proximo){
            if(strcmp(p -> branch_do_commit, master) == 0){
                guarda_info_commit(commits, p -> info, h -> ultimo -> info);
            }
        }
    }
}

void f_checkout(header *h, header *branch){
    char *checkout = "checkout";

    if(strcmp(h -> ultimo -> info, checkout) == 0){
        printf("Comando invalido!\n");
    }else{
        for(encad *p = branch -> primeiro; p != NULL; p = p -> proximo){
            if(strcmp(h -> ultimo -> info, p -> info) == 0){
                char *aux = branch -> primeiro -> info;
                branch -> primeiro -> info = h -> ultimo -> info;
                p -> info = aux;
                printf("Mudada para branch '%s' com sucesso!\n", branch -> primeiro -> info);
                return;
            }
        }
        printf("Branch nao existe!\n");
        return;
    }
}

void f_merge(header *h, char *branch_atual, char *branch_merge, header *branch){
    char *merge = "merge";
    char *first = "First commit";

    if(strcmp(branch_merge, merge) == 0){
        printf("Comando invalido!\n");
        return;

    }else{
        for(encad *p = branch -> primeiro; p != NULL; p = p -> proximo){
            if(strcmp(branch_merge, p -> info) == 0){
                printf("Mergeando branch '%s' com branch '%s'.\n", branch_atual, branch_merge);

                for(com *p = h -> prim; p != NULL; p = p -> proximo){
                    if(strcmp(p -> branch_do_commit, branch_merge) == 0){
                        if(strcmp(p -> info, first) != 0){
                            guarda_info_commit(h, p -> info, branch_atual);
                        }
                    }
                }

                for(com *p = h -> prim; p != NULL; p = p -> proximo){
                    if(strcmp(p -> branch_do_commit, branch_atual) == 0){
                        printf("%p %s\n", p, p -> info);
                    }
                }

                return;
            }
        }
        printf("Branch '%s' nao existe!\n", branch_merge);
        return;
    }
}

void limpa(header *h){
    encad *p = h -> primeiro;
    encad *aux;

    if(p == NULL){
        printf("\nLista vazia!\n\n");
        return;
    }

    while(p != NULL){
        aux = p -> proximo;
        free(p);
        p = aux;
    }

    free(h);
}

void limpa_commit(header *h){
    com *p = h -> prim;
    com *aux;

    if(p == NULL){
        printf("\nLista vazia!\n\n");
        return;
    }

    while(p != NULL){
        aux = p -> proximo;
        free(p);
        p = aux;
    }

    free(h);
}

void imprime_commits(header *h, char *branch_atual){
    
    for(com *p = h -> prim; p != NULL; p = p -> proximo){
        if(strcmp(p -> branch_do_commit, branch_atual) == 0){
            printf("%p %s\n", p, p -> info);
        }
    }

}




