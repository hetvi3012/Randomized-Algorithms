# Randomized Algorithms: Course Project Implementations

This repository contains C++ implementations of 8 key randomized algorithms from a course on Randomized Algorithms. The implementations are based on the concepts presented in:

* **Chapter 6: Markov Chains and Random Walks**
* **Chapter 7: Algebraic Techniques**

## About This Project

This project serves as a practical exploration of the theoretical concepts discussed in the course. Each algorithm is implemented as a standalone C++ file, demonstrating the core principles of its design, from random walks on a line to algebraic fingerprinting and polynomial identity testing.

## How to Compile and Run

All implementations are written in standard C++. You will need a C++ compiler (like `g++` or `clang++`) that supports the **C++17 standard** (or C++11 for the 2-SAT alternate).

To compile a specific algorithm (e.g., `freivalds.cpp`):

```bash
# Using g++
g++ -std=c++17 -Wall -o freivalds freivalds.cpp

# Then run it
./freivalds
```
## Chapter 6: Markov Chains and Random Walks

This chapter explores algorithms based on the statistical properties of random walks on graphs, such as hitting times, cover times, and the "mixing rate" of a Markov chain.

### 1. Randomized 2-SAT (Section 6.1)

* **File:** `2SAT.cpp`
* **Problem:** Find a satisfying truth assignment for a 2-CNF (Conjunctive Normal Form) formula, or determine that none exists.
* **Core Idea (The Random Walk):**
    * This algorithm is a classic example of a **biased random walk on a line**.
    * We start with an arbitrary truth assignment.
    * If the formula is not satisfied, we pick an unsatisfied clause.
    * We then randomly pick one of the two literals in that clause and **flip** its variable's truth value.
    * The "position" of our walk is the number of variables in our assignment that match a fixed (but unknown) satisfying assignment, $A$.
    * Because the clause is unsatisfied, at least one of its literals must be "wrong" (i.e., different from $A$). This means we have a **$\ge 1/2$ probability** of flipping a "wrong" variable to a "right" one, moving us *closer* to the solution $A$.
    * This bias guarantees that the walk will find a satisfying assignment (if one exists) in $O(n^2)$ expected steps. Our implementation runs for $2n^2$ steps as a timeout.

### 2. Directed s-t Connectivity (STCON) (Section 6.6.2)

* **File:** `STCON.cpp`
* **Problem:** Determine if a path exists from a vertex `s` to a vertex `t` in a **directed graph** using only **$O(\log n)$ memory**.
* **Core Idea (Balancing Probabilities):**
    * A simple random walk isn't enough, as it can get "stuck" in dead ends or parts of the graph that cannot reach `t`. This algorithm cleverly works by balancing two very small probabilities:
    * **Walk Phase:** The algorithm starts a random walk from `s` for $n-1$ steps. The probability of finding a specific path to `t` is tiny but non-zero (at least $p \ge 1/n^n$). If `t` is found, return `YES`.
    * **Quit Phase:** If the walk fails (hits a dead end or times out), the algorithm flips $k = n \log n$ coins. If all $k$ coins land heads (an event with probability $q = 1/2^k \approx 1/n^n$), the algorithm gives up and returns `NO`.
    * Because the probability of success ($p$) is at least as large as the probability of quitting ($q$), the algorithm is guaranteed to find the path (if one exists) with probability $> 1/2$ before it erroneously quits.

### 3. BPP Probability Amplification (Section 6.8)

* **File:** `BPP_Amplification.cpp`
* **Problem:** Reduce the error of a BPP (Bounded-error Probabilistic Polynomial-time) algorithm from a constant (e.g., 1/100) to an exponentially small value ($1/2^k$).
* **Core Idea (Rapidly Mixing Walk):**
    * Instead of running $k$ independent trials (which requires $k \times n$ random bits), this algorithm uses a **random walk on an implicit expander graph**.
    * The "vertices" of this graph are all $N = 2^n$ possible random strings.
    * The "edges" are defined by $d$ random "shift" vectors $S_1, \dots, S_d$. A step in the walk is defined as $r_i = r_{i-1} \oplus S_j$ for a random $j$.
    * Because this (implicit) graph is an expander, the random walk is **rapidly mixing** (Theorem 6.21). This means the sequence of strings $r_1, \dots, r_{7k}$ it generates becomes "un-correlated" very quickly.
    * This "pseudorandom" sequence is "random-like" enough to achieve the same exponential error reduction as a truly random sequence, but using far fewer initial random bits (only $n(d+1)$ bits).

---

## Chapter 7: Algebraic Techniques

This chapter explores algorithms that leverage algebra, primarily through **fingerprinting** (mapping large objects to small, random algebraic values) and **polynomial identity testing**.

### 1. Naive Deterministic Pattern Matching (Section 7.6)

* **File:** `Naive_Pattern_Match.cpp`
* **Problem:** Find all occurrences of a pattern $P$ (length $m$) in a text $T$ (length $n$).
* **Core Idea (The Baseline):**
    * This is not a randomized algorithm. It is the simple, deterministic "straw man" algorithm that runs in $O(nm)$ time. It serves as a baseline to demonstrate the power of the randomized Karp-Rabin algorithm.
    * **Implementation:** A simple nested loop. The outer loop iterates through all $n-m+1$ possible starting positions in $T$. The inner loop compares $P$ character-by-character at that position.

### 2. Karp-Rabin Pattern Matching (Monte Carlo) (Section 7.6)

* **File:** `Karp_Rabin_Monte_Carlo.cpp`
* **Problem:** Find all occurrences of $P$ in $T$ in $O(n+m)$ expected time.
* **Core Idea (Rolling Hash):**
    * This algorithm uses **algebraic fingerprinting**.
    * It treats the pattern $P$ and all $m$-length substrings of $T$ as large numbers.
    * Instead of comparing the numbers, it compares their "fingerprints," $hash(S) = S \mod p$, for a large prime $p$.
    * The key to its $O(n+m)$ runtime is the **rolling hash**. This is an algebraic recurrence that allows us to compute the hash of the *next* substring (e.g., $T[j+1 \dots j+m]$) from the hash of the *previous* one ($T[j \dots j+m-1]$) in $O(1)$ time.
    * This version is **Monte Carlo** because it *trusts* a hash match. It's possible (though unlikely) for two different strings to have the same hash, leading to a "false positive" match.

### 3. Karp-Rabin Pattern Matching (Las Vegas) (Section 7.6)

* **File:** `Karp_Rabin_Las_Vegas.cpp`
* **Problem:** Find all occurrences of $P$ in $T$ in $O(n+m)$ expected time, with **zero error**.
* **Core Idea (Verify on Match):**
    * This algorithm promotes the Monte Carlo version to a **Las Vegas** algorithm (zero error, randomized runtime).
    * **Implementation:** It is identical to the Monte Carlo version, with one crucial addition:
    * **When** the fingerprints $hash(P)$ and $hash(T[j\dots])$ match, the algorithm performs a final, $O(m)$ deterministic, character-by-character check.
    * It only reports a match if this deterministic check also passes. This eliminates all false positives, guaranteeing a correct answer. The expected runtime remains $O(n+m)$ because hash collisions are rare.

### 4. Freivalds' Technique (Section 7.1)

* **File:** `Freivalds_Technique.cpp`
* **Problem:** Verify if $A \times B = C$ for three $n \times n$ matrices in $O(n^2)$ time, which is much faster than the $O(n^3)$ required to compute the product $A \times B$.
* **Core Idea (Vector Fingerprint):**
    * Instead of checking the matrix identity $AB=C$, the algorithm checks a probabilistic identity $ABr=Cr$.
    * It generates a random $n \times 1$ vector $r$ with entries from $\{0, 1\}$.
    * It then computes two vectors: $v_1 = A \times (B \times r)$ and $v_2 = C \times r$.
    * This is fast because matrix-vector multiplication is only $O(n^2)$.
    * If $AB = C$, then $v_1$ will always equal $v_2$.
    * If $AB \neq C$, there is at most a **$1/2$ probability** that $v_1 = v_2$ (a false positive). By running the test $k$ times, we can reduce the error probability to $1/2^k$.

### 5. Perfect Matching in Bipartite Graphs (Section 7.3)

* **File:** `Perfect_Matching.cpp`
* **Problem:** Determine if a bipartite graph $G$ (with $n$ vertices on each side) has a perfect matching.
* **Core Idea (Polynomial Identity Testing):**
    * This algorithm brilliantly converts a graph problem into an algebraic one.
    * **Edmonds' Theorem (7.3)** states that a bipartite graph $G$ has a perfect matching *if and only if* the determinant of its symbolic **Edmonds Matrix** $A_G$ is not the zero polynomial.
    * We need to test if $\det(A_G) \equiv 0$. We use the **Schwartz-Zippel Lemma (7.2)** to do this.
    * We don't build the symbolic matrix. Instead, we create a single *numeric* matrix $A'$ by replacing each edge $(u_i, v_j)$ with a **random integer** from a large set $S$. All non-edges remain $0$.
    * We then compute the standard determinant $\det(A')$.
    * If $G$ has *no* perfect matching, $\det(A_G)$ was the zero polynomial, so $\det(A')$ will *always* be 0.
    * If $G$ *has* a perfect matching, $\det(A_G)$ was a non-zero polynomial, so $\det(A')$ will be **non-zero with high probability**.
    * Our implementation computes the determinant modulo a prime $p$ to prevent integer overflow.

## Conclusion

This project successfully implemented a suite of 8 algorithms, providing a practical demonstration of the two major paradigms presented in the course: **Random Walks** (Chapter 6) and **Algebraic Techniques** (Chapter 7).

The implementations from **Chapter 6** (2-SAT, STCON, BPP Amplification) highlight how the statistical properties of Markov chains can be harnessed for algorithm design. We saw how a complex problem can be modeled as:

* A **biased 1D random walk** to find a guaranteed solution (2-SAT).
* A clever **space-bounded walk** to solve directed connectivity (STCON).
* A **rapidly-mixing walk** on an implicit expander graph to achieve exponential probability amplification (BPP).

The implementations from **Chapter 7** (Freivalds', Karp-Rabin, Perfect Matching) demonstrate the power of **algebraic fingerprinting**. By mapping large, complex objects (matrices, strings, graphs) to small, random algebraic values (vectors, modular numbers, determinants), we were able to solve problems with remarkable efficiency. We saw this in:

* Checking matrix multiplication *faster* than computing it (Freivalds' Technique).
* Finding string patterns in linear time with a rolling hash (Karp-Rabin).
* Solving a core graph problem (Perfect Matching) with a non-obvious algebraic tool (the determinant).

Finally, this project explored the crucial difference between **Monte Carlo** algorithms (which are fast but can err, like Freivalds') and **Las Vegas** algorithms (which are always correct but have a variable runtime, like the verified Karp-Rabin).

Overall, this project bridges the gap between theory and practice, demonstrating that randomization is a powerful, flexible, and essential tool for creating algorithms that are often simpler, faster, or more space-efficient than their deterministic counterparts.
