#include <stdlib.h>
#include <stdio.h>

#define INF 999999

typedef struct vertex{
  int label;
  int cost;
}vertex;

typedef struct Path{
  int* vertexes;//vertexes composing the path
  int length;//amount of vertexes in the path
  int cost;//cost of the path
}Path;

#define PRINT(X,L)\
do{\
  int _i;\
  printf("Labels: ");\
  for(_i=0;_i<L;_i++){\
  if(X[_i].label < 9 )printf("0%d ",X[_i].label);\
  else printf("%d ",X[_i].label);}\
  printf("\n");\
  printf("Values: ");\
  for(_i=0;_i<L;_i++){\
  if(X[_i].cost!=INF && X[_i].cost < 9){printf("0%d ",X[_i].cost);}\
  else {if(X[_i].cost!=INF && X[_i].cost > 9)printf("%d ",X[_i].cost);\
  else printf("IN ");}}}\
  while(0)


#define SWAP(HEAP,A,B) vertex k; k.label=HEAP[A].label; k.cost = HEAP[A].cost; HEAP[A] = HEAP[B]; HEAP[B] = k

void heapfyVertex(vertex* heap,int size, int index){
  int aux;

  if(2*index+2 < size){
    aux = heap[2*index+1].cost < heap[2*index+2].cost ? 2*index+1:2*index+2;
    aux = heap[aux].cost < heap[index].cost? aux: index;

    if(aux != index){
      SWAP(heap,index,aux);
      heapfyVertex(heap,size,aux);
    }

    return;
  }
  if(2*index+1 < size && heap[2*index+1].cost < heap[index].cost){
      SWAP(heap,index,2*index+1);
      heapfyVertex(heap,size,2*index+1);
      return;
  }
}

void heapfy(vertex* heap,int size){
  if(size <= 1) return;
  int i,aux;
  for(i = size-1; i >= 0; i--){
    heapfyVertex(heap,size,i);
  }
}

int find(vertex* heap,int length,int index){
  int i;
  for(i = 0;i < length; i++)
    if(heap[i].label == index) return i;
  return -1;
}

Path* generatePath(int* parrent,int total, int source, int target){
  Path* result = malloc(sizeof(Path));
  result->vertexes = malloc(sizeof(int)*total);
  int length = 0;

  int current = parrent[target];
  result->vertexes[length] = target;

  while(current!= -1){
    result->vertexes[++length] = current;
    current = current != -1 ? parrent[current]: -1;
  }
  result->length = length;
  return result;

}

Path* dijkstra(int length,int** graph,int source, int target){
  int i,destiny,newPath, size = length;
  vertex* heap = malloc(length * sizeof *heap);
  int parrent[length];
  parrent[source] = -1;

  for(i = 0; i < length; i ++){
    heap[i].cost = graph[source][i];
    heap[i].label = i;

    if(heap[i].label!= source )
      parrent[i] = source;
  } heapfy(heap,length);

  while(size){
    if(heap[0].label == target) {
      Path* response = generatePath(parrent,length,source,target);
      response->cost = heap[0].cost;
      free(heap);
      return response;
    }

    SWAP(heap,0,size-1);
    size--;
    heapfy(heap,size);



    for(i=0;i<size;i++){
      newPath = (heap[0].cost + graph[heap[0].label][heap[i].label]);
      destiny = find(heap,size,heap[i].label);

      if(newPath <  heap[destiny].cost){
        heap[destiny].cost = newPath; //updating the min cost
        parrent[heap[i].label] = heap[0].label; //updating the path
      }
    }

  }
  free(heap);
  return NULL;
}


int main(){
  //graph for testing
  int sample_graph[6][6] =
  {
    {0 , 2, 13, INF, INF, INF},
    {2,  0, INF, 4 , INF, INF},
    {13, INF, 0, 2, INF, 10},
    {INF, 4, 2, 0, 5, INF},
    {INF, INF, INF, 5, 0, 2},
    {INF, INF, 10, INF, 2, 0}
  };

  int length = 6;
  int** grafo = malloc(sizeof(int*)*length);
  int i,j,source,target;

  printf("\n******************************************\n");
  printf("             Dijkstra algorithm\n");
  printf("******************************************\n");
  printf(">Graph represented by adjacency matrix\n");
  printf(">>IN = Infinity\n\n");
  printf("(!)base graph for testing\n");
  for(i = 0; i < length;i++){
    grafo[i] = malloc(sizeof(int)*length);
    for(j=0;j<6;j++){
      grafo[i][j] = sample_graph[i][j];
      if(grafo[i][j] > 9 && grafo[i][j] != INF ) printf("%d ",sample_graph[i][j]);
      else if(grafo[i][j] != INF) printf("0%d ",sample_graph[i][j]);
      else printf("IN ");
    }printf("\n");
  }
  printf("\nFinding the path with minimum cost:\n");
  printf(">Enter the source vertex:");
  scanf("%d",&source);
  printf(">Enter the target vertex:");
  scanf("%d",&target);

  if(source >= 0 && source < length && target >= 0 && target < length) {
    Path* result = dijkstra(length,grafo,source,target);
    printf("\nThe cost to go from the vertex (%d) to (%d) is %d\n", source,target,result->cost);
    printf("The path is: ");
    for(i=result->length;i>=0;i--){
      if(i != 0)printf("(%d) -> ",result->vertexes[i]);
      else printf("(%d)\n",result->vertexes[i]);
    }

    free(result->vertexes);
    free(result);
  }
  else{
    printf("\nERROR: Invalid source or target vertexes!!\n");
  }

  for(i=0;i<length;i++)
    free(grafo[i]);
  free(grafo);

  return 0;
}
