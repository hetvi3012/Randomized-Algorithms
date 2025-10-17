/*
 * ALGORITHM 3: BPP Probability Amplification (Section 6.8)
 *
 * This algorithm takes a BPP algorithm A (with error, say, 1/100)
 * and amplifies its correctness to have an error of 1/2^k.
 *
 * It does this by performing a random walk on an implicit
 * (n, d)-expander graph whose vertices are the N = 2^n possible
 * random strings.
 *
 * 1. We choose 'd' random n-bit strings S_1, ..., S_d.
 * 2. We start at a random n-bit string r_0.
 * 3. We walk for 7*k steps: r_i = r_{i-1} XOR S_j (j is random).
 * 4. We run A(x, r_i) at each step and take the majority vote.
 *
 * This works because the walk is "rapidly mixing," so the
 * sequence r_0...r_{7k} is "random-like" enough for the
 * Chernoff bound to apply, but uses far fewer initial random bits.
 */

#include <iostream>
#include <vector>
#include <string>
#include <numeric> // For std::iota
#include <random>
#include <chrono>

// Use a vector of bools to represent an n-bit string
using BitString = std::vector<bool>;

// --- Helper Functions ---

/**
 * @brief Creates a random n-bit string.
 */
BitString createRandomBitString(int n, std::mt19937& gen) {
    std::uniform_int_distribution<int> dist(0, 1);
    BitString s(n);
    for (int i = 0; i < n; ++i) {
        s[i] = (dist(gen) == 1);
    }
    return s;
}

/**
 * @brief Computes the bitwise XOR of two BitStrings.
 */
BitString bitwiseXOR(const BitString& a, const BitString& b) {
    int n = a.size();
    BitString result(n);
    for (int i = 0; i < n; ++i) {
        result[i] = a[i] ^ b[i];
    }
    return result;
}

/**
 * @brief Our "dummy" BPP algorithm to test.
 * It has a 1/100 (1%) error rate.
 */
bool dummyBPPAlgorithm(const std::string& input, const BitString& r, std::mt19937& gen) {
    std::uniform_int_distribution<int> error_dist(0, 99);
    
    // 99% of the time, it's correct
    bool shouldBeCorrect = (error_dist(gen) != 0); 
    
    bool correctAnswer = (input == "YES");

    if (shouldBeCorrect) {
        return correctAnswer;
    } else {
        // 1% of the time, it's wrong
        return !correctAnswer;
    }
}

// --- Core Algorithm ---

/**
 * @brief Runs the amplified BPP algorithm.
 * @param input The input to the BPP algorithm ("YES" or "NO").
 * @param n The number of random bits the BPP algorithm needs.
 * @param k The amplification factor (target error is 1/2^k).
 * @return The (highly likely) correct boolean answer.
 */
bool amplifyBPP(const std::string& input, int n, int k) {
    
    // 1. Set up random number generator
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);

    // 2. Define graph/walk parameters
    // The text uses d=7 for the Gabber-Galil expander
    const int d = 7; 
    // We take 7k steps (k "rounds" of 7 steps)
    const int walkLength = 7 * k; 

    // 3. Create the 'd' random n-bit "shift" vectors
    std::vector<BitString> S(d);
    for (int j = 0; j < d; ++j) {
        S[j] = createRandomBitString(n, gen);
    }

    // 4. Start at a random n-bit string r_0
    BitString current_r = createRandomBitString(n, gen);

    // 5. Perform the walk and collect votes
    int yes_votes = 0;
    int no_votes = 0;
    std::uniform_int_distribution<int> walk_dist(0, d - 1);

    for (int i = 0; i < walkLength; ++i) {
        // a. Run the algorithm on the *current* state
        // Note: The text runs it on r_i, not r_{i-1}. We'll adjust.
        
        // b. Perform the walk step: r_i = r_{i-1} XOR S_j
        int j = walk_dist(gen);
        current_r = bitwiseXOR(current_r, S[j]);

        // c. Run algorithm and tally vote
        bool result = dummyBPPAlgorithm(input, current_r, gen);
        if (result) {
            yes_votes++;
        } else {
            no_votes++;
        }
    }

    // 6. Return the majority vote
    return (yes_votes > no_votes);
}

// --- Main Function (Demonstration) ---

int main() {
    // Number of random bits our BPP algorithm uses
    int n = 20; 
    // We want to amplify correctness to an error rate of 1 / 2^k
    int k = 10; // Target error: 1 / 2^10 (~ 1/1000)

    std::cout << "Running BPP amplification for input 'YES'" << std::endl;
    std::cout << "Target error: 1/2^" << k << std::endl;
    std::cout << "Running 10 trials..." << std::endl;
    int correct_trials = 0;
    for (int i = 0; i < 10; ++i) {
        if (amplifyBPP("YES", n, k) == true) {
            correct_trials++;
        }
    }
    std::cout << "  Result: " << correct_trials << "/10 correct." << std::endl;

    std::cout << "---" << std::endl;

    std::cout << "Running BPP amplification for input 'NO'" << std::endl;
    std::cout << "Target error: 1/2^" << k << std::endl;
    std::cout << "Running 10 trials..." << std::endl;
    correct_trials = 0;
    for (int i = 0; i < 10; ++i) {
        if (amplifyBPP("NO", n, k) == false) {
            correct_trials++;
        }
    }
    std::cout << "  Result: " << correct_trials << "/10 correct." << std::endl;

    return 0;
}