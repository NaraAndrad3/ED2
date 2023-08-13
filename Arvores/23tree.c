#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define tam 256

typedef struct linha
{
    int num;
    struct linha *prox;
} Linha;

typedef struct arvore
{
    int nInfo; // número de informações
    char info1[50], info2[50];
    struct arvore *esq, *meio, *dir;
    Linha *linhaEsq, *linhaDir;
} No;

Linha *criaNoLinha(int linha)
{
    Linha *new = malloc(sizeof(Linha));
    new->num = linha;
    new->prox = NULL;
    return new;
}

Linha *insereLinha(Linha *raiz, int linha)
{
    if (raiz == NULL)
    {
        raiz = criaNoLinha(linha);
    }
    else
    {
        raiz->prox = insereLinha(raiz->prox, linha);
    }
    return raiz;
}

No *criaNo(char *word, No *esq, No *meio)
{
    No *new = malloc(sizeof(No));
    strcpy(new->info1, word);
    new->linhaEsq = NULL;

    new->linhaDir = NULL;
    new->esq = esq;
    new->meio = meio;
    new->dir = NULL;
    new->nInfo = 1;

    return new;
}

int verificaFolha(No *raiz)
{
    if (raiz->esq == NULL && raiz->meio == NULL && raiz->dir == NULL)
    {
        return 1; // é folha
    }
    else
    {
        return 0; // não é folha
    }
}
// só cai aqui se tiver apenas uma informação
int adicionaNo(No **raiz, No *novo, char *word, int linha)
{
    int cmp;
    cmp = strcasecmp(word, (*raiz)->info1);
    if (cmp > 0)
    { // word é maior
        strcpy((*raiz)->info2, word);
        (*raiz)->linhaDir = insereLinha((*raiz)->linhaDir, linha);
        (*raiz)->dir = novo;
        (*raiz)->nInfo = 2;
        return 1;
    }
    else
    { // cai aqui se word é menor
        strcpy((*raiz)->info2, (*raiz)->info1);
        (*raiz)->linhaDir = (*raiz)->linhaEsq;
        (*raiz)->linhaEsq = NULL;

        strcpy((*raiz)->info1, word);
        (*raiz)->linhaEsq = insereLinha((*raiz)->linhaEsq, linha);

        (*raiz)->dir = (*raiz)->meio;
        (*raiz)->meio = novo;
        (*raiz)->nInfo = 2;
        return -1;
    }
}

No *divideNo(No **raiz, No *novo, char *word, char *meio, int linha, Linha **aux)
{
    No *novoNo = NULL;
    int cmp, comp;
    cmp = strcasecmp(word, (*raiz)->info1); // verifica qual é maior
    comp = strcasecmp(word, (*raiz)->info2);
    if (comp > 0)
    { // info2 é o meio
        strcpy(meio, (*raiz)->info2);
        *aux = (*raiz)->linhaDir;
        (*raiz)->linhaDir = NULL;
        novoNo = criaNo(word, (*raiz)->dir, novo);
        novoNo->linhaEsq = insereLinha(novoNo->linhaEsq, linha); //
    }
    else if (cmp > 0 && comp < 0)
    { // word é o meio
        strcpy(meio, word);
        *aux = insereLinha(*aux, linha);

        novoNo = criaNo((*raiz)->info2, novo, (*raiz)->dir);
        novoNo->linhaEsq = (*raiz)->linhaDir;
        (*raiz)->linhaDir = NULL;
    }
    else
    { // info1 é o meio
        strcpy(meio, (*raiz)->info1);
        *aux = (*raiz)->linhaEsq;
        (*raiz)->linhaEsq = NULL;

        novoNo = criaNo((*raiz)->info2, (*raiz)->meio, (*raiz)->dir);
        novoNo->linhaEsq = (*raiz)->linhaDir;
        (*raiz)->linhaDir = NULL;

        strcpy((*raiz)->info1, word);
        (*raiz)->linhaEsq = insereLinha((*raiz)->linhaEsq, linha);
        (*raiz)->meio = novo;
    }
    (*raiz)->dir = NULL;
    (*raiz)->nInfo = 1;
    return novoNo;
}

No *insere(No *pai, No **raiz, char *word, char *meio, int linha, Linha **aux)
{
    No *novo;
    novo = NULL;
    // Linha *aux;
    // aux = NULL;

    if (*raiz == NULL)
    {
        *raiz = criaNo(word, NULL, NULL);
        (*raiz)->linhaEsq = insereLinha((*raiz)->linhaEsq, linha);
    }
    else
    {
        int ret;
        if (verificaFolha(*raiz) == 1)
        { // se a raiz é uma folha
            if ((*raiz)->nInfo == 1)
            {
                int cmp = strcasecmp(word, (*raiz)->info1);
                if (cmp == 0)
                {
                    (*raiz)->linhaEsq = insereLinha((*raiz)->linhaEsq, linha);
                }
                else
                    ret = adicionaNo(raiz, NULL, word, linha);
            }
            else
            {
                int c1 = strcasecmp(word, (*raiz)->info1);
                int c2 = strcasecmp(word, (*raiz)->info2);
                if (c1 == 0)
                {
                    (*raiz)->linhaEsq = insereLinha((*raiz)->linhaEsq, linha);
                }
                else if (c2 == 0)
                {
                    (*raiz)->linhaDir = insereLinha((*raiz)->linhaDir, linha);
                }
                else
                {
                    novo = divideNo(raiz, NULL, word, meio, linha, aux);
                    if (pai == NULL)
                    {
                        *raiz = criaNo(meio, *raiz, novo);
                        (*raiz)->linhaEsq = *aux;
                        novo = NULL;
                    }
                }
            }
        }
        else
        { // a raiz tem 2 elementos
            int cmp = strcasecmp(word, (*raiz)->info1);
            int comp = strcasecmp(word, (*raiz)->info2);
            if (cmp == 0)
            { // teste
                (*raiz)->linhaEsq = insereLinha((*raiz)->linhaEsq, linha);
            }
            else if (comp == 0)
            { // teste
                (*raiz)->linhaDir = insereLinha((*raiz)->linhaDir, linha);
            }
            else if (cmp < 0)
            {
                novo = insere(*raiz, &((*raiz)->esq), word, meio, linha, aux);
            }
            else if ((*raiz)->nInfo == 1 || (*raiz)->nInfo == 2 && comp < 0)
            {
                novo = insere(*raiz, &((*raiz)->meio), word, meio, linha, aux);
            }
            else
            {
                novo = insere(*raiz, &((*raiz)->dir), word, meio, linha, aux);
            }

            if (novo != NULL)
            {
                int ret;
                if ((*raiz)->nInfo == 1)
                {
                    int cmp = strcasecmp(word, (*raiz)->info1);
                    if (cmp == 0)
                    {
                        (*raiz)->linhaEsq = insereLinha((*raiz)->linhaEsq, linha);
                    }
                    else
                    {
                        ret = adicionaNo(raiz, novo, meio, linha);
                        if (ret == 1)
                        {
                            (*raiz)->linhaDir = *aux;
                        }
                        else
                        {
                            (*raiz)->linhaEsq = *aux;
                        }
                    }
                    novo = NULL;
                }
                else
                {
                    int c1 = strcasecmp(word, (*raiz)->info1);
                    int c2 = strcasecmp(word, (*raiz)->info2);
                    if (c1 == 0)
                    {
                        (*raiz)->linhaEsq = insereLinha((*raiz)->linhaEsq, linha);
                    }
                    else if (c2 == 0)
                    {
                        (*raiz)->linhaDir = insereLinha((*raiz)->linhaDir, linha);
                    }
                    else
                    {
                        novo = divideNo(raiz, novo, meio, meio, linha, aux);
                        if (pai == NULL)
                        {
                            *raiz = criaNo(meio, *raiz, novo);
                            (*raiz)->linhaEsq = *aux;
                            novo = NULL;
                        }
                    }
                }
            }
        }
    }
    return novo;
}

void imprimeLinhas(Linha *raiz)
{
    if (raiz != NULL)
    {
        printf("%d ", raiz->num);
        imprimeLinhas(raiz->prox);
    }
}

void imprime(No *raiz)
{
    if (raiz != NULL)
    {
        imprime(raiz->esq);
        printf("Info 1: %s  ---> Ocorrências: ", raiz->info1);
        imprimeLinhas(raiz->linhaEsq);
        printf("\n");
        if (raiz->nInfo == 2)
        {
            printf("Info 2: %s  ---> Ocorrências: ", raiz->info2);
            imprimeLinhas(raiz->linhaDir);
            printf("\n");
        }

        imprime(raiz->meio);
        imprime(raiz->dir);
    }
}

void addNo(No **raiz, FILE *arq, int linha, char *meio, Linha **aux)
{
    char linhas[tam];
    if (fgets(linhas, sizeof(linhas), arq))
    {
        char *word = strtok(linhas, ",");
        while (word != NULL)
        {
            size_t len = strlen(word);
            if (len > 0 && word[len - 1] == '\n')
            {
                word[len - 1] = '\0'; // Substitui o '\n' por '\0'
            }
            insere(NULL, raiz, word, meio, linha, aux);
            word = strtok(NULL, ",");
        }
        addNo(raiz, arq, linha + 1, meio, aux);
    }
}

void buscarPalavra(No *raiz, char *word, int *flag, int *contador)
{
    if (raiz != NULL)
    {
        (*contador)++;
        if (strcasecmp(word, raiz->info1) == 0)
        {
            *flag = 1;
            //return; -----> utilizado nos testes
            printf("-------------------------------------------------------\n");
            printf("Palavra: %s ---> Ocorrências: ", raiz->info1);
            imprimeLinhas(raiz->linhaEsq);
            printf("\n-------------------------------------------------------\n");
        }
        else if (raiz->nInfo == 2 && strcasecmp(word, raiz->info2) == 0)
        {
            *flag = 1;
            //return;
            printf("-------------------------------------------------------\n");
            printf("Palavra: %s ---> Ocorrências: ", raiz->info2);
            imprimeLinhas(raiz->linhaDir);
            printf("\n-------------------------------------------------------\n");
        }
        else
        {
            if (strcasecmp(word, raiz->info1) < 0)
            {
                buscarPalavra(raiz->esq, word, flag, contador);
            }
            else if (raiz->nInfo == 1 || (raiz->nInfo == 2 && strcasecmp(word, raiz->info2) < 0))
            {
                buscarPalavra(raiz->meio, word, flag, contador);
            }
            else
            {
                buscarPalavra(raiz->dir, word, flag, contador);
            }
        }
    }
}

void inserePalavra(char *filename, int line, char *palavra)
{
    FILE *arq;
    FILE *temp;
    char linha[256];
    int linha_atual = 1;

    arq = fopen(filename, "r"); // abre para leitura
    if (arq == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
    }

    // arquivo temporário onde os dados serão escritos
    temp = fopen("temp.txt", "w");
    if (temp == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
    }
    // Percorrer as linhas do arquivo original
    while (fscanf(arq, "%[^\n]%*c", linha) != EOF)
    {
        if (linha_atual == line)
        {
            // Adicionar a nova palavra na linha desejada
            strcat(linha, ",");
            strcat(linha, palavra);
        }

        // Escrever a linha no arquivo temporário
        fprintf(temp, "%s\n", linha);
        linha_atual++;
    }

    // Fechar os arquivos
    fclose(arq);
    fclose(temp);

    arq = fopen(filename, "w");
    if (arq == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
    }

    temp = fopen("temp.txt", "r");
    if (temp == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
    }

    while (fgets(linha, sizeof(linha), temp) != NULL)
    {
        fputs(linha, arq);
    }

    fclose(arq);
    fclose(temp);

    remove("temp.txt");
}

int removerPalavra(char *filename, int linha, char *word)
{
    FILE *arq, *temp;
    int flag = 0;
    arq = fopen(filename, "r");
    if (arq == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    temp = fopen("temp.txt", "w");
    if (temp == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        fclose(arq);
        exit(1);
    }

    char tamLinha[tam];
    int line = 1;

    while (fgets(tamLinha, sizeof(tamLinha), arq) != NULL)
    {
        if (line == linha)
        {
            char linhaModificada[tam];
            strcpy(linhaModificada, tamLinha); // copia a linha original para o linha modificada
            /*a linha modificada será lida paravra por palavra, até encontrar a palavra buscada,cada palavra
            diferente da buscada é concatenada a linhafinal, que será a linha escrita no arquivo temp após a remoção*/
            char *palavra = strtok(linhaModificada, " \n\t;.,:");
            char linhaFinal[tam] = "";
            int firstWord = 1;
            // int removeFlag = 0;  // Flag para indicar se uma palavra foi removida

            while (palavra != NULL)
            {
                if (strcasecmp(word, palavra) != 0)
                {
                    if (firstWord)
                    {
                        strcat(linhaFinal, palavra);
                        firstWord = 0;
                    }
                    else
                    {
                        strcat(linhaFinal, ",");
                        strcat(linhaFinal, palavra);
                    }
                }
                else
                {
                    flag = 1;
                }
                palavra = strtok(NULL, " \n\t;.,:");
            }

            fprintf(temp, "%s\n", linhaFinal);
        }
        else
        {
            fprintf(temp, "%s", tamLinha);
        }
        line++;
    }

    fclose(arq);
    fclose(temp);

    arq = fopen(filename, "w");
    if (arq == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
    }
    temp = fopen("temp.txt", "r");
    if (temp == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
    }

    while (fgets(tamLinha, sizeof(tamLinha), temp) != NULL)
    {
        fputs(tamLinha, arq);
    }

    fclose(arq);
    fclose(temp);

    remove("temp.txt");

    return flag;
}

int existe(No *raiz, char *word)
{
    if (strcasecmp(word, raiz->info1) == 0)
    {
        return 1;
    }
    if (strcasecmp(word, raiz->info2) == 0)
    {
        return 2;
    }
    return 0;
}

void balanceia(No **raiz, No **pai)
{

    if (*raiz != NULL)
    {
        printf("entra aqui no balanceamento?\n");
        if ((*raiz)->nInfo == 0)
        { // se esse nó está vazio
            if (*pai != NULL)
            {
                if (*raiz == (*pai)->dir)
                { // se eu estou a direita do pai
                    if ((*pai)->meio->nInfo == 2)
                    {
                        /*info2 do pai desce para o nó vazio,*/
                        strcpy((*raiz)->info1, (*pai)->info2);
                        (*raiz)->linhaEsq = (*pai)->linhaDir;
                        (*pai)->linhaDir = NULL;
                        (*raiz)->nInfo = 1;
                        /* info2 do meio sobe para a info2 do pai*/
                        strcpy((*pai)->info2, (*pai)->meio->info2);
                        (*pai)->linhaDir = (*pai)->meio->linhaDir;
                        (*pai)->meio->nInfo = 1;

                        (*raiz)->esq = (*pai)->meio->dir;
                        (*pai)->meio->dir = NULL;
                    }
                    else if ((*pai)->meio->nInfo == 1)
                    {
                       //       |100|   |
                      // {|50|60}|  {|150|200 |}
                        strcpy((*raiz)->info2, (*pai)->info2); // desce a info2 do pai
                        (*raiz)->linhaDir = (*pai)->linhaDir;
                        (*raiz)->dir = (*raiz)->meio;

                        // a info do centro vai para a raiz(dir)
                        strcpy((*raiz)->info1, (*pai)->meio->info1);
                        (*raiz)->linhaEsq = (*pai)->meio->linhaEsq;

                        (*raiz)->nInfo = 2;
                        (*pai)->nInfo = 1;

                        (*raiz)->meio = (*pai)->meio->meio;
                        (*raiz)->esq = (*pai)->meio->esq;
                        free((*pai)->meio);

                        (*pai)->meio = *raiz;
                        (*pai)->dir = NULL;
                    }
                }
                else if ((*raiz) == (*pai)->meio)
                {

                    if ((*pai)->esq->nInfo == 2)
                    {
                        // a info1 do pai desce para a raiz
                        strcpy((*raiz)->info1, (*pai)->info1);
                        (*raiz)->linhaEsq = (*pai)->linhaEsq;
                        (*raiz)->nInfo = 1;
                        strcpy((*pai)->info1, (*pai)->esq->info2);
                        (*pai)->linhaEsq = (*pai)->esq->linhaDir;
                        (*pai)->esq->linhaDir = NULL;

                        (*pai)->esq->nInfo = 1;

                        (*raiz)->esq = (*pai)->esq->dir;
                        (*pai)->esq->dir = NULL;
                    }
                    else if ((*pai)->esq->nInfo == 1)
                    {
                        if ((*pai)->nInfo == 2)
                        {
                            // info1 do pai desce
                            strcpy((*raiz)->info2, (*pai)->info1);
                            (*raiz)->linhaDir = (*pai)->linhaEsq;

                            // a info da esquerda passou para a raiz
                            strcpy((*raiz)->info1, (*pai)->esq->info1);
                            (*raiz)->linhaEsq = (*pai)->esq->linhaEsq;

                            (*raiz)->nInfo = 2;

                            (*raiz)->dir = (*raiz)->meio;
                            (*raiz)->meio = (*pai)->esq->meio;
                            (*raiz)->esq = (*pai)->esq->esq;

                            strcpy((*pai)->info1, (*pai)->info2);
                            (*pai)->linhaEsq = (*pai)->linhaDir;
                            (*pai)->linhaDir = NULL;

                            free((*pai)->esq);

                            (*pai)->esq = *raiz;
                            (*pai)->meio = (*pai)->dir;
                            (*pai)->dir = NULL;

                            (*pai)->nInfo = 1;
                        }
                        else if ((*pai)->nInfo == 1)
                        {
                            // a info do pai desceu pra raiz
                            strcpy((*raiz)->info2, (*pai)->info1);
                            (*raiz)->linhaDir = (*pai)->linhaEsq;

                            strcpy((*raiz)->info1, (*pai)->esq->info1);
                            (*raiz)->linhaEsq = (*pai)->esq->linhaEsq;

                            (*raiz)->nInfo = 2;

                            (*pai)->nInfo = 0;
                            (*raiz)->dir = (*raiz)->meio;
                            (*raiz)->meio = (*pai)->esq->meio;
                            (*raiz)->esq = (*pai)->esq->esq;

                            free((*pai)->esq);
                            (*pai)->esq = NULL;
                        }
                    }
                    else if (*raiz == (*pai)->esq)
                    {
                        if ((*pai)->meio->nInfo == 2)
                        {
                            // a info1 do pai desce para a raiz
                            strcpy((*raiz)->info1, (*pai)->info1);
                            (*raiz)->linhaEsq = (*pai)->linhaEsq;
                            (*raiz)->nInfo = 1;

                            // a info1 do meio sobre
                            strcpy((*pai)->info1, (*pai)->meio->info1);
                            (*pai)->linhaEsq = (*pai)->meio->linhaEsq;

                            strcpy((*pai)->meio->info1, (*pai)->meio->info2);
                            (*pai)->meio->linhaEsq = (*pai)->meio->linhaDir;
                            (*pai)->meio->nInfo = 1;

                            (*raiz)->esq = (*raiz)->meio;
                            (*raiz)->meio = (*pai)->meio->esq;

                            (*pai)->meio->esq = (*pai)->meio->meio;
                            (*pai)->meio->meio = (*pai)->meio->dir;
                            (*pai)->meio->dir = NULL;
                        }
                        else if ((*pai)->meio->nInfo == 1)
                        {
                            if ((*pai)->nInfo == 2)
                            {
                                strcpy((*raiz)->info1, (*pai)->info1);
                                (*raiz)->linhaEsq = (*pai)->linhaEsq;

                                // pai->meio ficou vazio aqui
                                strcpy((*raiz)->info2, (*pai)->meio->info1);
                                (*raiz)->linhaDir = (*pai)->meio->linhaEsq;

                                (*raiz)->nInfo = 2;

                                (*raiz)->esq = (*raiz)->meio;
                                (*raiz)->meio = (*pai)->meio->esq;
                                (*raiz)->dir = (*pai)->meio->meio;

                                strcpy((*pai)->info1, (*pai)->info2);
                                (*raiz)->linhaEsq = (*raiz)->linhaDir;
                                (*pai)->nInfo = 1;

                                free((*pai)->meio);
                                (*pai)->meio = (*pai)->dir;
                                (*pai)->dir = NULL;
                            }
                            else if ((*pai)->nInfo == 1)
                            {
                                strcpy((*raiz)->info1, (*pai)->info1);
                                (*raiz)->linhaEsq = (*pai)->linhaEsq;

                                (*raiz)->esq = (*raiz)->meio;

                                strcpy((*raiz)->info2, (*pai)->meio->info1);
                                (*raiz)->linhaDir = (*pai)->meio->linhaEsq;

                                (*raiz)->meio = (*pai)->meio->esq;
                                (*raiz)->dir = (*pai)->meio->meio;

                                (*pai)->nInfo = 0;
                                (*raiz)->nInfo = 2;

                                free((*pai)->meio);
                                (*pai)->meio = (*raiz);
                                (*pai)->esq = NULL;
                            }
                        }
                    }
                }
            }
            else if (pai == NULL)
            {
                *raiz = (*raiz)->meio;
            }
        }
    }
}

void maiorFilhoEsq(No **raiz, No **maiorFilho, No **paiM, int pos)
{
    if (*maiorFilho != NULL)
    {

        if ((*maiorFilho)->esq == NULL)
        { // significa que é uma folha

            char wordAux[50];
            Linha *aux;
            if (pos == 1)
            { // a posição da info em relação ao nó, ela é uma info 1
                strcpy(wordAux, (*raiz)->info1);
                aux = (*raiz)->linhaEsq;

                if ((*maiorFilho)->nInfo == 2)
                { // se o maior da esq tiver 2 filhos
                    strcpy((*raiz)->info1, (*maiorFilho)->info2);
                    (*raiz)->linhaEsq = (*maiorFilho)->linhaDir;
                    (*maiorFilho)->linhaDir = NULL;

                    strcpy((*maiorFilho)->info2, wordAux);
                    (*maiorFilho)->linhaDir = aux;
                }
                else
                {
                    strcpy((*raiz)->info1, (*maiorFilho)->info1);
                    (*raiz)->linhaEsq = (*maiorFilho)->linhaEsq;
                    (*maiorFilho)->linhaEsq = NULL;

                    strcpy((*maiorFilho)->info1, wordAux);
                    (*maiorFilho)->linhaEsq = aux;
                }
            }
            else if (pos == 2)
            {
                printf("Entrou aqui na remoção maiorFilho\n");
                strcpy(wordAux, (*raiz)->info2);
                aux = (*raiz)->linhaDir;

                if ((*maiorFilho)->nInfo == 2)
                {
                    strcpy((*raiz)->info2, (*maiorFilho)->info2);
                    (*raiz)->linhaDir = (*maiorFilho)->linhaDir;

                    strcpy((*maiorFilho)->info2, wordAux);
                    (*maiorFilho)->linhaDir = aux;
                }
                else
                {
                    strcpy((*raiz)->info2, (*maiorFilho)->info1);
                    (*raiz)->linhaDir = (*maiorFilho)->linhaEsq;

                    strcpy((*maiorFilho)->info1, wordAux);
                    (*maiorFilho)->linhaEsq = aux;
                }
            }
            removerArvore(maiorFilho, paiM, wordAux);
        }
        else
        {
            if ((*maiorFilho)->nInfo == 2)
            {
                maiorFilhoEsq(raiz, &(*maiorFilho)->dir, paiM, pos);
            }
            else if ((*maiorFilho)->nInfo == 1)
            {
                maiorFilhoEsq(raiz, &(*maiorFilho)->meio, paiM, pos);
            }
        }
    }
    balanceia(maiorFilho, paiM);
}

/*Só remove uma palavra se a lista de ocorrências for nula*/
void removerArvore(No **raiz, No **pai, char *word)
{
    if (*raiz != NULL)
    {
        if (strcasecmp((*raiz)->info1, word) == 0)
        {
            if (verificaFolha(*raiz) == 1)
            { // se a raiz é folha
                if ((*raiz)->nInfo == 2)
                {
                    strcpy((*raiz)->info1, (*raiz)->info2);
                    (*raiz)->linhaEsq = (*raiz)->linhaDir;
                    (*raiz)->linhaDir = NULL;
                    (*raiz)->nInfo = 1;
                }
                else if ((*raiz)->nInfo == 1)
                {
                    (*raiz)->nInfo = 0;
                    (*raiz)->linhaEsq = NULL;
                    (*raiz)->linhaDir = NULL;
                }
            }
            else
            {
                No **maiorFilho = &(*raiz)->esq; // passa a esquerda da raiz
                No **paiM = raiz;                // o pai da esquerda é a própria raiz
                // vai buscar o amior filho da esquerda e
                maiorFilhoEsq(raiz, maiorFilho, paiM, 1);
            }
        }
        else if ((*raiz)->nInfo == 2 && (strcasecmp((*raiz)->info2, word)) == 0)
        {

            if (verificaFolha(*raiz) == 1)
            {
                (*raiz)->nInfo = 1;
            }
            else
            {

                No **maiorFilho = &((*raiz)->meio);
                No **paiM = raiz;
                maiorFilhoEsq(raiz, paiM, maiorFilho, 2);

                printf("miorFilho %s\n", (*maiorFilho)->info1);
            }
        }
        else if ((strcasecmp(word, (*raiz)->info1)) < 0)
        {
            removerArvore(&(*raiz)->esq, raiz, word);
            //(*raiz)->NInfos == 1 || (*raiz)->NInfos == 2 && valor < (*raiz)->info2
        }
        else if ((*raiz)->nInfo == 1 || (*raiz)->nInfo == 2 && strcasecmp(word, (*raiz)->info2) < 0)
        {
            removerArvore(&(*raiz)->meio, raiz, word);
        }
        else
        {
            removerArvore(&(*raiz)->dir, raiz, word);
        }
    }
    balanceia(raiz, pai);
}

Linha *removeOcorrencia(Linha *raiz, int linha)
{
    if (raiz != NULL)
    {
        if (raiz->num == linha)
        {
            Linha *aux;
            aux = raiz;
            raiz = raiz->prox;
            free(aux);
        }
        else
        {
            raiz->prox = removeOcorrencia(raiz->prox, linha);
        }
    }
    return raiz;
}

int remover(No **raiz, No **pai, char *filename, int linha, char *word)
{
    int flag;
    if (*raiz != NULL)
    {

        if (strcasecmp(word, (*raiz)->info1) == 0)
        {
            printf("entrou aqui, caso 1\n");
            flag = removerPalavra(filename, linha, word); // remove do arquivo
            (*raiz)->linhaEsq = removeOcorrencia((*raiz)->linhaEsq, linha);
            if (flag == 1 && (*raiz)->linhaEsq == NULL)
            {
                removerArvore(raiz, pai, word); // remove da arvore se apalavra não tiver ocorrências
                printf("Removeu da árvore\n");
            }
        }
        else if ((*raiz)->nInfo == 2 && strcasecmp(word, (*raiz)->info2) == 0)
        {
            printf("entrou aqui, caso 2\n");
            flag = removerPalavra(filename, linha, word); // remove do arquivo
            printf("removeu a ocorrencia\n");
            (*raiz)->linhaDir = removeOcorrencia((*raiz)->linhaDir, linha);
            if (flag == 1 && (*raiz)->linhaDir == NULL)
            {
                printf("entrou aqui, caso 3\n");
                removerArvore(raiz, pai, word); // remove da arvore se apalavra não tiver ocorrências
                printf("Removeu da árvore\n");
            }
        }
        else
        {
            remover(&(*raiz)->esq, pai, filename, linha, word);
            remover(&(*raiz)->meio, pai, filename, linha, word);
            remover(&(*raiz)->dir, pai, filename, linha, word);
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

int menu()
{
    int opc;
    printf("1 - Ver todas as palavras do arquivo;\n");
    printf("2 - Buscar palavra no arquivo;\n");
    printf("3 - Inserir palavra em uma linha;\n");
    printf("4 - Excluir palavra de uma linha;\n");
    printf("0 - Sair\n");
    printf("Selecione a opção desejada: ");
    scanf("%d", &opc);
    return opc;
}

int main()
{
    No *raiz;
    Linha *aux;
    char meio[50];
    int linha, opc, flag = 0;

    aux = NULL;
    raiz = NULL;

    char filename[] = "arquivo.txt";
    FILE *arq;
    arq = fopen(filename, "r");

    addNo(&raiz, arq, 1, meio, &aux);
    fclose(arq);

    while ((opc = menu()) != 0)
    {
        switch (opc)
        {
        case 1: // imprime tudo em ordem
            imprime(raiz);
            break;
        case 2: // busca uma palavra
            int contador = 0;
            buscarPalavra(raiz, "nara", &flag, &contador);
            if (flag == 0)
            {
                printf("palavra não encontrada\n");
            }
            break;
        case 3:
            inserePalavra(filename, 3, "carrapato");

            insere(NULL, &raiz, "carrapato", meio, 3, &aux);
            addNo(&raiz, arq, 1, meio, &aux);
            break;
        case 4:
            // flag = removerPalavra(filename,3,"carrapato");
            No *pai;
            pai = NULL;
            flag = remover(&raiz, &pai, filename, 1, "nara");
            if (flag == 1)
            {
                printf("Palavra removida com sucesso\n");
            }
            else
            {
                printf("Erro ao remover palavra\n");
            }

            // removerArvore(&raiz,NULL,"carrapato");
            break;
        case 5:
            __clock_t inicio, fim;
            double tempo, tempoT, media = 0;
            int cont = 0;
            char *array[30] = {"planta", "gato", "borboleta", "nara", "risada", "enchente", "teclado",
                               "luz", "festa", "grama", "narciso", "fantasma", "quadro", "amor", "carrinho", "robô", "energia",
                               "riso", "tartaruga", "relâmpago", "estrela", "sombra", "sonho",
                               "curva", "fantasia", "eclipse", "música", "fotografia", "neve", "arco"};

            for (int i = 0; i < 30; i++)
            {
                inicio = clock();
                buscarPalavra(raiz, array[i], &flag, &cont);
                fim = clock();

                tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
                tempoT = tempoT + tempo;
                media = tempoT / 30;
                printf("%s  %d %.6f\n", array[i], cont, tempo);
                cont = 0;
            }
            printf("media %.6f",media);
            break;
        default:
            break;
        }
    }

    return 0;
}
