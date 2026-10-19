#include "CoffmanPrevention.hpp"
#include "Utils.hpp"
#include <iostream>
#include <string>

using namespace std;

void CoffmanPreventionSimulator::showMenu() {
    while (true) {
        printBanner("COFFMAN'S 4 CONDITIONS & DEADLOCK PREVENTION");
        cout << "A deadlock can occur ONLY if all four Coffman conditions hold simultaneously.\n"
             << "Deadlock Prevention works by negating at least one of these four conditions:\n\n"
             << "  1. " << Color::BOLD << "Mutual Exclusion" << Color::RESET
             << "   (Spooling / Read-Only Shared Access vs Exclusive Locks)\n"
             << "  2. " << Color::BOLD << "Hold and Wait" << Color::RESET
             << "      (All-or-Nothing & Release-Before-Request Protocols)\n"
             << "  3. " << Color::BOLD << "No Preemption" << Color::RESET
             << "      (Preempting resources from waiting/lower-priority processes)\n"
             << "  4. " << Color::BOLD << "Circular Wait" << Color::RESET
             << "      (Strict Total Ordering of Resource Types)\n"
             << "  5. " << Color::BOLD << "Interactive Circular Wait Order Validator" << Color::RESET
             << " (Test your own lock acquisition order)\n"
             << "  0. Return to Main Menu\n\n";

        int choice = getValidInt("Select a condition/strategy to simulate (0-5): ", 0, 5);
        if (choice == 0) break;

        switch (choice) {
            case 1: simulateMutualExclusion(); break;
            case 2: simulateHoldAndWait(); break;
            case 3: simulateNoPreemption(); break;
            case 4: simulateCircularWait(); break;
            case 5: interactiveLockOrderValidator(); break;
        }
    }
}

void CoffmanPreventionSimulator::simulateMutualExclusion() const {
    printSubHeader("1. Mutual Exclusion Condition & Prevention");
    cout << Color::RED << "[Unprevented Scenario - Exclusive Printer Lock]\n" << Color::RESET
         << "  - Process P1 acquires exclusive lock on Printer (R1) and waits for Scanner (R2).\n"
         << "  - Process P2 holds Scanner (R2) and requests Printer (R1).\n"
         << "  - Because Printer is non-shareable (Mutual Exclusion holds), P2 blocks -> DEADLOCK!\n\n";

    cout << Color::GREEN << "[Prevention Strategy - Spooling / Shareable Virtualization]\n" << Color::RESET
         << "  - Instead of processes locking physical Printer R1 directly, we introduce a Spooler Daemon.\n"
         << "  - P1 and P2 write their print jobs concurrently to a shared disk spool directory.\n"
         << "  - Neither P1 nor P2 blocks on R1! Mutual Exclusion is eliminated for client processes.\n"
         << Color::YELLOW << "  * Limitation: Cannot be applied to intrinsically non-shareable resources like mutex locks.\n" << Color::RESET;
}

void CoffmanPreventionSimulator::simulateHoldAndWait() const {
    printSubHeader("2. Hold and Wait Condition & Prevention");
    cout << "Scenario: P1 needs [Disk, Printer] and P2 needs [Printer, Disk].\n\n";

    cout << Color::RED << "[Unprevented Execution (Incremental Requesting)]\n" << Color::RESET
         << "  T=1: P1 acquires [Disk]\n"
         << "  T=2: P2 acquires [Printer]\n"
         << "  T=3: P1 requests [Printer] while holding [Disk] -> BLOCKED\n"
         << "  T=4: P2 requests [Disk] while holding [Printer] -> BLOCKED (DEADLOCK!)\n\n";

    cout << Color::GREEN << "[Prevention Protocol A: All-or-Nothing Atomic Allocation]\n" << Color::RESET
         << "  Rule: A process must request and be allocated ALL required resources before execution begins.\n"
         << "  T=1: P1 requests [Disk, Printer] atomically -> Both free -> GRANTED [Disk, Printer].\n"
         << "  T=2: P2 requests [Printer, Disk] atomically -> Not all free -> P2 WAITS (holds nothing!).\n"
         << "  T=3: P1 finishes and releases [Disk, Printer].\n"
         << "  T=4: P2 acquires [Printer, Disk] and finishes. -> NO DEADLOCK!\n\n";

    cout << Color::GREEN << "[Prevention Protocol B: Release-Before-Request]\n" << Color::RESET
         << "  Rule: A process must release all currently held resources before requesting a new set.\n"
         << "  T=1: P1 acquires [Disk], processes data, then RELEASES [Disk].\n"
         << "  T=2: P1 now requests [Disk, Printer] together -> GRANTED.\n";
}

void CoffmanPreventionSimulator::simulateNoPreemption() const {
    printSubHeader("3. No Preemption Condition & Prevention");
    cout << "Scenario: P1 holds [R1] and requests [R2] (currently held by waiting process P2).\n\n";

    cout << Color::RED << "[Unprevented Execution (No Preemption Allowed)]\n" << Color::RESET
         << "  - Once P1 acquires R1, R1 cannot be forcibly taken away.\n"
         << "  - P1 waits for R2, while P2 waits for R1 -> DEADLOCK!\n\n";

    cout << Color::GREEN << "[Prevention Protocol (Preemption on Blocked Request)]\n" << Color::RESET
         << "  Rule: If a process holding resources requests another resource that cannot be immediately\n"
         << "        allocated, all resources currently held by the process are implicitly preempted.\n"
         << "  Simulation Trace:\n"
         << "    Step 1: P1 holds [R1]. P2 holds [R2].\n"
         << "    Step 2: P1 requests [R2]. [R2] is busy.\n"
         << "    Step 3: OS PREEMPTS [R1] from P1! P1's state is saved, and P1 is put in waiting queue for [R1, R2].\n"
         << "    Step 4: P2 now requests [R1] -> [R1] is free! P2 acquires [R1], finishes, and releases [R1, R2].\n"
         << "    Step 5: P1 wakes up, acquires [R1, R2], and completes! -> DEADLOCK PREVENTED!\n";
}

void CoffmanPreventionSimulator::simulateCircularWait() const {
    printSubHeader("4. Circular Wait Condition & Prevention (Resource Hierarchy)");
    cout << "Rule: Assign a unique global rank F(Ri) to every resource type:\n"
         << "      F(Tape) = 1,  F(Disk) = 5,  F(Printer) = 12\n"
         << "      Every process must request resources in strictly increasing order of F(Ri).\n\n";

    cout << Color::RED << "[Unprevented Execution (Violation of Ordering)]\n" << Color::RESET
         << "  - P1 acquires Disk (Rank 5), then requests Printer (Rank 12). (5 < 12)\n"
         << "  - P2 acquires Printer (Rank 12), then requests Disk (Rank 5). (12 > 5 -> VIOLATION!)\n"
         << "  - Cycle forms: P1 -> R_Printer -> P2 -> R_Disk -> P1 (DEADLOCK!)\n\n";

    cout << Color::GREEN << "[Prevented Execution (Strict Ascending Order Enforced)]\n" << Color::RESET
         << "  - Both P1 and P2 MUST request Disk (Rank 5) BEFORE Printer (Rank 12).\n"
         << "  - T=1: P1 acquires Disk (Rank 5).\n"
         << "  - T=2: P2 wants Disk (Rank 5) and Printer (Rank 12). It must request Disk (5) first.\n"
         << "         Since P1 holds Disk (5), P2 blocks BEFORE acquiring Printer (12)!\n"
         << "  - T=3: P1 acquires Printer (Rank 12) freely, completes, and releases both.\n"
         << "  - T=4: P2 wakes up and proceeds. -> CIRCULAR WAIT IMPOSSIBLE!\n";
}

bool CoffmanPreventionSimulator::validateLockOrder(const vector<int>& resourceSequence, bool verbose) const {
    int ranks[4] = {10, 20, 30, 40};
    string names[4] = {"R0(Auth)", "R1(Account)", "R2(Logger)", "R3(Network)"};

    int highestRankHeld = -1;
    int highestResHeld = -1;

    for (size_t i = 0; i < resourceSequence.size(); i++) {
        int r = resourceSequence[i];
        if (r < 0 || r > 3) return false;

        if (verbose) {
            cout << "    -> Process requests " << names[r] << " (Rank " << ranks[r] << ")... ";
        }

        if (ranks[r] <= highestRankHeld) {
            if (verbose) {
                cout << Color::RED << Color::BOLD << "REJECTED!\n" << Color::RESET;
                cout << Color::RED
                     << "       [Coffman Circular Wait Prevention Triggered]\n"
                     << "       Process already holds " << names[highestResHeld] << " (Rank " << highestRankHeld << ").\n"
                     << "       Cannot request a resource of lower or equal rank (" << ranks[r] << ") without releasing higher-rank locks first!\n"
                     << Color::RESET;
            }
            return false;
        } else {
            if (verbose) {
                cout << Color::GREEN << "GRANTED (Strictly Increasing: "
                     << (highestRankHeld == -1 ? "None" : to_string(highestRankHeld))
                     << " < " << ranks[r] << ")\n" << Color::RESET;
            }
            highestRankHeld = ranks[r];
            highestResHeld = r;
        }
    }

    if (verbose) {
        cout << "\n" << Color::GREEN << Color::BOLD
             << "[✓] Acquisition sequence complies with Resource Ordering! Circular Wait is mathematically impossible."
             << Color::RESET << "\n";
    }
    return true;
}

void CoffmanPreventionSimulator::interactiveLockOrderValidator() const {
    printSubHeader("Interactive Circular Wait Lock-Order Validator");
    cout << "System Resources and Assigned Hierarchy Ranks:\n"
         << "  R0 (Mutex_Auth)    : Rank 10\n"
         << "  R1 (Mutex_Account) : Rank 20\n"
         << "  R2 (Mutex_Logger)  : Rank 30\n"
         << "  R3 (Mutex_Network) : Rank 40\n\n";

    int numReq = getValidInt("How many resources will the process request sequentially (2-4)? ", 2, 4);
    vector<int> seq;
    for (int step = 1; step <= numReq; step++) {
        int r = getValidInt("  Step " + to_string(step) + " - Enter Resource ID to acquire (0-3): ", 0, 3);
        seq.push_back(r);
        if (!validateLockOrder(seq, false)) {
            // Print full trace up to rejection
            validateLockOrder(seq, true);
            return;
        }
    }
    validateLockOrder(seq, true);
}
