#include "graph.h"
#include <limits.h>
#include <stdlib.h>
void dijkstra(Graph* self, int start, int* distance, int* previous)
{
	int* Q = malloc(self->V * sizeof * Q);

	for (int v = 0; v < self->V; v++) {
		distance[v] = INT_MAX;
		previous[v] = -1;
		Q[v] = 1;
	}
	distance[start] = 0;
	int vertex_count = 0;
	while (vertex_count < self->V) {
		int u = -1;
		for (int v = 0; v < self->V; v++) {
			if (Q[v] && (u == -1 || distance[v] < distance[u])) {
				u = v;
			}
		}
		Q[u] = 0;
		vertex_count++;
		EdgeNodePtr current = self->edges[u].head;
		while (current != NULL) {
			int alt = distance[u] + current->edge.weight;
			int dest = current->edge.to_vertex;

			if (Q[dest] && alt < distance[dest]) {
				distance[dest] = alt;
				previous[dest] = u;
			}

			current = current->next;
		}
	}
}

void get_path(int end, int* previous, EdgeList* path)
{
	int v = end;
	while (v != -1) {
		EdgeNodePtr node = malloc(sizeof * node);
		node->edge.to_vertex = v;
		node->edge.weight = 0;
		node->next = path->head;
		path->head = node;
		v = previous[v];
	}
}
