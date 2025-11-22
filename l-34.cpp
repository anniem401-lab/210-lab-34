// COMSC-210 | Lab 34 | Annie Morales
// IDE used: Visual Studio Code

/*
    Steps Completed:
    Step 1
    Step 2
    Step 3
    Step 4
    Step 5
*/

/*
    To Do: Step 6
    Place a menu in front of all this functionality.
*/

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

const int SIZE = 10; // Initial size: 7

struct Edge {
    int src, dest, weight;
};

typedef pair<int, int> Pair; // Creates alias 'Pair' for the pair<int,int> data type

// Disjoint Set
class DisjointSet {
private:
    vector<int> parent, rank;

public:
    DisjointSet(int n) {
        parent.resize(n);
        rank.resize(n, 0);

        for (int i = 0; i < n; i++)
        parent[i] = i;
    }

    int Find(int x) {
        if (parent[x] != x)
            parent[x] = Find(parent[x]);
        return parent[x];
    }

    void Union(int x, int y) {
        int rx = Find(x);
        int ry = Find(y);

        if (rx != ry) {
            if (rank[rx] > rank[ry]) parent[ry] = rx;
            else if (rank[ry] > rank[rx]) parent[rx] = ry;
            else {
                parent[ry] = rx;
                rank[rx]++;
            }
        }
    }
};

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

    // Names the nodes (Bus Route Network)
    string stopName(int id) {
        switch (id) {
            case 0: return "Central Station";
            case 1: return "Market Street";
            case 2: return "Riverside Depot";
            case 3: return "Hillcrest Terminal";
            case 4: return "Oakwood Stop";
            case 5: return "Midtown Hub";
            case 6: return "Lakeside Stop";
            case 7: return "University Gate";
            case 8: return "East End Station";
            case 9: return "Airport Shuttle Point";
        }
        return "Unknown Stop";
    }

    // Prints the (Bus Route Network)
    void printTransitNetwork() {
        cout << "\nCity Bus Route Network Overview:\n";
        cout << "====================================\n";

        for (int i = 0; i < adjList.size(); i++) {
            cout << "Stop " << i << " (" << stopName(i) << ") connects to:\n";

            for (auto &edge : adjList[i]) {
                cout << "  -> Stop " << edge.first
                    << " (" << stopName(edge.first)
                    << ") - Travel Time: " << edge.second << " min\n";
            }
        }
    }

    // Formatting
    void DFSFormattedUntil(int v, vector<bool>& visited) {
        visited[v] = true;

        cout << "Inspecting Stop " << v
            << " (" << stopName(v) << ")\n";

        for (auto &edge : adjList[v]) {
            int next = edge.first;
            if (!visited[next]) {
                cout << "  -> Can travel to Stop " << next
                    << " (" << stopName(next)
                    << ") - " << edge.second << " min\n";
                DFSFormattedUntil(next, visited);    
            }
        }
    }

    // DFS formatting
    void DFSFormatted(int start) {
        vector<bool> visited(SIZE, false);

        cout << "Transit Route Trace (DFS) from Stop "
            << start << " (" << stopName(start) << "):\n";
        cout << "Purpose: Tracing all stops reachable during a route scan\n";
        cout << "=====================================\n";
        
        DFSFormattedUntil(start, visited);
        cout << endl;
    }

    // BFS formatting
    void BFSFormatted(int start) {
        vector<bool> visited(SIZE, false);
        queue<int> q;

        cout << "Layer-by-layer Transit Expansion (BFS) from Stop "
            << start << " (" << stopName(start) << "):\n";
        cout << "Purpose: Listing stops by increasing travel distance\n";
        cout << "=====================================================\n";

        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            cout << "Checking Stop " << v
                << " (" << stopName(v) << ")\n";

            for (auto &edge : adjList[v]) {
                int next = edge.first;

                if (!visited[next]) {
                    visited[next] = true;
                    q.push(next);

                    cout << "  -> Next stop: " << "Stop " << next
                        << " (" << stopName(next)
                        << ") - " << edge.second << " min\n";
                }
            }
        }
    }

    void shortestPath (int start) {
        // Distance array
        vector<int> dist(SIZE, INT_MAX);

        // Min-priority queue (distance, nose)
        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;

        // Initial node distance = 0
        dist[start] = 0;
        pq.push({0, start});

        while (!pq.empty()) {
            auto [currentDist, u] = pq.top();
            pq.pop();

            for (auto &edge : adjList[u]) {
                int v = edge.first;
                int weight = edge.second;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }

        cout << "Shortest path from node " << start << ":\n";
        for (int i = 0; i < SIZE; i++) {
            cout << start << " -> " << i << " : "
                << (dist[i] == INT_MAX ? -1 : dist[i]) << endl;
        }
    }

    // Minimum Spanning Tree
    void minimumSpanningTree(const vector<Edge>& edges) {
        cout << "\nMinimum Spanning Tree edges:\n";

        // Sorting edges by weight
        vector<Edge> sortedEdges = edges;
        sort(sortedEdges.begin(), sortedEdges.end(),
            [](Edge a, Edge b) { return a.weight < b.weight; });

        DisjointSet ds(SIZE);

        int edgesUsed = 0;

        // Process edges in ascending weight order
        for (auto &edge : sortedEdges) {
            int root1 = ds.Find(edge.src);
            int root2 = ds.Find(edge.dest);
                
            if (root1 != root2) {
                cout << "Edge from " << edge.src
                    << " to " << edge.dest
                    << " with travel time: " << edge.weight << " min\n";

                ds.Union(root1, root2);
                edgesUsed++;

                //When MST contains SIZE-1 edges
                if (edgesUsed == SIZE - 1 )
                        break;
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
    graph.printTransitNetwork(); cout << endl;
    
    // DFS starting from vertex 0
    graph.DFSFormatted(0);

    // BFS starting from vertex 0
    graph.BFSFormatted(0); cout << endl;

    // Displays shortest path
    graph.shortestPath(0);

    // Minimum Spanning Tree
    graph.minimumSpanningTree(edges);

    // Menu
    //while (choice > 0 || choice < 5)
    int choice;
    cout << "\nCity Bus Route Network Menu:\n";
    cout << "[1] Display";
    cout << "[2] Display";
    cout << "[3] Display";
    cout << "[4] Display";
    cout << "[5] Display";
    cout << "[0] Display";

    if (choice = 0)

    return 0;
}