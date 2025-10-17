/*
 * ALGORITHM 1: Randomized 2-SAT (Section 6.1)
 *
 * This algorithm finds a satisfying assignment for a 2-CNF formula.
 * It works by starting with a random assignment and then, for a
 * polynomial number of steps, picking an unsatisfied clause and
 * randomly flipping one of its literals.
 *
 * This process is a biased random walk on the "state space" of
 * assignments, biased towards a satisfying one.
 * The expected runtime is O(n^2), so we use a O(n^2) timeout.
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <random>       // For std::mt19937
#include <chrono>       // For seeding
#include <utility>      // For std::pair
#include <cstdlib>      // For std::abs

// A Clause is a pair of integers.
using Clause = std::pair<int, int>;

// An Assignment is a vector of booleans, 0-indexed.
using Assignment = std::vector<bool>;

// --- Helper Functions ---

/**
 * @brief Checks if a single clause is satisfied by the current assignment.
 */
bool isClauseSatisfied(const Clause& clause, const Assignment& assignment) {
    int lit1 = clause.first;
    int lit2 = clause.second;

    // Get the truth value of the first literal
    bool val1 = (lit1 > 0) ? 
                assignment[lit1 - 1] : 
                !assignment[std::abs(lit1) - 1];

    // Get the truth value of the second literal
    bool val2 = (lit2 > 0) ? 
                assignment[lit2 - 1] : 
                !assignment[std::abs(lit2) - 1];

    return val1 || val2;
}

/**
 * @brief Finds the index of the first unsatisfied clause.
 * @return Index of an unsatisfied clause, or -1 if all are satisfied.
 */
int findUnsatisfiedClause(const std::vector<Clause>& clauses, const Assignment& assignment) {
    for (int i = 0; i < clauses.size(); ++i) {
        if (!isClauseSatisfied(clauses[i], assignment)) {
            return i;
        }
    }
    return -1; // All clauses are satisfied
}

// --- Core Algorithm ---

/**
 * @brief Solves a 2-SAT instance using a randomized walk.
 * @param clauses The 2-CNF formula.
 * @param n The number of variables (variables are 1 to n).
 * @return A pair where .first is true if a solution was found,
 * and .second is the satisfying assignment.
 */
std::pair<bool, Assignment> solve2SAT(const std::vector<Clause>& clauses, int n) {
    
    // 1. Set up high-quality random number generator
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> bool_dist(0, 1);

    // 2. Start with a random initial assignment
    Assignment assignment(n);
    for (int i = 0; i < n; ++i) {
        assignment[i] = (bool_dist(gen) == 1);
    }

    // 3. Set timeout. Theory says expected time is O(n^2).
    // We'll use a 2*n*n timeout for this Monte Carlo algorithm.
    long long timeout = 2LL * n * n;

    for (long long k = 0; k < timeout; ++k) {
        // 4. Check if the current assignment is a solution
        int unsatIndex = findUnsatisfiedClause(clauses, assignment);
        
        if (unsatIndex == -1) {
            // Success! Return {true, solution}
            return {true, assignment};
        }

        // 5. Not a solution: Pick the unsatisfied clause
        const Clause& unsatClause = clauses[unsatIndex];

        // 6. Randomly pick one of the two literals to flip
        int litToFlip = (bool_dist(gen) == 0) ? unsatClause.first : unsatClause.second;
        
        // 7. Flip the corresponding variable's assignment
        int varIndex = std::abs(litToFlip) - 1;
        assignment[varIndex] = !assignment[varIndex];
    }

    // 8. Timeout reached, no solution found. Return {false, empty_assignment}
    return {false, {}};
}

// --- Main Function (Demonstration) ---

int main() {
    // Example 1: A satisfiable formula
    // (x1 \/ x2) /\ (!x1 \/ x3) /\ (x1 \/ !x3)
    int n1 = 3;
    std::vector<Clause> clauses1 = {{1, 2}, {-1, 3}, {1, -3}};

    std::cout << "Solving (x1 v x2) & (!x1 v x3) & (x1 v !x3)..." << std::endl;
    
    // Check the 'bool' (first) part of the pair
    std::pair<bool, Assignment> result1 = solve2SAT(clauses1, n1);
    if (result1.first) {
        std::cout << "  SATISFIABLE. Solution:" << std::endl;
        const Assignment& solution = result1.second;
        for (int i = 0; i < n1; ++i) {
            std::cout << "    x" << (i + 1) << " = " << (solution[i] ? "T" : "F") << std::endl;
        }
    } else {
        std::cout << "  UNSATISFIABLE (or timeout reached)" << std::endl;
    }

    std::cout << "---" << std::endl;

    // Example 2: An unsatisfiable formula
    // (x1 \/ x1) /\ (!x1 \/ !x1)
    int n2 = 1;
    std::vector<Clause> clauses2 = {{1, 1}, {-1, -1}};
    
    std::cout << "Solving (x1) & (!x1)..." << std::endl;
    std::pair<bool, Assignment> result2 = solve2SAT(clauses2, n2);
    if (result2.first) {
        // This is extremely unlikely but theoretically possible if timeout is huge
        std::cout << "  SATISFIABLE. Solution:" << std::endl;
    } else {
        std::cout << "  UNSATISFIABLE (or timeout reached)" << std::endl;
    }

    return 0;
}