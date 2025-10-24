#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

bool utsUstcon(const vector<vector<int>>& labeledGraph, int degree, int startNode, int endNode, const vector<int>& universalTraversalSequence) {
    
    if (startNode == endNode) {
        return true;
    }

    int currentNode = startNode;

    for (int label : universalTraversalSequence) {
        if (label >= degree || label < 0) {
            cerr << "Warning: UTS label " << label << " is out of bounds for degree " << degree << endl;
            continue;
        }

        if (currentNode < 0 || currentNode >= static_cast<int>(labeledGraph.size())) {
             cerr << "Warning: Current node " << currentNode << " is out of graph bounds." << endl;
            return false;
        }

        if (labeledGraph[currentNode].empty()) {
            cerr << "Warning: Node " << currentNode  << " has no outgoing edges defined." << endl;
            return false;
        }

        currentNode = labeledGraph[currentNode][label];

        if (currentNode == endNode) {
            return true;
        }
    }

    return false;
}

int main() {
    cout << boolalpha;

    int numVertices1 = 3;
    int degree1 = 2;
    vector<vector<int>> graph1(numVertices1, vector<int>(degree1));

    graph1[0] = {1, 2};
    graph1[1] = {0, 2};
    graph1[2] = {0, 1};

    vector<int> uts1 = {0, 1, 1, 0, 1, 0};

    cout << "--- Example 1 (Connected Triangle, d=2) ---" << endl;
    cout << "Checking for path from 0 to 2..." << endl;
    
    if (utsUstcon(graph1, degree1, 0, 2, uts1)) {
        cout << "Result: Connected!" << endl;
    } else {
        cout << "Result: Not Connected." << endl;
    }

    int numVertices2 = 4;
    int degree2 = 1;
    vector<vector<int>> graph2(numVertices2, vector<int>(degree2));

    graph2[0] = {1};
    graph2[1] = {0};
    graph2[2] = {3};
    graph2[3] = {2};

    vector<int> uts2 = {0, 0, 0, 0};

    cout << "\n--- Example 2 (Disconnected, d=1) ---" << endl;
    cout << "Checking for path from 0 to 3..." << endl;

    if (utsUstcon(graph2, degree2, 0, 3, uts2)) {
        cout << "Result: Connected!" << endl;
    } else {
        cout << "Result: Not Connected." << endl;
    }

    cout << "Checking for path from 0 to 1..." << endl;

    if (utsUstcon(graph2, degree2, 0, 1, uts2)) {
        cout << "Result: Connected!" << endl;
    } else {
        cout << "Result: Not Connected." << endl;
    }

    return 0;
}