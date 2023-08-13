#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define nVertices 81
#define pinos 3
#define discos 4
#define INF 999
#define peso 1

//CADA CONFIGURAÇÂO È UM VÈRTICE

//configurações
typedef struct no{
    int conf[discos];
}No;

typedef struct grafo
{
    No *vertices;
    int **matriz;
}Grafo;

//cria a matriz de adjacencias e a preenche com 0 (e tbm retorna ela)
int **initMatriz(){
    int **matriz = malloc(nVertices * sizeof(int*)); //linhas
    for(int i = 0; i<nVertices;i++){
        matriz[i] = malloc(nVertices *sizeof(int)); //colunas
    }

    for(int i = 0;i<nVertices;i++){
        for(int j = 0; j< nVertices;j++){
            matriz[i][j] = 0;
        }
    }
    return matriz;
}

//cria o grafo
Grafo *initGraph(int num_vertices){
    Grafo *graph = malloc(sizeof(Grafo));
    if(graph){
        graph->vertices = malloc(num_vertices * sizeof(No));
        graph->matriz = initMatriz();
    }
    return graph;
}

void insereVertices(Grafo *graph, No *config){
    for(int i = 0; i < nVertices;i++){
        for(int j = 0; j < discos;j++){
            graph->vertices[i].conf[j] = config[i].conf[j];
        }
           
    }
}
//apenas para imprimir as configurações dos vertices
void imprimirConfigs(Grafo *graph){
    for(int i = 0; i < nVertices; i++){
        printf("vertices[%d] = (", i); // Iniciando a impressão para um vértice
        for(int j = 0; j < 4; j++){
            printf("%d", graph->vertices[i].conf[j]);
            if(j < 3) { // Se não for o último elemento, imprime a vírgula
                printf(",");
            }
        }
        printf(")\n"); // Finaliza a impressão para um vértice
    }
}

void createMatriz(No *vertices, Grafo *graph){
    for(int i = 0; i < nVertices; i++){
        for(int j = 1; j < 4; j++){ // Começando de 1 porque o índice 0 é o próprio vértice
            int indiceConexao = vertices[i].conf[j];
            if(indiceConexao != -1 && indiceConexao >= 0 && indiceConexao < nVertices){
                graph->matriz[i][indiceConexao] = 1;
            }
        }
    }
}

void imprimirMatrizAdj(Grafo *graph){
    int i, j;
    
    printf("Matriz de Adjacências:\n");

    // Imprimir os índices das colunas
    printf("    ");  // Espaço para alinhar com os índices das linhas
    for(j = 0; j < 15; j++){
        printf("%d   ", j);
    }
    printf("\n");
    printf("----");  // Separador
    for(j = 0; j < 15; j++){
        printf("--");
    }
    printf("\n");

    // Imprimir os valores da matriz
    for(i = 0; i < 15; i++){
        printf("%d |   ", i);  // Imprime o índice da linha
        for(j = 0; j < 15; j++){
            printf("%d   ", graph->matriz[i][j]);
        }
        printf("\n");
    }
}


//Imprime o vértice como se fosse a torre
void imprimirVertice(Grafo *graph, int vertice){
    // Se o índice fornecido é inválido, retorna sem fazer nada
    if(vertice < 0 || vertice >= nVertices){
        printf("Índice de vértice inválido!\n");
        return;
    }

    int discosNasTorres[3] = {0}; // Contador de discos em cada torre

    // Conta quantos discos estão em cada torre
    for(int i = 0; i < 4; i++){
        discosNasTorres[graph->vertices[vertice].conf[i]-1]++;
    }
    printf("-------------------------------------------------------------\n");
    printf(" %d\n", vertice);
    for(int i = 0; i < 3; i++){
        printf("Torre %d = ", i+1);
        for(int j = 0; j < 4; j++){
            if(graph->vertices[vertice].conf[j] == i+1){
                printf("%d,", j+1);
            }
        }
        printf(" (discos)\n");
    }
    printf("-------------------------------------------------------------\n");
}
//função retorna quais caminhos podem ser seguidos pelo jogador
void mostraCaminho(Grafo *graph, int verticeAtual, int *idCaminho){
    int cont = 0;
    for(int i = 0; i < nVertices; i++){
        if(graph->matriz[verticeAtual][i] == 1){
            //printf("possivel caminho: vertice = %d\n",i);
            idCaminho[cont] = i;
            cont ++;
        }
    }
    for(int i = cont; i<3;i++){
        idCaminho[i] = -1;
    }

}

//detecta e imprime qua movimento foi feito no jogo
void detectarMovimento(Grafo *graph, int vertice, int newVertice){
    for(int i = 0; i < 4; i++){
        if(graph->vertices[vertice].conf[i] != graph->vertices[newVertice].conf[i]){
            printf("*************************************************\n");
            printf("Disco %d movido para a Torre %d.\n\n", i + 1, graph->vertices[newVertice].conf[i]);
            printf("*************************************************\n");
        }
    }
}

//essa só apresenta quais jogadas válidas podem ser feitas
void preverMovimento(Grafo *graph, int vertice, int newVertice){
    for(int i = 0; i < 4; i++){
        if(graph->vertices[vertice].conf[i] != graph->vertices[newVertice].conf[i]){
            printf("*************************************************\n");
            printf("Disco %d pode ser movido para a Torre %d.\n\n", i + 1, graph->vertices[newVertice].conf[i]);
            printf("*************************************************\n");
        }
    }
}

//verifica se o jogador venceu o jogo
void jogador(Grafo *graph, int torreAtual, int *cont,int torre){
    int vitoria = 80;
    int vitoria2 = 65;
    if(torre == 1){ //torre 2
        for(int i = 0; i < 4;i++){
            if(graph->vertices[torreAtual].conf[i] == graph->vertices[vitoria].conf[i]){
                (*cont)++;
            }
        }
    }else if(torre == 2){ //torre 3
        for(int i = 0; i < 4;i++){
            if(graph->vertices[torreAtual].conf[i] == graph->vertices[vitoria2].conf[i]){
                (*cont)++;
            }
        }
    }
    
}

void jogar(Grafo *graph, int vertice,int torre){
    int idCaminho[3];
    int play, cont;
    cont = 0;
    printf("\n");
    printf("Estado atual da Torre: \n");
    imprimirVertice(graph,vertice);
    printf("\n");

    printf("Jogadas possíveis: \n");
    mostraCaminho(graph,vertice,idCaminho);
    printf("Configuração da torre em cada caminho:\n\n");
    for(int i = 0; i < 3;i++){
        if(idCaminho[i] != -1){
            printf("Jogada %d\n",i);
            preverMovimento(graph,vertice,idCaminho[i]);
            printf("\n");
        }  
    }

    printf("\n");

    printf("Escolha sua jogada: ");
    scanf("%d",&play);
    detectarMovimento(graph,vertice,idCaminho[play]);

    jogador(graph,idCaminho[play],&cont,torre);
    if(cont == 4){
        printf("-------------------------------------------------\n");
        printf("Parabéns!! Você venceu o jogo Torre de Hanói!!\n");
        printf("-------------------------------------------------\n");
    }else{
        jogar(graph,idCaminho[play],torre);
    } 
}

//retorna o vértice que ainda não teve a menor distancia definitiva determinada em relação a origem
int menorDistancia(int *distancia,bool registro[]){
    int min,idMin;
    min=INF;

    for(int i = 0;i<nVertices;i++){
        if(registro[i] == false && distancia[i] <= min){
            min = distancia[i];
            idMin = i;
        }
    }
    return idMin;
}

void menorCaminho(int *caminho,int destino){
    if(caminho[destino] == -1){
        return;
    }
    menorCaminho(caminho,caminho[destino]);
    printf("%d ",destino);
}

//retorna os vizinhos
int *adjacentes(Grafo *graph, int vertice, int *cont){
    int *vizinhos = (int*) malloc(nVertices * sizeof(int));
    *cont = 0; // Inicializar a contagem de vizinhos

    for(int i = 0; i < nVertices; i++) {
        if(graph->matriz[vertice][i] == 1) {
            vizinhos[*cont] = i;
            (*cont)++;
        }
    }
    return vizinhos;
    printf("até aqui foi\n");
}

//para diminuir o número de comparações o algoritmo só irá realizar comparações a partir dos
//vértices vizinhos
void dijkstra(Grafo *graph, int origem,int destino){
    int distancia[nVertices]; //armazena as distancias mais curtas entre a origem e todos os vértices do grafo
    bool registro[nVertices];//registra para quais vértices já foram encontradas a menor distancia a partir da origem
    int caminho[nVertices]; //armazena os vertices que compõe o menor caminho da origem ate o destino

    //inicialização dos vetores
    for(int i = 0; i < nVertices;i++){
        distancia[i] = INF; //coloca todas as distancias como infinito
        registro[i] = false; //coloca todas as posições como false
        caminho[i] = -1; 
    }

    distancia[origem] = 0; //seta a distancia da origem como sendo zero

    for(int i = 0; i < nVertices-1;i++){
        int auxD = menorDistancia(distancia,registro);
        registro[auxD] = true;
       
        if(auxD == destino){
            break;
        }
        int cont;
        int *vizinhos = adjacentes(graph,auxD,&cont);

        for(int k = 0; k < cont; k++) {
            int j = vizinhos[k];
            //printf("Compara[%d][%d]\n",auxD,j);
            if(!registro[j] && distancia[auxD] + peso < distancia[j]) {
                caminho[j] = auxD;
                distancia[j] = distancia[auxD] + peso;
                
            }
    }
    free(vizinhos);
    }

    
    printf("O melhor caminho de %d para %d é: %d ", origem, destino, origem);
    menorCaminho(caminho, destino);
    printf("\nDistância: %d arestas\n", distancia[destino]);
}

int menu(){
    int op;

    printf("1 - Matriz de Adjacências.\n");
    printf("2 - Ver conteudo do vértice.\n");
    printf("3 - Ver todas as possiveis configurações.\n");
    printf("4 - Jogar.\n");
    printf("5 - Calcular Distancia com o Dijkstra.\n");
    printf("0 - Sair.\n");
    printf("Selecione a Opçao desejada: ");
    scanf("%d",&op);
    return op;
}


int main(){
    int opc;
    Grafo *graph;
    graph = initGraph(nVertices);
    No configurations[nVertices] = {{1,1,1,1},{1,1,1,2},{1,1,1,3},{1,1,3,2},{1,1,2,3},{1,1,3,3},{1,1,3,1},{1,1,2,1},{1,1,2,2},{1,2,3,3},
                                    {1,3,2,2},{1,2,3,1},{1,2,3,2},{1,3,2,3},{1,3,2,1},{1,2,2,1},{1,2,1,2},{1,3,1,3},{1,3,3,1},{1,2,2,2}
                                    ,{1,2,2,3},{1,2,1,3},{1,2,1,1},{1,3,1,1},{1,3,1,2},{1,3,3,2},{1,3,3,3},{3,2,2,2},{2,3,3,3},{3,2,2,3},
                                    {3,2,2,1},{2,3,3,1},{2,3,3,2},{3,2,1,3},{3,2,3,1},{2,3,2,1},{2,3,1,2},{3,2,1,1},{3,2,1,2},{3,2,3,2},
                                    {3,2,1,1},{2,3,2,2},{2,3,2,3},{2,3,1,3},{2,3,1,1},{3,3,1,1},{3,1,3,3},{2,1,2,2},{2,2,1,1},{3,3,1,2},
                                    {3,3,2,3},{3,1,3,1},{3,1,3,2},{2,1,2,3},{2,1,2,1},{2,2,1,2},{2,2,1,3},{3,3,3,2},{3,3,2,3},{3,1,2,1},
                                    {3,1,1,2},{2,1,1,3},{2,1,3,1},{2,2,3,2},{2,2,2,3},{3,3,3,3},{3,3,3,1},{3,3,2,1},{3,3,2,2},{3,1,2,2},
                                    {3,1,2,3},{3,1,1,3},{3,1,1,1},{2,1,1,1},{2,1,1,2},{2,1,3,2},{2,1,3,3},{2,2,3,3},{2,2,3,1},{2,2,2,1},
                                    {2,2,2,2}};
    
    //vetor que armazena as conexões [vertice Atual, aresta1, aresta2,aresta3], quando não tiver aresta 3 será preenchido com -1
    No vertices[nVertices] = {{0,1,2,-1},{1,0,2,3},{2,0,1,4},{3,1,5,6},{4,2,7,8},{5,3,6,9},{6,5,3,7},{7,4,6,8},{8,4,7,10},
                            {9,5,11,12},{10,8,13,14},{11,9,12,15},{12,9,11,16},{13,10,14,17},{14,10,13,18},{15,11,19,20},
                            {16,12,21,22},{17,13,23,24},{18,14,25,26},{19,15,20,27},{20,15,19,21},{21,16,20,22},{22,16,21,23},
                            {23,17,22,24},{24,17,23,25},{25,18,24,26},{26,18,25,28},{27,19,29,30},{28,26,31,32},{29,27,30,33},
                            {30,27,29,34},{31,28,32,35},{32,28,31,36},{33,29,37,38},{34,30,39,40},{35,31,41,42},{36,32,43,44},
                            {37,33,38,45},{38,33,37,39},{39,34,38,40},{40,34,39,46},{41,35,42,47},{42,35,41,43},{43,36,42,44},
                            {44,36,43,48},{45,37,49,50},{46,40,51,52},{47,41,53,54},{48,44,55,56},{49,45,50,57},{50,45,49,58},
                            {51,46,52,59},{52,46,51,60},{53,54,61},{54,47,53,62},{55,48,56,63},{56,48,55,64},{57,49,65,66},
                            {58,50,67,68},{59,51,69,70},{60,52,71,72},{61,53,73,74},{62,54,75,76},{63,55,77,78},{64,56,79,80},
                            {65,57,66,-1},{66,57,65,67},{67,58,66,68},{68,58,67,69},{69,59,68,70},{70,59,69,71},{71,60,70,72},
                            {72,60,71,73},{73,61,72,74},{74,61,73,75},{75,62,74,76},{76,62,75,77},{77,63,76,78},{78,63,77,79},
                            {79,64,78,80},{80,64,79,-1}};

    
    insereVertices(graph,configurations);

    createMatriz(vertices,graph);

    while ((opc = menu())!=0)
    {
        switch (opc)
        {
        case 1:
            imprimirMatrizAdj(graph);
            break;
        case 2:
            int v = 29;
            imprimirVertice(graph,v);
            break;
        case 3:
            imprimirConfigs(graph);
            break;
        case 4:
            int esc,escVertice = 0;
            int torre = 2;
            printf("-------------------------------------------------\n");
            printf("Seja Bem-Vindo ao Jogo Torre de Hanói!\n");
            printf("-------------------------------------------------\n");
            
            printf("Deseja mover os discos para qual torre? (1 - Torre1 2 - Torre3)");
            scanf("%d",&torre);

            printf("Deseja escolher um vértice para iniciar o jogo? (1 - Sim 2 - Não)\n");
            scanf("%d",&esc);
            if(esc == 1){
                printf("De qual vértice deseja iniciar o jogo?\n");
                scanf("%d",&escVertice);
                if(escVertice >= 0 && escVertice <= 80){
                    jogar(graph, escVertice,torre);
                }else{
                    printf("Vértice Inexistente.\n");
                }
            }else if(esc == 2){
                jogar(graph,escVertice,torre); //implementar
            }else{
                printf("Opção inválida.\n");
            }
            break;
        case 5:
            int origem, destino;
            printf("Digite o vértice de origem: ");
            scanf("%d",&origem);
            if(origem >= 0 && origem <= 80){
                printf("Digite o Vértice de destino: ");
                scanf("%d",&destino);
                if(destino >= origem && destino >= 0 && destino <= 80){
                    dijkstra(graph,origem,destino);
                }else{
                    printf("Vértice inválido!\n");
                }
            }else{
                    printf("Vértice inválido!\n");
            }
            
            break;
        default:

            break;
        }
    }
    

}