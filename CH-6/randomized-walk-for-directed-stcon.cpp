#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

bool randomWalkStcon(const vector<vector<int>>& graph, int startNode, int endNode) {
    if (startNode == endNode) {
        return true;
    }

    int numVertices = static_cast<int>(graph.size());
    if (numVertices == 0) {
        return false;
    }

    long long maxSteps = 2LL * numVertices * numVertices * numVertices;
    if (numVertices == 1) maxSteps = 0;

    int currentNode = startNode;

    for (long long step = 0; step < maxSteps; ++step) {
        if (graph[currentNode].empty()) {
            currentNode = startNode;
            continue;
        }

        int randomIndex = rand() % graph[currentNode].size();
        currentNode = graph[currentNode][randomIndex];

        if (currentNode == endNode) {
            return true;
        }
    }

    return false;
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    cout << boolalpha;

    int numVertices1 = 4;
    vector<vector<int>> graph1(numVertices1);
    graph1[0].push_back(1);
    graph1[1].push_back(2);
    graph1[2].push_back(3);

    cout << "--- Example 1 (Connected Line) ---" << endl;
    cout << "Checking for path from 0 to 3..." << endl;
    
    if (randomWalkStcon(graph1, 0, 3)) {
        cout << "Result: Connected!" << endl;
    } else {
        cout << "Result: Not Connected (or algorithm timed out)." << endl;
    }

    int numVertices2 = 4;
    vector<vector<int>> graph2(numVertices2);
    graph2[0].push_back(1);
    graph2[2].push_back(3);

    cout << "\n--- Example 2 (Disconnected) ---" << endl;
    cout << "Checking for path from 0 to 3..." << endl;

    if (randomWalkStcon(graph2, 0, 3)) {
        cout << "Result: Connected!" << endl;
    } else {
        cout << "Result: Not Connected (or algorithm timed out)." << endl;
    }

    int numVertices3 = 5;
    vector<vector<int>> graph3(numVertices3);
    graph3[0].push_back(1);
    graph3[1].push_back(2);
    graph3[3].push_back(4);

    cout << "\n--- Example 3 (With Sink) ---" << endl;
    cout << "Checking for path from 0 to 2 (should find)..." << endl;
    if (randomWalkStcon(graph3, 0, 2)) {
        cout << "Result: Connected!" << endl;
    } else {
        cout << "Result: Not Connected (or algorithm timed out)." << endl;
    }

    cout << "Checking for path from 0 to 4 (should not find)..." << endl;
    if (randomWalkStcon(graph3, 0, 4)) {
        cout << "Result: Connected!" << endl;
    } else {
        cout << "Result: Not Connected (or algorithm timed out)." << endl;
    }

    return 0;
}