#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>

#define tam 256

typedef struct linha {
    int numLinha;
    struct linha *prox;
} Linha;

typedef struct arvore {
    char info[50];
    int cor;
    struct arvore *esq, *dir;
    Linha *linhas;
} No;

Linha *criaLinha(int nLinha) {
    Linha *newLine = (Linha *)malloc(sizeof(Linha));
    if (newLine) {
        newLine->numLinha = nLinha;
        newLine->prox = NULL;
        return newLine;
    }
    return NULL;
}

Linha *insereLinha(Linha *raiz, int linha) {
    if (raiz == NULL) {
        raiz = criaLinha(linha);
    } else {
        raiz->prox = insereLinha(raiz->prox, linha);
    }
    return raiz;
}

No *criaNo(const char *info, int nLinha) {
    No *newWord = (No *)malloc(sizeof(No));
    if (newWord) {
        strcpy(newWord->info, info);
        newWord->linhas = NULL;
        newWord->linhas = insereLinha(newWord->linhas, nLinha);
        newWord->cor = 0;
        newWord->esq = NULL;
        newWord->dir = NULL;
        return newWord;
    }
    return NULL;
}

No *rotacaoEsquerda(No *raiz) {
    No *aux;
    aux = raiz->dir;
    raiz->dir = aux->esq;
    aux->esq = raiz;
    aux->cor = raiz->cor;
    raiz->cor = 0; // vermelho
    return aux;
}

No *rotacaoDireita(No *raiz) {
    No *aux;
    aux = raiz->esq;
    raiz->esq = aux->dir;
    aux->dir = raiz;
    aux->cor = raiz->cor;
    raiz->cor = 0; // vermelho
    return aux;
}

void trocaCor(No *raiz) {
    if (raiz->cor == 0) {
        raiz->cor = 1;
    } else {
        raiz->cor = 0;
    }

    if (raiz->esq->cor == 0) {
        raiz->esq->cor = 1;
    } else {
        raiz->esq->cor = 0;
    }

    if (raiz->dir->cor == 0) {
        raiz->dir->cor = 1;
    } else {
        raiz->dir->cor = 0;
    }
}

int retornaCor(No *raiz) {
    if (raiz == NULL) {
        return 1;
    } else {
        return raiz->cor;
    }
}

No *insere(No *raiz, char *word, int linha) {
    if (raiz == NULL) {
        raiz = criaNo(word, linha); // todo nó recém inserido é vermelho
    } else {
        int cmp;
        cmp = strcasecmp(word, raiz->info);
        if (cmp == 0) { // significa que a palavra já existe
            raiz->linhas = insereLinha(raiz->linhas, linha);
        } else if (cmp < 0) { // word é menor que a palavra da raiz
            raiz->esq = insere(raiz->esq, word, linha);
        } else { // word é maior
            raiz->dir = insere(raiz->dir, word, linha);
        }
    }
    /*se um nó tem um filho vermelho, que ele seja filho da esquerda*/
    if (retornaCor(raiz->dir) == 0 && retornaCor(raiz->esq) == 1) {
        raiz = rotacaoEsquerda(raiz);
    }
    /*Não podem ter nós vermelhos consecutivos*/
    if (retornaCor(raiz->esq) == 0 && retornaCor(raiz->esq->esq) == 0) {
        raiz = rotacaoDireita(raiz);
    }
    /*Nãp pode, ter 2 nós vermelhos paralelos*/
    if (retornaCor(raiz->esq) == 0 && retornaCor(raiz->dir) == 0) {
        trocaCor(raiz);
    }
    return raiz;
}


void buscarPalavra(No *raiz, char *word,int *contador){
    int cmp;
    if(raiz != NULL){
        (*contador) ++;
        cmp = strcasecmp(word,raiz->info);
        if(cmp == 0){ //palavra encontrada
            printf("Palavra '%s' encontrada!\n",word);
            printf("Linha de ocorrência: ");
            if (imprimeLinhas(raiz->linhas) == 0) {
                printf("\n");
            }
            // return; -----> para os testes
        }else if(cmp < 0){
            buscarPalavra(raiz->esq, word,contador);
        }else{
            buscarPalavra(raiz->dir,word,contador);
        }
    }else{
        printf("\nA palavra não consta no arquivo de entrada.\n\n");
    }
    
    
}

int  imprimeLinhas(Linha *raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->numLinha);
        return imprimeLinhas(raiz->prox); 
    }
    return 0;
}

void imprime(No *raiz) {
    if (raiz != NULL) {
        imprime(raiz->esq);
        printf("%s: Ocorrências: ", raiz->info);
        imprimeLinhas(raiz->linhas);
        printf("\n");
        imprime(raiz->dir);
    }
}

int inserePalavra(char *filename,int line,char *palavra){
    FILE *arq;
    FILE *temp;
    char linha[256];
    int linha_atual = 1;
    int flag = 0;
    arq = fopen(filename,"r"); //abre para leitura
    if(arq == NULL){
        printf("Erro ao abrir o arquivo.\n");
    }

    //arquivo temporário onde os dados serão escritos
    temp = fopen("temporario.txt","w");
    if(temp == NULL){
        printf("Erro ao abrir o arquivo.\n");
    }
    // Percorrer as linhas do arquivo original
    while (fscanf(arq, "%[^\n]%*c", linha) != EOF) {
        if (linha_atual == line) {
            // Adicionar a nova palavra na linha desejada
            strcat(linha, ",");
            strcat(linha, palavra);
            flag = 1;
        }

        // Escrever a linha no arquivo temporário
        fprintf(temp, "%s\n", linha);
        linha_atual++;
    }

    // Fechar os arquivos
    fclose(arq);
    fclose(temp);

    arq = fopen(filename,"w");
    if(arq == NULL){
        printf("Erro ao abrir o arquivo.\n");
    }

    temp = fopen("temporario.txt","r");
    if(temp == NULL){
        printf("Erro ao abrir o arquivo.\n");
    }

    while (fgets(linha, sizeof(linha), temp) != NULL) {
        fputs(linha, arq);
    }

    fclose(arq);
    fclose(temp);

    remove("temp.txt");
    return flag;

}

int removerPalavra(char *filename, int linha, char *word) {
    FILE *arq, *temp;
    int flag = 0;
    arq = fopen(filename, "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        fclose(arq);
        exit(1);
    }

    char tamLinha[tam];
    int line = 1;

    while (fgets(tamLinha, sizeof(tamLinha), arq) != NULL) {
        if (line == linha) {
            char linhaModificada[tam]; 
            strcpy(linhaModificada, tamLinha); //copia a linha original para o linha modificada
            /*a linha modificada será lida paravra por palavra, até encontrar a palavra buscada,cada palavra
            diferente da buscada é concatenada a linhafinal, que será a linha escrita no arquivo temp após a remoção*/
            char *palavra = strtok(linhaModificada, " \n\t;.,:");
            char linhaFinal[tam] = "";
            int firstWord = 1;
            //int removeFlag = 0;  // Flag para indicar se uma palavra foi removida

            while (palavra != NULL) {
                if (strcasecmp(word, palavra) != 0) {
                    if (firstWord) {
                        strcat(linhaFinal, palavra);
                        firstWord = 0;
                    } else {
                        strcat(linhaFinal, ",");
                        strcat(linhaFinal, palavra);
                    }
                } else {
                    flag = 1;
                }
                palavra = strtok(NULL, " \n\t;.,:");
            }

            fprintf(temp, "%s\n", linhaFinal);

        } else {
            fprintf(temp, "%s", tamLinha);
        }
        line++;
    }

    fclose(arq);
    fclose(temp);

    arq = fopen(filename,"w");
    if(arq == NULL){
        printf("Erro ao abrir o arquivo.\n");
    }
    temp = fopen("temp.txt","r");
    if(temp == NULL){
        printf("Erro ao abrir o arquivo.\n");
    }

    while (fgets(tamLinha, sizeof(tamLinha), temp) != NULL) {
        fputs(tamLinha, arq);
    }

    fclose(arq);
    fclose(temp);

    remove("temp.txt");

    return flag;
}

No *insereNo(No *raiz, char *word, int linha){
    raiz = insere(raiz,word,linha);
    if(raiz != NULL){
        raiz->cor = 1;
    }
    return raiz;
}

Linha *removerOcorrencia(Linha *raiz, int linha){
    if(raiz != NULL){
        if(raiz->numLinha == linha){
            Linha *aux;
            aux = raiz;
            raiz = raiz->prox;
            free(aux);
        }else{
            raiz->prox = removerOcorrencia(raiz->prox,linha);
        }
    }
    return raiz;
}

No *moveEsquerda(No *raiz){
    trocaCor(raiz);
    if(retornaCor(raiz->dir->esq) == 0){
        raiz->dir = rotacaoDireita(raiz->dir);
        raiz = rotacaoEsquerda(raiz);
        trocaCor(raiz);
    }
    return raiz;
}

No *moveDireita(No *raiz){
    trocaCor(raiz);
    if((retornaCor(raiz->esq->esq)) == 0){
        raiz = rotacaoDireita(raiz);
        trocaCor(raiz);
    }
    return raiz;
}
//é passada a direita como parametro
No *menorFilho(No *raiz){
    if(raiz->esq == NULL){
        return raiz;
    }else{
        return menorFilho(raiz->esq);
    }
}

No *balanceia(No *raiz){
    if(retornaCor(raiz->dir) == 0){
        raiz = rotacaoEsquerda(raiz);
    }
    if(raiz->esq != NULL && retornaCor(raiz->esq) == 0 && retornaCor(raiz->esq->esq) == 0){
        raiz = rotacaoDireita(raiz);
    }
    if(retornaCor(raiz->esq) == 0 && retornaCor(raiz->dir) == 0){
        trocaCor(raiz);
    }
    return raiz;
}

No *removerArvore(No *raiz, char *word){
    if(strcasecmp(word,raiz->info) < 0){
        if(retornaCor(raiz->esq) == 1 && retornaCor(raiz->esq->esq) == 1){
            raiz = moveEsquerda(raiz);
        }
        raiz->esq = removerArvore(raiz->esq,word);
    }else{
        if(retornaCor(raiz->esq) == 0){
            raiz = rotacaoDireita(raiz);
        }
        //só apaga se não tiver nenhuma ocorrência
        if(strcasecmp(word,raiz->info) == 0 && raiz->esq == NULL){ 
            free(raiz);
            //raiz = NULL;
            return NULL;
        }
        if(retornaCor(raiz->dir) == 1 && retornaCor(raiz->dir->esq) == 1){
            raiz = moveDireita(raiz);
        }
        if(strcasecmp(word,raiz->info) == 0){
            No *filho;
            Linha *aux;
            filho = menorFilho(raiz->dir);
            char p[50];
            //guarda as informações da raiz
            strcpy(p,raiz->info);
            aux = raiz->linhas;
            //troca a raiz com o menor da esquerda da direita
            strcpy(raiz->info, filho->info);
            raiz->linhas = filho->linhas;

            strcpy(filho->info,p);
            filho->linhas = aux;
            removerArvore(raiz->dir, word);

        }else{
            raiz->dir = removerArvore(raiz->dir,word);
        }
    }
    return balanceia(raiz);
}

//No **raiz, No **pai, char *filename,int linha,char *word
int remover(No **raiz, char *filename, int linha, char *word){
    int flag = 0;
    if(*raiz != NULL){
        if(strcasecmp(word,(*raiz)->info) == 0){
            printf("entrou aqui\n");
            int x = removerPalavra(filename,linha,word);
            printf("O valor de x é: %d\n",x);
            (*raiz)->linhas = removerOcorrencia((*raiz)->linhas,linha);
            if(x == 1 && (*raiz)->linhas == NULL){ //não tem mais ocorrências no arquivo
                *raiz = removerArvore(*raiz, word);
                flag = 1;
            }
        }else{
            if(strcasecmp(word,(*raiz)->info) < 0){
                flag = remover(&(*raiz)->esq, filename,linha,word);
            }
            if(strcasecmp(word,(*raiz)->info) > 0){
                flag = remover(&(*raiz)->dir, filename,linha,word);
            }
        }
    }
    return flag;
}

void adicionaNo(No **raiz, FILE *arq, int linha) {
    char linhas[tam];
    if (fgets(linhas, sizeof(linhas), arq)) {
        char *word = strtok(linhas, ",");
        while (word != NULL) {
            size_t len = strlen(word);
            if (len > 0 && word[len - 1] == '\n') {
                word[len - 1] = '\0'; // Substitui o '\n' por '\0'
            }
            *raiz = insereNo(*raiz, word,linha);
            word = strtok(NULL, ",");
        }
        adicionaNo(raiz, arq, linha + 1);
    }
}


int menu(){
    int opc;
    printf("1 - Ver todas as palavras do arquivo;\n");
    printf("2 - Buscar palavra no arquivo;\n");
    printf("3 - Inserir palavra em uma linha;\n");
    printf("4 - Excluir palavra de uma linha;\n");
    printf("0 - Sair\n");
    printf("Selecione a opção desejada: ");
    scanf("%d",&opc);
    return opc;
}

int main(){
    No *raiz;
    raiz = NULL;

    int opc,flag;
    char filename[] = "arquivo.txt";
    int linha;
    char auxWord[50];

    FILE *arq;
    arq=fopen(filename,"r");
    adicionaNo(&raiz,arq,1);
    fclose(arq);

    while ((opc = menu()) != 0)
    {
        switch (opc)
        {
        case 1:
            imprime(raiz);
            break;
        case 2:
            int contador =0;
            buscarPalavra(raiz,"nara",&contador);
            break;
        case 3:
            linha = 2;
            strcpy(auxWord,"gato");
            flag = inserePalavra(filename,linha,auxWord);
            raiz = insere(raiz,"gato",2);
            if(flag == 1){
                printf("\n");
                printf("Palavra '%s' inserida na linha '%d'\n",auxWord,linha);
                printf("\n");
            }else{
                printf("\n");
                printf("Não foi possivel realizar a inserção\n");
                printf("\n");
            }
            break;
        case 4:
            linha = 1;
            strcpy(auxWord,"nara");
            flag = remover(&raiz,filename,linha,auxWord);
            if(flag == 1){
                printf("\n");
                printf("Palavra '%s' removida da linha '%d' com sucesso!\n",auxWord,linha);
                printf("\n");
            }else{
                printf("Não foi possivel realizar a remoção\n");
            }
            
            break;
        case 5:
            __clock_t inicio, fim;
            double tempo, tempoT, media = 0;
            int cont = 0;
            char *array[30] = {"planta", "gato", "borboleta", "nara", "risada", "enchente", "teclado",
                               "luz", "festa", "grama", "narciso", "fantasma", "quadro", "amor", "carrinho", "robô", "energia",
                               "riso", "tartaruga", "relâmpago", "estrela", "sombra", "sonho",
                               "curva", "fantasia", "eclipse", "música", "fotografia", "neve", "arco"};
            
            for(int i = 0;i<30;i++){
                inicio = clock();
                buscarPalavra(raiz, array[i],&cont);
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
    


}

