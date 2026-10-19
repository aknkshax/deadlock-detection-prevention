#include "ResourceManager.hpp"
#include "Utils.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

SystemResourceManager::SystemResourceManager() {
    loadDefaultPreset();
}

void SystemResourceManager::calculateNeed() {
    need.assign(numProcesses, vector<int>(numResources, 0));
    for (int i = 0; i < numProcesses; i++) {
        for (int j = 0; j < numResources; j++) {
            need[i][j] = max(0, maxDemand[i][j] - allocation[i][j]);
        }
    }
}

void SystemResourceManager::loadDefaultPreset() {
    numProcesses = 5;
    numResources = 3;
    available = {3, 3, 2};
    allocation = {
        {0, 1, 0}, // P0
        {2, 0, 0}, // P1
        {3, 0, 2}, // P2
        {2, 1, 1}, // P3
        {0, 0, 2}  // P4
    };
    maxDemand = {
        {7, 5, 3}, // P0
        {3, 2, 2}, // P1
        {9, 0, 2}, // P2
        {2, 2, 2}, // P3
        {4, 3, 3}  // P4
    };
    calculateNeed();
}

void SystemResourceManager::loadDeadlockedPreset() {
    numProcesses = 5;
    numResources = 3;
    available = {0, 0, 0};
    allocation = {
        {0, 1, 0}, // P0
        {2, 0, 0}, // P1
        {3, 0, 3}, // P2
        {2, 1, 1}, // P3
        {0, 0, 2}  // P4
    };
    maxDemand = {
        {0, 1, 0}, // P0 requests {0, 0, 0}
        {4, 0, 2}, // P1 requests {2, 0, 2}
        {3, 0, 4}, // P2 requests {0, 0, 1}
        {3, 1, 1}, // P3 requests {1, 0, 0}
        {0, 0, 4}  // P4 requests {0, 0, 2}
    };
    calculateNeed();
}

void SystemResourceManager::inputCustomState() {
    printSubHeader("Custom System Configuration");
    numProcesses = getValidInt("Enter number of processes (1-15): ", 1, 15);
    numResources = getValidInt("Enter number of resource types (1-10): ", 1, 10);

    available.resize(numResources);
    allocation.assign(numProcesses, vector<int>(numResources));
    maxDemand.assign(numProcesses, vector<int>(numResources));

    cout << "\nEnter Available instances for each resource:\n";
    for (int j = 0; j < numResources; j++) {
        available[j] = getValidInt("  Resource R" + to_string(j) + ": ", 0, 1000);
    }

    cout << "\nEnter Allocation Matrix (currently allocated resources):\n";
    for (int i = 0; i < numProcesses; i++) {
        cout << "  Process P" << i << " (" << numResources << " space-separated integers): ";
        for (int j = 0; j < numResources; j++) {
            cin >> allocation[i][j];
        }
    }

    cout << "\nEnter Max Demand Matrix (maximum resources needed):\n";
    for (int i = 0; i < numProcesses; i++) {
        cout << "  Process P" << i << " (" << numResources << " space-separated integers): ";
        for (int j = 0; j < numResources; j++) {
            cin >> maxDemand[i][j];
            if (maxDemand[i][j] < allocation[i][j]) {
                cout << Color::YELLOW << "    [Note] Max < Allocation for R" << j
                     << "; adjusting Max to match Allocation (" << allocation[i][j] << ").\n" << Color::RESET;
                maxDemand[i][j] = allocation[i][j];
            }
        }
    }
    calculateNeed();
    cout << Color::GREEN << "\n[✓] Custom state loaded successfully!\n" << Color::RESET;
}

void SystemResourceManager::displayState() const {
    printSubHeader("Current System Resource State");

    cout << Color::BOLD << left << setw(10) << "Process"
         << setw(numResources * 4 + 4) << "Allocation"
         << setw(numResources * 4 + 4) << "Max Demand"
         << setw(numResources * 4 + 4) << "Need (Max-Alloc)"
         << "Available" << Color::RESET << "\n";

    cout << string(10 + 3 * (numResources * 4 + 4) + numResources * 4, '-') << "\n";

    for (int i = 0; i < numProcesses; i++) {
        cout << left << setw(10) << ("P" + to_string(i));

        string allocStr = "";
        for (int j = 0; j < numResources; j++) allocStr += to_string(allocation[i][j]) + "   ";
        cout << setw(numResources * 4 + 4) << allocStr;

        string maxStr = "";
        for (int j = 0; j < numResources; j++) maxStr += to_string(maxDemand[i][j]) + "   ";
        cout << setw(numResources * 4 + 4) << maxStr;

        string needStr = "";
        for (int j = 0; j < numResources; j++) needStr += to_string(need[i][j]) + "   ";
        cout << setw(numResources * 4 + 4) << needStr;

        if (i == 0) {
            for (int j = 0; j < numResources; j++) cout << available[j] << "   ";
        }
        cout << "\n";
    }
}

bool SystemResourceManager::runSafetyAlgorithm(bool verbose) const {
    if (verbose) printSubHeader("Banker's Algorithm: Safety Verification");

    vector<int> work = available;
    vector<bool> finish(numProcesses, false);
    vector<int> safeSequence;

    if (verbose) {
        cout << "Initial Work Vector (Available): [ ";
        for (int w : work) cout << w << " ";
        cout << "]\n\n";
    }

    int count = 0;
    while (count < numProcesses) {
        bool foundProcess = false;
        for (int i = 0; i < numProcesses; i++) {
            if (!finish[i]) {
                bool canProceed = true;
                for (int j = 0; j < numResources; j++) {
                    if (need[i][j] > work[j]) {
                        canProceed = false;
                        break;
                    }
                }

                if (canProceed) {
                    if (verbose) {
                        cout << Color::GREEN << "  [Step " << (count + 1) << "] P" << i
                             << " can execute " << Color::RESET << "(Need: [ ";
                        for (int x : need[i]) cout << x << " ";
                        cout << "] <= Work: [ ";
                        for (int x : work) cout << x << " ";
                        cout << "])\n";
                    }

                    for (int j = 0; j < numResources; j++) {
                        work[j] += allocation[i][j];
                    }
                    finish[i] = true;
                    safeSequence.push_back(i);
                    foundProcess = true;
                    count++;

                    if (verbose) {
                        cout << "           P" << i << " finishes and releases Allocation. New Work: [ ";
                        for (int x : work) cout << x << " ";
                        cout << "]\n";
                    }
                }
            }
        }
        if (!foundProcess) {
            break;
        }
    }

    if (count == numProcesses) {
        if (verbose) {
            cout << "\n" << Color::GREEN << Color::BOLD
                 << "[SAFE STATE] System is SAFE! No deadlock will occur.\n"
                 << "Safe Execution Sequence: < ";
            for (size_t i = 0; i < safeSequence.size(); i++) {
                cout << "P" << safeSequence[i] << (i + 1 < safeSequence.size() ? " -> " : " ");
            }
            cout << ">" << Color::RESET << "\n";
        }
        return true;
    } else {
        if (verbose) {
            cout << "\n" << Color::RED << Color::BOLD
                 << "[UNSAFE STATE] System is UNSAFE! Deadlock may occur.\n"
                 << "Processes unable to complete: ";
            for (int i = 0; i < numProcesses; i++) {
                if (!finish[i]) cout << "P" << i << " ";
            }
            cout << Color::RESET << "\n";
        }
        return false;
    }
}

bool SystemResourceManager::requestResourcesDirect(int pid, const vector<int>& request, bool verbose) {
    if (verbose) {
        cout << "\nChecking request [ ";
        for (int r : request) cout << r << " ";
        cout << "] for Process P" << pid << "...\n";
    }

    for (int j = 0; j < numResources; j++) {
        if (request[j] > need[pid][j]) {
            if (verbose) {
                cout << Color::RED << Color::BOLD
                     << "[DENIED] Error: Process P" << pid << " has exceeded its maximum claim for R" << j
                     << " (Requested: " << request[j] << ", Need: " << need[pid][j] << ").\n"
                     << Color::RESET;
            }
            return false;
        }
    }
    if (verbose) cout << "  [✓] Step 1 Passed: Request <= Need[P" << pid << "]\n";

    for (int j = 0; j < numResources; j++) {
        if (request[j] > available[j]) {
            if (verbose) {
                cout << Color::YELLOW << Color::BOLD
                     << "[WAIT] Resources not currently available for R" << j
                     << " (Requested: " << request[j] << ", Available: " << available[j]
                     << "). P" << pid << " must wait.\n" << Color::RESET;
            }
            return false;
        }
    }
    if (verbose) {
        cout << "  [✓] Step 2 Passed: Request <= Available\n";
        cout << "  [*] Step 3: Tentatively allocating resources and testing for safety...\n";
    }

    for (int j = 0; j < numResources; j++) {
        available[j] -= request[j];
        allocation[pid][j] += request[j];
        need[pid][j] -= request[j];
    }

    if (runSafetyAlgorithm(verbose)) {
        if (verbose) {
            cout << Color::GREEN << Color::BOLD
                 << "\n[GRANTED] Request is safe! Resources have been permanently allocated to P" << pid << ".\n"
                 << Color::RESET;
        }
        return true;
    } else {
        for (int j = 0; j < numResources; j++) {
            available[j] += request[j];
            allocation[pid][j] -= request[j];
            need[pid][j] += request[j];
        }
        if (verbose) {
            cout << Color::RED << Color::BOLD
                 << "\n[ROLLED BACK] Granting this request would put the system in an UNSAFE state.\n"
                 << "Allocation has been rolled back and P" << pid << " must wait.\n" << Color::RESET;
        }
        return false;
    }
}

void SystemResourceManager::requestResources() {
    printSubHeader("Banker's Resource-Request Algorithm");
    int pid = getValidInt("Enter Process ID making the request (0 to " + to_string(numProcesses - 1) + "): ", 0, numProcesses - 1);

    vector<int> request(numResources);
    cout << "Enter Request Vector for P" << pid << " (" << numResources << " space-separated integers): ";
    for (int j = 0; j < numResources; j++) {
        cin >> request[j];
    }

    requestResourcesDirect(pid, request, true);
}

vector<int> SystemResourceManager::runDeadlockDetection(bool verbose) const {
    if (verbose) {
        printSubHeader("Deadlock Detection Algorithm (Allocation vs. Current Requests)");
        cout << "Note: Processes with 0 Allocation are initially marked Finish = true.\n\n";
    }

    vector<int> work = available;
    vector<bool> finish(numProcesses, false);

    for (int i = 0; i < numProcesses; i++) {
        bool hasAllocation = false;
        for (int j = 0; j < numResources; j++) {
            if (allocation[i][j] > 0) {
                hasAllocation = true;
                break;
            }
        }
        if (!hasAllocation) {
            finish[i] = true;
            if (verbose) cout << "  Process P" << i << " holds 0 resources -> Finish[P" << i << "] = true\n";
        }
    }

    bool progress = true;
    while (progress) {
        progress = false;
        for (int i = 0; i < numProcesses; i++) {
            if (!finish[i]) {
                bool canSatisfy = true;
                for (int j = 0; j < numResources; j++) {
                    if (need[i][j] > work[j]) {
                        canSatisfy = false;
                        break;
                    }
                }
                if (canSatisfy) {
                    if (verbose) {
                        cout << Color::GREEN << "  [✓] P" << i << " request satisfied. " << Color::RESET
                             << "Releasing its allocated resources to Work.\n";
                    }
                    for (int j = 0; j < numResources; j++) {
                        work[j] += allocation[i][j];
                    }
                    finish[i] = true;
                    progress = true;
                }
            }
        }
    }

    vector<int> deadlocked;
    for (int i = 0; i < numProcesses; i++) {
        if (!finish[i]) deadlocked.push_back(i);
    }

    if (verbose) {
        if (deadlocked.empty()) {
            cout << "\n" << Color::GREEN << Color::BOLD
                 << "[NO DEADLOCK DETECTED] All active processes can complete!" << Color::RESET << "\n";
        } else {
            cout << "\n" << Color::RED << Color::BOLD
                 << "[DEADLOCK DETECTED!] The system is currently deadlocked.\n"
                 << "Deadlocked Processes: ";
            for (int pid : deadlocked) cout << "P" << pid << " ";
            cout << Color::RESET << "\n";
        }
    }
    return deadlocked;
}
