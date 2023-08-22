#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct grafo
{
    int *vertices; //vetor com os vértices
    float **matriz;//matriz de adjascencias
}Grafo;

//cria e inicializa a matriz de adjacencias com -1
float **createMatriz(int nVertices){
    float **matriz = malloc(nVertices * sizeof(float*));
    for(int i = 0; i < nVertices;i++){
        matriz[i] = malloc(nVertices * sizeof(float));
    }

    for(int i = 0; i < nVertices; i++){
        for(int j = 0; j < nVertices; j++){
            matriz[i][j] = -1;
        }
    }

    return matriz;
}

Grafo *createGraph(int nVertices){
    Grafo *graph = malloc(sizeof(Grafo));
    if(graph){
        graph->vertices = malloc(nVertices * sizeof(char));
        graph->matriz = createMatriz(nVertices);
    }
    return graph;
}

Grafo *insereVertices(Grafo *graph,int *vertices, int nVertices){
    for(int i = 0; i < nVertices; i++){
        graph->vertices[i] = vertices[i];
    }
    return graph;
}

void mostrarVertices(Grafo *graph, int nVertices){
    for(int i = 0; i < nVertices; i++){
        printf("Vertice[%d] = %d\n",i, graph->vertices[i]);
    }
}

void adicionaArestas(Grafo *graph,char origem, char destino, float peso){
    graph->matriz[origem][destino] = peso;
}


void imprimirMatrizAdj(Grafo *graph,int nVertices){
    int i, j;
    
    printf("Matriz de Adjacências:\n");

    // Imprimir os índices das colunas
    printf("    ");  // Espaço para alinhar com os índices das linhas
    for(j = 0; j < nVertices; j++){
        printf("%d   ", j);
    }
    printf("\n");
    printf("----");  // Separador
    for(j = 0; j < nVertices; j++){
        printf("--");
    }
    printf("\n");

    // Imprimir os valores da matriz
    for(i = 0; i < nVertices; i++){
        printf("%d |   ", i);  // Imprime o índice da linha
        for(j = 0; j < nVertices; j++){
            printf("%.2f   ", graph->matriz[i][j]);
        }
        printf("\n");
    }
}

void pesosArestas(Grafo *graph, int nVertices){
    for(int i = 0; i < nVertices; i++){
        for(int j = 0; j<nVertices;j++){
            if(graph->matriz[i][j] != -1){
                printf("Aresta (%d,%d) = %.2f\n",i,j,graph->matriz[i][j]);
            }
        }
    }
}

void inicializa(float *confiabilidades, int *caminho, bool *registro,int nVertices){
    for(int i = 0; i < nVertices; i++){
        confiabilidades[i] = -1;
        caminho[i] = -1;
        registro[i] = false;
    }
}

int maiorConfiabilidade(float *confiabilidades,bool *resgistro ,int nVertice){
    float conf = -1;
    int idConf = -1;

    for(int i = 0; i < nVertice; i++){
        if(resgistro[i] == false && idConf == -1 || resgistro[i] == false && confiabilidades[i] > conf){
            idConf = i;
            conf = confiabilidades[i];
        }
    }
    return idConf;
}


void menorCaminho(int *caminho,int destino){
    if(caminho[destino] == -1){
        return;
    }
    menorCaminho(caminho,caminho[destino]);
    printf("%d ",destino);
}

void DijkstraConfiabilidade(Grafo *graph,int nVertices ,int origem, int destino){
    float confiabilidades[nVertices];
    int caminho[nVertices];
    bool registro[nVertices];
    int vertice;

    inicializa(confiabilidades,caminho,registro,nVertices);

    confiabilidades[origem] = 1.0;

    vertice = origem;
    while (vertice != -1)
    {
       for(int i = 0; i < nVertices; i++){
            if(graph->matriz[vertice][i] != -1 && registro[i] == false){
                float conf = confiabilidades[vertice] * graph->matriz[vertice][i];
                if(conf > confiabilidades[i]){
                    confiabilidades[i] = conf;
                    caminho[i] = vertice;
                }
            }
        }
        registro[vertice] = true;
        vertice = maiorConfiabilidade(confiabilidades,registro,nVertices);
    }
    
    printf("O caminho mais confiável de %d para %d é: %d ", origem, destino,origem);
    menorCaminho(caminho, destino);
    printf("\nDistância: %.2f\n", confiabilidades[destino]);

}

Grafo *geraGrafo(Grafo *graph,int nVertices, int arestas){
    graph = createGraph(nVertices);

    int *vertices = malloc(nVertices * sizeof(int));
    for(int i = 0; i < nVertices; i++){
        vertices[i] = i+1;
    }
    insereVertices(graph, vertices, nVertices);

    for (int i = 0; i < arestas; i++) {
        int origem = rand() % nVertices;
        int destino = rand() % nVertices;
        while (destino == origem) {
            destino = rand() % nVertices;
        }
        float peso = (float)rand() / RAND_MAX;
        adicionaArestas(graph, origem, destino, peso);
    }
    free(vertices);
    return graph;

}

int menu(){
    int op;
    printf("1 - Gerar grafo aleatório.\n");
    printf("2 - Ver todos os vértices.\n");
    printf("3 - Calcular caminho mais seguro.\n");
    printf("4 - Ver matriz de adjacencias.\n");
    printf("5 - Ver pesos de todas as areas.\n");
    printf("0 - sair.\n");
    printf("Selecione a opção desejada:  ");
    scanf("%d",&op);
    return op;
}

/*Quanto mais próx de 1 o peso, mais confiável o canal*/
int main(){

    Grafo *graph;
    int opc;
    
    while ((opc = menu()) != 0)
    {
        switch (opc)
        {
        case 1:
            graph = geraGrafo(graph,8,20);
            break;
        case 2:
            mostrarVertices(graph,8);
            break;
        case 3:
            DijkstraConfiabilidade(graph,8,0,6);
            break;
        case 5:
            pesosArestas(graph,8);
            break;
        case 6: //*********CASE APENAS PARA TESTES***************
            int nVertices = 8;
            graph = createGraph(nVertices);
            int vertices[8] = {1,2,3,4,5,6,7,8};
            float pesos[11] = {0.2, 0.5, 0.4,0.9,1,0.2,0.7,0.1,0.2,0.3,1};
            insereVertices(graph,vertices,nVertices);

            adicionaArestas(graph,0,1,pesos[0]);
            adicionaArestas(graph,0,2,1);
            adicionaArestas(graph,2,1,pesos[2]);
            adicionaArestas(graph,2,7,1);
            adicionaArestas(graph,7,3,0.1);
            adicionaArestas(graph,7,4,1);
            adicionaArestas(graph,1,6,pesos[6]);
            adicionaArestas(graph,6,5,pesos[7]);
            adicionaArestas(graph,5,3,pesos[8]);
            adicionaArestas(graph,6,3,pesos[9]);
            adicionaArestas(graph,4,3,1);
            DijkstraConfiabilidade(graph,nVertices,0,4);
            break;
        default:
            break;
        }
    }
    

    
    

}