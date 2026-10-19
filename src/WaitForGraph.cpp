#include "WaitForGraph.hpp"
#include "Utils.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

WaitForGraphDetector::WaitForGraphDetector(int n) : numProcesses(n), adj(n) {}

void WaitForGraphDetector::setGraph(int n, const vector<pair<int, int>>& edges) {
    numProcesses = n;
    adj.assign(numProcesses, {});
    for (const auto& edge : edges) {
        if (edge.first >= 0 && edge.first < n && edge.second >= 0 && edge.second < n) {
            adj[edge.first].push_back(edge.second);
        }
    }
}

bool WaitForGraphDetector::dfsCycle(int u, vector<int>& state, vector<int>& parent, vector<int>& cyclePath) const {
    state[u] = 1; // Visiting (in recursion stack)
    for (int v : adj[u]) {
        if (state[v] == 0) {
            parent[v] = u;
            if (dfsCycle(v, state, parent, cyclePath)) return true;
        } else if (state[v] == 1) {
            cyclePath.push_back(v);
            int curr = u;
            while (curr != v && curr != -1) {
                cyclePath.push_back(curr);
                curr = parent[curr];
            }
            cyclePath.push_back(v);
            reverse(cyclePath.begin(), cyclePath.end());
            return true;
        }
    }
    state[u] = 2; // Visited
    return false;
}

bool WaitForGraphDetector::detectCycle(vector<int>& cyclePath, bool verbose) const {
    cyclePath.clear();
    if (verbose) {
        printSubHeader("Wait-For Graph Adjacency List");
        for (int i = 0; i < numProcesses; i++) {
            cout << "  P" << i << " waits for: ";
            if (adj[i].empty()) cout << "(none)";
            for (int v : adj[i]) cout << "P" << v << " ";
            cout << "\n";
        }
    }

    vector<int> state(numProcesses, 0);
    vector<int> parent(numProcesses, -1);
    bool hasDeadlock = false;

    for (int i = 0; i < numProcesses; i++) {
        if (state[i] == 0) {
            if (dfsCycle(i, state, parent, cyclePath)) {
                hasDeadlock = true;
                break;
            }
        }
    }

    if (verbose) {
        if (hasDeadlock) {
            cout << "\n" << Color::RED << Color::BOLD
                 << "[DEADLOCK DETECTED!] Circular Wait cycle identified in Wait-For Graph:\n  ";
            for (size_t i = 0; i < cyclePath.size(); i++) {
                cout << "P" << cyclePath[i] << (i + 1 < cyclePath.size() ? " -> " : "");
            }
            cout << Color::RESET << "\n";
        } else {
            cout << "\n" << Color::GREEN << Color::BOLD
                 << "[NO DEADLOCK] The Wait-For Graph is a Directed Acyclic Graph (DAG)."
                 << Color::RESET << "\n";
        }
    }
    return hasDeadlock;
}

void WaitForGraphDetector::runInteractiveDemo() {
    printBanner("WAIT-FOR GRAPH (WFG) DEADLOCK DETECTION");
    cout << "In single-instance resource systems, a deadlock exists if and only if\n"
         << "there is a directed cycle in the Wait-For Graph (Pi -> Pj means Pi waits for Pj).\n\n";

    cout << "Select mode:\n"
         << "  1. Load Preset Deadlock Graph (P0 -> P1 -> P2 -> P0 & P2 -> P3)\n"
         << "  2. Enter Custom Wait-For Edges\n";
    int mode = getValidInt("Choice: ", 1, 2);

    if (mode == 1) {
        setGraph(4, {{0, 1}, {1, 2}, {2, 0}, {2, 3}});
    } else {
        int n = getValidInt("Enter number of processes (2-15): ", 2, 15);
        int edgesCount = getValidInt("Enter number of wait-for edges: ", 0, n * n);
        vector<pair<int, int>> edges;
        cout << "Enter edges as: <waiting_pid> <holding_pid> (0-indexed):\n";
        for (int e = 0; e < edgesCount; e++) {
            int u = getValidInt("  Edge " + to_string(e + 1) + " - Waiting Process P: ", 0, n - 1);
            int v = getValidInt("  Edge " + to_string(e + 1) + " - Holding Process P: ", 0, n - 1);
            edges.push_back({u, v});
        }
        setGraph(n, edges);
    }

    vector<int> cyclePath;
    detectCycle(cyclePath, true);
}
