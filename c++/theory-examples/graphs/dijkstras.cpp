// reference: http://www.geeksforgeeks.org/greedy-algorithms-set-6-dijkstras-shortest-path-algorithm/
//
#include <iostream> 
#include <vector>

using namespace std;

const int V = 9;
const int MAX_VALUE=9999;

int minDistance( int dist[V], bool visited[V] ){

    // set our min index
    int min_index;

    // min value
    int min_dist = MAX_VALUE;

    // iterate over every node
    for( size_t i=0; i<V; i++ ){
        if( visited[i] == false && dist[i] < min_dist ){
            min_dist = dist[i];
            min_index = i;
        }

    }
    
    return min_index;
}

void dijkstras( int graph[V][V], int start ){

    /// Distances between the start and itself
    int  dist[V];

    /// Have we visited this node yet
    bool visited[V];

    /// initialize our arrays
    for( int i=0; i<V; i++ ){
        dist[i] = MAX_VALUE;
        visited[i] = false;
    }

    /// Set the source to 0 in order to search it first
    dist[start] = 0;

    // iterate over every vertice
    for( int i=0; i<V-1; i++ ){

        // find the min distance from vertices not yet processed
        int u = minDistance( dist, visited );

        // mark the node as visited
        visited[u]=true;

        // compare every other node which touches this node
        for( size_t v=0; v<V; v++ ){
            if( visited[v] == false && graph[u][v] > 0 && dist[u] != MAX_VALUE && dist[u] + graph[u][v] < dist[v] ){
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    for( size_t i=0; i<V; i++ ){
        cout << "Node: " << i << ", dist: " << dist[i] << endl;
    }

}



int main( int argc, char* argv[] ){

    /* Let us create the example graph discussed above */
    int graph[V][V] = {{0,  4, 0,  0,  0,  0, 0,  8, 0},
                       {4,  0, 8,  0,  0,  0, 0, 11, 0},
                       {0,  8, 0,  7,  0,  4, 0,  0, 2},
                       {0,  0, 7,  0,  9, 14, 0,  0, 0},
                       {0,  0, 0,  9,  0, 10, 0,  0, 0},
                       {0,  0, 4,  0, 10,  0, 2,  0, 0},
                       {0,  0, 0, 14,  0,  2, 0,  1, 6},
                       {8, 11, 0,  0,  0,  0, 1,  0, 7},
                       {0,  0, 2,  0,  0,  0, 6,  7, 0}
                    };
    
    // call dijkstra's algorithm
    dijkstras( graph, 0);
}
