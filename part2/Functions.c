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
        guarda_info(h, buffer, 0);
    }
    
    return h;

}

void guarda_info_hash(hash *h, char *info){
    int pos = funcao_hash();

    guarda_info((h + pos) -> encad, info, pos);
}

void guarda_info(header *h, char *info, int chave){
    encad *x = (encad*) malloc(sizeof(encad));
    
    if(h -> primeiro == NULL){
        h -> primeiro = x;
        h -> ultimo = x;
        x -> proximo = NULL;
        x -> anterior = NULL;
        x -> info = info;
        x -> chave = chave;
    }else{
        h -> ultimo -> proximo = x;
        x -> anterior = h -> ultimo;
        x -> proximo = NULL;
        h -> ultimo = x;
        x -> info = info;
        x -> chave = chave;
    }
}

void guarda_info_commit_hash(hash *h, char *info, char *branch_commit){
    int pos = funcao_hash();

    guarda_info_commit((h + pos) -> hist_commits, info, branch_commit, pos);
}

void guarda_info_commit(header *h, char *info, char *branch_commit, int chave){
    com *x = (com*) malloc(sizeof(com));
    
    if(h -> prim == NULL){
        h -> prim = x;
        h -> ult = x;
        x -> proximo = NULL;
        x -> anterior = NULL;
        x -> info = info;
        x -> branch_do_commit = branch_commit;
        x -> chave = chave;
    }else{
        h -> ult -> proximo = x;
        x -> anterior = h -> ult;
        x -> proximo = NULL;
        h -> ult = x;
        x -> info = info;
        x -> branch_do_commit = branch_commit;
        x -> chave = chave;
    }

}

void verifica_comando(header *comando, hash *branch, hash *commits, int *branch_atual){
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
            f_commit(comando, commits, branch_atual, branch);

        }else if(strcmp(comando -> primeiro -> proximo -> info, checkout) == 0){
            f_checkout(comando, branch, branch_atual, commits);

        }else if(strcmp(comando -> primeiro -> proximo -> info, merge) == 0){
            f_merge(commits, branch_atual, comando -> ultimo -> info, branch);

        }else if(strcmp(comando -> primeiro -> proximo -> info, log) == 0){
            if(*branch_atual == -1)
                f_log(comando, commits, "Master");
            else
                f_log(comando, commits, (branch + *branch_atual) -> encad -> primeiro -> info);

        }else{
            printf("Comando nao reconhecido!\n");
            return;

        }
    }
}

void f_clear(void){

    system("clear");

}

void f_commit(header *h, hash *commits, int *branch_atual, hash *branch){
    char *err = "-m";
    char *err2 = "commit";

    if(strcmp(h -> ultimo -> info, err) == 0 || strcmp(h -> ultimo -> info, err2) == 0 || strcmp(h -> ultimo -> anterior -> info, err) != 0){
        printf("Comando invalido!\n");

    }else{
        for(int i = 0; tam > i; i++){
            for(com *p = (commits + i) -> hist_commits -> prim; p != NULL; p = p -> proximo){
                if(strcmp(p -> info, h -> ultimo -> info) == 0){
                    printf("Commit '%s' ja feito, mude o nome.\n", p -> info);
                    return;
                }
            }
        }

        if(*branch_atual == -1){
            guarda_info_commit_hash(commits, h -> ultimo -> info, "Master");
            printf("Commit '%s' criado com sucesso!\n", h -> ultimo -> info);
        }else{
            guarda_info_commit_hash(commits, h -> ultimo -> info, (branch + *branch_atual) -> encad -> primeiro -> info);
            printf("Commit '%s' criado com sucesso!\n", h -> ultimo -> info);
        }
    }

}

void f_log(header *h, hash *commits, char *branch_atual){
    char *log = "log";

    if(strcmp(h -> ultimo -> info, log) == 0){
        printf("Branch_atual: %s\n", branch_atual);
        imprime_commits(commits);
    }else{
        printf("Comando invalido!\n");
    }
}

void f_branch(header *h, hash *branch, hash *commits){
    char *branch_cod = "branch";
    char *master = "Master";

    if(strcmp(h -> ultimo -> info, branch_cod) == 0){
        for(int i = 0; tam > i; i++){
            for(encad *p = (branch + i) -> encad -> primeiro; p != NULL; p = p -> proximo){
                printf("%d %s\n", p -> chave, p -> info);
            }
        }
    }else{
        for(int i = 0; tam > i; i++){
            for(encad *p = (branch + i) -> encad -> primeiro; p != NULL; p = p -> proximo){
                if(strcmp(p -> info, h -> ultimo -> info) == 0){
                    printf("Branch '%s' ja existe.\n", h -> ultimo -> info);
                    return;
                }
            }
        }

        guarda_info_hash(branch, h -> ultimo -> info);
        printf("Branch '%s' criada com sucesso!\n", h -> ultimo -> info);

        // for(int i = 0; tam > i; i++){
        //     for(com *p = (commits + i) -> hist_commits -> prim; p != NULL; p = p -> proximo){
        //         if(strcmp(p -> branch_do_commit, master) == 0){
        //             guarda_info_commit_hash(commits, p -> info, h -> ultimo -> info);
        //         }
        //     }
        // }
    }
}

void f_checkout(header *h, hash *branch, int *branch_atual, hash *commits){
    char *checkout = "checkout";
    int cond;

    if(strcmp(h -> ultimo -> info, checkout) == 0 || strcmp(h -> ultimo -> anterior -> info, checkout) != 0){
        printf("Comando invalido!\n");
    }else{
        for(int i = 0; tam > i; i++){
            for(encad *p = (branch + i) -> encad -> primeiro; p != NULL; p = p -> proximo){
                if(strcmp(h -> ultimo -> info, p -> info) == 0){
                    *branch_atual = p -> chave;
                    printf("Mudada para branch '%s' com sucesso!\n", h -> ultimo -> info);
                    return;
                }                
            }

            for(com *p = (commits + i) -> hist_commits -> prim; p != NULL; p = p -> proximo){
                if(atoi(h -> ultimo -> info) == i && p != NULL){
                    printf("Mudada para commit '%s' com sucesso!\n", p -> info);
                    return;
                }                
            }

        }
        printf("Chave não existe!\n");
        return;
    }
}

void f_merge(hash *commits, int *branch_atual, char *branch_merge, hash *branch){
    char *merge = "merge";
    char *first = "First commit";
    char *branch_now;
    
    if(*branch_atual == -1){
        branch_now = (char*) malloc(10 * sizeof(char));
        branch_now = "Master";
    }else{
        branch_now = (branch + *branch_atual) -> encad -> primeiro -> info;
    }

    if(strcmp(branch_merge, merge) == 0){
        printf("Comando invalido!\n");
        return;

    }else{
        for(int i = 0; tam > i; i++){
            for(encad *p = (branch + i) -> encad -> primeiro; p != NULL; p = p -> proximo){
                if(strcmp(branch_merge, p -> info) == 0){
                    printf("Mergeando branch '%s' com branch '%s'.\n", branch_now, branch_merge);

                    for(int i = 0; tam > i; i++){
                        for(com *p = (commits + i) -> hist_commits -> prim; p != NULL; p = p -> proximo){
                            if(strcmp(p -> branch_do_commit, branch_merge) == 0){
                                if(strcmp(p -> info, first) != 0){
                                    guarda_info_commit_hash(commits, p -> info, branch_now);
                                }
                            }
                        }
                    }
                    return;
                }
            }
        }
        printf("Branch '%s' nao existe!\n", branch_merge);
        return;
    }
}

void limpa_hash(hash *h){
    for(int i = 0; tam > i; i++){
        limpa((h + i) -> encad);
        limpa_commit((h + i) -> hist_commits);
    }
}

void limpa(header *h){
    encad *p = h -> primeiro;
    encad *aux;

    if(p == NULL){
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
        return;
    }

    while(p != NULL){
        aux = p -> proximo;
        free(p);
        p = aux;
    }

    free(h);
}

void imprime_commits(hash *h){
    for(int i = 0; tam > i; i++){
        for(com *p = (h + i) -> hist_commits  -> prim; p != NULL; p = p -> proximo){   
            printf("%d %s\n", p -> chave, p -> info);
            printf("From branch: %s\n", p -> branch_do_commit);
        }
    }
}

