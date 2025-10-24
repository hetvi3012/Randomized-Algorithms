# Randomized Algorithms: Course Project Implementations

This repository contains C++ implementations of key randomized algorithms from a course on Randomized Algorithms. The implementations are based on the concepts presented in:

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

* **File:** `randomized-2-sat.cpp`
* **Problem:** Find a satisfying truth assignment for a 2-CNF (Conjunctive Normal Form) formula, or determine that none exists.
* **Core Idea (The Random Walk):**
    * This algorithm is a classic example of a **biased random walk on a line**.
    * We start with an arbitrary truth assignment.
    * If the formula is not satisfied, we pick an unsatisfied clause.
    * We then randomly pick one of the two literals in that clause and **flip** its variable's truth value.
    * The "position" of our walk is the number of variables in our assignment that match a fixed (but unknown) satisfying assignment, $A$.
    * Because the clause is unsatisfied, at least one of its literals must be "wrong" (i.e., different from $A$). This means we have a **$\ge 1/2$ probability** of flipping a "wrong" variable to a "right" one, moving us *closer* to the solution $A$.
    * This bias guarantees that the walk will find a satisfying assignment (if one exists) in $O(n^2)$ expected steps. Our implementation runs for $2n^2$ steps as a timeout.
* **Implementation Details:**
    * Uses `Literal` and `Clause` structures for clean formula representation
    * Random initial assignment followed by iterative variable flipping
    * Tests two formulas: one satisfiable (3 variables) and one unsatisfiable (1 variable)

### 2. Undirected s-t Connectivity (USTCON) via Random Walk

* **File:** `randomized-ustcon.cpp`
* **Problem:** Determine if a path exists from vertex `s` to vertex `t` in an **undirected graph** using a simple random walk.
* **Core Idea (Cover Time):**
    * For undirected graphs, a simple random walk is sufficient due to the **cover time** theorem.
    * The algorithm starts at `s` and takes random steps along edges.
    * At each step, it randomly selects a neighbor and moves to it.
    * If it reaches `t`, the path exists. If the walk hits a dead-end (no neighbors), it declares no path exists.
    * The **cover time** of a connected undirected graph is $O(n^3)$, meaning the walk will visit all reachable vertices within $2n^3$ steps with high probability.
* **Implementation Details:**
    * Adjacency list representation using `vector<vector<int>>`
    * Maximum walk length: $2n^3$ steps
    * Early termination on reaching target or hitting dead-end
    * Tests both connected and disconnected graph scenarios

### 3. Universal Traversal Sequence (UTS) for USTCON

* **File:** `universal-traversal-sequence.cpp`
* **Problem:** Use a **deterministic** Universal Traversal Sequence to solve USTCON on regular graphs without using randomness during the walk.
* **Core Idea (Derandomization):**
    * A Universal Traversal Sequence is a predetermined sequence of edge labels that, when followed from any starting vertex in any $d$-regular graph on $n$ vertices, will visit all vertices.
    * The algorithm takes a **labeled graph** where edges at each vertex are labeled $0, 1, \ldots, d-1$.
    * Instead of making random choices, it follows the UTS: at each step, it takes the edge with the label specified by the next element in the sequence.
    * The existence of polynomial-length UTS sequences remains an open problem, but they provide a theoretical framework for derandomizing USTCON.
* **Implementation Details:**
    * Requires graphs with labeled edges (represented as `vector<vector<int>>` where `graph[u][label] = v`)
    * Takes a pre-computed UTS as input
    * Purely deterministic traversal following the sequence
    * Demonstrates the concept on small regular graphs (triangle with degree 2, disconnected graph with degree 1)

### 4. Directed s-t Connectivity (STCON) via Random Walk with Restart

* **File:** `randomized-walk-for-directed-stcon.cpp`
* **Problem:** Determine if a path exists from vertex `s` to vertex `t` in a **directed graph** using a random walk with a restart mechanism.
* **Core Idea (Handling Sinks):**
    * Unlike undirected graphs, directed graphs can have **sinks** (vertices with no outgoing edges) or regions unreachable from certain starting points.
    * A pure random walk can get "stuck" in a sink that isn't the target.
    * This implementation uses a **restart strategy**: when the walk reaches a node with no outgoing edges, it **restarts from `s`**.
    * The restart mechanism gives the walk multiple attempts to explore different paths.
    * While not as theoretically robust as the logspace STCON algorithm (Section 6.6.2), it provides a simple heuristic.
* **Implementation Details:**
    * Uses $2n^3$ maximum steps (similar to USTCON)
    * Restarts from `s` when encountering a node with no neighbors
    * Tests connected line graphs, disconnected graphs, and graphs with sinks
    * Shows both successful pathfinding and proper failure detection

### 5. Random Walk on Expander Graph (Mixing Time Simulation)

* **File:** `random-walk-on-expander-graph.cpp`
* **Problem:** Demonstrate the concept of **rapid mixing** on expander graphs versus slow mixing on non-expander graphs.
* **Core Idea (Expander Properties):**
    * An **expander graph** has good connectivity properties that cause random walks to "mix" quickly, meaning the probability distribution over vertices approaches uniform in few steps.
    * A **non-expander** (like a path) has poor connectivity, causing random walks to mix slowly.
    * This implementation simulates many random walks and measures the distribution of ending vertices after a fixed number of steps.
    * On the expander (complete graph $K_5$), the distribution becomes nearly uniform after just 4 steps.
    * On the path graph, the distribution remains heavily localized near the starting vertex.
* **Implementation Details:**
    * Compares complete graph (expander) vs. path graph (non-expander)
    * Runs 100,000 simulations for statistical accuracy
    * Measures and displays distribution percentages
    * Visualizes the mixing rate difference empirically
    * Demonstrates foundation for BPP amplification technique

### 6. BPP Probability Amplification (Section 6.8)

* **File:** `probability-amplification.cpp`
* **Problem:** Reduce the error of a BPP (Bounded-error Probabilistic Polynomial-time) algorithm from a constant (e.g., 1/100) to an exponentially small value ($1/2^k$) using fewer random bits than independent trials.
* **Core Idea (Rapidly Mixing Walk):**
    * Instead of running $k$ independent trials (which requires $k \times n$ random bits), this algorithm uses a **random walk on an implicit expander graph**.
    * The "vertices" of this graph are all $N = 2^n$ possible random strings.
    * The "edges" are defined implicitly: from a random string $r$, we can transition to a "neighbor" by flipping a single bit (determined by a hash function).
    * A step in the walk either stays at the current node (with probability 1/2) or moves to a neighbor (with probability 1/2, choosing uniformly among 7 possible neighbors).
    * Because this implicit graph has expander properties, the walk **mixes rapidly**. After just $\beta = 3$ steps, the new random string is "sufficiently independent" from the previous one.
    * The algorithm runs the BPP algorithm $7k$ times, using this rapidly-mixing walk to generate "pseudo-random" inputs.
    * The majority vote across these $7k$ trials reduces the error exponentially, but uses only $O(n + k)$ random bits instead of $O(kn)$.
* **Implementation Details:**
    * Simulates a mock BPP algorithm with 1% error rate
    * Generates random strings of length $n = 20$
    * Amplifies probability for $k = 5$ (target error $< 1/32$)
    * Uses walk parameter $\beta = 3$ and $7k$ trials
    * Compares error rates: single run (~1%) vs. amplified (<0.01%)
    * Demonstrates bit complexity: single run uses $n$ bits, amplified uses $n + O(k)$ bits
    * Runs 20,000 simulations to measure empirical error rates
    * Shows effectiveness of expander-based derandomization technique

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
