/*
 * ALGORITHM 2: Directed s-t Connectivity (STCON) in O(log n) Space
 *
 * This algorithm (Section 6.6.2) checks if a path exists from
 * vertex 's' to vertex 't' in a *directed* graph using only
 * O(log n) memory.
 *
 * It runs in a loop, alternating between two phases:
 * 1. WALK: Perform a random walk of n-1 steps. If 't' is found, return YES.
 * 2. QUIT: Flip k = n*log(n) coins. If all are HEADS, return NO.
 *
 * The probability of finding a path (if one exists) in one walk is
 * at least 1/n^n. The probability of quitting is 1/2^(n*log n) = 1/n^n.
 * Because P(success) > P(quit), this algorithm will eventually
 * find the path with probability > 1/2.
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <chrono>

// Graph is an Adjacency List: adj[u] -> vector of neighbors of u
using Graph = std::vector<std::vector<int>>;

/**
 * @brief Solves STCON in log-space using a randomized algorithm.
 * @param adj The graph's adjacency list.
 * @param s The start vertex.
 * @param t The target vertex.
 * @return true if a path is found, false if the quit condition is met.
 */
bool solveSTCON(const Graph& adj, int s, int t) {
    int n = adj.size();
    if (n == 0) return false;

    // 1. Set up random number generator
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> coin_dist(0, 1);

    // 2. Define algorithm parameters
    // Length of the random walk (max simple path)
    int walkLimit = n - 1; 
    // Number of "quit coins" to flip. We need P(quit) = 1/n^n
    // So we flip k coins where 2^k = n^n -> k = n*log2(n)
    int quitCoinFlips = static_cast<int>(std::ceil(n * std::log2(n)));

    // 3. Start the main loop. This runs until we return.
    while (true) {
        
        // --- PHASE 1: The Random Walk ---
        int current_v = s;
        for (int step = 0; step < walkLimit; ++step) {
            // Found it!
            if (current_v == t) {
                return true;
            }

            // Hit a dead end
            if (adj[current_v].empty()) {
                break; // End this walk and go to Phase 2
            }

            // Perform one random step
            int out_degree = adj[current_v].size();
            std::uniform_int_distribution<int> edge_dist(0, out_degree - 1);
            int neighbor_index = edge_dist(gen);
            current_v = adj[current_v][neighbor_index];
        }
        
        // Walk finished without finding t.

        // --- PHASE 2: The Quit Condition ---
        bool all_heads = true;
        for (int i = 0; i < quitCoinFlips; ++i) {
            if (coin_dist(gen) == 0) { // 0 = Tails
                all_heads = false;
                break;
            }
        }

        if (all_heads) {
            // Extremely unlikely, but this is the "quit" condition
            return false;
        }
        
        // If we didn't quit, the while(true) loop repeats.
    }
}

// --- Main Function (Demonstration) ---

int main() {
    // 5 vertices (0 to 4)
    int n = 5;
    Graph adj1(n);

    // Case 1: Path exists (0 -> 1 -> 3 -> 4)
    adj1[0].push_back(1);
    adj1[0].push_back(2);
    adj1[1].push_back(3);
    adj1[2].push_back(3);
    adj1[3].push_back(4);
    // 4 is a dead end
    
    int s1 = 0, t1 = 4;
    std::cout << "Checking for path from " << s1 << " to " << t1 << " (exists)..." << std::endl;
    // We run it 10 times to show it's reliable
    int successes = 0;
    for (int i = 0; i < 10; ++i) {
        if (solveSTCON(adj1, s1, t1)) {
            successes++;
        }
    }
    std::cout << "  Result: " << successes << "/10 runs found the path." << std::endl;

    std::cout << "---" << std::endl;

    // Case 2: No path exists (t=2 is not reachable from s=0)
    Graph adj2(n);
    adj2[0].push_back(1);
    adj2[1].push_back(0);
    adj2[3].push_back(2);
    
    int s2 = 0, t2 = 2;
    std::cout << "Checking for path from " << s2 << " to " << t2 << " (does not exist)..." << std::endl;
    bool found = solveSTCON(adj2, s2, t2);
    std::cout << "  Result: " << (found ? "Path found (ERROR)" : "No path (Correct)") << std::endl;
    
    return 0;
}