/***********************************************************************************************************
Clase: Estructuras de Datos 
Profesor: Jhon Corredor 
Estudiantes: Diego Fernando Zabala  y david villareal 
Fecha: 2025/04/30
Tema: Grafos
Descripción: Este codigo muestra la implementación de algoritmos de recorrido Euleriano y Hamiltoniano en un grafo no dirigido.

*************************************************************************************************************/




#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <cstring>

using namespace std;

// Clase para manejar recorridos Eulerianos
class Euler {
    int V; // número de vértices
    vector<list<int>> adj; // lista de adyacencia

public:
    Euler(int V) : V(V), adj(V) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void rmvEdge(int u, int v); // elimina arista
    void printEulerTour(); // imprime recorrido Euleriano
    void printEulerUtil(int s); // utilidad recursiva para recorrido
    int DFSCount(int v, vector<bool>& visited); // DFS para contar nodos alcanzables
    bool isValidNextEdge(int u, int v); // verifica si una arista puede ser recorrida
    int iseuler(const vector<int>& degree); // verifica existencia de camino/circuito Euleriano
};

void Euler::printEulerTour() {
    int u = 0;
    // Busca un vértice impar para empezar el recorrido si existe
    for (int i = 0; i < V; i++) {
        if (adj[i].size() % 2 != 0) {
            u = i;
            break;
        }
    }
    printEulerUtil(u);
    cout << endl;
}

void Euler::printEulerUtil(int u) {
    // Recorrido DFS modificado, eliminando aristas a medida que se recorren
    for (auto it = adj[u].begin(); it != adj[u].end(); ++it) {
        int v = *it;
        if (v != -1 && isValidNextEdge(u, v)) {
            cout << u << "-" << v << " ";
            rmvEdge(u, v);
            printEulerUtil(v);
        }
    }
}

bool Euler::isValidNextEdge(int u, int v) {
    // Si sólo queda una arista, es válida
    int count = count_if(adj[u].begin(), adj[u].end(), [](int x) { return x != -1; });
    if (count == 1) return true;

    // Compara cantidad de nodos alcanzables antes y después de remover la arista
    vector<bool> visited(V, false);
    int count1 = DFSCount(u, visited);

    rmvEdge(u, v);
    fill(visited.begin(), visited.end(), false);
    int count2 = DFSCount(u, visited);

    addEdge(u, v); // restaurar arista

    return (count1 <= count2); // si disminuye, era un puente
}

void Euler::rmvEdge(int u, int v) {
    // Marca la arista como removida (lazy delete usando -1)
    auto iv = find(adj[u].begin(), adj[u].end(), v);
    if (iv != adj[u].end()) *iv = -1;

    auto iu = find(adj[v].begin(), adj[v].end(), u);
    if (iu != adj[v].end()) *iu = -1;
}

int Euler::DFSCount(int v, vector<bool>& visited) {
    // Cuenta nodos alcanzables desde v
    visited[v] = true;
    int count = 1;
    for (int nbr : adj[v]) {
        if (nbr != -1 && !visited[nbr]) {
            count += DFSCount(nbr, visited);
        }
    }
    return count;
}

int Euler::iseuler(const vector<int>& degree) {
    // Determina si existe camino o circuito Euleriano
    int count = count_if(degree.begin(), degree.end(), [](int d) { return d % 2 != 0; });
    if (count == 0) {
        cout << "Euler's circuit exists:\n";
        return 1;
    } else if (count == 2) {
        cout << "Euler's path exists:\n";
        return 1;
    } else {
        cout << "No Euler's path or circuit exists.\n";
        return -1;
    }
}

// Clase para encontrar ciclos y caminos Hamiltonianos
class Hamiltonian {
    vector<int> path; // camino actual
    bool cycleFound; // si se encontró ciclo

public:
    Hamiltonian(int n) : path(n, -1), cycleFound(false) {}

    bool isSafe(const vector<vector<bool>>& graph, int pos, int v); // verifica validez del siguiente nodo
    bool hamCycleUtil(const vector<vector<bool>>& graph, int pos, int start); // backtracking recursivo
    bool hamCycle(const vector<vector<bool>>& graph); // función principal
    void printSolution(); // imprime el camino/ciclo
};

bool Hamiltonian::isSafe(const vector<vector<bool>>& graph, int pos, int v) {
    // Verifica conexión y no repetición de vértice
    if (!graph[path[pos - 1]][v]) return false;
    if (find(path.begin(), path.begin() + pos, v) != path.begin() + pos) return false;
    return true;
}

bool Hamiltonian::hamCycleUtil(const vector<vector<bool>>& graph, int pos, int start) {
    int n = path.size();
    if (pos == n) {
        // Verifica si el último vértice conecta con el inicial (ciclo)
        if (graph[path[pos - 1]][path[0]]) {
            cout << "Hamiltonian cycle exists\n";
            cycleFound = true;
        } else {
            cout << "Hamiltonian path exists\n";
        }
        return true;
    }

    // Intenta todos los vértices
    for (int v = 0; v < n; ++v) {
        if (v == start) continue;
        if (isSafe(graph, pos, v)) {
            path[pos] = v;
            if (hamCycleUtil(graph, pos + 1, start)) return true;
            path[pos] = -1; // backtrack
        }
    }
    return false;
}

bool Hamiltonian::hamCycle(const vector<vector<bool>>& graph) {
    // Intenta desde cada vértice
    int n = path.size();
    for (int i = 0; i < n; ++i) {
        fill(path.begin(), path.end(), -1);
        path[0] = i;
        if (hamCycleUtil(graph, 1, i)) {
            printSolution();
            return true;
        }
    }
    cout << "No Hamiltonian path or cycle exists.\n";
    return false;
}

void Hamiltonian::printSolution() {
    // Imprime camino encontrado
    for (size_t i = 0; i < path.size() - 1; ++i)
        cout << path[i] << "-" << path[i + 1] << " ";
    if (cycleFound) cout << path.back() << "-" << path[0];
    cout << "\n";
}

int main() {
    // Punto de entrada principal
    int n, edges;
    cout << "Enter the number of vertices of the graph: ";
    cin >> n;

    Euler g1(n);
    Hamiltonian g2(n);

    vector<int> degree(n, 0); // grados para verificar euler
    vector<vector<bool>> graph(n, vector<bool>(n, false)); // matriz para hamilton

    cout << "Enter the total number of edges: ";
    cin >> edges;

    // Entrada de aristas
    for (int i = 0; i < edges; ++i) {
        int v1, v2;
        cout << "Enter edge (u v): ";
        cin >> v1 >> v2;
        if (v1 >= n || v2 >= n || v1 < 0 || v2 < 0) {
            cout << "Invalid vertex. Vertices must be between 0 and " << n - 1 << "\n";
            --i;
            continue;
        }
        graph[v1][v2] = graph[v2][v1] = true;
        g1.addEdge(v1, v2);
        degree[v1]++;
        degree[v2]++;
    }

    cout << endl;
    g2.hamCycle(graph); // intenta encontrar ciclo/recorrido hamiltoniano
    cout << endl;
    if (g1.iseuler(degree) == 1) // verifica e imprime camino euleriano si existe
        g1.printEulerTour();

    return 0;
}