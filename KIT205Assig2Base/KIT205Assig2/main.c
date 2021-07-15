#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

int** make_dem(int size, int roughness) {
	int seed = time(NULL);
	srand(seed);
	int** dem = malloc(size * sizeof * dem);
	for (int x = 0; x < size; x++) {
		dem[x] = malloc(size * sizeof * dem[x]);
		for (int y = 0; y < size; y++) {
			dem[x][y] = -1;
		}
	}
	int r = roughness;

	dem[0][0] = 50 - r / 2 + rand() % r;
	dem[size - 1][0] = 50 - r / 2 + rand() % r;
	dem[0][size - 1] = 50 - r / 2 + rand() % r;
	dem[size - 1][size - 1] = 50 - r / 2 + rand() % r;

	for (int step = (size - 1); step > 0; step /= 2) {
		r = r > 1 ? r / 2 : r;
		if (r < 1) r = 1;
		for (int cx = 0; cx < (size - 1) / step; cx++) {
			for (int cy = 0; cy < (size - 1) / step; cy++) {
				int a = dem[cx * step][cy * step];
				int b = dem[cx * step + step][cy * step];
				int c = dem[cx * step][cy * step + step];
				int d = dem[cx * step + step][cy * step + step];

				dem[cx * step + step / 2][cy * step + step / 2] = (a + b + c + d) / 4 + rand() % r - r / 2;

				dem[cx * step + step / 2][cy * step] = (a + b) / 2 + rand() % r - r / 2;
				dem[cx * step][cy * step + step / 2] = (a + c) / 2 + rand() % r - r / 2;
				dem[cx * step + step][cy * step + step / 2] = (b + d) / 2 + rand() % r - r / 2;
				dem[cx * step + step / 2][cy * step + step] = (c + d) / 2 + rand() % r - r / 2;
			}
		}
	}
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			dem[x][y] = dem[x][y] < 0 ? 0 : dem[x][y];
			dem[x][y] = dem[x][y] > 99 ? 99 : dem[x][y];
		}
	}
	return dem;
}

int risk_func(int diff) {
	return 1 + diff * diff;
}

void print_2D(int** array2D, int size) {
	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			if (array2D[x][y] >= 0) {
				printf("%2d ", array2D[x][y]);
			}
			else {
				printf("() ");
			}
		}
		printf("\n");
	}
}

void print_2D_ascii(int** array2D, int size) {
	char* shades = " .-:=+*#%@";
	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			if (array2D[x][y] >= 0) {
				char shade = shades[array2D[x][y] * 10 / 100];
				printf("%c%c", shade, shade);
			}
			else {
				printf("()");
			}
		}
		printf("\n");
	}
}

Graph construct_graph(Graph G, int vertices, int size, int** dem) {
	for (int start = 0; start < vertices; start++) {
		int x = start / size;
		int y = start % size;
		int destinations[4];//North, East, South, West 4 destinations
		int moves = 0;
		//moving North
		if (x != 0) {
			destinations[moves++] = start - size;
		}
		//moving East
		if (y != size - 1) {
			destinations[moves++] = start + 1;
		}
		//moving South
		if (x != size - 1) {
			destinations[moves++] = start + size;
		}
		//moving West
		if (y != 0) {
			destinations[moves++] = start - 1;
		}


		for (int i = 0; i < moves; i++) {
			int dest = destinations[i];

			int dest_x = dest / size;
			int dest_y = dest % size;
			//passing the difference between start and destination points through risk function
			int risk = risk_func(dem[dest_x][dest_y] - dem[x][y]);

			if (start > 0 || start <= G.V) {
				if (dest > 0 || dest <= G.V)
				{
					EdgeNodePtr node = malloc(sizeof * node);
					node->edge.to_vertex = dest;
					node->edge.weight = risk;
					node->next = G.edges[start].head;
					G.edges[start].head = node;
				}
			}
		}
	}
	return G;
}
/*
function for copying a DEM and set heights changed to -1 for any vertex in given path
Returns a copy of DEM
*/
int** copy_dem(int size, int** dem, EdgeList path) {
	int** copied_dem = malloc(size * sizeof * copied_dem);
	for (int x = 0; x < size; x++) {
		copied_dem[x] = malloc(size * sizeof * copied_dem[x]);

		for (int y = 0; y < size; y++) {
			copied_dem[x][y] = dem[x][y];
		}
	}
	EdgeNodePtr current = path.head;

	while (current != NULL) {
		int vertex = current->edge.to_vertex;

		int x = vertex / size;
		int y = vertex % size;
		//height changed to -1 for any node in the path
		copied_dem[x][y] = -1;

		current = current->next;
	}
	return copied_dem;
}

void destroy_all(Graph* G, int** dem, int** copied_dem, int size) {
	//destroying copied dem first
	for (int x = 0; x < size; x++) {
		free(copied_dem[x]);
	}
	free(copied_dem);

	//destroying the graph
	for (int v = 0; v < G->V; v++) {
		EdgeNodePtr current = G->edges[v].head;

		while (current != NULL) {
			EdgeNodePtr to_free = current;
			current = current->next;
			free(to_free);
		}
	}
	free(G->edges);

	//destroying the main dem
	for (int x = 0; x < size; x++) {
		free(dem[x]);
	}
	free(dem);
}
int main() {
	//Generating a dem
	const int SIZE = 33;
	int** dem = make_dem(SIZE, SIZE * 4);
	print_2D(dem, SIZE);
	//print_2D_ascii(dem, 33);
	const int VERTICES = SIZE * SIZE;
	//Creating a new graph
	Graph G;
	G.V = VERTICES;
	G.edges = malloc(G.V * sizeof * G.edges);

	for (int v = 0; v < G.V; v++) {
		G.edges[v].head = NULL;
	}
	//converting it to an adjacency list 
	G = construct_graph(G, VERTICES, SIZE, dem);
	//creating distance and previous array for dijkstra function
	int* distance = malloc(G.V * sizeof * distance);
	int* previous = malloc(G.V * sizeof * previous);
	dijkstra(&G, 0, distance, previous);
	// creating edgelist for the getpath function
	EdgeList path;
	path.head = NULL;
	get_path(SIZE * SIZE - 1, previous, &path);
	//Copying the DEM and getting the height changed to -1 for the path
	int** copied_dem = copy_dem(SIZE, dem, path);
	//Print the copied DEM to show the path
	print_2D_ascii(copied_dem, SIZE);
	//Freeing memory
	destroy_all(&G, dem, copied_dem, SIZE);
	free(distance);
	free(previous);
	//code to keep visual studio console running
	int stop;
	printf("Freed all memory. ");
	scanf_s("%d", &stop);

	return 0;
}