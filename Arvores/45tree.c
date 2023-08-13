#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define tam 256

//código,tipo, marca, tamanho, quantidade e preço.
typedef struct arvore{
    int cod;
    char tipo[15];
    char marca[15];
    int tamanho;
    int quantidade;
    float preco;
    int linha; //posição das infos no arquivo
}No;


typedef struct arv{
    No *info1,*info2,*info3,*info4;
    struct arv *esq,*meioEsq,*meio,*meioDir,*dir;
    int nInfo;
}Arvore;

Arvore *criaArvore(No *info, Arvore *esq,Arvore *meioEsq, Arvore *meio, Arvore *meioDir, Arvore *dir){
    Arvore *new = malloc(sizeof(Arvore));

    new->info1 = info; // a info 1 aponta para o nó com as informações
    new->info2 = NULL;
    new->info3 = NULL;
    new->info4 = NULL;

    new->esq = esq;
    new->meioEsq = meioEsq;
    new->meio = meio;
    new->meioDir = meioDir;
    new->dir = dir;

    new->nInfo = 1;

    return new;
}

//cria um nó
No *criaNo(int cod, char *tipo, char *marca, int tamanho, int quant, float preco, int linha){
    No *info = malloc(sizeof(No));

    if(info){
        info->cod = cod;
        strcpy(info->tipo,tipo);
        strcpy(info->marca,marca);
        info->tamanho = tamanho;
        info->quantidade = quant;
        info->preco = preco;
        info->linha = linha;
    }
    return info;
}

int verificaFolha(Arvore *raiz){
    if(raiz->esq == NULL && raiz->meioEsq == NULL && raiz->meio == NULL){
        return 1;//é folha
    }else{
        return 0;//não é folha
    }
}

No *adicionaNo(Arvore **raiz, No *info, Arvore *filho){
    if((*raiz)->nInfo == 1){
        if(info->cod < (*raiz)->info1->cod){
            (*raiz)->info2 = (*raiz)->info1;
            (*raiz)->meio = (*raiz)->meioEsq;
            (*raiz)->meioEsq = filho;
            (*raiz)->info1 = info;            
        }else{
            (*raiz)->info2 = info;
            (*raiz)->meio = filho;
        }
        (*raiz)->nInfo = 2;
    }else if((*raiz)->nInfo == 2){
        if(info->cod > (*raiz)->info2->cod){
            (*raiz)->info3 = info;
            (*raiz)->meioDir = filho;
        }else if(info->cod < (*raiz)->info1->cod){
            (*raiz)->info3 = (*raiz)->info2;
            (*raiz)->meioDir = (*raiz)->meio;
            (*raiz)->meio = (*raiz)->meioEsq;
            (*raiz)->info2 = (*raiz)->info1;
            (*raiz)->meioEsq = filho;
            (*raiz)->info1 = info;
            (*raiz)->meioEsq = filho;
        }else{
            (*raiz)->info3 = (*raiz)->info2;
            (*raiz)->meioDir = (*raiz)->meio;
            (*raiz)->info2 = info;
            (*raiz)->meio = filho;
        }
        (*raiz)->nInfo = 3;
    }else if((*raiz)->nInfo == 3){
        if(info->cod > (*raiz)->info3->cod){
            (*raiz)->info4 = info;
            (*raiz)->dir = filho;
        }else if(info->cod > (*raiz)->info2->cod){
            (*raiz)->info4 = (*raiz)->info3;
            (*raiz)->dir = (*raiz)->meioDir;
            (*raiz)->info3 = info;
            (*raiz)->meioDir = filho;
        }else if(info->cod > (*raiz)->info1->cod){
            (*raiz)->info4 = (*raiz)->info3;
            (*raiz)->dir = (*raiz)->meioDir;
            (*raiz)->info3 = (*raiz)->info2;
            (*raiz)->meioDir = (*raiz)->meio;
            (*raiz)->info2 = info;
            (*raiz)->meio = filho;
        }else{ //se info < info1
            (*raiz)->info4 = (*raiz)->info3;
            (*raiz)->dir = (*raiz)->meioDir;
            (*raiz)->info3 = (*raiz)->info2;
            (*raiz)->meioDir = (*raiz)->meio;
            (*raiz)->info2 = (*raiz)->info1;
            (*raiz)->meio = (*raiz)->meioEsq;
            (*raiz)->info1 = info;
            (*raiz)->meioEsq = filho;


        }
        (*raiz)->nInfo = 4;
    }
}

Arvore *divideNo(Arvore **raiz,No *info,No **meio,Arvore *filho){
    Arvore *maior;
    if(info->cod > (*raiz)->info4->cod){
        *meio = (*raiz)->info3;
        maior = criaArvore(info,(*raiz)->meioDir,filho,NULL,NULL,NULL);
        adicionaNo(&maior,(*raiz)->info4,(*raiz)->dir);
    }else if(info->cod > (*raiz)->info3->cod && info->cod < (*raiz)->info4->cod){
        *meio = (*raiz)->info3;
        maior = criaArvore((*raiz)->info4, (*raiz)->meioDir,(*raiz)->dir,NULL,NULL,NULL);
        adicionaNo(&maior,info,filho);
    }else if(info->cod > (*raiz)->info2->cod && info->cod < (*raiz)->info3->cod){
        *meio = info;
        maior = criaArvore((*raiz)->info4, filho,(*raiz)->dir,NULL,NULL,NULL);
        adicionaNo(&maior,(*raiz)->info3,(*raiz)->meioDir);
        
    } else if (info->cod > (*raiz)->info1->cod && info->cod < (*raiz)->info2->cod){
        *meio = (*raiz)->info2;
        (*raiz)->info2 = info;
        maior = criaArvore((*raiz)->info4, (*raiz)->meio, (*raiz)->dir, NULL,NULL,NULL);
        adicionaNo(&maior,(*raiz)->info3,(*raiz)->meioDir);
        (*raiz)->meio = filho;
    }else{
        *meio = (*raiz)->info2;
        (*raiz)->info2 = (*raiz)->info1;
        //MaiorNO = CriaNo((*no)->info4, (*no)->centro, (*no)->dir, NULL, NULL, NULL);
        maior = criaArvore((*raiz)->info4,(*raiz)->meio,(*raiz)->dir,NULL,NULL,NULL);
        adicionaNo(&maior,(*raiz)->info3, (*raiz)->meioDir);
        (*raiz)->meio = (*raiz)->meioEsq;
        (*raiz)->info1 = info;
        (*raiz)->meioEsq = filho;
    }
    (*raiz)->meioDir = NULL;
    (*raiz)->dir = NULL;
    (*raiz)->nInfo = 2;
    return maior;
}

Arvore *insere(Arvore **raiz, Arvore *pai, No *info,No **meio){
    if(*raiz == NULL){
        *raiz = criaArvore(info, NULL, NULL, NULL, NULL, NULL);
        
    }else{
        Arvore *maior = NULL;
        if(verificaFolha(*raiz)){
            if((*raiz)->nInfo < 4){
                adicionaNo(raiz, info, NULL);
            } else {
                maior = divideNo(raiz, info, meio, NULL);
                if(pai == NULL){
                    *raiz = criaArvore(*meio, *raiz, maior, NULL, NULL, NULL);
                    maior =  NULL;
                }
            }
        }else{
            if(info->cod < (*raiz)->info1->cod){
                maior = insere(&(*raiz)->esq, *raiz, info,meio);
            }else if((*raiz)->nInfo == 1 || info->cod < (*raiz)->info2->cod){
                maior = insere(&(*raiz)->meioEsq, *raiz, info,meio);
            }else if((*raiz)->nInfo == 2 || info->cod < (*raiz)->info3->cod){
                maior = insere(&(*raiz)->meio, *raiz, info,meio);
            }else if((*raiz)->nInfo == 3 || info->cod < (*raiz)->info4->cod){
                maior = insere(&(*raiz)->meioDir, *raiz, info,meio);
            }else{
                maior = insere(&(*raiz)->dir, *raiz, info,meio);
            }

            if(maior != NULL){
                if((*raiz)->nInfo < 4){
                    adicionaNo(raiz, *meio, maior);
                    maior = NULL;
                } else {
                    maior = divideNo(raiz, *meio, meio, maior);
                    if(pai == NULL){
                        *raiz = criaArvore(*meio, *raiz, maior, NULL, NULL, NULL);
                        maior = NULL;
                    }
                }
            }
        }
        return maior;
    }
}

void dados(No *raiz){
    if(raiz != NULL){
        printf("-----------------------------------------------\n");
        printf("Código: %d\n",raiz->cod);
        printf("Tipo:   %s\n",raiz->tipo);
        printf("Marca:  %s\n",raiz->marca);
        printf("Tamanho: %d\n",raiz->tamanho);
        printf("Preço: %.2f\n",raiz->preco);
        printf("Quantidade %d\n",raiz->quantidade);
        printf("Linha: %d\n",raiz->linha);
        printf("-----------------------------------------------\n");
    }
}

void imprimir(Arvore *raiz, int nivel) {
    if (raiz != NULL) {
        
        for (int i = 0; i < nivel; i++) {
            printf("\t");
        }
        printf("Nivel %d: ", nivel);
        
        dados(raiz->info1);
        if (raiz->nInfo == 2) {
            dados(raiz->info2);
        }
        if (raiz->nInfo == 3) {
            dados(raiz->info2);
            dados(raiz->info3);
        }
        if (raiz->nInfo == 4) {
            dados(raiz->info2);
            dados(raiz->info3);
            dados(raiz->info4);
        }

        imprimir(raiz->esq, nivel + 1);
        imprimir(raiz->meioEsq, nivel + 1);
        imprimir(raiz->meio, nivel + 1);
        imprimir(raiz->meioDir, nivel + 1);
        imprimir(raiz->dir, nivel + 1);
    }
}
//lê o arquivo e adiciona os elementos na árvore
void addNo(Arvore **raiz, No *novo, No *meio,FILE *arq){
    char linhas[tam];
    char *item;
    int cod, tamanho, quant, linha = 1;
    float preco;
    char tipo[15], marca[15];
    if(arq == NULL){
        printf("Erro na abertura do arquivo!\n");
        exit(1);
    }
    while (fgets(linhas,sizeof(linhas),arq)){
        
        item = strtok(linhas,",");
        cod = atoi(item);

        item = strtok(NULL,",");
        strcpy(tipo,item);

        item = strtok(NULL,",");
        strcpy(marca,item);

        item = strtok(NULL,",");
        tamanho = atoi(item);

      
        item = strtok(NULL,",");
        quant = atoi(item);

        item = strtok(NULL,"\n");
        preco = atof(item);
        
        novo = criaNo(cod,tipo,marca,tamanho,quant,preco,linha);
        insere(raiz, NULL, novo,&meio);

        linha ++;

    }
}

int buscarItem(Arvore *raiz, int cod){
    if(raiz == NULL){
         return 0;
    }
    if(cod == raiz->info1->cod){
        dados(raiz->info1);
        return 1;
    }
    if(raiz->nInfo >= 2 && cod == raiz->info2->cod){
        dados(raiz->info2);
        return 1;
    }
    if(raiz->nInfo >= 3 && cod == raiz->info3->cod){
        dados(raiz->info3);
        return 1;
    }
    if(raiz->nInfo == 4 && cod == raiz->info4->cod){
        dados(raiz->info4);
        return 1;
    }
    if(cod < raiz->info1->cod)
        return buscarItem(raiz->esq, cod);
    else if(raiz->nInfo < 2 || cod < raiz->info2->cod)
        return buscarItem(raiz->meioEsq, cod);
    else if(raiz->nInfo < 3 || cod < raiz->info3->cod)
        return buscarItem(raiz->meio, cod);
    else if(raiz->nInfo < 4 || cod < raiz->info4->cod)
        return buscarItem(raiz->meioDir, cod);
    else
        return buscarItem(raiz->dir, cod);
}

//essa função apenas retorna se um valor já existe ou não
int existe(Arvore *raiz,int cod,int *contador){
    if(raiz == NULL){
        return 0;
    }else{
        (*contador) ++;
        if(cod == raiz->info1->cod){
            return 1;
        }
        if(raiz->nInfo >= 2 && cod == raiz->info2->cod){
            return 1;
        }
        if(raiz->nInfo >= 3 && cod == raiz->info3->cod){
            return 1;
        }
        if(raiz->nInfo == 4 && cod == raiz->info4->cod){
            return 1;
        }else{
            if(cod < raiz->info1->cod)
                return existe(raiz->esq, cod,contador);
            else if(raiz->nInfo < 2 || cod < raiz->info2->cod)
                return existe(raiz->meioEsq, cod,contador);
            else if(raiz->nInfo < 3 || cod < raiz->info3->cod)
                return existe(raiz->meio, cod,contador);
            else if(raiz->nInfo < 4 || cod < raiz->info4->cod)
                return existe(raiz->meioDir, cod,contador);
            else
                return existe(raiz->dir, cod,contador);

        }
        
    }
    
}

int countLines(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo: ");
        return -1;
    }

    char line[256];
    int count = 0;

    while (fgets(line, sizeof(line), file)) {
        count++;
    }

    fclose(file);
    return count;
}

//addNo(Arvore **raiz, No *novo, No *meio,FILE *arq)
int adicionarItem(Arvore *raiz,No *novo,No *meio,char *filename,int cod,char *tipo, char *marca, int tamanho, int quant, float preco){
    char linhas[tam];
    int contador = 0;
    FILE *arq;
    int nLinha = countLines(filename);
    int id = existe(raiz,cod,&contador);
    if(id != 1){//significa que o código ainda não foi registrado
        arq = fopen(filename,"a");
        if(arq != NULL){
            fprintf(arq,"\n%d,%s,%s,%d,%d,%.2f",cod,tipo,marca,tamanho,quant,preco);
            fclose(arq);
            novo = criaNo(cod,tipo,marca,tamanho,quant,preco,nLinha+1);
            insere(&raiz,NULL,novo,&meio);
            return 1;
        }else{
            return 0; //problemas ao abrir o arquiov
        } 
    }else{
        return 2; //código já existe
    }
}
//adiciona itens que já estão cadastrados
int addItemExiste(Arvore *raiz, char *filename, int quantidade, int id) {
    char linhas[tam];
    FILE *arq, *temp;
    int contador = 0;
    int idC = existe(raiz, id,&contador);

    if (idC == 1) {
        arq = fopen(filename, "r");
        if (arq == NULL) {
            printf("Erro ao abrir arquivo.\n");
            exit(1);
        }

        temp = fopen("temp.txt", "w");
        if (temp == NULL) {
            printf("Erro ao abrir arquivo.\n");
            exit(1);
        }

        char *item;
        int cod, tamanho, quant, linha = 1;
        float preco;
        char tipo[15], marca[15];

        while (fgets(linhas, sizeof(linhas), arq)) {
            char linhas_copy[tam];
            strcpy(linhas_copy, linhas);  // copiar linha para evitar alterar durante a tokenização

            item = strtok(linhas, ",");
            cod = atoi(item);

            item = strtok(NULL, ",");
            strcpy(tipo, item);

            item = strtok(NULL, ",");
            strcpy(marca, item);

            item = strtok(NULL, ",");
            tamanho = atoi(item);

            item = strtok(NULL, ",");
            quant = atoi(item);

            item = strtok(NULL, "\n");
            preco = atof(item);

            if (id == cod) {
                quant = quant + quantidade;
                fprintf(temp,"%d,%s,%s,%d,%d,%.2f\n", cod, tipo, marca, tamanho, quant, preco);
            } else {
                fprintf(temp, "%s", linhas_copy);
            }

            linha++;
        }

        fclose(arq);
        fclose(temp);

        arq = fopen(filename, "w");
        if (arq == NULL) {
            printf("Erro ao abrir arquivo.\n");
            exit(1);
        }

        temp = fopen("temp.txt", "r");
        if (temp == NULL) {
            printf("Erro ao abrir arquivo.\n");
            exit(1);
        }

        while (fgets(linhas, sizeof(linhas), temp)) {
            fprintf(arq, "%s", linhas);
        }

        fclose(arq);
        fclose(temp);

        remove("temp.txt");

        return 1;  // Retorna 1 se o item for atualizado
    }
    
    return 0;  // Retorna 0 se o item não for encontrado
}


int venderItem(Arvore *raiz,char *filename, int id,int quantidade){
    char linhas[tam];
    FILE *arq, *temp;
    int contador;
    int idC = existe(raiz, id,&contador); //verifica se o item existe no sistema
    if (idC == 1) {
        arq = fopen(filename, "r");
        if (arq == NULL) {
            printf("Erro ao abrir arquivo.\n");
            exit(1);
        }

        temp = fopen("temp.txt", "w");
        if (temp == NULL) {
            printf("Erro ao abrir arquivo.\n");
            exit(1);
        }

        char *item;
        int cod, tamanho, quant, linha = 1;
        float preco;
        char tipo[15], marca[15];

        while (fgets(linhas, sizeof(linhas), arq)) {
            char linhas_copy[tam];
            strcpy(linhas_copy, linhas);  // copiar linha para evitar alterar durante a tokenização

            item = strtok(linhas, ",");
            cod = atoi(item);

            item = strtok(NULL, ",");
            strcpy(tipo, item);

            item = strtok(NULL, ",");
            strcpy(marca, item);

            item = strtok(NULL, ",");
            tamanho = atoi(item);

            item = strtok(NULL, ",");
            quant = atoi(item);

            item = strtok(NULL, "\n");
            preco = atof(item);

            if (id == cod) {
                quant = quant - quantidade;
                fprintf(temp,"%d,%s,%s,%d,%d,%.2f\n", cod, tipo, marca, tamanho, quant, preco);
            } else {
                fprintf(temp, "%s", linhas_copy);
            }

            linha++;
        }

        fclose(arq);
        fclose(temp);

        arq = fopen(filename, "w");
        if (arq == NULL) {
            printf("Erro ao abrir arquivo.\n");
            exit(1);
        }

        temp = fopen("temp.txt", "r");
        if (temp == NULL) {
            printf("Erro ao abrir arquivo.\n");
            exit(1);
        }

        while (fgets(linhas, sizeof(linhas), temp)) {
            fprintf(arq, "%s", linhas);
        }

        fclose(arq);
        fclose(temp);

        remove("temp.txt");

        return 1;  // Retorna 1 se o item for atualizado
    }
    
    return 0;  // Retorna 0 se o item não for encontrado
}


void balanceia(Arvore **raiz, Arvore **pai){
    if(*raiz != NULL){
        if((*raiz)->nInfo == 0){
            if(*pai != NULL){
                if(*raiz == (*pai)->dir){
                    if((*pai)->meioDir->nInfo == 4){
                        (*raiz)->info1 = (*pai)->info4;
                        (*raiz)->nInfo = 1;

                        (*pai)->info2 = (*pai)->meioDir->info4;
                        (*pai)->meioDir->nInfo = 3;

                        (*raiz)->esq = (*pai)->meioDir->dir;
                        (*pai)->meioDir->dir = NULL;

                    }else if((*pai)->meioDir->nInfo = 3){
                        (*raiz)->info1 = (*pai)->info4;
                        (*raiz)->nInfo = 1;

                        (*pai)->info4 = (*raiz)->info3;
                        (*pai)->meioDir->nInfo = 2;

                        (*raiz)->esq = (*pai)->meioDir->meioDir;
                        (*pai)->meioDir->meioDir = NULL;
                    }else if((*pai)->meioDir->nInfo = 2){
                        (*raiz)->info1 = (*pai)->info4;
                        (*raiz)->nInfo = 1;

                        (*pai)->info4 = (*pai)->meioDir->info2;
                        (*pai)->meioDir->nInfo = 1;

                        (*raiz)->esq = (*pai)->meioDir->meio;
                        (*pai)->meioDir->meio = NULL;
                    }else{
                        (*raiz)->info2 = (*pai)->info4;
                        (*raiz)->meio = (*raiz)->meioEsq;

                        (*raiz)->info1 = (*pai)->meioDir->info1;

                        (*raiz)->nInfo = 2;
                        (*pai)->nInfo = 1;

                        (*raiz)->meioEsq = (*pai)->meioDir->meioEsq;
                        (*raiz)->esq = (*pai)->meioDir->esq;

                        free((*pai)->meioDir);
                        (*pai)->meioDir = *raiz;
                        (*pai)->dir = NULL;


                    }
                }else if(*raiz == (*pai)->meioDir){
                    if((*pai)->meio->nInfo == 4){
                        (*raiz)->info1 = (*pai)->info3;
                        (*raiz)->nInfo = 1;

                        (*pai)->info3 = (*pai)->meio->info4;
                        (*pai)->meio->nInfo = 3;

                        (*raiz)->esq = (*pai)->meio->dir;
                        (*pai)->meio->dir = NULL;
                    }else if((*pai)->meio->nInfo == 3){
                        (*raiz)->info1 = (*pai)->info3;
                        (*raiz)->nInfo = 1;

                        (*pai)->info3 = (*pai)->meio->info3;
                        (*pai)->meio->nInfo = 2;

                        (*raiz)->esq = (*pai)->meio->meioDir;
                        (*pai)->meio->meioDir = NULL;

                    }else if((*pai)->meio->nInfo == 2){
                        (*raiz)->info1 = (*pai)->info3;
                        (*raiz)->nInfo = 1;

                        (*pai)->info3 = (*pai)->meio->info2;
                        (*pai)->meio->nInfo = 1;

                        (*raiz)->esq = (*pai)->meio->meio;
                        (*pai)->meio->meio = NULL;
                    }else{
                        (*raiz)->info2 = (*pai)->info3;
                        (*raiz)->meio = (*raiz)->meioEsq;

                        (*raiz)->info1 = (*pai)->meio->info1;
                        (*raiz)->nInfo = 2;

                        free((*pai)->meio);

                        (*pai)->meio = *raiz;

                        (*pai)->info3 = (*pai)->info4;
                        (*pai) ->meioDir = (*pai)->dir;
                        (*pai)->dir = NULL;

                        (*pai)->nInfo = 3;
                    }
                }else if(*raiz == (*pai)->meio){
                    if((*pai)->meioEsq->nInfo == 4){
                        (*raiz)->info1 = (*pai)->info2;
                        (*raiz)->nInfo = 1;

                        (*pai)->info2 = (*pai)->meioEsq->info4;
                        (*pai)->meioEsq->nInfo = 3;

                        (*raiz)->esq = (*pai)->meioEsq->dir;
                        (*pai)->meioEsq->dir = NULL;
                    }else if((*pai)->meioEsq->nInfo == 3){
                        (*raiz)->info1 = (*pai)->info2;
                        (*raiz)->nInfo = 1;

                        (*pai)->info2 = (*pai)->meioEsq->info3;
                        (*pai)->meioEsq->nInfo = 2;

                        (*raiz)->esq = (*pai)->meioEsq->meioDir;
                        (*pai)->meioEsq->meioDir = NULL;

                    }else if((*pai)->meioEsq->nInfo == 2){
                        (*raiz)->info1 = (*pai)->info2;
                        (*raiz)->nInfo = 1;

                        (*pai)->info2 = (*pai)->meioEsq->info2;
                        (*pai)->meioEsq->nInfo = 1;

                        (*raiz)->esq = (*pai)->meioEsq->meio;
                        (*pai)->meioEsq->meio = NULL;
                    }else{
                        (*raiz)->info2 = (*pai)->info2;
                        (*raiz)->meio = (*raiz)->meioEsq;

                        (*raiz)->info1 = (*pai)->meioEsq->info1;
                        (*raiz)->meioEsq = (*pai)->meioEsq->meioEsq;
                        (*raiz)->esq = (*pai)->meioEsq->esq;

                        free((*pai)->meioEsq);

                        (*pai)->meioEsq = *raiz;
                        (*pai)->info2 = (*pai)->info3;
                        (*pai)->meio = (*pai)->meioDir;

                        (*pai)->info3 = (*pai)->info4;
                        (*pai)->meioDir = (*pai)->dir;
                        (*pai)->dir = NULL;
                        (*pai)->nInfo = 3;
                    }
                }else if(*raiz == (*pai)->meioEsq){
                    if((*pai)->esq->nInfo == 4){
                        (*raiz)->info1 = (*pai)->info1;
                        (*raiz)->nInfo = 1;

                        (*pai)->info1 = (*pai)->esq->info4;
                        (*pai)->esq->nInfo = 3;

                        (*raiz)->esq = (*pai)->esq->dir;
                        (*pai)->esq->dir = NULL;
                    }else if((*pai)->esq->nInfo == 3){
                        (*raiz)->info1 = (*pai)->info1;
                        (*raiz)->nInfo = 1;

                        (*pai)->info1 = (*pai)->esq->info3;
                        (*pai)->esq->nInfo = 2;

                        (*raiz)->esq = (*pai)->esq->meioDir;
                        (*pai)->esq->meioDir = NULL;

                    }else if((*pai)->esq->nInfo == 2){
                        (*raiz)->info1 = (*pai)->info1;
                        (*raiz)->nInfo = 1;

                        (*pai)->info1 = (*pai)->esq->info2;
                        (*pai)->esq->nInfo = 1;

                        (*raiz)->esq = (*pai)->esq->meio;
                        (*pai)->esq->meio = NULL;
                    }else{
                        (*raiz)->info2 = (*pai)->info1;
                        (*raiz)->meio = (*raiz)->meioEsq;

                        (*raiz)->info1 = (*pai)->esq->info1;
                        (*raiz)->nInfo = 2;

                        (*raiz)->meioEsq = (*pai)->esq->meioEsq;
                        (*raiz)->esq = (*pai)->esq->esq; 

                        free((*pai)->esq);

                        (*pai)->esq = *raiz;

                        (*pai)->info1 = (*pai)->info2;
                        (*pai)->meioEsq = (*pai)->meio;
                        (*pai)->info2 = (*pai)->info3;
                        (*pai)->meio = (*pai)->meioDir;
                        (*pai)->info3 = (*pai)->info4;
                        (*pai)->meioDir = (*pai)->dir;
                        (*pai)->dir = NULL;
                        (*pai)->info4 = NULL;
                        
                        (*pai)->nInfo = 3;
                    }
                }else if(*raiz == (*pai)->esq){
                    if((*pai)->meioEsq->nInfo == 4){
                        (*raiz)-> info1 = (*pai)->info1;
                        (*raiz)->nInfo = 1;

                        (*pai)->info1 = (*pai)->meioEsq->info1;

                        (*pai)->meioEsq->info1 = (*pai)->meioEsq->info2;
                        (*pai)->meioEsq->meioEsq = (*pai)->meioEsq->meio;

                        (*pai)->meioEsq->info2 = (*pai)->meioEsq->info3;
                        (*pai)->meioEsq->meio = (*pai)->meioEsq->meioDir;

                        (*pai)->meioEsq->info3 = (*pai)->meioEsq->info4;
                        (*pai)->meioEsq->meioDir = (*pai)->meioEsq->dir;
                        (*pai)->meioEsq->dir = NULL;
                        (*pai)->info4 = NULL;

                        (*pai)->meioEsq->nInfo = 3;
                    }else if((*pai)->meioEsq->nInfo == 3){
                        (*raiz)->info1 = (*pai)->info1;
                        (*raiz)->nInfo = 1;

                        (*pai)->info1 = (*pai)->meioEsq->info1;

                        (*pai)->meioEsq->info1 = (*pai)->meioEsq->info2;
                        (*pai)->meioEsq ->meioEsq = (*pai)->meioEsq->meio;

                        (*pai)->meioEsq->info2 = (*pai)->meioEsq->info3;
                        (*pai)->meioEsq->meio = (*pai)->meioEsq->meioDir;

                        (*pai)->meioEsq->meioDir = NULL;
                        (*pai)->meioEsq->info3 = NULL;

                        (*pai)->meioEsq->nInfo = 3;

                    }else if((*pai)->meioEsq->nInfo == 2){
                        (*raiz)->info1 = (*pai)->info1;
                        (*raiz)->nInfo = 1;

                        (*pai)->info1 = (*pai)->meioEsq->info1;
                        (*pai)->meioEsq->info1 = (*pai)->meioEsq->info2;
                        (*pai)->meioEsq->meioEsq = (*pai)->meioEsq->meio;
                        (*pai)->meioEsq->meio = NULL;
                        (*pai)->info2 = NULL;

                        (*pai)->meioEsq->nInfo = 1;


                    }else if((*pai)->meioEsq->nInfo == 1){
                        if((*pai)->nInfo == 4){
                            (*raiz)->info1 = (*pai)->info1;

                            (*raiz)->info2 = (*pai)->meioEsq->info1;

                            (*raiz)->nInfo = 2;

                            (*raiz)->esq = (*raiz)->meio;
                            (*raiz)->meioEsq = (*pai)->meioEsq->esq;
                            (*raiz)->meio = (*pai)->meioEsq->meioEsq;

                            free((*pai)->meioEsq);

                            (*pai)->info1 = (*pai)->info2;
                            (*pai)->meioEsq = (*pai)->meio;

                            (*pai)->info2 = (*pai)->info3;
                            (*pai)->meio = (*pai)->meioDir;

                            (*pai)->info3 = (*pai)->info4;
                            (*pai)->meioDir = (*pai)->dir;

                            (*pai)->dir = NULL;
                            (*pai)->info4 = NULL;
  
                        }else if((*pai)->nInfo == 3){
                            (*raiz)->info1 = (*pai)->info1;

                            (*raiz)->info2 = (*pai)->meioEsq->info1;
                            (*raiz)->nInfo = 2;
                            
                            (*raiz)->esq = (*raiz)->meio;
                            (*raiz)->meioEsq = (*pai)->meioEsq->esq;
                            (*raiz)->meio = (*pai)->meioEsq->meioEsq;

                            free((*pai)->meioEsq);

                            (*pai)->info1 = (*pai)->info2;
                            (*pai)->meioEsq = (*pai)->meio;

                            (*pai)->info2 = (*pai)->info3;
                            (*pai)->meio = (*pai)->meioDir;

                            (*pai)->meioDir = NULL;
                            (*pai)->info3 = NULL;

                        }else if((*pai)->nInfo == 2){
                            (*raiz)->info1 = (*raiz)->info1;

                            (*raiz)->info2 = (*pai)->meioEsq->info1;
                            (*raiz)->nInfo = 2;

                            (*raiz)->esq = (*raiz)->meio;
                            (*raiz)->meioEsq = (*pai)->meioEsq->esq;
                            (*raiz)->meio = (*pai)->meioEsq->meioEsq;

                            free((*pai)->meioEsq);

                            (*pai)->info1 = (*pai)->info2;
                            (*pai)->meioEsq = (*pai)->meio;

                            (*pai)->meio = NULL;
                            (*pai)->info2 =NULL;
                        }else if((*pai)->nInfo == 1){
                            (*raiz)->info1 = (*pai)->info1;

                            (*raiz)->info2 = (*pai)->meioEsq->info1;
                            (*raiz)->nInfo = 2;

                            (*raiz)->esq = (*raiz)->meio;
                            (*raiz)->meioEsq = (*pai)->meioEsq->esq;
                            (*raiz)->meio = (*pai)->meioEsq->meioEsq;

                            (*pai)->nInfo = 0;
                            (*raiz)->nInfo = 2;

                            free((*pai)->meioEsq);
                            (*pai)->meio = (*raiz);
                            (*pai)->esq = NULL;
                        }
                    }
                }
            }else if(*pai == NULL){
                  
                *raiz = (*raiz)->meioEsq;
            }
        }
    }
}



void maiorFilhoEsq(Arvore **raiz, Arvore **maiorFilho, Arvore **paiM, int pos){
    No *aux;
    if(maiorFilho != NULL){
        if((*maiorFilho)->esq == NULL){ //significa que é folha
            if(pos == 1){
                aux = (*raiz)->info1;
                if((*maiorFilho)->nInfo == 4){
                    (*raiz)->info1 = (*maiorFilho)->info4;

                    (*maiorFilho)->info4 = aux;

                }else if((*maiorFilho)->nInfo == 3){
                    (*raiz)->info1 = (*maiorFilho)->info3;
                    (*maiorFilho)->info3 = aux;
                }else if((*maiorFilho)->nInfo == 2){
                    (*raiz)->info1 = (*maiorFilho)->info2;

                    (*maiorFilho)->info2 = aux;
                }else{
                    (*raiz)->info1 = (*maiorFilho)->info1;

                }
            }else if(pos == 2){
                aux = (*raiz)->info2;

                if((*maiorFilho)->nInfo == 4){
                    (*raiz)->info2 = (*maiorFilho)->info4;

                    (*maiorFilho)->info4 = aux;
                }else if((*maiorFilho)->nInfo == 3){
                    (*raiz)->info2 = (*maiorFilho)->info3;

                    (*maiorFilho)->info3 = aux;
                }else if((*maiorFilho)->nInfo == 2){
                    (*raiz)->info2 = (*maiorFilho)->info2;

                    (*maiorFilho)->info2 = aux;
                }else{
                    (*raiz)->info2 = (*maiorFilho)->info1;

                    (*maiorFilho)->info1 = aux;
                }
            }else if(pos == 3){
                aux = (*raiz)->info3;

                if((*maiorFilho)->nInfo == 4){
                    (*raiz)->info3 = (*maiorFilho)->info4;

                    (*maiorFilho)->info4 = aux;
                }else if((*maiorFilho)->nInfo == 3){
                    (*raiz)->info3 = (*maiorFilho)->info3;

                    (*maiorFilho)->info3 = aux;
                }else if((*maiorFilho)->nInfo == 2){
                    (*raiz)->info3 = (*maiorFilho)->info2;

                    (*maiorFilho)->info2 = aux;
                }else{
                    (*raiz)->info3 = (*maiorFilho)->info1;

                    (*maiorFilho)->info1 = aux;
                }
            }else{
                aux = (*raiz)->info4;

                if((*maiorFilho)->nInfo == 4){
                    (*raiz)->info4 = (*maiorFilho)->info4;

                    (*maiorFilho)->info4 = aux;
                }else if((*maiorFilho)->nInfo == 3){
                    (*raiz)->info4 = (*maiorFilho)->info3;

                    (*maiorFilho)->info3 = aux;
                }else if((*maiorFilho)->nInfo == 2){
                    (*raiz)->info4 = (*maiorFilho)->info2;

                    (*maiorFilho)->info2 = aux;
                }else{
                    (*raiz)->info4 = (*maiorFilho)->info1;

                    (*maiorFilho)->info1 = aux;
                }
            }
            removerArvore(maiorFilho,paiM,aux->cod);
        }else{
            if((*maiorFilho)->nInfo == 4){
                maiorFilhoEsq(raiz,&(*maiorFilho)->dir,paiM,pos);
            }else if((*maiorFilho)->nInfo == 3){
                maiorFilhoEsq(raiz,&(*maiorFilho)->meioDir,paiM,pos);
            }else if((*maiorFilho)->nInfo == 2){
                maiorFilhoEsq(raiz,&(*maiorFilho)->meio,paiM,pos);
            }else if((*maiorFilho)->nInfo == 1){
                maiorFilhoEsq(raiz,&(*maiorFilho)->meioEsq,paiM,pos);
            }
        }
    }
    balanceia(maiorFilho,paiM);
}

void removerArvore(Arvore **raiz, Arvore **pai,int cod){
    if(*raiz != NULL){
        if(cod == (*raiz)->info1->cod){
            if(verificaFolha(*raiz) == 1){ //é folha
                if((*raiz)->nInfo == 4){
                    (*raiz)->info1 = (*raiz)->info2;
                    (*raiz)->meioEsq = (*raiz)->meio;
                    (*raiz)->info2 = (*raiz)->info3;
                    (*raiz)->meio = (*raiz)->meioDir;
                    (*raiz)->info3 = (*raiz)->info4;
                    (*raiz)->meioDir = (*raiz)->dir;
                    (*raiz)->dir = NULL;
                    (*raiz)->nInfo = 3;
                }else if((*raiz)->nInfo == 3){
                    (*raiz)->info1 = (*raiz)->info2;
                    (*raiz)->meioEsq = (*raiz)->meio;
                    (*raiz)->info2 = (*raiz)->info3;
                    (*raiz)->meio = (*raiz)->meioDir;
                    (*raiz)->meioDir = NULL;
                    (*raiz)->info3 = NULL;
                    (*raiz)->nInfo = 2;
                }else if((*raiz)->nInfo == 2){
                    (*raiz)->info1 = (*raiz)->info2;
                    (*raiz)->meioEsq = (*raiz)->meio;
                    (*raiz)->meio = NULL;
                    (*raiz)->nInfo = 1;
                }else{
                    (*raiz)->nInfo = 0;
                    (*raiz)->esq = NULL;
                    (*raiz)->meioEsq = NULL;
                }
            }else{
                Arvore **maiorF = &(*raiz)->esq; //passa a esquerda da raiz
                Arvore **paiM = raiz;
                maiorFilhoEsq(raiz,maiorF,paiM, 1);
            }
        }else{
            if(cod < (*raiz)->info1->cod){
                removerArvore(&(*raiz)->esq,raiz,cod);

            }else if((*raiz)->nInfo > 1 && cod == (*raiz)->info2->cod){
                if(verificaFolha(*raiz) == 1){
                    if((*raiz)->nInfo == 4){
                        (*raiz)->info2 = (*raiz)->info3;
                        (*raiz)->meio = (*raiz)->meioDir;

                        (*raiz)->info3 = (*raiz)->info4;
                        (*raiz)->meioDir = (*raiz)->dir;
                        (*raiz)->dir = NULL;
                        (*raiz)->info4 = NULL;

                        (*raiz)->nInfo = 3;
                    }else if((*raiz)->nInfo == 3){
                        (*raiz)->info2 = (*raiz)->info3;
                        (*raiz)->meio = (*raiz)->meioDir;

                        (*raiz)->meioDir = NULL;
                        (*raiz)->info3 = NULL;

                        (*raiz)->nInfo = 2;

                    }else if((*raiz)->nInfo == 2){
                        (*raiz)->nInfo = 1;
                        (*raiz)->meio = NULL;
                    }
                }else{
                    Arvore **maiorF = &(*raiz)->meioEsq; //passa a esquerda da raiz
                    Arvore **paiM = raiz;
                    maiorFilhoEsq(raiz,maiorF,paiM, 2);
                }
            }else if((*raiz)->nInfo == 1 || (*raiz)->nInfo > 1 && cod < (*raiz)->info2->cod){
                printf("Entrpu aqui1\n");
                removerArvore(&(*raiz)->meioEsq,raiz,cod);
            }else if((*raiz)->nInfo > 2 && cod == (*raiz)->info3->cod){
                if(verificaFolha(*raiz) == 1){
                    if((*raiz)->nInfo == 4){
                        (*raiz)->info3 = (*raiz)->info4;
                        (*raiz)->meioDir = (*raiz)->dir;

                        (*raiz)->dir = NULL;
                        (*raiz)->info4 = NULL;
                        (*raiz)->nInfo = 3;
                    }else if((*raiz)->nInfo == 3){
                        (*raiz)->nInfo = 2;
                        (*raiz)->info3 = NULL;
                    }
                }else{
                    Arvore **maiorF = &(*raiz)->meio; //passa a esquerda da raiz
                    Arvore **paiM = raiz;
                    maiorFilhoEsq(raiz,maiorF,paiM, 3);
                }
            }
            else if( (*raiz)->info3 && cod < (*raiz)->info3->cod){ //erro de segmentação aqui
                printf("Entrpu aqui2\n");
                removerArvore(&(*raiz)->meio,raiz,cod);

            }else if((*raiz)->nInfo > 3 && cod == (*raiz)->info4->cod){
                if(verificaFolha(*raiz)== 1){
                    (*raiz)->meioDir = (*raiz)->dir;
                    (*raiz)->info4 = NULL;
                    (*raiz)->nInfo = 3;
                }else{
                    Arvore **maiorF = &(*raiz)->meioDir; //passa a esquerda da raiz
                    Arvore **paiM = raiz;
                    maiorFilhoEsq(raiz,maiorF,paiM, 4);
                }
            }else{
                removerArvore(&(*raiz)->dir,raiz,cod);
            }
        }
    }
    balanceia(raiz,pai);
}
//void removerArvore(Arvore **raiz, Arvore **pai,int cod)
void verificaRemocao(Arvore **raiz,Arvore **pai,int cod,int *flag){
    if(*raiz != NULL){
        if(cod == (*raiz)->info1->cod){
            if((*raiz)->info1->quantidade <= 0){
                removerArvore(raiz,pai,cod);
                *flag = 1;
            }
        }else if((*raiz)->info2 && cod == (*raiz)->info2->cod){
            if((*raiz)->info2->quantidade <= 0){
                removerArvore(raiz,pai,cod);
                *flag = 1;
            }
        }else if((*raiz)->info3 && cod == (*raiz)->info3->cod){
            if((*raiz)->info3->quantidade <= 0){
                removerArvore(raiz,pai,cod);
                *flag = 1;
            }
        }else if((*raiz)->info4 && cod == (*raiz)->info4->cod){
            if((*raiz)->info4->quantidade <= 0){
                removerArvore(raiz,pai,cod);
                *flag = 1;
            }
        }else{
            if(cod < (*raiz)->info1->cod){
                verificaRemocao(&(*raiz)->esq,pai,cod,flag);
            }else if((*raiz)->nInfo < 2 || cod < (*raiz)->info2->cod){
                verificaRemocao(&(*raiz)->meioEsq,pai,cod,flag);
            }else if((*raiz)->nInfo < 3 || cod < (*raiz)->info3->cod){
                verificaRemocao(&(*raiz)->meio,pai,cod,flag);
            }else if((*raiz)->nInfo < 4 || cod < (*raiz)->info4->cod){
                verificaRemocao(&(*raiz)->meioDir,pai,cod,flag);
            }else{
                verificaRemocao(&(*raiz)->dir,pai,cod,flag);
            }
        }
    }
}

int menu(){
    int op;
    printf("1 - Ver todos os calçados.\n");
    printf("2 - Buscar calçado especifico.\n");
    printf("3 - Adicionar calçados.\n");
    printf("4 - Vender calçados.\n");
    printf("0 - Sair.\n");
    printf("Selecione a opção desejada: ");
    scanf("%d",&op);
    return op;
}

int main(){
    Arvore *raiz = NULL;
    No *novo = NULL;
    No *meio = NULL;
    int opc,flag;
    char filename[] = "calcados.txt";
    FILE *arq;

    arq = fopen(filename,"r");
    addNo(&raiz,novo,meio,arq);
    fclose(arq);

    while ((opc = menu()) != 0)
    {
        switch (opc)
        {
        case 1:
            imprimir(raiz,0);
            break;
        case 2:
            flag = buscarItem(raiz,77);
            if(flag != 1){
                printf("-----------------------------------------------\n");
                printf("Este item não conta nos registro do sistema.\n");
                printf("-----------------------------------------------\n");
            }
            break;
        case 3:

            int esc;
            printf("1 - Adicionar Item já cadastrado.\n2 - Adicionar novo Item.\n Escolha a opção desejada: ");
            scanf("%d",&esc);
            if(esc == 1){
                flag =  addItemExiste(raiz,filename,2,23);
                if(flag == 1){
                    printf("-----------------------------------------------\n");
                    printf("Novo item inserido com sucesso.\n");
                    printf("-----------------------------------------------\n");
                    //atualiza a arvore
                    raiz = NULL;

                    arq = fopen(filename,"r");
                    addNo(&raiz,novo,meio,arq);
                    fclose(arq);

                }else{
                    printf("-----------------------------------------------\n");
                    printf("Erro ao registrar novo item. Tente novamente.\n");
                    printf("-----------------------------------------------\n");
                }
            }else if(esc == 2){
                flag = adicionarItem(raiz,novo,meio,filename,21,"tenis","zara",32,1,90);
                if(flag == 1){
                    printf("-----------------------------------------------\n");
                    printf("Novo item inserido com sucesso.\n");
                    printf("-----------------------------------------------\n");
                }else{
                    printf("-----------------------------------------------\n");
                    printf("Erro ao registrar novo item. Tente novamente.\n");
                    printf("-----------------------------------------------\n");
                }
            }else{
                    printf("-----------------------------------------------\n");
                    printf("Opção Indisponivel.\n");
                    printf("-----------------------------------------------\n");
            }
            break;
        case 4:
            Arvore *pai;
            pai = NULL;
            int i = 0;
            flag = venderItem(raiz,filename,23,2);
            raiz = NULL;
            arq = fopen(filename,"r");
            addNo(&raiz,novo,meio,arq);
            fclose(arq);
            if(flag == 1){
                verificaRemocao(&raiz,&pai,23,&i);
                printf("-----------------------------------------------\n");
                printf("Novo item vendido com sucesso.\n");
                printf("-----------------------------------------------\n");
                if(i == 1){
                    printf("-----------------------------------------------\n");
                    printf("Item fora de estoque.\n");
                    printf("-----------------------------------------------\n");
                }
            }else{
                printf("-----------------------------------------------\n");
                printf("Erro ao vender item. Tente novamente.\n");
                printf("-----------------------------------------------\n");
            }
            break;
        case 5:
            int contador = 0;
            clock_t inicio, fim;
            double tempo,tempoT = 0,media = 0;
            int x;
            int cont[30] = {10,11,23,100,9,12,3,1,91,2,13,7,941,623,67,19,94,16,42,33,721,31,93,60,81,161,27,22,437,301};

            for(x = 0; x<30;x++){
                inicio = clock();
                flag = existe(raiz,cont[x],&contador);
                fim = clock();

                tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
                tempoT = tempoT + tempo;
                media = tempoT/30;
                printf("%d\t   %d   %.6f\n",cont[x],contador,tempo);
                contador = 0;
            }
            printf("media %.6f",media);
            break;
        default:
            break;
        }
    }
    
    
    

    return 0;
}
