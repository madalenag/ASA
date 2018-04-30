#include <stdlib.h>
#include <stdio.h>
#include "limits.h"

#define FALSE 0
#define TRUE 1
#define UNDEFINED -1
#define min(a, b) (a<b ? a : b)


//QUEUE

struct node {
   int key;
   struct node *next;
   struct node *prev;
};

struct node *head = NULL;
struct node *last = NULL;
struct node *current = NULL;

int isEmpty() {
   return head == NULL;
}

/*int length() {
   int length = 0;
   struct node *current;

   for(current = head; current != NULL; current = current->next){
      length++;
   }

   return length;
}*/

void displayForward() {
   struct node *ptr = head;
   printf("\n[ ");

   while(ptr != NULL) {
      printf("(%d) ",ptr->key);
      ptr = ptr->next;
   }
   printf(" ]");
}


void insertLast(int key) {
   struct node *link = (struct node*) malloc(sizeof(struct node));
   link->key = key;

   if(isEmpty()) {
   	  head = link;
      last = link;
   } else {
      last->next = link;
      link->prev = last;
   }
   last = link;
}


int deleteFirst() {
   struct node *tempLink = head;

   if(head->next == NULL){
      last = NULL;
   } else {
      head->next->prev = NULL;
   }

   head = head->next;
   return tempLink->key;
}


//Graph basis
typedef struct{
  int u, v;
  int direction;
  int capacity;
  int flow;
} Edge;

typedef struct {
  Edge vec[6];
} Vertex;


struct graph {
  long int V, E;
  Vertex *vertexes;
};

typedef struct graph *Graph;

Graph graphInit(int, int);
void graphDestroy();
void graphPrint();
void edmondsKarp();

Graph g;

Graph graphInit(int M, int N){
  g = (Graph)malloc(sizeof(struct graph));
  g->V= M * N;
  g->E= 0;
  g->vertexes = malloc((M*N) * sizeof(Vertex));
  int i, j, k;

  Edge edge;
  edge.capacity = -1;
  edge.u = -1;
  edge.v= -1;
  edge.flow = -1;

  for (i = 0; i < M ; i++) {
      for (j = 0; j < N ; j++) {
        for (k = 0; k < 6; k++) {
          g->vertexes[i * N + j].vec[k] = edge;
      }
    }
  }
  return g;
}


void adjListPrint(Vertex v) {
  int j;

  for (j = 0; j < 6; j++) {
    printf("%d -> %d, ", v.vec[j].u, v.vec[j].v);
  }
}


void graphPrint(){
  int i;
  for (i = 0; i < g->V; i++){
    printf("%d | ", i);
    adjListPrint(g->vertexes[i]);
    printf("\n");
  }
}

void graphDestroy(){
  free(g->vertexes);
  free(g);
}



//MAIN

int main(int argc, char const *argv[]) {
  int N, M, i, j, peso;
  int lp, cp;
  Edge edge;

  scanf("%d\n", &M);
  scanf("%d\n", &N);

  g = graphInit(M, N);

  for (i = 0; i < M; i++){
    for (j = 0; j < N; j++) {
      scanf("%d", &lp);
      edge.u =  i * N + j;
      edge.v = M * N + 1;
      edge.capacity = lp;
      edge.flow = 0;
      edge.direction = 5;
      g->vertexes[ i * N + j].vec[5] = edge;
    }
  }

  for (i = 0; i < M; i++){
    for (j = 0; j < N; j++) {
      scanf("%d", &cp);
      edge.v = i * N + j;
      edge.u = M * N;
      edge.capacity = cp;
      edge.direction = 0;
      edge.flow = min(g->vertexes[i * N + j].vec[5].capacity, cp);
      g->vertexes[i * N + j].vec[5].flow = min(g->vertexes[i * N + j].vec[5].capacity, cp);
      g->vertexes[i * N + j].vec[0] = edge;
    }
  }

  for (i = 0; i < M; i++){
    for (j = 0; j < N - 1; j++) {
      scanf("%d", &peso);

      edge.u = i * N + j;
      edge.v = i * N + j + 1;
      edge.capacity = peso;
      edge.flow = 0;
      edge.direction = 4;
      g->vertexes[i * N + j].vec[4] = edge;

      edge.u = i * N + j + 1;
      edge.v = i * N + j;
      edge.direction = 1;
      g->vertexes[i * N + j + 1].vec[1] = edge;
    }
  }


  for (i = 0; i < M - 1; i++){
    for (j = 0; j < N ; j++) {
      scanf("%d", &peso);

      edge.u = i * N + j;
      edge.v = (i + 1) * N + j;
      edge.capacity = peso;
      edge.flow = 0;
      edge.direction = 3;
      g->vertexes[i * N + j].vec[3] = edge;

      edge.u = (i + 1) * N + j;
      edge.v = i * N + j;
      edge.direction = 2;
      g->vertexes[(i + 1) * N + j].vec[2] = edge;
    }
  }

  //graphPrint();


  puts("");

  edmondsKarp();
  //graphDestroy(graph);

  return 0;
}


//EDMONDS KARP

void edmondsKarp() {
  int i, ver, direcao;
  long long int df;
  long long int flow = 0;
  int t = g->V + 2;
  Edge e;
  Edge pred[t];

  do {
  	for (i = 0; i < t; i++) {
	    pred[i].capacity = -1;
	    pred[i].v = -1;
	    pred[i].u = -1;
  	}

    for (i = 0; i < g->V; i++) {
      e = g->vertexes[i].vec[0];
      if (e.capacity > e.flow) {
        pred[e.v] = e;
        insertLast(e.v);
      }
    }

    while (!isEmpty()) {
      ver = deleteFirst();
      
      for (i = 1; i < 6; i++) {
        e = g->vertexes[ver].vec[i];
        if (pred[e.v].capacity == -1 && e.v != 6 && e.capacity > e.flow) {
          pred[e.v] = e;
          insertLast(e.v);
        }
      }
    }

    /*for (i = t-1 ; pred[i].capacity != -1; i = pred[i].u)
    	printf("%d -> %d\n c/direção: %d\n", pred[i].u, pred[i].v, pred[i].direction);
    puts("");*/

    if (pred[t - 1].capacity != -1) {
    	df = INT_MAX;
    	
    	for (i = t - 1; pred[i].capacity != -1; i = pred[i].u)
	    	df = min(df, pred[i].capacity - pred[i].flow);

    	g->vertexes[pred[t-1].u].vec[5].flow += df;
    	ver = pred[t-1].u;

    	for (i = ver ; pred[i].capacity != -1; i = pred[i].u) {
    		if (pred[i].u == 6) 
    			g->vertexes[i].vec[0].flow += df;

    		else {
    			direcao = pred[i].direction;
	    		g->vertexes[pred[i].u].vec[direcao].flow += df;
	    		//printf("%d -> %d tem flow: %d\n", g->vertexes[pred[i].u].vec[direcao].u, g->vertexes[pred[i].u].vec[direcao].v, g->vertexes[pred[i].u].vec[direcao].flow);

	    		if (direcao == 1) direcao = 4;
	    		else if (direcao == 3) direcao = 2;
	    		else if (direcao == 4) direcao = 1;
	    		else if (direcao == 2) direcao = 3;
	    		//printf("fc : %d -> %d tem flow: %d c/direcao: %d\n", g->vertexes[i].vec[direcao].u, g->vertexes[i].vec[direcao].v, g->vertexes[i].vec[direcao].flow, direcao);
		    	g->vertexes[i].vec[direcao].flow -= df;
		    }
	    }
	    flow += df;
    }

    printf("%lld\n", flow );
  } while (pred[t-1].capacity != -1);
}

