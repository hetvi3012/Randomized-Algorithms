/*
 * ALGORITHM 2: Karp-Rabin Pattern Matching (Monte Carlo)
 *
 * Implements the O(n+m) randomized algorithm.
 * Uses a rolling hash to find matches.
 * This Monte Carlo version assumes a hash match is a true match
 * and has a small (but non-zero) probability of false positives.
 */

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

// We use long long to avoid overflow during intermediate calculations
using ll = long long;

// d: size of the alphabet (e.g., 256 for ASCII)
const int d = 256;
// p: A large prime number for the modulo operation
const ll p = 1000000007;

/**
 * @brief Helper function to compute (base^exp) % mod efficiently.
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
 * @brief Finds potential occurrences of a pattern in a text using Karp-Rabin.
 * @param text The text string to search in.
 * @param pattern The pattern string to search for.
 * @return A vector of indices where a hash match occurred.
 */
std::vector<int> karpRabinMonteCarlo(const std::string& text, const std::string& pattern) {
    std::vector<int> matches;
    int n = text.length();
    int m = pattern.length();
    
    if (m == 0 || m > n) return matches;

    ll patternHash = 0;
    ll textHash = 0;
    ll h = power(d, m - 1); // h = d^(m-1) % p

    // Calculate the hash value of the pattern and the first window of the text
    for (int i = 0; i < m; ++i) {
        patternHash = (d * patternHash + pattern[i]) % p;
        textHash = (d * textHash + text[i]) % p;
    }

    // Slide the pattern over the text one by one
    for (int j = 0; j <= n - m; ++j) {
        
        // Check if the hash values match
        if (patternHash == textHash) {
            // Monte Carlo: We trust the hash and report a match
            matches.push_back(j);
        }

        // Calculate the hash value for the next window
        if (j < n - m) {
            // Remove leading digit, add trailing digit
            // textHash = (d * (textHash - text[j] * h) + text[j + m]) % p;
            
            ll term1 = (textHash - (ll)text[j] * h) % p;
            ll term2 = (d * (term1 + p)) % p; // Add p to handle potential negative
            textHash = (term2 + text[j + m]) % p;
        }
    }
    return matches;
}

// Main function to demonstrate the algorithm
int main() {
    std::string text = "abacaabaccabacabaabb";
    std::string pattern = "abacab";

    std::cout << "Text:    " << text << std::endl;
    std::cout << "Pattern: " << pattern << std::endl;

    std::vector<int> matches = karpRabinMonteCarlo(text, pattern);

    std::cout << "Karp-Rabin (Monte Carlo) matches found at indices: ";
    if (matches.empty()) {
        std::cout << "None";
    } else {
        for (int index : matches) {
            std::cout << index << " ";
        }
    }
    std::cout << std::endl;
    
    // Example of a false positive (unlikely with this prime, but possible)
    std::string text2 = "AABBAACA";
    std::string pattern2 = "BBAA"; // Hash might collide with "AACA"
    std::cout << "\nText:    " << text2 << std::endl;
    std::cout << "Pattern: " << pattern2 << std::endl;
    std::vector<int> matches2 = karpRabinMonteCarlo(text2, pattern2);
    std::cout << "Karp-Rabin (Monte Carlo) matches: ";
    for (int index : matches2) std::cout << index << " ";
    std::cout << "(Note: may contain false positives)" << std::endl;


    return 0;
}