#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

struct Literal {
    int variableIndex;
    bool isNegated;
};

struct Clause {
    Literal firstLiteral;
    Literal secondLiteral;
};

bool evaluateLiteral(const Literal& lit, const vector<bool>& assignment) {
    bool value = assignment[lit.variableIndex];
    return lit.isNegated ? !value : value;
}

bool isClauseSatisfied(const Clause& clause, const vector<bool>& assignment) {
    return evaluateLiteral(clause.firstLiteral, assignment) || evaluateLiteral(clause.secondLiteral, assignment);
}

vector<int> findUnsatisfiedClauses(const vector<Clause>& formula, const vector<bool>& assignment) {
    vector<int> unsatisfiedIndices;
    for (int i = 0; i < static_cast<int>(formula.size()); ++i) {
        if (!isClauseSatisfied(formula[i], assignment)) {
            unsatisfiedIndices.push_back(i);
        }
    }
    return unsatisfiedIndices;
}

bool solveTwoSat(int numVariables, const vector<Clause>& formula, vector<bool>& assignment) {
    
    assignment.assign(numVariables, false);
    for (int i = 0; i < numVariables; ++i) {
        assignment[i] = rand() % 2;
    }

    long long maxIterations = 2LL * numVariables * numVariables;
    if (numVariables == 0) maxIterations = 0;

    for (long long iter = 0; iter < maxIterations; ++iter) {
        vector<int> unsatisfied = findUnsatisfiedClauses(formula, assignment);

        if (unsatisfied.empty()) {
            return true;
        }

        int clauseIndex = unsatisfied[rand() % unsatisfied.size()];
        const Clause& targetClause = formula[clauseIndex];

        int literalToFlip = rand() % 2;
        int varToFlip;
        
        if (literalToFlip == 0) {
            varToFlip = targetClause.firstLiteral.variableIndex;
        } else {
            varToFlip = targetClause.secondLiteral.variableIndex;
        }

        assignment[varToFlip] = !assignment[varToFlip];
    }

    return false;
}

void printSolution(int numVariables, const vector<bool>& solution) {
    cout << "Solution:" << endl;
    for (int i = 0; i < numVariables; ++i) {
        cout << "  x" << (i + 1) << " = " 
                  << solution[i] << endl;
    }
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    cout << boolalpha;

    int numVars1 = 3;
    vector<Clause> formula1;
    formula1.push_back({{0, false}, {1, false}});
    formula1.push_back({{0, true}, {1, true}});
    formula1.push_back({{0, false}, {2, true}});

    cout << "--- Example 1 (Satisfiable) ---" << endl;
    cout << "(x1 or x2) and (!x1 or !x2) and (x1 or !x3)" << endl;
    
    vector<bool> solution1;
    if (solveTwoSat(numVars1, formula1, solution1)) {
        cout << "Formula is Satisfiable!" << endl;
        printSolution(numVars1, solution1);
    } else {
        cout << "Formula is Unsatisfiable (or algorithm timed out)." << endl;
    }

    int numVars2 = 1;
    vector<Clause> formula2;
    formula2.push_back({{0, false}, {0, false}});
    formula2.push_back({{0, true}, {0, true}});

    cout << "\n--- Example 2 (Unsatisfiable) ---" << endl;
    cout << "(x1) and (!x1)" << endl;

    vector<bool> solution2;
    if (solveTwoSat(numVars2, formula2, solution2)) {
        cout << "Formula is Satisfiable!" << endl;
        printSolution(numVars2, solution2);
    } else {
        cout << "Formula is Unsatisfiable (or algorithm timed out)." << endl;
    }

    return 0;
}