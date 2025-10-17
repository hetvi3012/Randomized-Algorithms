/*
 * ALGORITHM 5: Perfect Matching in Bipartite Graphs
 *
 * Implements the randomized algorithm from Section 7.3.
 * Uses Edmonds' Theorem and the Schwartz-Zippel Lemma.
 *
 * 1. Create a matrix A' from the graph's adjacency matrix.
 * 2. If (u_i, v_j) is an edge, A'[i][j] = random number from Z_p.
 * 3. If (u_i, v_j) is not an edge, A'[i][j] = 0.
 * 4. Compute det(A') mod p.
 * 5. If det != 0, a perfect matching exists (w.h.p.)
 *
 * This implementation requires a modular determinant, which is non-trivial.
 */

#include <iostream>
#include <vector>
#include <random>
#include <chrono>

using ll = long long;
using Matrix = std::vector<std::vector<ll>>;

// A large prime for our finite field Z_p
const ll p = 1000000007;

// --- Modular Arithmetic Helpers ---

/**
 * @brief Computes (base^exp) % p efficiently.
 */
ll power(ll base, ll exp) {
    ll res = 1;
    base %= p;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % p;
        base = (base * base) % p;
        exp /= 2;
    }
    return res;
}

/**
 * @brief Computes the modular multiplicative inverse of n (mod p)
 * using Fermat's Little Theorem. Assumes p is prime.
 * We need this for division: a / b % p = a * b^(p-2) % p
 */
ll modInverse(ll n) {
    return power(n, p - 2);
}

/**
 * @brief Computes the determinant of a matrix modulo p.
 * Uses Gaussian elimination.
 * @param A The matrix. Will be modified in-place.
 * @return The determinant (mod p).
 */
ll modularDeterminant(Matrix& A) {
    int n = A.size();
    ll det = 1;

    for (int k = 0; k < n; ++k) {
        // Find pivot
        int pivot = k;
        while (pivot < n && A[pivot][k] == 0) {
            pivot++;
        }

        if (pivot == n) {
            return 0; // No non-zero pivot, determinant is 0
        }

        // Swap rows k and pivot
        if (k != pivot) {
            std::swap(A[k], A[pivot]);
            det = (p - det) % p; // Flip sign of determinant
        }

        // For all rows below the pivot row
        for (int i = k + 1; i < n; ++i) {
            // We want to make A[i][k] = 0
            // factor = A[i][k] / A[k][k]
            ll factor = (A[i][k] * modInverse(A[k][k])) % p;

            // R_i = R_i - factor * R_k
            for (int j = k; j < n; ++j) {
                ll term = (factor * A[k][j]) % p;
                A[i][j] = (A[i][j] - term + p) % p; // Add p to handle negatives
            }
        }
    }

    // Determinant is the product of the diagonal elements
    for (int i = 0; i < n; ++i) {
        det = (det * A[i][i]) % p;
    }

    return (det + p) % p;
}

/**
 * @brief Checks if a bipartite graph has a perfect matching.
 * @param graph An n x n adjacency matrix where graph[i][j] = 1
 * if an edge exists between U_i and V_j.
 * @return true if a perfect matching likely exists, false otherwise.
 */
bool hasPerfectMatching(const std::vector<std::vector<int>>& graph) {
    int n = graph.size();
    if (n == 0) return true;

    // 1. Set up random number generator
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    std::uniform_int_distribution<ll> distribution(1, p - 1); // Random values in [1, p-1]

    // 2. Create the symbolic matrix A' with random numbers
    Matrix A_prime(n, std::vector<ll>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (graph[i][j] == 1) {
                A_prime[i][j] = distribution(generator);
            } else {
                A_prime[i][j] = 0;
            }
        }
    }

    // 3. Compute the determinant modulo p
    ll det = modularDeterminant(A_prime);

    // 4. If det != 0, a matching exists (w.h.p.)
    return det != 0;
}

// Main function to demonstrate the algorithm
int main() {
    // Case 1: A graph WITH a perfect matching
    // (0,0), (1,2), (2,1) is a perfect matching
    std::vector<std::vector<int>> graph1 = {
        {1, 1, 0},
        {0, 0, 1},
        {0, 1, 1}
    };

    std::cout << "Checking graph 1 (has matching): ";
    bool result1 = hasPerfectMatching(graph1);
    std::cout << (result1 ? "Matching likely found" : "No matching") << std::endl;

    // Case 2: A graph WITHOUT a perfect matching
    // (Hall's condition fails: U_0 and U_1 map to only V_1)
    std::vector<std::vector<int>> graph2 = {
        {0, 1, 0},
        {0, 1, 0},
        {1, 0, 1}
    };
    
    std::cout << "Checking graph 2 (no matching): ";
    bool result2 = hasPerfectMatching(graph2);
    std::cout << (result2 ? "Matching likely found (False Positive)" : "No matching") << std::endl;

    return 0;
}