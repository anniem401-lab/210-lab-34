// COMSC-210 | Lab 34 | Annie Morales
// IDE used: Visual Studio Code

/*
    Steps Completed:
    Step 1
*/

/*
    To Do: Step 2
    Change the graph by deleting atleast 2 nodes and adding atleast 6 nodes. Change the weights too.
*/

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

const int SIZE = 10; // Initial size: 7

struct Edge {
    int src, dest, weight;
};

typedef pair<int, int> Pair; // Creates alias 'Pair' for the pair<int,int> data type

class Graph {
public:
    // a vector of vectors of Pairs to represent an adjacency list
    vector<vector<Pair>> adjList;

    // Graph Constructor
    Graph(vector<Edge> const &edges) {
        // resize the vector to hold SIZE elements of type vector<Edge>
        adjList.resize(SIZE);

        // add edges to the directed graph
        for (auto &edge: edges) {
            int src = edge.src;
            int dest = edge.dest;
            int weight = edge.weight;

            // insert at the end
            adjList[src].push_back(make_pair(dest, weight));
            // for an undirected graph, add an edge from dest to src also
            adjList[dest].push_back(make_pair(src, weight));
        }
    }

    // Print the graph's adjacency list
    void printGraph() {
        cout << "\nGraph's adjacency list:" << endl;
        for (int i = 0; i < adjList.size(); i++) {
            cout << i << " --> ";
            for (Pair v : adjList[i])
                cout << "(" << v.first << ", " << v.second << ") ";
            cout << endl;
        }
    }
    
    void DFSUntil(int v, vector<bool>& visited) {
        visited[v] = true;
        cout << v << " ";

        for (auto &edge : adjList[v]) {
            int next = edge.first;
            if (!visited[next])
            DFSUntil(next, visited);
        }
    }

    void DFS(int start) {
        vector<bool> visited(SIZE, false);
        cout << "DFS starting at vertex " << start << ": \n";
        DFSUntil(start, visited);
        cout << endl;
    }

    void BFS(int start) {
        vector<bool> visited(SIZE, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "BFS starting at vertex " << start << ": \n";

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            cout << v << " ";

            for (auto &edge : adjList[v]) {
                int next = edge.first;
                if (!visited[next]) {
                    visited[next] = true;
                    q.push(next);
                }
            }
        }
    }
};

int main() {
    // Creates a vector of graph edges/weights
    vector<Edge> edges = {
        // (x, y, w) —> edge from x to y having weight w
        {0,1,6},{0,2,4},{3,9,3},
        {2,3,3},{2,6,1},{5,6,3},
        {4,5,6},{2,5,3},{4,8,6},
        {8,5,3},{4,8,6},{3,5,7},
        {7,1,4}
        /*
            Step 2 check:
            Weights have been changed.
            2 nodes have been removed.    
            6 nodes have been added.
        */
    };

    // Creates graph
    Graph graph(edges);

    // Prints adjacency list representation of graph
    graph.printGraph(); cout << endl;
    
    // DFS starting from vertex 0
    graph.DFS(0);

    // BFS starting from vertex 0
    graph.BFS(0);

    return 0;
}