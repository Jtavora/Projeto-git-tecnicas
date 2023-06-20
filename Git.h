#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define tam 101

typedef struct hash{
    header *encad;
    header *hist_commits;

}hash;

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
    int chave;
    
}encad;

typedef struct hist_commits{
    struct hist_commits *proximo;
    struct hist_commits *anterior;
    char *info;
    char *branch_do_commit;
    int chave;
    
}com;

int gerarNumeroAleatorio();
int funcao_hash(void);
hash *inicia_hash(void);
header *cria_header(void);
void limpa(header *h);
void limpa_commit(header *h);
void imprime_commits(header *h, char *branch_atual);
header *separa_string(char *c);
void guarda_info(header *h, char *info);
void guarda_info_commit(header *h, char *info, char *branch_commit);
void verifica_comando(header *comando, header *branch, header *commits);
void f_clear(void);
void f_commit(header *h, header *commits, char *branch_atual);
void f_branch(header *h, header *branch, header *commits);
void f_checkout(header *h, header *branch);
void f_merge(header *h, char *branch_atual, char *branch_merge, header *branch);
void f_log(header *h, header *commits, header *branch);


