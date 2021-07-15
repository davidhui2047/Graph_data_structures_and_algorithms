#pragma once

typedef struct edge {
	int to_vertex;
	int weight;
} Edge;

typedef struct edgeNode {
	Edge edge;
	struct edgeNode *next;
} *EdgeNodePtr;

typedef struct edgeList {
	EdgeNodePtr head;
} EdgeList;

typedef struct graph {
	int V;
	EdgeList *edges;
} Graph;

// calculates shortest paths from start
// stores the distances in the distance array
// stores the path reconstruction info in the previous array
void dijkstra(Graph* self, int start, int *distance, int *previous);

// builds a path to the end vertex based on the path info stored in the previous array
// stores the path as a list of "edges" in path
void get_path(int end, int *previous, EdgeList *path);