#include "Utils.hpp"
#include "ResourceManager.hpp"
#include "WaitForGraph.hpp"
#include "CoffmanPrevention.hpp"
#include <iostream>
#include <cassert>
#include <vector>

using namespace std;

int main() {
    printBanner("AUTOMATED UNIT TEST SUITE: DEADLOCK DETECTION & PREVENTION");
    int passed = 0;

    // Test 1: Banker's Algorithm Safe State
    {
        printSubHeader("Test 1: Banker's Algorithm - Safe State Verification");
        SystemResourceManager mgr;
        mgr.loadDefaultPreset();
        bool isSafe = mgr.runSafetyAlgorithm(false);
        assert(isSafe == true);
        cout << Color::GREEN << "  [PASS] Default 5-process, 3-resource state verified as SAFE.\n" << Color::RESET;
        passed++;
    }

    // Test 2: Banker's Resource-Request Algorithm (Safe Request Granted)
    {
        printSubHeader("Test 2: Banker's Resource-Request - Valid Safe Request");
        SystemResourceManager mgr;
        mgr.loadDefaultPreset();
        // P1 requests (1, 0, 2) which is <= Need(1, 2, 2) and <= Available(3, 3, 2) and leaves system safe
        bool granted = mgr.requestResourcesDirect(1, {1, 0, 2}, false);
        assert(granted == true);
        cout << Color::GREEN << "  [PASS] Safe resource request (1, 0, 2) by P1 was GRANTED.\n" << Color::RESET;
        passed++;
    }

    // Test 3: Banker's Resource-Request Algorithm (Exceeding Max Claim Denied)
    {
        printSubHeader("Test 3: Banker's Resource-Request - Exceeding Maximum Claim");
        SystemResourceManager mgr;
        mgr.loadDefaultPreset();
        // P1 Need is (1, 2, 2); requesting (2, 0, 0) exceeds Need
        bool granted = mgr.requestResourcesDirect(1, {2, 0, 0}, false);
        assert(granted == false);
        cout << Color::GREEN << "  [PASS] Request exceeding Need was properly DENIED.\n" << Color::RESET;
        passed++;
    }

    // Test 4: Matrix Deadlock Detection Algorithm
    {
        printSubHeader("Test 4: Matrix Deadlock Detection - Active Deadlock Identification");
        SystemResourceManager mgr;
        mgr.loadDeadlockedPreset();
        vector<int> deadlocked = mgr.runDeadlockDetection(false);
        assert(!deadlocked.empty());
        cout << Color::GREEN << "  [PASS] Deadlocked processes accurately detected (Count: "
             << deadlocked.size() << ").\n" << Color::RESET;
        passed++;
    }

    // Test 5: Wait-For Graph Cycle Detection (Deadlock Present vs Acyclic)
    {
        printSubHeader("Test 5: Wait-For Graph (WFG) DFS Cycle Detection");
        WaitForGraphDetector wfg;
        vector<int> cycle;

        // Cyclic graph: P0 -> P1 -> P2 -> P0
        wfg.setGraph(4, {{0, 1}, {1, 2}, {2, 0}, {2, 3}});
        bool hasCycle = wfg.detectCycle(cycle, false);
        assert(hasCycle == true);
        assert(!cycle.empty());

        // Acyclic graph (DAG): P0 -> P1 -> P2 -> P3
        wfg.setGraph(4, {{0, 1}, {1, 2}, {2, 3}});
        bool hasCycleDAG = wfg.detectCycle(cycle, false);
        assert(hasCycleDAG == false);

        cout << Color::GREEN << "  [PASS] WFG cycle detector accurately distinguishes cyclic deadlocks from DAGs.\n" << Color::RESET;
        passed++;
    }

    // Test 6: Coffman Circular Wait Prevention (Lock Hierarchy Validator)
    {
        printSubHeader("Test 6: Coffman Circular Wait - Resource Hierarchy Validator");
        CoffmanPreventionSimulator coffman;
        // Strictly ascending ranks: R0(10) -> R2(30) -> R3(40) => Valid
        assert(coffman.validateLockOrder({0, 2, 3}, false) == true);
        // Out-of-order ranks: R2(30) -> R1(20) => Rejected
        assert(coffman.validateLockOrder({2, 1}, false) == false);
        cout << Color::GREEN << "  [PASS] Circular wait hierarchy validator enforces strictly increasing ranks.\n" << Color::RESET;
        passed++;
    }

    cout << "\n" << Color::GREEN << Color::BOLD
         << "==========================================================================\n"
         << "  ALL " << passed << "/6 TEST SUITES PASSED SUCCESSFULLY!\n"
         << "=========================================================================="
         << Color::RESET << "\n";

    return 0;
}
