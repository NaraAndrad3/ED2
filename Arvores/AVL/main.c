#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "disciplinas.h"

int menu(){
    int opc;
    printf("\t\tMENU\n");
 
    printf(" 1  - Inserir Curso.\n");
    printf(" 2  - Ver Todos Cursos Cadastrados.\n");
    printf(" 3  - Buscar Curso especifico.\n");
    printf(" 4  - Buscar Curso por quantidade de blocos.\n");
    printf(" 5  - Inserir Disciplina.\n");
    printf(" 6  - Ver Todas as Disciplinas do Curso.\n");
    printf(" 7  - Buscar disciplina especifica.\n");
    printf(" 8  - Imprimir Disciplina de Um bloco especifico\n");
    printf(" 9  - Buscar disciplina por carga horária.\n");
    printf(" 10  - Excluir Curso.\n");
    printf(" 11 - Excluir Disciplina.\n");
    printf(" 0  - Sair\n");
    printf("\tSelecione a opção desejada: ");

    scanf("%d",&opc);
    return opc;
}

int main(){
    Curso *raiz,*new;
    raiz = NULL;
    new = NULL;

    Disciplina *raizD,*newD;
    raizD = NULL;
    newD = NULL;

    clock_t inicio, fim;
    double tempo, tempoTotal,mediaBuscas;
    int opc;
    //opc = menu();
    while((opc = menu()) != 0){
        switch (opc)
        {
            case 1:
                int codCurso,blocos,semanas;
                char nome[20];
                
                printf("Nome do Curso: ");
                scanf("%s",nome);
                printf("Código do curso: ");
                scanf("%d",&codCurso);
                printf("Numero de blocos:");
                scanf("%d",&blocos);
                printf("Numero de semanas: ");
                scanf("%d",&semanas);
                new = criarCurso(codCurso,nome,blocos,semanas);
                inicio = clock();
                //inicio do tempo de inserção
                raiz = inserirCurso(raiz,new);
                fim = clock();//fim do tempo de inserção

                tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;//tempo medido em segundos
                printf("Tempo de inserção: %f segundos\n", tempo);
               
                tempoTotal += tempo;
                
                break;
            case 2://imprimir todos os cursos
                printf("\n----Cursos Cadastrados----\n");
                imprimirCursos(raiz);
                break;
            case 3://imprimir um curso especifico
                int cod;
                double tempoB;
                cod = 33;
                inicio = clock();
                imprimirCursoEspecifico(raiz,cod);
                fim = clock();
                tempoB = (double)(fim - inicio)/CLOCKS_PER_SEC;
                mediaBuscas += tempoB;
                printf("Tempo de Busca: %f segundos\n",tempoB);
                break;
            case 4: //imprimir curso por numero de blocos
                int bloc;
                bloc = 8;
                ImprimirCursoBloco(raiz,bloc);
                break;
            case 5:// inserir disciplinas
                int codDsc,bloco,c_Horaria,idC;
                char nomeDsc[20];

                printf("Codigo do curso: ");
                scanf("%d",&idC);
                printf("Nome da Disciplina: ");
                scanf("%s",nomeDsc);
                printf("Código da Disciplina: ");
                scanf("%d",&codDsc);
                printf("Bloco da Disciplina: ");
                scanf("%d",&bloco);
                printf("Carga horária da Disciplina: ");
                scanf("%d",&c_Horaria);

                newD = criaDisciplina(codDsc,nomeDsc,bloco,c_Horaria);
                inserirDisciplinaCurso(&raiz,idC,newD);
                
                break;
            case 6://ver todas as disciplinas de um curso
                int codigo;
                printf("Codigo do curso: ");
                scanf("%d",&codigo);
                imprimirDisciplinasEmOrdem(raiz,codigo);
                break;
            case 7: // buscar disciplina especifica
                int curso, dsc;
                printf("Codigo do curso: ");
                scanf("%d",&curso);
                printf("Codigo da disciplina: ");
                scanf("%d",&dsc);
                printf("----------Dados da Disciplina---------\n");
                imprimirDisciplinaEspecifica(raiz,curso,dsc);

                break;
            case 8:
                int blc, nCurso;
                printf("Codigo do curso: ");
                scanf("%d",&nCurso);
                printf("Numero de Bloco: ");
                scanf("%d",&blc);
                printf("\tDisciplinas do Bloco: %d\n",blc);
                imprimirDisciplinasDoBloco(raiz,nCurso,blc);
                break;
            case 9:// imprimir discs com a msm carga horaria
                int carga, crs;
                printf("Codigo do curso: ");
                scanf("%d",&crs);
                printf("Carga horária a ser visualizada: ");
                scanf("%d",&carga);
                imprimirDisciplinasPelaCargaH(raiz,crs,carga);
                break;
            case 10:// Remover um curso
                int idCurso;
                printf("Codigo do curso a ser removido: ");
                scanf("%d",&idCurso);
                raiz = buscaCursoRemover(raiz,idCurso);
                break;
            case 11: // remover uma disciplina
                int idDsc,idCrs;
                printf("Codigo do curso: ");
                scanf("%d",&idCrs);
                printf("Codigo da disciplina: ");
                scanf("%d",&idDsc);
                buscarCursoRemoverDsc(&raiz,idCrs,idDsc);
                break;
            default:
                printf("Opção não disponivel. Verifique o menu e tente novamente\n");
                break;
            }
            
        }
        printf("Sistema Finalizado\n");
        double media = tempoTotal/30;
        double m = mediaBuscas/30;
        printf("Media: %f  Media Buscas: %f",media,m);
        
    
    
}