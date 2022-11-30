#include <iostream>
#include <vector>

// Representación del grafo?
// - Matriz? MUY COSTOSO
// - Guardar solo los vertices con los que esta conectado y sus pesos.
class Graph {
private:
    // ...
public:

    struct Edge {
        int V1;       // First vertix.
        int V2;       // Second vertix.
        float weight; // Edge weight.

        Edge (int V1, int V2, float weight) : V1(V1), V2(V2), weight(weight) {}

        friend std::ostream& operator<<(std::ostream& os, const Edge& e) {
            return os << "EDGE { V1: " << e.V1 << ", V2: " << e.V2 << ", weight: " << e.weight << "}";
        }
    };

    int V; // Number of Vertex.
    std::vector<Edge> edges;

    Graph(int V, std::vector<Edge> edges) : V(V), edges(edges) {}

    friend std::ostream& operator<<(std::ostream& os, const Graph& g) {
        os << "GRAPH {" 
           << "\n  Vertix: " << g.V 
           << "\n  Edges:  " << g.edges.size()
           << "\n  Edges {";
        for (auto& e : g.edges) {
            os << "\n      " << e;
        }
        return os << "\n  }\n}";
    }
};

void karger_algorythm(Graph& g) {

}

// Normalizar el grafo con probabilidades:
//  - Suma de todos los pesos y normalizarlos de 0 a 1 (= probabilidad). 
//  - Selección probabilista uniforme: se divide el rango k partes iguales y se 
//  selecciona un numero que se encuentre en una de las k partes.
//  - Selección probabilista no uniforme: se divide el rango k partes no iguales, la 
//  selección funciona igual.
int main () {
    Graph g(5, {
        Graph::Edge(1,2,1.0),
        Graph::Edge(1,4,8.0),
        Graph::Edge(2,3,5.0),
        Graph::Edge(3,4,7.0),
    });

    std::cout << g << std::endl;
}