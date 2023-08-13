#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "disciplinas.h"

typedef struct disciplina{
    int codDisciplina;
    char nome[20];
    int bloco;
    int cargaHoraria;
    int altura;
    struct disciplina *esq,*dir;
}Disciplina;

typedef struct curso{
    int cod;
    char nome[20];
    int blocos;
    int semanas;
    int altura;
    struct curso *esq,*dir;
    Disciplina *disc;//cada curso tem sua árvore de disciplinas
}Curso;

Curso *criarCurso(int cod, char *nome_curs, int bloco_curs, int semanas){
    Curso *newCourse = (Curso*)malloc(sizeof(Curso));
    strcpy(newCourse->nome,nome_curs);
    newCourse->cod = cod;
    newCourse->blocos = bloco_curs;
    newCourse->semanas = semanas;
    newCourse->esq = NULL;
    newCourse->esq = NULL;
    newCourse->disc = NULL;
    newCourse->altura = 0;
    return newCourse;
}

int alturaNo(Curso *raiz){
    if(raiz == NULL){
        return -1; // se for nulo, H = -1
    }else{
        return raiz->altura;
    }
}

fatorBalanceamento(Curso *raiz){
    if(raiz){
        return (alturaNo(raiz->esq) - alturaNo(raiz->dir));
    }else{
        return 0;
    }
}

int maiorH(int esq_h, int dir_h){
    if(esq_h > dir_h){
        return esq_h;
    }
    else{
        return dir_h;
    }
}

Curso *rotacaoEsquerda(Curso *raiz){
    Curso *aux, *filhoAux;

    aux = raiz->dir;
    filhoAux = aux->esq;
    raiz->dir = filhoAux;
    aux->esq = raiz;
    raiz = aux;

    //atualiza a altura dos nós a medida em que são rotacionados
    aux->altura = maiorH(alturaNo(aux->esq),alturaNo(aux->dir))+1;
    raiz->altura = maiorH(alturaNo(raiz->esq),alturaNo(raiz->dir))+1;
    return aux; // retorna a nova raiz
}

Curso *rotacaoDireita(Curso *raiz){
    Curso *aux,*filhoAux;

    aux = raiz->esq;
    filhoAux = aux->dir;
    raiz->esq = filhoAux;
    aux->dir = raiz;
    raiz = aux;
    // a função maiorH é a mesma que a Curso está usando
    aux->altura = maiorH(alturaNo(aux->esq), alturaNo(aux->dir)) + 1;
    raiz->altura = maiorH(alturaNo(raiz->esq), alturaNo(raiz->dir)) + 1;
    return raiz;
}

Curso *balacearCurso(Curso *raiz){
    int fatorB;
    fatorB = fatorBalanceamento(raiz);
    if (fatorB == 2){
        int fatorEsq;
        fatorEsq = fatorBalanceamento(raiz->esq);
        if (fatorEsq < 0){
            raiz->esq = rotacaoEsquerda(raiz->esq);
        }
        raiz = rotacaoDireita(raiz);
    } else {
        if (fatorB == -2){
            int fatorDir;
            fatorDir = fatorBalanceamento(raiz->dir);
            if (fatorDir > 0){
                raiz->dir = rotacaoDireita(raiz->dir);
            }
            raiz = rotacaoEsquerda(raiz);
        }
    }
    return raiz;
}

Curso *inserirCurso(Curso *raiz,Curso *new){
    if(raiz == NULL){
        raiz = new;
    }else{
        if(new->cod < raiz->cod){
            raiz->esq = inserirCurso(raiz->esq,new);
        }
        if(new->cod > raiz->cod){
            raiz->dir = inserirCurso(raiz->dir,new);
        }
        return raiz;Curso *removerCurso;
    }

    raiz->altura = maiorH(alturaNo(raiz->esq), alturaNo(raiz->dir)) +1;
    raiz = balacearCurso(raiz); 
}

//(9) Excluir um curso dado o código do mesmo, desde que não tenha nenhuma disciplina cadastrada para
//aquele curso

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
            if(verificaFolhaCurso(raiz) == 1){
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
    
    raiz->altura = maiorH(alturaNo(raiz->esq),alturaNo(raiz->dir))+1;
    if(raiz != NULL){
        raiz = balacearCurso(raiz);
        return raiz;
    }
}
}

Curso *buscaCursoRemover(Curso *raiz, int codCurso){
    if(raiz != NULL){
        if(codCurso == raiz->cod && raiz->disc == NULL){
                raiz = removerCurso(raiz);
                return raiz;
              
        }else{
            if(codCurso < raiz->cod){
                raiz->esq = buscaCursoRemover(raiz->esq,codCurso);
            }
            if(codCurso > raiz->cod){
                raiz->dir = buscaCursoRemover(raiz->dir,codCurso);
            }
            return raiz;
        }
    }else{
        return NULL;
    }

}

Disciplina *criaDisciplina(int codDisciplina, char *nome_dsc, int bloco_dsc, int cargaHoraria){
    Disciplina *newDisciplina = (Disciplina*)malloc(sizeof(Disciplina));
    newDisciplina->bloco = bloco_dsc;
    strcpy(newDisciplina->nome, nome_dsc);
    newDisciplina->codDisciplina = codDisciplina;
    newDisciplina->cargaHoraria = cargaHoraria;
    newDisciplina->esq = NULL;
    newDisciplina->dir = NULL;
    newDisciplina->altura = 0;

    return newDisciplina;
}

int alturaNoDsc(Disciplina *raiz){
    if(raiz == NULL){
        return -1; // se for nulo, H = -1
    }else{
        return raiz->altura;
    }
}

Disciplina *RotacaoEsquerdaDsc(Disciplina *raizD){
    Disciplina *aux, *filhoAux;

    aux = raizD->dir;
    filhoAux = aux->esq;
    raizD->dir = filhoAux;
    aux->esq = raizD;
    raizD = aux;

    aux->altura = maiorH(alturaNoDsc(aux->esq),alturaNoDsc(aux->dir))+1;
    raizD->altura = maiorH(alturaNoDsc(raizD->esq),alturaNoDsc(raizD->dir))+1;
    return aux; // retorna a nova raiz
}

Disciplina *RotacaoDireitaDsc(Disciplina *raizD){
    Disciplina *aux,*filhoAux;

    aux = raizD->esq;
    filhoAux = aux->dir;
    raizD->esq = filhoAux;
    aux->dir = raizD;
    raizD = aux;
    
    aux->altura = maiorH(alturaNoDsc(aux->esq), alturaNoDsc(aux->dir)) + 1;
    raizD->altura = maiorH(alturaNoDsc(raizD->esq), alturaNoDsc(raizD->dir)) + 1;
    return raizD;
}

Disciplina *BalancearNoDisciplina(Disciplina *raizD){
    int fatorB;
    fatorB = fatorBalanceamentoDisciplina(raizD);
    if (fatorB == 2){
        int fatorEsq;
        fatorEsq = fatorBalanceamentoDisciplina(raizD->esq);
        if (fatorEsq < 0){
            raizD->esq = RotacaoEsquerdaDsc(raizD->esq);
        }
        raizD = RotacaoDireitaDsc(raizD);
    } else {
        if (fatorB == -2){
            int fatorDir;
            fatorDir = fatorBalanceamentoDisciplina(raizD->dir);
            if (fatorDir > 0){
                raizD->dir = RotacaoDireitaDsc(raizD->dir);
            }
            raizD = RotacaoEsquerdaDsc(raizD);
        }
    }
    return raizD;
}

int fatorBalanceamentoDisciplina(Disciplina *raizD){
    if(raizD){
        return (alturaNoDsc(raizD->altura) - alturaNoDsc(raizD->altura));
    }else{
        return 0;
    }
}


Disciplina *inserirDisciplina(Disciplina *raiz,Disciplina *new){
    if(raiz == NULL){
        raiz = new;
        return raiz;
    }else{
        if(new->codDisciplina < raiz->codDisciplina){
            raiz->esq = inserirDisciplina(raiz->esq,new);
        }
        if(new->codDisciplina > raiz->codDisciplina){
            raiz->dir= inserirDisciplina(raiz->dir,new);
        }
        return raiz;
    }

    raiz->altura = maiorH(alturaNoDsc(raiz->esq), alturaNoDsc(raiz->dir))+1;
    raiz = BalancearNoDisciplina(raiz);
    return raiz;
}

void inserirDisciplinaCurso(Curso **raiz,int codCurso,Disciplina *new){
    if(*raiz != NULL){
        if((*raiz)->cod == codCurso && (*raiz)->blocos>= new->bloco){
            
            (*raiz)->disc = inserirDisciplina((*raiz)->disc,new);
        }else{
            if(codCurso < (*raiz)->cod){
                inserirDisciplinaCurso(&(*raiz)->esq,codCurso,new);
            }
            if(codCurso > (*raiz)->cod){
                inserirDisciplinaCurso(&(*raiz)->dir,codCurso,new);
            }

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
//verifica se o nó tem 1 filho e retorna o end dele
Disciplina *verificaUmFilho(Disciplina *raizD){
    if(raizD->esq == NULL && raizD->dir != NULL){
        return raizD->dir;
    }else if(raizD->esq != NULL && raizD->dir == NULL){
        return raizD->esq;
    }else{
        return NULL;
    }
}

//retorna o maior filho da esquerda de um nó
Disciplina* maiorFilhoEsquerda(Disciplina* raizD) {
    if (raizD == NULL) {
        return NULL;
    }
    if (raizD->dir == NULL) {
        return raizD;
    }
    return maiorFilhoEsquerda(raizD->dir);
}

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

Disciplina *removerDisciplina(Disciplina *raiz,int codDsc){
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
                raiz = filho;
                free(aux);
                return raiz;
            }else{
                aux = maiorFilhoEsquerda(raiz->esq);
                swap(raiz,aux);
                raiz->esq = removerDisciplina(raiz->esq,codDsc);
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
    }else{
        return NULL;
    }

    raiz->altura = maiorH(alturaNoDsc(raiz->esq), alturaNoDsc(raiz->dir))+1;
    if(raiz != NULL){
        raiz = BalancearNoDisciplina(raiz);
        return raiz;
    }
}

void buscarCursoRemoverDsc(Curso **raiz,int codCurso,int codDsc){
    if(*raiz != NULL){
        if((*raiz)->cod == codCurso){
            (*raiz)->disc = removerDisciplina((*raiz)->disc,codDsc);
        }
        else{
            if(codCurso < (*raiz)->cod){
                buscarCursoRemoverDsc(&(*raiz)->esq,codCurso,codDsc);
            }
            if(codCurso > (*raiz)->cod){
                buscarCursoRemoverDsc(&(*raiz)->dir,codCurso,codDsc);
            }
        }
    }
}
//--------------------------------------------------------------------------------------------

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

void ImprimirCursoBloco(Curso *raiz,int blocos)
{
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

void dadosDisciplina(Disciplina *raizD){
    printf("-------------------------\n");
    printf("Nome: %s\n",raizD->nome);
    printf("Código: %d\n",raizD->codDisciplina);
    printf("Bloco: %d\n",raizD->bloco);
    printf("Carga Horaria: %d\n",raizD->cargaHoraria);
    printf("-------------------------\n");
}

void imprimirDisciplinas(Disciplina *raizD){
    if(raizD!=NULL){
        imprimirDisciplinas(raizD->esq);
        dadosDisciplina(raizD);
        imprimirDisciplinas(raizD->dir);
    }
}

void imprimirDisciplinasEmOrdem(Curso *raiz,int codCurso){
    if(raiz != NULL){
        if(raiz->cod == codCurso){
            imprimirDisciplinas(raiz->disc);
        }else{
            if(codCurso < raiz->cod){
                imprimirDisciplinasEmOrdem(raiz->esq,codCurso);
            }
            if(codCurso > raiz->cod){
                imprimirDisciplinasEmOrdem(raiz->dir,codCurso);
            }
        }
    }
}

//(5) Imprimir os dados de uma disciplina dado o código dela e do curso ao qual ela pertence;

void buscaDisciplinaPorCodigo(Disciplina *raizD, int codDisciplina){
    if(raizD != NULL){
        if(raizD->codDisciplina == codDisciplina){
            dadosDisciplina(raizD);
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
            dadosDisciplina(raiz);
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
//busca as disciplinas a partir da carga horaria e as imprime
void buscaDisciplnasPorCargaH(Disciplina *raizD, int c_horaria){
    if(raizD != NULL){
        buscaDisciplnasPorCargaH(raizD->esq,c_horaria);
        if(raizD->cargaHoraria == c_horaria){
            dadosDisciplina(raizD);
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































