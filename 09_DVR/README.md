`DVR` stands for `Distance-Vector Routing`, which is a routing algorithm used in computer networks to determine the best path for data packets to travel from a source node to a destination node. It's a decentralized routing algorithm where each node maintains information about the distance (cost) to reach other nodes in the network and the next hop node to reach them.

The theory behind the Distance-Vector Routing algorithm is based on the concept of `exchanging routing information between neighboring nodes iteratively until convergence is achieved.`
<br></br>
Sure, I'd be happy to explain the code to you line by line:

```c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_NODES 20
```
These are preprocessor directives that include the necessary header files for input/output operations (`stdio.h`), memory allocation (`stdlib.h`), and mathematical functions (`math.h`). The `#define` directive is used to define a constant `MAX_NODES` with a value of 20.

```c
struct node {
    unsigned dist[MAX_NODES];
    unsigned from[MAX_NODES];
};
```
Here, a structure named `node` is defined. It contains two arrays: `dist` and `from`, both with a maximum size of `MAX_NODES`. This structure seems to be used to store distance and routing information for nodes.

```c
int main() {
    int costmat[MAX_NODES][MAX_NODES];
    int nodes, i, j, k, count = 0;
```
The `main` function starts here. It declares several variables, including `costmat` (a 2D array to store the cost matrix), `nodes` (the number of nodes in the network), and loop control variables `i`, `j`, `k`, and `count`.

```c
    printf("Enter the number of nodes: ");
    scanf("%d", &nodes);
```
The program prompts the user to input the number of nodes in the network using the `scanf` function and stores the value in the `nodes` variable.

```c
    printf("Enter the cost matrix:\n");
    for (i = 0; i < nodes; i++) {
        for (j = 0; j < nodes; j++) {
            scanf("%d", &costmat[i][j]);
        }
    }
```
This loop is used to input the cost matrix for the network. The user is prompted to input the cost values for each connection between nodes. The values are stored in the `costmat` array.

```c
    // Skipping lines that populate the initial distance and routing information for each node.
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
```
This is the core of the code and implements the distance-vector routing algorithm. It iteratively updates the routing information for each node in the network based on the cost matrix. The nested loops check if there's a better route (lower cost) to get from node `i` to node `j` through node `k`. If such a route is found, the routing information is updated, and `count` is incremented. The process continues until no further updates are needed (i.e., `count` remains zero).

```c
    // Skipping lines that print the final routing information for each node.
    return 0;
}
```
The `main` function ends here. It returns 0, indicating successful execution of the program.

The code essentially implements the distance-vector routing algorithm to find the shortest paths and corresponding routing information between nodes in a network based on the provided cost matrix.