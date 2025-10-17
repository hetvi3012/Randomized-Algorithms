/*
 * ALGORITHM 4: Freivalds' Technique
 *
 * Implements the O(n^2) randomized algorithm to verify A * B = C.
 * This is much faster than the O(n^3) required to compute A * B.
 * The result is correct with probability >= 1/2.
 * To get error probability <= 1/2^k, run this k times.
 */

#include <iostream>
#include <vector>
#include <random>       // For std::mt19937
#include <chrono>       // For seeding the random generator

// Define a Matrix as a 2D vector
using Matrix = std::vector<std::vector<int>>;
// Define a Vector as a 1D vector
using Vector = std::vector<int>;

/**
 * @brief Multiplies an n x n matrix by an n x 1 vector.
 * @return An n x 1 vector (the result).
 */
Vector matrixVectorMultiply(const Matrix& M, const Vector& r) {
    int n = M.size();
    Vector result(n, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            result[i] += M[i][j] * r[j];
        }
    }
    return result;
}

/**
 * @brief Verifies if A * B = C using Freivalds' technique (single iteration).
 * @return true if A(Br) == Cr, false otherwise.
 */
bool freivaldsVerify(const Matrix& A, const Matrix& B, const Matrix& C) {
    int n = A.size();
    if (n == 0 || A[0].size() != n || B.size() != n || B[0].size() != n || C.size() != n || C[0].size() != n) {
        return false; // Invalid dimensions
    }

    // 1. Set up a high-quality random number generator
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> distribution(0, 1);

    // 2. Generate random n x 1 vector r with {0, 1} entries
    Vector r(n);
    for (int i = 0; i < n; ++i) {
        r[i] = distribution(generator);
    }

    // 3. Compute v1 = B * r  (O(n^2))
    Vector Br = matrixVectorMultiply(B, r);
    
    // 4. Compute v2 = A * (B * r)  (O(n^2))
    Vector A_Br = matrixVectorMultiply(A, Br);

    // 5. Compute v3 = C * r  (O(n^2))
    Vector Cr = matrixVectorMultiply(C, r);

    // 6. Compare v2 and v3  (O(n))
    return A_Br == Cr;
}

// Helper function to print a matrix
void printMatrix(const Matrix& M) {
    for (const auto& row : M) {
        for (int val : row) {
            std::cout << val << "\t";
        }
        std::cout << std::endl;
    }
}

// Main function to demonstrate the algorithm
int main() {
    int n = 3;
    
    // Case 1: A * B = C (Correct)
    Matrix A = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    Matrix B = {{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};
    // C is the correct product of A and B
    Matrix C_correct = {{30, 24, 18}, {84, 69, 54}, {138, 114, 90}};

    // Case 2: A * B != C (Incorrect)
    Matrix C_incorrect = {{30, 24, 18}, {84, 69, 54}, {138, 114, 91}}; // One entry is wrong
    
    std::cout << "Verifying A * B = C_correct (should be true):" << std::endl;
    bool result1 = freivaldsVerify(A, B, C_correct);
    std::cout << "Result: " << (result1 ? "Verified" : "Failed") << std::endl;

    std::cout << "\nVerifying A * B = C_incorrect (should be false):" << std::endl;
    bool result2 = freivaldsVerify(A, B, C_incorrect);
    std::cout << "Result: " << (result2 ? "Verified" : "Failed") << std::endl;
    
    // Note: result2 has a < 50% chance of being "Verified" (false positive).
    // To be sure, we run it k times.
    int k = 10;
    bool overallResult = true;
    for(int i = 0; i < k; ++i) {
        if (!freivaldsVerify(A, B, C_incorrect)) {
            overallResult = false;
            break; // Found a discrepancy, definitely not equal
        }
    }
    std::cout << "After " << k << " iterations, verification of incorrect C: ";
    std::cout << (overallResult ? "Verified (False Positive)" : "Failed (Correctly Identified)") << std::endl;


    return 0;
}