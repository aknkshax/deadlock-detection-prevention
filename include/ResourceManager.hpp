#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <vector>
#include <string>

/**
 * @class SystemResourceManager
 * @brief Simulates OS processes and multi-instance resources using Banker's
 *        Algorithm (Avoidance) and Matrix Deadlock Detection.
 */
class SystemResourceManager {
private:
    int numProcesses;
    int numResources;
    std::vector<int> available;
    std::vector<std::vector<int>> maxDemand;
    std::vector<std::vector<int>> allocation;
    std::vector<std::vector<int>> need;

    void calculateNeed();

public:
    SystemResourceManager();

    void loadDefaultPreset();
    void loadDeadlockedPreset();
    void inputCustomState();
    void displayState() const;

    // Banker's Safety Algorithm
    bool runSafetyAlgorithm(bool verbose = true) const;

    // Banker's Resource-Request Algorithm
    void requestResources();
    bool requestResourcesDirect(int pid, const std::vector<int>& request, bool verbose = true);

    // Deadlock Detection Algorithm (Multi-instance matrix detection)
    std::vector<int> runDeadlockDetection(bool verbose = true) const;
};

#endif // RESOURCE_MANAGER_HPP
