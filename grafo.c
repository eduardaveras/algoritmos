#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct nolista{
    int vertice;
    double peso;
    struct nolista* next;
}noLista;

typedef struct {
    int x;
    int y;
    int moeda;
    noLista* head;
} ListaAdj;

typedef struct {
    int vertices;
    int arestas;
    ListaAdj* adj;
}Grafo;

Grafo* cria_grafo(int v, int* X, int* Y, int qtd, int* moeda){
    Grafo* grafo = (Grafo*) malloc(sizeof(Grafo));
    grafo->vertices=v;
    grafo->arestas=0;
    grafo->adj = (ListaAdj*) malloc(v*sizeof(ListaAdj));
    for(int i=0; i<v; i++){
        grafo->adj[i].head=NULL;
        grafo->adj[i].x=X[i];
        grafo->adj[i].y=Y[i];
        grafo->adj[i].moeda=0;
    }

    //se der tle, colocar numa bst!
    for(int i=0; i<qtd; i++){
        grafo->adj[moeda[i]].moeda=1;
    }

    return grafo;
}

double calcula_energia(Grafo* grafo, int v0, int vf){
    double energiaGasta;
    if(grafo->adj[vf].moeda==1)
        energiaGasta = -((grafo->adj[vf].x - grafo->adj[v0].x)*(grafo->adj[vf].x - grafo->adj[v0].x) + (grafo->adj[vf].y - grafo->adj[v0].y)*(grafo->adj[vf].y - grafo->adj[v0].y));
    else 
        energiaGasta = (grafo->adj[vf].x - grafo->adj[v0].x)*(grafo->adj[vf].x - grafo->adj[v0].x) + (grafo->adj[vf].y - grafo->adj[v0].y)*(grafo->adj[vf].y - grafo->adj[v0].y);
    return energiaGasta;
}

noLista* cria_lista(int vertice, double peso){
    noLista* temp = (noLista*) malloc(sizeof(noLista));

    temp->vertice=vertice;
    temp->peso=peso;
    temp->next=NULL;

    return temp;
}

void add_arestas(Grafo* grafo, int v0, int v1, double peso){
    noLista* novoNo = cria_lista(v1, peso);

    noLista* cur = grafo->adj[v0].head;

    if(grafo->adj[v0].head!=NULL){
        while(cur->next!=NULL)
            cur=cur->next;
        cur->next=novoNo;
    }
    else grafo->adj[v0].head=novoNo;

    grafo->arestas++;
}

void bellman_ford(Grafo* grafo, int s){
    int** D = (int**) malloc(sizeof(int*)*grafo->vertices); 
    for(int i=0; i<grafo->vertices; i++)
        D[i] = (int*) malloc(sizeof(int)*grafo->vertices);

    int* F = (int*) malloc(sizeof(int)*grafo->vertices);
    int v=0;
    double w=0;

    //inicializando os vetores:
    for(int i=0; i<grafo->vertices; i++){
        D[0][i] = INT_MAX;
        F[i] = -1;
    }

    D[0][s] = 0;

    for(int k=1; k<grafo->vertices; k++){
        //copiando os valores de k-1 para k
        for(int temp=0; temp<grafo->vertices; temp++)
            D[k][temp] = D[k-1][temp];
        

        for(int u=0; u<grafo->vertices; u++){
            noLista* e = grafo->adj[u].head;
            while(e){
                v = e->vertice;
                w = e->peso;

                if(D[k-1][u] + w < D[k][v]){
                    D[k][v] = D[k-1][u] + w;
                    F[v] = u;
                } 
                e = e->next;
            }
        }
    }

    for(int u=0; u<grafo->vertices; u++){
        noLista* e = grafo->adj[u].head;
        while(e){
            int v = e->vertice;
            int w = e->peso;

            if(D[grafo->vertices-1][u] + w < D[grafo->vertices-1][v] && D[grafo->vertices-1][u] != INT_MAX){
                printf("LOOP\n");
                return ; 
            }
            e = e->next;
        }
    }

    printf("%d ", D[grafo->vertices-1][grafo->vertices-1]);

    int tam=1;
    int* caminho = (int*) malloc(sizeof(int)*tam);

    caminho[0] = grafo->vertices-1;
    for(tam=1; tam<grafo->vertices; tam++){
        caminho = (int*) realloc(caminho, sizeof(int)*(tam+1));
        caminho[tam]=F[caminho[tam-1]];

        if(caminho[tam]==-1)
            break;
    }

    for(int i=tam-1; i>=0; i--){
        printf("%d", caminho[i]);
        if(caminho[i]!=(grafo->vertices-1)) printf(" ");
    }

    printf("\n");

    //deslocando memoria
    for(int i=0; i<grafo->vertices; i++)
        free(D[i]);
    free(D);
    free(F);
    free(caminho);
}

int main(){
    int testes;
    int num_vertices, qtd_arestas, qtd_moedas;
    
    scanf("%d", &testes);
    while(testes){
        scanf("%d", &num_vertices);
    
        int* X = (int*) malloc(num_vertices*sizeof(int));
        int* Y = (int*) malloc(num_vertices*sizeof(int));
        int* arestas = NULL;

        for(int i=0; i<num_vertices; i++)
            scanf("%d %d", &X[i], &Y[i]);

        scanf("%d", &qtd_moedas);
        int* moedas = (int*) malloc(qtd_moedas*sizeof(int));
        for(int i=0; i<qtd_moedas; i++)
            scanf("%d", &moedas[i]);

        Grafo* grafo = cria_grafo(num_vertices, X, Y, qtd_moedas, moedas); //cria um grafo sem arestas

        for(int i=0; i<num_vertices; i++){
            scanf("%d", &qtd_arestas);
            arestas = (int*) realloc(arestas, sizeof(int)*qtd_arestas);

            for(int j=0; j<qtd_arestas; j++){
                scanf("%d", &arestas[j]);
                int peso = calcula_energia(grafo, i, arestas[j]);
                add_arestas(grafo, i, arestas[j], peso);
            }
        }

        bellman_ford(grafo, 0);

        testes--;
    }  

    return 0; 

}
