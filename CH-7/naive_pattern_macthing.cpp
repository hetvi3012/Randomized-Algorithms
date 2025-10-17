/*
 * ALGORITHM 1: Naive Deterministic Pattern Matching
 *
 * Implements the baseline O(nm) string matching algorithm.
 * This is the "straw man" to show the speedup of Karp-Rabin.
 */

#include <iostream>
#include <string>
#include <vector>

/**
 * @brief Finds all occurrences of a pattern in a text using the naive method.
 * @param text The text string to search in.
 * @param pattern The pattern string to search for.
 * @return A vector of 0-based indices where the pattern starts in the text.
 */
std::vector<int> naivePatternMatch(const std::string& text, const std::string& pattern) {
    std::vector<int> matches;
    int n = text.length();
    int m = pattern.length();

    if (m == 0) return matches;

    // Loop through all possible starting positions in the text
    for (int j = 0; j <= n - m; ++j) {
        
        // Check for a match starting at index j
        int i;
        for (i = 0; i < m; ++i) {
            if (text[j + i] != pattern[i]) {
                break; // Mismatch, break inner loop
            }
        }

        // If the inner loop completed, we found a match
        if (i == m) {
            matches.push_back(j);
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

    std::vector<int> matches = naivePatternMatch(text, pattern);

    std::cout << "Naive matches found at indices: ";
    if (matches.empty()) {
        std::cout << "None";
    } else {
        for (int index : matches) {
            std::cout << index << " ";
        }
    }
    std::cout << std::endl;

    return 0;
}