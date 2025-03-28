#include "dijkstras.h"

struct Node {
    int vertex;
    int distance;
    
    Node(int v, int d) : vertex(v), distance(d) {}
    
    bool operator>(const Node& other) const {
        return distance > other.distance;
    }
};

vector<int> dijkstra_shortest_path(const Graph& G, int source, vector<int>& previous) {
    int n = G.numVertices;
    
    vector<int> distance(n, INF);
    previous.assign(n, -1);
    vector<bool> visited(n, false);
    
    priority_queue<Node, vector<Node>, greater<Node>> pq;
    
    distance[source] = 0;
    pq.push(Node(source, 0));
    
    // Safety counter to prevent infinite loops
    int iterations = 0;
    const int MAX_ITERATIONS = 1000000; // Adjust based on expected graph size
    
    while (!pq.empty() && iterations < MAX_ITERATIONS) {
        iterations++;
        int u = pq.top().vertex;
        pq.pop();
        
        // Skip if already processed (better path already found)
        if (visited[u]) continue;
        
        visited[u] = true;
        
        // Process all outgoing edges
        for (const Edge& edge : G[u]) {
            int v = edge.dst;
            int weight = edge.weight;
            
            // Skip invalid edges or already finalized vertices
            if (weight < 0 || visited[v]) continue;
            
            // Relaxation step
            if (distance[u] != INF && distance[u] + weight < distance[v]) {
                distance[v] = distance[u] + weight;
                previous[v] = u;
                pq.push(Node(v, distance[v]));
            }
        }
    }
    
    if (iterations >= MAX_ITERATIONS) {
        cerr << "Warning: Maximum iterations reached in Dijkstra's algorithm. Possible infinite loop." << endl;
    }
    
    return distance;
}

vector<int> extract_shortest_path(const vector<int>& /*distances*/, const vector<int>& previous, int destination) {
    vector<int> path;
    for (int v = destination; v != -1; v = previous[v]) {
        path.push_back(v);
    }
    
    reverse(path.begin(), path.end());
    return path;
}

void print_path(const vector<int>& path, int total) {
    if (path.empty()) {
        cout << "\nTotal cost is " << total << endl;
        return;
    }
    
    for (size_t i = 0; i < path.size(); i++) {
        cout << path[i];
        if (i < path.size() - 1) {
            cout << " ";
        }
    }
    
    cout << " \nTotal cost is " << total << endl;
}