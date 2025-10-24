#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <map>
#include <functional>
#include <cmath>
#include <random>

using namespace std;

mt19937 generator(static_cast<unsigned int>(time(nullptr)));

string generateRandomString(int n) {
    string s = "";
    uniform_int_distribution<int> distribution(0, 1);
    for (int i = 0; i < n; ++i) {
        s += (distribution(generator) == 0 ? '0' : '1');
    }
    return s;
}

bool mockBppAlgorithm(const string& x, const string& r, bool correctAnswer) {
    hash<string> hasher;
    if (hasher(r) % 100 == 0) {
        return !correctAnswer;
    }
    return correctAnswer;
}

string getNeighbor(const string& r, int edgeLabel) {
    hash<string> hasher;
    string nextR = r;
    int n = static_cast<int>(r.length());
    if (n == 0) return "";

    size_t hashVal = hasher(r + to_string(edgeLabel));
    int bitToFlip = hashVal % n;
    
    nextR[bitToFlip] = (nextR[bitToFlip] == '0' ? '1' : '0');
    return nextR;
}

string takeOneStep(const string& r) {
    uniform_int_distribution<int> coinFlip(0, 1);
    uniform_int_distribution<int> edgeSelect(0, 6);

    if (coinFlip(generator) == 0) {
        return r;
    } else {
        int edgeLabel = edgeSelect(generator);
        return getNeighbor(r, edgeLabel);
    }
}

string walkBetaSteps(string r, int beta) {
    string currentNode = r;
    for (int i = 0; i < beta; ++i) {
        currentNode = takeOneStep(currentNode);
    }
    return currentNode;
}

bool amplify(const string& x, int n, int k, bool correctAnswer) {
    const int beta = 3; 
    const int numTrials = 7 * k; 

    vector<bool> results;
    string currentR = generateRandomString(n);
    
    for (int i = 1; i <= numTrials; ++i) {
        currentR = walkBetaSteps(currentR, beta);
        results.push_back(mockBppAlgorithm(x, currentR, correctAnswer));
    }

    int yesVotes = 0;
    for (bool res : results) {
        if (res) {
            yesVotes++;
        }
    }

    return (yesVotes > numTrials / 2);
}

int main() {
    const int n = 20;
    const int k = 5;
    const int numSimulations = 20000;
    const string x = "test_input";

    const int beta = 3;
    const int numTrials = 7 * k;
    const int bitsPerStep = 4;
    long amplifiedBitCost = n + numTrials * beta * bitsPerStep;


    cout << "Simulating BPP Amplification..." << endl;
    cout << "Algorithm 'A' uses n=" << n << " bits." << endl;
    cout << "Target amplification k=" << k << " (Error < " << 1.0 / pow(2, k) << ")" << endl;
    cout << "Mock error rate of 'A' is 1/100 (0.01)." << endl;
    cout << "Running " << numSimulations << " simulations..." << endl;
    
    cout << "\n--- Random Bit Cost ---" << endl;
    cout << "Single Run: " << n << " bits" << endl;
    cout << "Amplified Run (n + 7k*beta*4): " << amplifiedBitCost << " bits (which is n + O(k))" << endl;

    const bool correctAnswer1 = true;
    int singleRunWrong1 = 0;
    int amplifiedWrong1 = 0;

    for (int i = 0; i < numSimulations; ++i) {
        string r = generateRandomString(n);
        if (mockBppAlgorithm(x, r, correctAnswer1) != correctAnswer1) {
            singleRunWrong1++;
        }
        if (amplify(x, n, k, correctAnswer1) != correctAnswer1) {
            amplifiedWrong1++;
        }
    }
    
    double singleRunError1 = static_cast<double>(singleRunWrong1) / numSimulations;
    double amplifiedError1 = static_cast<double>(amplifiedWrong1) / numSimulations;

    cout << "\n--- Test Case 1 (Correct Answer = true) ---" << endl;
    cout << "Single Run Error: " << fixed << setprecision(5) << singleRunError1 << " (" << singleRunWrong1 << "/" << numSimulations << ")" << endl;
    cout << "Amplified Error:  " << fixed << setprecision(5) << amplifiedError1 << " (" << amplifiedWrong1 << "/" << numSimulations << ")" << endl;

    const bool correctAnswer2 = false;
    int singleRunWrong2 = 0;
    int amplifiedWrong2 = 0;

    for (int i = 0; i < numSimulations; ++i) {
        string r = generateRandomString(n);
        if (mockBppAlgorithm(x, r, correctAnswer2) != correctAnswer2) {
            singleRunWrong2++;
        }
        if (amplify(x, n, k, correctAnswer2) != correctAnswer2) {
            amplifiedWrong2++;
        }
    }

    double singleRunError2 = static_cast<double>(singleRunWrong2) / numSimulations;
    double amplifiedError2 = static_cast<double>(amplifiedWrong2) / numSimulations;

    cout << "\n--- Test Case 2 (Correct Answer = false) ---" << endl;
    cout << "Single Run Error: " << fixed << setprecision(5) << singleRunError2 << " (" << singleRunWrong2 << "/" << numSimulations << ")" << endl;
    cout << "Amplified Error:  " << fixed << setprecision(5) << amplifiedError2 << " (" << amplifiedWrong2 << "/" << numSimulations << ")" << endl;
              
    return 0;
}