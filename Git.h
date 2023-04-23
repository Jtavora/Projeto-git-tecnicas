#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct header{
    struct Encad *primeiro;
    struct Encad *ultimo;
    struct hist_commits *prim;
    struct hist_commits *ult;
}header;

typedef struct Encad{
    struct Encad *proximo;
    struct Encad *anterior;
    char *info;
    
}encad;

typedef struct hist_commits{
    struct hist_commits *proximo;
    struct hist_commits *anterior;
    char *info;
    char *branch_do_commit;
    
}com;

header *cria_header(void);
void limpa(header *h);
void imprime_commits(header *h, char *branch_atual);
header *separa_string(char *c);
void guarda_info(header *h, char *info);
void guarda_info_commit(header *h, char *info, char *branch_commit);
void verifica_comando(header *comando, header *branch, header *commits);
void f_commit(header *h, header *commits, char *branch_atual);
void f_branch(header *h, header *branch);
void f_checkout(header *h, header *branch);
void f_merge(header *h, char *branch_atual, char *branch_merge);
void f_log(header *h, header *commits, header *branch);


