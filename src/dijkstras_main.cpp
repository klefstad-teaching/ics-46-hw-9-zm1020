#include "dijkstras.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <graph_file>" << endl;
        return 1;
    }
    
    string filename = argv[1];
    Graph G;
    
    try {
        file_to_graph(filename, G);
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    int source = 0;
    vector<int> previous;
    
    vector<int> distances = dijkstra_shortest_path(G, source, previous);
    
    cout << "Shortest paths from vertex " << source << ":" << endl;
    
    for (int i = 0; i < G.numVertices; i++) {
        if (i == source) continue;
        
        vector<int> path = extract_shortest_path(distances, previous, i);
        print_path(path, distances[i]);
    }
    
    return 0;
}