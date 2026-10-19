#ifndef WAIT_FOR_GRAPH_HPP
#define WAIT_FOR_GRAPH_HPP

#include <vector>

/**
 * @class WaitForGraphDetector
 * @brief Detects deadlocks in single-instance resource systems using
 *        Depth-First Search (DFS) cycle detection on a Wait-For Graph (WFG).
 */
class WaitForGraphDetector {
private:
    int numProcesses;
    std::vector<std::vector<int>> adj;

    bool dfsCycle(int u, std::vector<int>& state, std::vector<int>& parent, std::vector<int>& cyclePath) const;

public:
    WaitForGraphDetector(int n = 4);
    void setGraph(int n, const std::vector<std::pair<int, int>>& edges);
    bool detectCycle(std::vector<int>& cyclePath, bool verbose = true) const;
    void runInteractiveDemo();
};

#endif // WAIT_FOR_GRAPH_HPP
