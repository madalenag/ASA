#include <stdlib.h>
#include <stdio.h>
#include "limits.h"

#define FALSE 0
#define TRUE 1
#define UNDEFINED -1
#define min(a, b) (a<b ? a : b)

/*typedef struct node *link;

struct node {
  int v;
  link next;
};

link top = NULL;

link NEW(int v){
  link x = (link)malloc(sizeof(struct node));
  x->v = v;
  x->next = NULL;
  return x; 
}


void insertFirst(int v)
{
  link x;
  if(top == NULL)
   top = NEW(v);
  
  for(x = top; x->next != NULL; x = x->next)
    ;
  x->next = NEW(v);
}


int deleteLast() {
  int v;
  link old;

  if (!(top == NULL)) {
    v = top->v;
    old = top;
    top = top->next;
    free(old);
    return v;
  }
  return -1;
}

int isEmpty() {
  return top == NULL;
}

void printStack() {
  link x;
  if(top == NULL)
   return;
  
  for(x = top; x->next != NULL; x = x->next)
    printf("%d\n", x->v);
}

*/

struct node {
   int key;
   struct node *next;
   struct node *prev;
};

struct node *head = NULL;
struct node *last = NULL;


int isEmpty() {
   return head == NULL;
}


void displayForward() {
   struct node *ptr = head;
   printf("\n[ ");

   while(ptr != NULL) {
      printf("(%d) ",ptr->key);
      ptr = ptr->next;
   }
   printf(" ]");
}



void insertFirst(int key) {


   struct node *link = (struct node*) malloc(sizeof(struct node));
   link->key = key;
  
   if(isEmpty()) {

      last = link;
   } else {

      head->prev = link;
   }


   link->next = head;
  

   head = link;
}

struct node* deleteLast() {

   struct node *tempLink = last;
  

   if(head->next == NULL) {
      head = NULL;
   } else {
      last->prev->next = NULL;
   }
  
   last = last->prev;
  

   return tempLink;
}



typedef struct{
  int u, v;
  int direction;
  int capacity;
  int flow;
} Edge;

typedef struct {
  int classification;
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
void classificator();

Graph g;
long long int flow = 0;

Graph graphInit(int M, int N){
  g = (Graph)malloc(sizeof(struct graph));
  g->V= M * N;
  g->vertexes = malloc((M*N) * sizeof(Vertex));
  int i, j, k;

  Edge edge;
  edge.capacity = -1;
  edge.u = -1;
  edge.v= -1;
  edge.flow = -1;
  edge.direction = -1;

  for (i = 0; i < M ; i++) {
      for (j = 0; j < N ; j++) {
        for (k = 0; k < 6; k++) {
          g->vertexes[i * N + j].vec[k] = edge;
          g->vertexes[i * N + j].classification = 0;
      }
    }
  }
  return g;
}


void adjListPrint(Vertex v) {
  int j;

  for (j = 0; j < 6; j++) {
    printf("%d, ",v.classification);
  }
}


void graphPrint(int N, int M){
  printf("%lld\n\n", flow);
  int i, j;
  for (i = 0; i < M; i++){
    for (j = 0; j < N; j++) {
      if (g->vertexes[i * N + j].classification)
        printf("P ");
      else
        printf("C ");
    }
    puts("");
  }
}

void graphDestroy(){
  free(g->vertexes);
  free(g);
}



int main(int argc, char const *argv[]) {
  int N, M, i, j, peso;
  int lp, cp;
  Edge edge;

  scanf("%d", &M);
  scanf("%d", &N);

  g = graphInit(M, N);

  /*printf("here");*/

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
      flow += min(g->vertexes[i * N + j].vec[5].capacity, cp);
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
  
  edmondsKarp();
  classificator();
  graphPrint(N, M);
  graphDestroy();

  return 0;
}



void edmondsKarp() {
  int i, ver = 0, direcao;
  long long int df;
  int t = g->V + 2;
  Edge e;
  Edge* pred = (Edge*) malloc(sizeof(Edge)*t);
  struct node *x;


  do {
  	for (i = 0; i < t; i++) {
	    pred[i].capacity = -1;
	    pred[i].v = -1;
	    pred[i].u = -1;
      pred[i].direction = -1;
      pred[i].flow = -1;
  	}

    for (i = 0; i < g->V; i++) {
      e = g->vertexes[i].vec[0];
      if (e.capacity > e.flow && e.v != -1) {
        /*printf("1 %d\n", e.v);*/
        pred[e.v] = e;
        insertFirst(e.v);  
      }
    }


    while (!isEmpty()) {
      x = deleteLast();
      ver = x->key;
      free(x);
      
      for (i = 1; i < 6; i++) {
        e = g->vertexes[ver].vec[i];
        if (e.v != -1 && pred[e.v].capacity == -1 && e.capacity > e.flow) {
          pred[e.v] = e;
          if (e.v != t-1) {
            /*printf("2 %d\n", e.v);*/
            insertFirst(e.v);
          }
        }
      }
    }

    if (pred[t - 1].capacity != -1) {
    	df = INT_MAX;
    	
    	for (i = t - 1; pred[i].capacity != -1; i = pred[i].u) 
	    	df = min(df, pred[i].capacity - pred[i].flow);

    	g->vertexes[pred[t-1].u].vec[5].flow += df;
    	ver = pred[t-1].u;

    	for (i = ver ; pred[i].capacity != -1; i = pred[i].u) {
    		if (pred[i].u == g->V) 
    			g->vertexes[i].vec[0].flow += df;

    		else {
    			direcao = pred[i].direction;
	    		g->vertexes[pred[i].u].vec[direcao].flow += df;          

	    		if (direcao == 1) direcao = 4;
	    		else if (direcao == 3) direcao = 2;
	    		else if (direcao == 4) direcao = 1;
	    		else if (direcao == 2) direcao = 3;
          g->vertexes[i].vec[direcao].flow = g->vertexes[i].vec[direcao].capacity - df;
		    }
	    }
	    flow += df;
      /*printf("%lld", flow);*/
    }
  } while (pred[t-1].capacity != -1);

  free(pred);

}


void classificator() {
  int i;
  Edge e;
  int ver = 0;
  int t = g->V + 2;
  struct node *x;

  for (i = 0; i < g->V; i++) {
      e = g->vertexes[i].vec[0];
      if (e.capacity > e.flow && e.v != -1) {
        g->vertexes[i].classification = 1;
        insertFirst(e.v);
      }
  }

  while (!isEmpty()) {
    x= deleteLast();
    ver = x->key;
    free(x);

      for (i = 1; i < 6; i++) {
        e = g->vertexes[ver].vec[i];
        if (e.v != -1) {
          if (g->vertexes[e.v].classification == 0 && e.capacity > e.flow) {
            if (e.v != t-1) {
              insertFirst(e.v);
              g->vertexes[e.v].classification = 1;
            }
          }
        }
      }
  }
}