#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "disciplinas.h"

typedef struct disciplina{
    int codDisciplina;
    char nome[20];
    int bloco; // deve ser menor ou IGUAL ao do curso
    int cargaHoraria;
    struct disciplina *esq,*dir;
}Disciplina;

typedef struct curso{
    int cod;
    char nome[20];
    int blocos;
    int semanas;
    struct curso *esq,*dir;
    Disciplina *disc;//cada curso tem sua árvore de disciplinas
}Curso;

//cria e retorna um curso
Curso *criarCurso(int cod,char nome[],int blocos,int semanas){
    Curso *newCurso = malloc(sizeof(Curso));

    newCurso->cod = cod;
    newCurso->blocos = blocos;
    newCurso->semanas = semanas;
    strcpy(newCurso->nome,nome);
    newCurso->esq = NULL;
    newCurso->dir = NULL;
    newCurso->disc = NULL;

    return newCurso;
}

Curso *inserirCurso(Curso *raiz,Curso *new){
    if(raiz == NULL){
        raiz = new;
        return raiz;
    }else{
        if(new->cod < raiz->cod){
            raiz->esq = inserirCurso(raiz->esq,new);
        }
        if(new->cod > raiz->cod){
            raiz->dir = inserirCurso(raiz->dir,new);
        }
        return raiz;
    }
}

//cria um nó disciplina
Disciplina *criarDisciplina(int cod, char nome[], int bloco, int cargaH){
    Disciplina *newDsc = malloc(sizeof(Disciplina));
    newDsc->bloco = bloco;
    newDsc->cargaHoraria = cargaH;
    newDsc->codDisciplina = cod;
    strcpy(newDsc->nome,nome);
    newDsc->esq = NULL;
    newDsc->dir = NULL;

    return newDsc;
}
//insere uma disciplina na árvore de disciplinas de um determinado curso
Disciplina *inserirDisciplina(Disciplina *raiz, Disciplina *new){
    if(raiz == NULL){
        raiz = new;
        return raiz;
    }else{
        if(new->codDisciplina < raiz->codDisciplina){
            raiz->esq = inserirDisciplina(raiz->esq, new);
        }
        if(new->codDisciplina > raiz->codDisciplina){
            raiz->dir = inserirDisciplina(raiz->dir, new);
        }
        return raiz;
    }
}

//(9) Excluir um curso dado o código do mesmo, desde que não tenha nenhuma disciplina cadastrada para aquele curso.
int verificaFolhaCurso(Curso *raiz){
    if(raiz ->esq == NULL && raiz->dir == NULL){
        return 1;
    }else{
        return -1;
    }
}
Curso *verificaFilhoCurso(Curso *raiz){
    if(raiz->esq != NULL && raiz->dir == NULL){
        return raiz->esq;
    }else if(raiz->esq == NULL && raiz->dir != NULL){
        return raiz->dir;
    }else{
        return NULL;
    }
}
//retorna o maior filho da esquerda
Curso *verificaMaiorFilho(Curso* raiz) {
    if (raiz == NULL) {
        return NULL;
    }
    if (raiz->dir == NULL) {
        return raiz;
    }
    return verificaMaiorFilho(raiz->dir);
}

void swapCurso(Curso *raiz,Curso *filho){
    Curso *aux;
    aux = malloc(sizeof(Curso));

    aux->cod = raiz->cod;
    aux->blocos = raiz->blocos;
    aux->semanas = raiz->semanas;
    strcpy(aux->nome,raiz->nome);

    raiz->blocos = filho->blocos;
    raiz->cod = filho->cod;
    raiz->semanas = filho->semanas;
    strcpy(raiz->nome,filho->nome);

    filho->blocos = filho->blocos;
    filho->cod = filho->cod;
    filho->semanas = filho->semanas;
    strcpy(filho->nome,filho->nome);

    free(aux);
}

Curso *removerCurso(Curso *raiz){
    Curso *aux,*filho;
    if(raiz != NULL){
        if((verificaFolhaCurso(raiz))== 1){
            aux = raiz;
            raiz = NULL;
            free(aux);
            return raiz;
        }else if((filho = verificaFilhoCurso(raiz)) != NULL){
            aux = raiz;
            raiz = filho;
            free(aux);
            return raiz;
        }else{
            aux = verificaMaiorFilho(raiz->esq);
            swapCurso(raiz,aux);
            raiz->esq = removerCurso(raiz->esq);
            return raiz;
        }
    }
}


Curso *buscarCursoRemover(Curso *raiz,int cod){
    if(raiz != NULL){
        if(cod == raiz->cod && raiz->disc == NULL){
            raiz = removerCurso(raiz);
            return raiz;
        }else{
            if(cod < raiz->cod){
                raiz->esq = buscarCursoRemover(raiz->esq,cod);
            }
            if(cod > raiz->cod){
                raiz->dir = buscarCursoRemover(raiz->dir,cod);
            }
            return raiz;
        }
    }
}


//(8) Excluir uma disciplina dado o código da disciplina e o código do curso;
int verificaNoFolha(Disciplina *raizD){
    if(raizD->esq == NULL && raizD->dir == NULL){
        return 1;
    }else{
        return -1;
    }
}

Disciplina *verificaUmFilho(Disciplina *raizD){
    if(raizD->esq == NULL && raizD->dir != NULL){
        return raizD->dir;
    }else if(raizD->esq != NULL && raizD->dir == NULL){
        return raizD->esq;
    }else{
        return NULL;
    }
}
//retorna o maior filho da esquerda
Disciplina* verificaMaiorFilhoDsc(Disciplina* raiz) {
    if (raiz == NULL) {
        return NULL;
    }
    if (raiz->dir == NULL) {
        return raiz;
    }
    
    return verificaMaiorFilhoDsc(raiz->dir);
}
//realiza a troca entre a raiz a ser apagada e o maior filho da esquerda
void swap(Disciplina *raizD, Disciplina *no){
    Disciplina *aux = malloc(sizeof(Disciplina));

    aux->codDisciplina = raizD->codDisciplina;
    strcpy(aux->nome,raizD->nome);
    aux->cargaHoraria = raizD->cargaHoraria;
    aux->bloco = raizD->bloco;

    raizD->codDisciplina = no->codDisciplina;
    strcpy(raizD->nome,no->nome);
    raizD->cargaHoraria = no->cargaHoraria;
    raizD->bloco = no->bloco;

    no->codDisciplina = aux->codDisciplina;
    strcpy(no->nome,aux->nome);
    no->cargaHoraria = aux->cargaHoraria;
    no->bloco = aux->bloco;

    free(aux);
    
}

Disciplina *removerDisciplina(Disciplina *raiz, int codDsc){
    Disciplina *aux,*filho;
    if(raiz != NULL){
        if(raiz->codDisciplina == codDsc){
            if((verificaNoFolha(raiz)) == 1){
                aux = raiz;
                raiz = NULL;
                free(aux);
                return raiz;
            }else if((filho = verificaUmFilho(raiz)) != NULL){
                aux = raiz;
                raiz = filho;;
                free(aux);
                return raiz;
            }else{
                aux = verificaMaiorFilhoDsc(raiz->esq);
                swap(raiz,aux);
                raiz->esq = removerDisciplina(raiz->esq, codDsc);
                return raiz;
            }
        }else{
            if(codDsc < raiz->codDisciplina){
                raiz->esq = removerDisciplina(raiz->esq,codDsc);
            }
            if(codDsc > raiz->codDisciplina){
                raiz->dir = removerDisciplina(raiz->dir,codDsc);
            }
            return raiz;
        }
    }
}
//busca o curso de onde a disciplina será removida
void buscarCursoRemoverDisciplina(Curso **raiz,int cod,int codDsc){
    if(*raiz != NULL){
        if((*raiz)->cod == cod){
            (*raiz)->disc = removerDisciplina((*raiz)->disc, codDsc);
        }else{
            if(cod < (*raiz)->cod){
                buscarCursoRemoverDisciplina(&(*raiz)->esq,cod,codDsc);
            }
            if(cod > (*raiz)->cod){
                buscarCursoRemoverDisciplina(&(*raiz)->dir,cod,codDsc);
            }
        }
    }
}

//busca o curso especifico onde a disciplina será inserida
void inserirDisciplinanoCurso(Curso **raiz, int cod, Disciplina *newDsc){
    if(*raiz != NULL){
        if((*raiz)->cod == cod && newDsc->bloco <= (*raiz)->blocos){
            (*raiz)->disc = inserirDisciplina((*raiz)->disc, newDsc);
        }else{
            if(cod < (*raiz)->cod){
                inserirDisciplinanoCurso(&(*raiz)->esq,cod,newDsc);
            }
            if(cod > (*raiz)->cod){
                inserirDisciplinanoCurso(&(*raiz)->dir,cod,newDsc);
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------

//(1) Imprimir a árvore de cursos em ordem crescente pelo código do curso;

void dadosCurso(Curso *raiz){
    printf("-------------------------\n");
    printf("Nome: %s\n",raiz->nome);
    printf("Codigo: %d\n",raiz->cod);
    printf("Semanas: %d\n",raiz->semanas);
    printf("Blocos: %d\n",raiz->blocos);
    printf("-------------------------\n");
}

void imprimirCursos(Curso *raiz){
    if(raiz != NULL){
        imprimirCursos(raiz->esq);
        dadosCurso(raiz);
        imprimirCursos(raiz->dir);
    }
}

//(2) Imprimir os dados de um curso dado o código do mesmo;
void imprimirCursoEspecifico(Curso *raiz,int cod){
    if(raiz != NULL){
        if(cod == raiz->cod){
            dadosCurso(raiz);
            //return;
        }else{
            if(cod < raiz->cod){
                imprimirCursoEspecifico(raiz->esq,cod);
            }
            if(cod > raiz->cod){
                imprimirCursoEspecifico(raiz->dir,cod);
            }
        } 
    }
}

//(3) Imprimir todos os cursos com a mesma quantidade de blocos, onde o usuário informe a quantidade de blocos;

void ImprimirCursoBloco(Curso *raiz,int blocos){
    if (raiz != NULL)
    {
        ImprimirCursoBloco(raiz->esq,blocos);
        if (raiz->blocos == blocos)
        {
            printf("-------------------------\n");
            printf("Nome do Curso: %s\n", raiz->nome);
            printf("Quantidade de blocos: %d\n", raiz->blocos);
            printf("-------------------------\n");
        }
        ImprimirCursoBloco(raiz->dir,blocos);
    }
}

//(4) Imprimir a árvore de disciplinas em ordem crescente pelo código das disciplinas dado o código do curso

void dadosDisciplinas(Disciplina *raiz){
    printf("-------------------------\n");
    printf("Nome: %s\n",raiz->nome);
    printf("Codigo: %d\n",raiz->codDisciplina);
    printf("Bloco: %d\n",raiz->bloco);
    printf("Carga Horaria: %d\n",raiz->cargaHoraria);
    printf("-------------------------\n");
}

void imprimirDisciplinasCurso(Disciplina *raiz){
    if(raiz != NULL){
        imprimirDisciplinasCurso(raiz->esq);
        dadosDisciplinas(raiz);
        imprimirDisciplinasCurso(raiz->dir); 
    }
}
//busca o curso das disciplinas a serem impressas
void imprimirDisciplinas(Curso *raiz, int cod){
    if(raiz != NULL){
        if(cod == raiz->cod){
            imprimirDisciplinasCurso(raiz->disc);
        }else{
            if(cod < raiz->cod){
                imprimirDisciplinas(raiz->esq, cod);
            }
            if(cod > raiz->cod){
                imprimirDisciplinas(raiz->dir, cod);
            }
        }
    }
}

//(5) Imprimir os dados de uma disciplina dado o código dela e do curso ao qual ela pertence;
void buscaDisciplinaPorCodigo(Disciplina *raizD, int codDisciplina){
    if(raizD != NULL){
        if(raizD->codDisciplina == codDisciplina){
            dadosDisciplinas(raizD);
        }else{
            if(codDisciplina < raizD->codDisciplina){
                buscaDisciplinaPorCodigo(raizD->esq,codDisciplina);
            }
            if(codDisciplina > raizD->codDisciplina){
                buscaDisciplinaPorCodigo(raizD->dir,codDisciplina);
            }
        }
    }
}
void imprimirDisciplinaEspecifica(Curso *raiz, int codCurso, int codDisciplina){
    if(raiz != NULL){
        if(raiz->cod == codCurso){
            buscaDisciplinaPorCodigo(raiz->disc,codDisciplina);
        }else{
            if(codCurso < raiz->cod){
                imprimirDisciplinaEspecifica(raiz,codCurso,codDisciplina);
            }
            if(codCurso > raiz->cod){
                imprimirDisciplinaEspecifica(raiz->dir,codCurso,codDisciplina);
            }
        }
        
    }
}

//(6) Imprimir as disciplinas de um determinado bloco de um curso, dado o bloco e o código do curso;

void imprimirDisciplinasBloco(Disciplina *raiz, int bloco){
    if(raiz != NULL){
        imprimirDisciplinasBloco(raiz->esq,bloco);
        if(raiz->bloco == bloco){
            dadosDisciplinas(raiz);
        }
        imprimirDisciplinasBloco(raiz->dir,bloco);
    }
}

void imprimirDisciplinasDoBloco(Curso *raiz,int cod,int bloco){
    if(raiz != NULL){
        if(raiz->cod == cod && bloco <= raiz->blocos){
            imprimirDisciplinasBloco(raiz->disc, bloco);
        }else{
            if(cod < raiz->cod){
                imprimirDisciplinasDoBloco(raiz->esq,cod,bloco);
            }
            if(cod > raiz->cod){
                imprimirDisciplinasDoBloco(raiz->dir,cod,bloco);
            }
        }
    }
}

//(7) Imprimir todas as disciplinas de um determinado curso com a mesma carga horária, onde o código
//do curso e a carga horária devem ser informadas pelo usuário;

void buscaDisciplnasPorCargaH(Disciplina *raizD, int c_horaria){
    if(raizD != NULL){
        buscaDisciplnasPorCargaH(raizD->esq,c_horaria);
        if(raizD->cargaHoraria == c_horaria){
            dadosDisciplinas(raizD);
        }
        buscaDisciplnasPorCargaH(raizD->dir,c_horaria);
    }
}
//busca o curso e imprime as disciplinas de carga horaria iguais a que se pede
void imprimirDisciplinasPelaCargaH(Curso *raiz, int codCurso,int c_horaria){
    if(raiz != NULL){
        if(codCurso == raiz->cod){
            buscaDisciplnasPorCargaH(raiz->disc,c_horaria);
        }else{
            if(codCurso < raiz->cod){
                imprimirDisciplinasPelaCargaH(raiz->esq,codCurso,c_horaria);
            }
            if(codCurso > raiz->cod){
                imprimirDisciplinasPelaCargaH(raiz->dir,codCurso,c_horaria);
            }
        }
    }
}
