#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <map>
#include <string>

using namespace std;

int runRandomWalk(const vector<vector<int>>& graph, int startNode, int numSteps) {
    int currentNode = startNode;

    for (int i = 0; i < numSteps; ++i) {
        const vector<int>& neighbors = graph[currentNode];
        
        if (neighbors.empty()) {
            break;
        }

        int randomIndex = rand() % neighbors.size();
        currentNode = neighbors[randomIndex];
    }
    return currentNode;
}

map<int, int> simulateMixing(const vector<vector<int>>& graph, int startNode, int numSteps, int numSimulations) {
    map<int, int> distribution;
    for (size_t i = 0; i < graph.size(); ++i) {
        distribution[static_cast<int>(i)] = 0;
    }

    for (int i = 0; i < numSimulations; ++i) {
        int endNode = runRandomWalk(graph, startNode, numSteps);
        distribution[endNode]++;
    }
    return distribution;
}

void printDistribution(const string& title, const map<int, int>& distribution, int numSimulations) {
    cout << "--- " << title << " ---" << endl;
    cout << "Distribution from Node 0 after " << numSimulations << " simulations:" << endl;

    for (const auto& pair : distribution) {
        double percentage = 100.0 * static_cast<double>(pair.second) / numSimulations;
        cout << "Node " << pair.first << ": " << setw(7) << pair.second << " visits (" << fixed << setprecision(2) << percentage << "%)" << endl;
    }
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    int numVertices = 5;
    int numSteps = 4;
    int numSimulations = 100000;
    int startNode = 0;

    vector<vector<int>> graphExpander(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            if (i != j) {
                graphExpander[i].push_back(j);
            }
        }
    }

    vector<vector<int>> graphPath(numVertices);
    graphPath[0] = {1};
    graphPath[1] = {0, 2};
    graphPath[2] = {1, 3};
    graphPath[3] = {2, 4};
    graphPath[4] = {3};

    
    map<int, int> expanderDist = simulateMixing(graphExpander, startNode, numSteps, numSimulations);
    printDistribution("Expander-like Graph (Complete K_5)", expanderDist, numSimulations);

    cout << endl;

    map<int, int> pathDist = simulateMixing(graphPath, startNode, numSteps, numSimulations);
    printDistribution("Slow-mixing Graph (Path 0-1-2-3-4)", pathDist, numSimulations);

    return 0;
}