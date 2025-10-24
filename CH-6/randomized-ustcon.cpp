#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

bool randomWalkUstcon(const vector<vector<int>>& graph, int startNode, int endNode) {
    
    if (startNode == endNode) {
        return true;
    }

    long long numVertices = static_cast<long long>(graph.size());
    if (numVertices == 0) {
        return false;
    }

    long long maxSteps = 2 * numVertices * numVertices * numVertices;
    if (numVertices == 1 && startNode != endNode) maxSteps = 0;

    int currentNode = startNode;

    for (long long step = 0; step < maxSteps; ++step) {
        if (currentNode == endNode) {
            return true;
        }

        const vector<int>& neighbors = graph[currentNode];

        if (neighbors.empty()) {
            return false;
        }

        int randomIndex = rand() % neighbors.size();
        currentNode = neighbors[randomIndex];
    }

    return false;
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    cout << boolalpha;

    int numVertices1 = 4;
    vector<vector<int>> graph1(numVertices1);
    graph1[0].push_back(1);
    graph1[1].push_back(0);
    graph1[1].push_back(2);
    graph1[2].push_back(1);
    graph1[2].push_back(3);
    graph1[3].push_back(2);

    cout << "--- Example 1 (Connected) ---" << endl;
    cout << "Graph: 0-1, 1-2, 2-3" << endl;
    cout << "Checking for path from 0 to 3..." << endl;
    
    if (randomWalkUstcon(graph1, 0, 3)) {
        cout << "Result: Connected!" << endl;
    } else {
        cout << "Result: Not Connected (or algorithm timed out)." << endl;
    }

    int numVertices2 = 4;
    vector<vector<int>> graph2(numVertices2);
    graph2[0].push_back(1);
    graph2[1].push_back(0);
    graph2[2].push_back(3);
    graph2[3].push_back(2);

    cout << "\n--- Example 2 (Disconnected) ---" << endl;
    cout << "Graph: 0-1, 2-3" << endl;
    cout << "Checking for path from 0 to 3..." << endl;

    if (randomWalkUstcon(graph2, 0, 3)) {
        cout << "Result: Connected!" << endl;
    } else {
        cout << "Result: Not Connected (or algorithm timed out)." << endl;
    }

    return 0;
}