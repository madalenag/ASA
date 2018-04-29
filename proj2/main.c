#include <stdlib.h>
#include <stdio.h>
#include "limits.h"

#define FALSE 0
#define TRUE 1
#define UNDEFINED -1
#define min(a, b) (a<b ? a : b)

typedef struct node *link;

struct node {
  int v;
  link next;
};

struct vertex {
  int on_stack;
  int low_link;
  int index;
  int minScc;
};

struct graph {
  long int V, E;
  link *adj;
  struct vertex *vertexes;
};

typedef struct graph *Graph;

typedef struct {
  int u, v;
} Edge;

Graph graphInit(int);
void graphInsert(Graph, int, int);
void graphDestroy(Graph);
void graphPrint(Graph);
void tarjan(Graph g);
void connections(Graph g);
int eliminateDuplicates(Edge* v, Edge *vec, int dim);

link top;
int idx = 0;
int in = 0;
int j = 0;
int num = 0;

int main(int argc, char const *argv[]) {
  int N, M, u, v, i;
  Graph graph;

  scanf("%d\n", &N);
  scanf("%d\n", &M);

  if(N < 2 || M <1){
    exit(1);
  }

  graph = graphInit(N);

  for (i = 0; i < M; i++){
    scanf("%d %d", &v, &u);
    graphInsert(graph, v, u);
  }

  tarjan(graph);
  connections(graph);
  graphDestroy(graph);
  
  return 0;
}

link insertBegin(int v, link head){
  link x = (link)malloc(sizeof(struct node));
  x->v = v + 1;
  x->next = head;
  return x; 
}


Graph graphInit(int V){
  int v;
  Graph g = (Graph)malloc(sizeof(struct graph));
  g->V=V;
  g->E=0;
  g->adj = (link*)malloc(V * sizeof(struct node));
  g->vertexes = malloc(V * sizeof(struct vertex));
  
  for (v = 0; v < V; v++){
    g->vertexes[v].on_stack = FALSE;
    g->vertexes[v].low_link = -1;
    g->vertexes[v].index = -1;
    g->adj[v] = NULL;
  }
  return g;
}

void graphInsert(Graph g, int v, int u) {
  v--;
  u--;
  
  g->adj[v] = insertBegin(u, g->adj[v]);
  g->E++; 
}


void adjListPrint(link aux) {
  while(aux != NULL) {
    printf(("%d "),aux->v);
    aux = aux->next;
  }
  printf("\n");

}

void deleteList(link head) {
  link tmp;
  while (head != NULL) {
    tmp = head;
    head = head->next;
    free(tmp);
  }
}

void graphPrint(Graph g){
  int i;
  for (i = 0; i < g->V; i++){
    printf("%d | ", i+1);
    adjListPrint(g->adj[i]);    
  }
}

void graphDestroy(Graph g){
  int v;
  int V = g->V;

  for(v = 0; v < V; v++)
    deleteList(g->adj[v]);
  
  free(g->adj);
  free(g->vertexes);
  free(g);
}




void initStack() {
  top = NULL;
}

void push(int value) {
  link new = (link)malloc(sizeof(struct node));
  new->v = value;
  new->next = top;
  top = new;
}


int pop() {
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



void scc(int v, Graph g, int SCCs[]) {
  int u, k;
  long long int min;
  
  g->vertexes[v].low_link = idx;
  g->vertexes[v].index = idx;
  idx++;
  push(v);
  g->vertexes[v].on_stack = TRUE;

  link aux = g->adj[v];
  while(aux != NULL) {
    u = aux->v - 1;
    if (g->vertexes[u].index == UNDEFINED) {
      scc(u, g, SCCs);
      int low_link_v = g->vertexes[v].low_link;
      int low_link_u = g->vertexes[u].low_link;
      g->vertexes[v].low_link= min(low_link_v, low_link_u);
    }
    else if (g->vertexes[u].on_stack) {
      int low_link_v = g->vertexes[v].low_link;
      int index_u = g->vertexes[u].index;
      g->vertexes[v].low_link= min(low_link_v, index_u);  
    }
    aux = aux->next;
  }

  if (g->vertexes[v].low_link == g->vertexes[v].index) {
    num++;
    min = INT_MAX;
    do {
      u = pop();
      SCCs[in++] = u;
      min = min(u, min);
      g->vertexes[u].on_stack = FALSE;  
    } while ( u != v);

    for (k = j; k < in; k++) 
      g->vertexes[SCCs[k]].minScc = min;    
    j = in;
  }

}


void tarjan(Graph g) {
  initStack();
  int SCCs[g->V];
  int i;
  for (i = 0; i < g->V; i++) {
    if (g->vertexes[i].index == UNDEFINED){
      scc(i, g, SCCs);
    }
  }

  printf("%d\n", num);
}

int comparator(const void *p, const void *q) {
  int u1 = ((Edge *)p)->u;
  int u2 = ((Edge *)q)->u;
  int v1 = ((Edge *)p)->v;
  int v2 = ((Edge *)q)->v;

  if (u1 == u2)
    return v1 - v2;
  return u1 - u2;
}


int eliminateDuplicates(Edge* v, Edge *vec, int dim) {
  int i;
  int count = 0;

  Edge edge;
  edge.u = 0;
  edge.v = 0;


  for (i = 0; i < dim; i++) {
    if (v[i].u != edge.u || v[i].v != edge.v) {
      vec[count].u = v[i].u;
      vec[count].v = v[i].v;
      edge.u = v[i].u;
      edge.v = v[i].v;
      count++;
    }
  }
  return count;
}


void connections(Graph g) {
  int i;
  Edge *v = (Edge*) malloc (sizeof(Edge) * g->E);
  Edge *vec = (Edge*) malloc (sizeof(Edge) * g->E);
  int count = 0;

  for (i = 0; i < g->V; i++) {
    link aux = g->adj[i];
    while(aux != NULL) {
      Edge edge;
      edge.u = g->vertexes[i].minScc;
      edge.v = g->vertexes[aux->v - 1].minScc;
      if (edge.u != edge.v) 
        v[count++] = edge;
      aux = aux->next;
    }
  }

  qsort(v, count, sizeof(Edge), comparator);
  count = eliminateDuplicates(v, vec, count);

  printf("%d\n", count ); 

  for (i = 0; i < count; i++)
    printf("%d %d\n", vec[i].u + 1, vec[i].v + 1);

  free(v);
  free(vec);
}
