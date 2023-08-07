#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_NODES 20

struct node {
    unsigned dist[MAX_NODES];
    unsigned from[MAX_NODES];
}rt[10];

int main() {
    int costmat[MAX_NODES][MAX_NODES];
    int nodes, i, j, k, count = 0;

    printf("Enter the number of nodes: ");
    scanf("%d", &nodes);

    printf("Enter the cost matrix:\n");
    for (i = 0; i < nodes; i++) {
        for (j = 0; j < nodes; j++) {
            scanf("%d", &costmat[i][j]);
        }
    }

    for (i = 0; i < nodes; i++) {
        for (j = 0; j < nodes; j++) {
            costmat[i][i] = 0;
            rt[i].dist[j] = costmat[i][j];
            rt[i].from[j] = j;
        }
    }

    do {
        count = 0;
        for (i = 0; i < nodes; i++) {
            for (j = 0; j < nodes; j++) {
                for (k = 0; k < nodes; k++) {
                    int new_dist = rt[i].dist[k] + rt[k].dist[j];
                    if (rt[i].dist[j] > new_dist) {
                        rt[i].dist[j] = new_dist;
                        rt[i].from[j] = k;
                        count++;
                    }
                }
            }
        }
    } while (count != 0);

    for (i = 0; i < nodes; i++) {
        printf("\n\nFOR ROUTER %d\n", i + 1);
        for (j = 0; j < nodes; j++) {
            printf("node %d via %d distance %d\n", j + 1, rt[i].from[j] + 1, rt[i].dist[j]);
        }
    }

    return 0;
}
