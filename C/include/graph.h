#ifndef __GRAPH_H__
#define __GRAPH_H__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#ifndef GRAPH_ATTRIBUTES
#define GRAPH_ATTRIBUTES extern
#endif//GRAPH_ATTRIBUTES
 

#include <stdbool.h>
#include <stddef.h>

typedef struct {
  unsigned char *edges;
  size_t node_count;
} Graph;

GRAPH_ATTRIBUTES Graph* graph_create(size_t num_nodes);
GRAPH_ATTRIBUTES void graph_destroy(Graph* graph);
GRAPH_ATTRIBUTES bool graph_connect(Graph* graph, size_t from, size_t to);
GRAPH_ATTRIBUTES bool graph_connected(Graph* graph, size_t from, size_t to);
GRAPH_ATTRIBUTES void graph_print(Graph* graph);


#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__GRAPH_H__

// #define GRAPH_IMPLEMENTATIONS
#ifdef GRAPH_IMPLEMENTATIONS
#ifndef __GRAPH_IMP__
#define __GRAPH_IMP__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

GRAPH_ATTRIBUTES Graph* graph_create(size_t num_nodes) {
  Graph* g = (Graph*)calloc(1, sizeof(Graph));
  if(NULL == g) {
    fprintf(stderr, "Calloc error\n");
    exit(1);
  }
  g->node_count = num_nodes;
  g->edges = (unsigned char*)calloc((int)ceil(log2(num_nodes))*2, sizeof(unsigned char));
  if(NULL == g->edges) {
    fprintf(stderr, "Calloc error\n");
    exit(1);
  }
  return g;
}

GRAPH_ATTRIBUTES void graph_destroy(Graph* graph) {
  if(NULL == graph) return;
  free(graph->edges);
  free(graph);
}

GRAPH_ATTRIBUTES bool graph_connect(Graph* graph, size_t from, size_t to) {
  if(NULL == graph || from >= graph->node_count || to >= graph->node_count) return false;
  if(graph_connected(graph, from, to)) return false;
  size_t bit = to*graph->node_count+from;
  graph->edges[bit/8] |= (1 << (bit%8));
  return true;
}

GRAPH_ATTRIBUTES bool graph_connected(Graph* graph, size_t from, size_t to) {
  if(NULL == graph || from >= graph->node_count || to >= graph->node_count) return false;
  size_t bit = to*graph->node_count+from;
  return (graph->edges[bit/8] >> (bit%8)) & 1;
}

GRAPH_ATTRIBUTES void graph_print(Graph* graph) {
  for(size_t from = 0; from < graph->node_count; from++) {
    for(size_t to = 0; to < graph->node_count; to++) {
      if(graph_connected(graph, from, to)) printf("%ld -> %ld\n", from, to);
    }
  }

}


#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__GRAPH_IMP__
#undef GRAPH_IMPLEMENTATIONS
#endif//GRAPH_IMPLEMENTATIONS

