#include <algorithm>
#include <chrono>
#include <cstring>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>
#include <unordered_map>


const int PRINT_EDGES = 20;
std::random_device rd;
std::mt19937 e2(rd());

struct Edge {
    int V1;            // First vertix.
    int V2;            // Second vertix.
    int k_V1;          // Value after applying karger.
    int k_V2;          // Value after applying karger.
    double weight;     // Edge weight (connection robustness).
    double nor_weight; // Normalized weight, it will depend on the current graph.

    Edge (int V1, int V2, double weight)
        : V1(V1), V2(V2), k_V1(V1), k_V2(V2), weight(weight) {}

    bool operator!=(const Edge& e) {
        return weight != e.weight || V1 != e.V1 || V2 != e.V2; 
    }
};

std::ostream& operator<<(std::ostream& os, const Edge& e) {
    return os << "EDGE { V1: " << e.V1
        << ", V2: " << e.V2
        //<< ", k_V1: " << e.k_V1
        //<< ", k_V2: " << e.k_V2
        << ", weight: " << e.weight
        << ", normalized weight: " << e.nor_weight
        << "}";
}

// Representación del grafo?
// - Matriz? MUY COSTOSO
// - Guardar solo los vertices con los que esta conectado y sus pesos.
class Graph {
private:
    bool normalized;         // Validator.
public:

    int n_vertices;          // Number of vertex.
    double total_weight;     // Graph total_weight.
    std::vector<Edge> edges; // Graph edges.

    Graph() : normalized(false), n_vertices(0), total_weight(0) {}
    Graph(int n_vertices, std::vector<Edge> edges) {
        normalized = true;
        this->n_vertices = n_vertices;
        this->total_weight = 0.0;
        for (auto& e : edges) {
            total_weight += e.weight;
        }
        for (auto& e : edges) {
            e.nor_weight = e.weight / total_weight;
        }
        this->edges = edges;
    }

    void normalize() {
        if (!normalized) {
            for (auto& e : edges) {
                e.nor_weight = e.weight / total_weight;
            }
        }
    }

    void add(Edge e) {
        total_weight += e.weight;
        edges.push_back(e);
    }

    void erase(std::vector<Edge>::iterator it) {

        // Copying the edge before deleting it.
        Edge aux = *it;

        // Removing the given edge.
        edges.erase(it);
        
        // Updating number of vertices.
        n_vertices--;

        // Recalculating total weight.
        total_weight -= aux.weight;

        // Replacing the first vertice with the second vertice.
        for (auto e = edges.begin(); e != edges.end();) {
            if ((e->k_V1 == aux.k_V1 && e->k_V2 == aux.k_V2) 
                || (e->k_V1 == aux.k_V2 && e->k_V2 == aux.k_V1))
            {
                total_weight -= e->weight;
                e = edges.erase(e);
            } else {
                if (e->k_V1 == aux.k_V1) {
                    e->k_V1 = aux.k_V2;
                } else if (e->k_V2 == aux.k_V1) {
                    e->k_V2 = aux.k_V2;
                }
                ++e;
            }
        }

        // Updating normals.
        for (auto& e : edges) {
            e.nor_weight = e.weight / total_weight;
        }

    }

};

std::ostream& operator<<(std::ostream& os, const Graph& g) {
    os << "GRAPH {" 
        << "\n  Number of vertix: " << g.n_vertices
        << "\n  Total weight: " << g.total_weight
        << "\n  Edges (#" << g.edges.size() << ") {";
    if (g.edges.size() < PRINT_EDGES) {
        for (auto& e : g.edges) {
            os << "\n    " << e;
        }
    } else {
        for (int i = 0; i < PRINT_EDGES; i++) {
            os << "\n    " << g.edges[i];
        }
        os << "\n    ...";
        os << "\n    " << g.edges[g.edges.size()-1];
    }
    return os << "\n  }\n}";
}

struct Cut {
    double cut_weight;
    std::vector<Edge> cut_edges;
};

struct Cut_hash {
    std::size_t operator()(const Cut& cut) const {
        std::size_t hash = 0;
        hash ^= std::hash<double>()(cut.cut_weight);
        hash ^= std::hash<int>()(cut.cut_edges.size());
        for (const auto& e : cut.cut_edges) {
            hash ^= std::hash<int>()(e.V1) ^ std::hash<int>()(e.V2)
                ^ std::hash<double>()(e.weight);
        }
        return hash;
    }
};

struct Cut_equal {
    bool operator()(const Cut& a, const Cut& b) const {
        if (a.cut_weight == b.cut_weight) {
            if (a.cut_edges.size() == b.cut_edges.size()) {
                for (int i = 0; i < a.cut_edges.size(); i++) {
                    if (a.cut_edges[i].V1 != b.cut_edges[i].V1
                        || a.cut_edges[i].V2 != b.cut_edges[i].V2
                        || a.cut_edges[i].weight != b.cut_edges[i].weight)
                    {
                        return false;
                    }
                }
                return true;
            }
            return false;
        }
        return false;
    }
};

bool operator<(const Cut& a, const Cut& b) {
    return a.cut_weight < b.cut_weight ||
        (a.cut_weight == b.cut_weight && a.cut_edges.size() < b.cut_edges.size());
}

/**
 * @brief Applies the Karger algorithm into the graph to find the minimum cut.
 * 
 * @param g 
 * @return Graph 
 */
Cut karger(Graph g) {

    // Number for edge probability generation.
    std::uniform_real_distribution<double> dist(0, 1);
    while (g.n_vertices > 2) {
        // Probability that will determinate which edge will be selected.
        double edge_probability  = dist(e2);
        // Acumulated weight for accomplish the no uniform distribution.
        double acumulated_weight = 0.0;
        for (auto it = g.edges.begin(); it != g.edges.end(); it++) {
            acumulated_weight += it->nor_weight;
            if (acumulated_weight > edge_probability) {
                //std::cout << "=================================\n";
                //std::cout << "Deleting edge " << *it 
                //    << " with acumulated weight " << acumulated_weight
                //    << " and probability " << edge_probability << "\n";
                g.erase(it);
                //std::cout << g << std::endl;
                //std::cout << "=================================\n";
                break;
            }
        }
    }
    return {g.total_weight, g.edges};
}

/**
 * @brief Generate a new random graph.
 * 
 * @param max_vertices Maximum vertices that the graph can has (the graph could generate with less vertices).
 * @param max_edges    Maximum edges that the graph would have.
 * @param max_weight   Maximum weight that the edges could receive after being generated randomly.
 * @param max_connections_per_vertice Maximum connections that a vector could have, the number of each vertice will be generated between 1 and this value.
 * @param initial_prob Initial probability to generate a new edge.
 * @param decrease_coef Decrease probability coefficient. As new edges are added, the probability of adding new edges decreases.
 */
Graph generate_graph(int max_vertices, int max_edges, double max_weight, 
    int max_connections_per_vertice, double initial_prob, double decrease_coef) 
{
    // Probability of adding a new edge.
    double prob = initial_prob;
    // Saves the generated vertices info:
    // - Index.
    // - Current number of connections.
    // - Max number of connections that they could have.
    std::map<int, std::vector<int>> v_info;

    // Vertice uniform int distribution generation.
    std::uniform_int_distribution<int> v_dist(1, max_vertices);
    // Vertice max connections uniform int distribution generation.
    std::uniform_int_distribution<int> m_dist(1, max_connections_per_vertice);
    // Edge max weight real distribution generation.
    std::uniform_real_distribution<double> w_dist(0.1, max_weight);
    // Edge probability real distribution generation.
    std::uniform_real_distribution<double> edge_prob(0, 1);
    // New graph.
    Graph g;
    for (int e = 0; e < max_edges; e++) {

        // Generating a new edge.
        Edge new_edge(v_dist(e2), v_dist(e2), w_dist(e2));

        // Generate new vertice until is different from the first one.
        while (new_edge.V1 == new_edge.V2) {
            new_edge.V2 = new_edge.k_V2 = v_dist(e2);
        }

        // Check if the first vertice is already added or not and if it has
        // reached its maximum number of connections.
        if (v_info.find(new_edge.V1) == v_info.end()) {
            v_info.insert({new_edge.V1, {1, m_dist(e2)}});
        } else if (v_info[new_edge.V1][0] >= v_info[new_edge.V1][1]) {
            continue;
        }

        // Check if the second vertice is already added or not and if it has 
        // reached its maximum number of connections.
        if (v_info.find(new_edge.V2) == v_info.end()) {
            v_info.insert({new_edge.V2, {1, m_dist(e2)}});
        } else if (v_info[new_edge.V2][0] >= v_info[new_edge.V2][1]) {
            continue;
        }

        // If the generated probability is less than the current probability, 
        // then add the edge to the graph.
        if (edge_prob(e2) < initial_prob) {
            g.add(new_edge);
            prob *= (1 - decrease_coef); 
        }
    }
    g.n_vertices = v_info.size();
    g.normalize();

    return g;
}

void exit(int status, std::string msg) {
    if (status) {
        std::cerr << msg + "\n";
    }
    std::cerr << "\nusage: ./practica3 <repetitions> t<n> [t5 options]"
        << "\n  <repetitions> ...... Number of repetitions"
        << "\n  t<n> ............... Number of test: 1,2,3,4 or 5."
        << "\n  [t5 options] ....... Options for test 5."
        << "\n    <max vertices> ... Graph max vertices." 
        << "\n    <max_edges> ...... Graph max edges."
        << "\n    <max_weight> ..... Max possible weight to generate."
        << "\n    <max_conns> ...... Max possible connections per vertice." 
        << "\n    <initial_prob> ... Initial probability to generate an edge."
        << "\n    <decrease_coef> .. Probability decrease coefficient."
        << "\n  verbose ............ Shows extra info inside the execution.\n\n";
    exit(status);
}

// Normalizar el grafo con probabilidades:
//  - Suma de todos los pesos y normalizarlos de 0 a 1 (= probabilidad). 
//  - Selección probabilista uniforme: se divide el rango k partes iguales y se 
//  selecciona un numero que se encuentre en una de las k partes.
//  - Selección probabilista no uniforme: se divide el rango k partes no iguales, la 
//  selección funciona igual.
int main (int argc, char* argv[]) {

    if (argc-1 < 2) exit(1, "\nerror: not enough arguments.");

    bool verbose = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "verbose") == 0) {
            verbose = true;
        }
    }

    int reps = std::stoi(argv[1]);
    int test = std::stoi(std::string(argv[2]).substr(1));
    Graph g;
    if (test == 1) {
        g = Graph(4, {
            Edge(1,2,1.0),
            Edge(1,4,8.0),
            Edge(2,3,5.0),
            Edge(3,4,7.0),
        });
    } else if(test == 2) {
        g = Graph(4, {
            Edge(1,2,1.0),
            Edge(1,4,8.0),
            Edge(2,3,5.0),
            Edge(3,4,7.0),
            Edge(4,4,9.0),
        });
    } else if (test == 3) {
        g = Graph(10, {
            Edge(1,2,80),
            Edge(1,3,50),
            Edge(3,5,40),
            Edge(4,9,70),
            Edge(7,4,20),
            Edge(8,6,90),
            Edge(9,7,10),
            Edge(10,8,30)
        });
    } else if (test == 4) {
        g = Graph(10, {
            Edge(1,2,80),
            Edge(1,7,65),
            Edge(1,3,50),
            Edge(3,5,40),
            Edge(5,10,15),
            Edge(3,8,67),
            Edge(4,9,70),
            Edge(7,4,20),
            Edge(8,6,90),
            Edge(4,2,53),
            Edge(2,8,20),
            Edge(9,7,10),
            Edge(10,8,30),
            Edge(10,6,76),
            Edge(2,6,24),
            Edge(1,9,56),
            Edge(5,8,97),
            Edge(7,2,30)
        });
    } else if (test == 5) {
        if (argc-3 != 6) exit(1, "\nerror: not enough arguments for test 5.");
        g = generate_graph(std::stoi(argv[3]), std::stoi(argv[4]), std::stod(argv[5]),
            std::stoi(argv[6]), std::stod(argv[7]), std::stod(argv[8]));
    } else {
        exit(1, "\nerror: unknown test.");
    }

    std::cout << g << std::endl;

    std::unordered_map<Cut, int, Cut_hash, Cut_equal> cuts;
    auto global_s = std::chrono::steady_clock::now();
    for (int i = 0; i < reps; i++) {
        Cut r = karger(g);
        if (verbose) {
            auto local_s = std::chrono::steady_clock::now();
            std::cout << "================================= "
                << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - local_s).count()
                << "s\n";
            std::cout << "Corte minimo del grafo original:\n";
            for (auto& e : r.cut_edges) {
                std::cout << e << "\n"; 
            }
            std::cout << "=================================\n";
        }

        if (cuts.find(r) == cuts.end()) {
            cuts.insert({r, 1});
        } else {
            cuts[r]++;
        }
    }
    auto global_t = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - global_s).count();
    //for (auto& cut : cuts) {
    //    std::cout << cut.first << "(" << cut.second << "\" << i ")" << "\n";
    //}
    auto max = std::max_element(cuts.begin(), cuts.end(),
        [](const std::pair<Cut, int>& p1, const std::pair<Cut, int>& p2) {
            return p1.second < p2.second; }
    );

    std::cout << "Minimum cut with max frecuency validation ("
        << max->second << "/" << reps << "). Time: " << global_t << "us\n"
        << "Weight: " << max->first.cut_weight << "\n";
    for (auto& e : max->first.cut_edges) {
        std::cout << "  " << e << "\n";
    }
}