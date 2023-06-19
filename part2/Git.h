#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define tam 101

typedef struct hash{
    struct header *encad;
    struct header *hist_commits;

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

void guarda_info_hash(hash *h, char *info);
void guarda_info_commit_hash(hash *h, char *info, char *branch_commit);
int gerarNumeroAleatorio();
int funcao_hash(void);
hash *inicia_hash(void);
header *cria_header(void);
void limpa(header *h);
void limpa_commit(header *h);
void imprime_commits(hash *h);
header *separa_string(char *c);
void guarda_info(header *h, char *info, int chave);
void guarda_info_commit(header *h, char *info, char *branch_commit, int chave);
void verifica_comando(header *comando, hash *branch, hash *commits, int *branch_atual);
void f_clear(void);
void f_commit(header *h, hash *commits, int *branch_atual, hash *branch);
void f_branch(header *h, hash *branch, hash *commits);
void f_checkout(header *h, hash *branch, int *branch_atual);
void f_merge(hash *h, int *branch_atual, char *branch_merge, hash *branch);
void f_log(header *h, hash *commits, char *branch_atual);
void limpa_hash(hash *h);


