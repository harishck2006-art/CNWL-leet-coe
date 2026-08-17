#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STOPS 1000000

typedef struct {
    int* data;
    int size;
    int cap;
} Vector;

void initVector(Vector* v) {
    v->data = NULL;
    v->size = 0;
    v->cap = 0;
}

void pushVector(Vector* v, int val) {
    if (v->size == v->cap) {
        v->cap = v->cap == 0 ? 2 : v->cap * 2;
        v->data = (int*)realloc(v->data, v->cap * sizeof(int));
    }
    v->data[v->size++] = val;
}

int numBusesToDestination(int** routes, int routesSize, int* routesColSize, int source, int target) {
    if (source == target) return 0;

    int max_stop = -1;
    for (int i = 0; i < routesSize; i++) {
        for (int j = 0; j < routesColSize[i]; j++) {
            if (routes[i][j] > max_stop) {
                max_stop = routes[i][j];
            }
        }
    }
    if (source > max_stop || target > max_stop) return -1;

    Vector* stopToBuses = (Vector*)malloc((max_stop + 1) * sizeof(Vector));
    for (int i = 0; i <= max_stop; i++) {
        initVector(&stopToBuses[i]);
    }

    for (int i = 0; i < routesSize; i++) {
        for (int j = 0; j < routesColSize[i]; j++) {
            pushVector(&stopToBuses[routes[i][j]], i);
        }
    }

    int* queue = (int*)malloc((routesSize + max_stop + 2) * sizeof(int));
    int head = 0, tail = 0;

    int* busVisited = (int*)calloc(routesSize, sizeof(int));
    int* stopVisited = (int*)calloc(max_stop + 1, sizeof(int));

    queue[tail++] = source;
    stopVisited[source] = 1;

    int busesTaken = 0;

    while (head < tail) {
        int levelSize = tail - head;
        for (int i = 0; i < levelSize; i++) {
            int currStop = queue[head++];
            if (currStop == target) {
                for (int j = 0; j <= max_stop; j++) free(stopToBuses[j].data);
                free(stopToBuses);
                free(queue);
                free(busVisited);
                free(stopVisited);
                return busesTaken;
            }

            Vector buses = stopToBuses[currStop];
            for (int j = 0; j < buses.size; j++) {
                int bus = buses.data[j];
                if (busVisited[bus]) continue;
                busVisited[bus] = 1;

                for (int k = 0; k < routesColSize[bus]; k++) {
                    int nextStop = routes[bus][k];
                    if (!stopVisited[nextStop]) {
                        stopVisited[nextStop] = 1;
                        queue[tail++] = nextStop;
                    }
                }
            }
        }
        busesTaken++;
    }

    for (int j = 0; j <= max_stop; j++) free(stopToBuses[j].data);
    free(stopToBuses);
    free(queue);
    free(busVisited);
    free(stopVisited);

    return -1;
}
