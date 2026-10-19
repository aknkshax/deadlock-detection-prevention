#include "Utils.hpp"
#include "ResourceManager.hpp"
#include "WaitForGraph.hpp"
#include "CoffmanPrevention.hpp"
#include <iostream>

using namespace std;

int main() {
    SystemResourceManager manager;
    WaitForGraphDetector wfg;
    CoffmanPreventionSimulator coffman;

    while (true) {
        printBanner("OS DEADLOCK DETECTION & PREVENTION SYSTEM (C++)");
        cout << "  " << Color::BOLD << "[Banker's Algorithm & Matrix Detection]" << Color::RESET << "\n"
             << "   1. Display Current Resource Allocation State\n"
             << "   2. Run Banker's Safety Algorithm (Find Safe Sequence)\n"
             << "   3. Simulate Dynamic Resource Request (Banker's Request Algorithm)\n"
             << "   4. Run Matrix Deadlock Detection Algorithm\n"
             << "   5. Load Preset: Safe State (5 Processes, 3 Resources)\n"
             << "   6. Load Preset: Deadlocked State\n"
             << "   7. Input Custom Processes & Resource Matrices\n\n"
             << "  " << Color::BOLD << "[Graph Detection & Coffman Prevention]" << Color::RESET << "\n"
             << "   8. Wait-For Graph (WFG) Cycle Detection (DFS)\n"
             << "   9. Coffman's 4 Conditions & Prevention Simulator\n"
             << "   0. Exit\n\n";

        int choice = getValidInt("Enter your choice (0-9): ", 0, 9);
        if (choice == 0) {
            cout << Color::CYAN << "\nExiting Deadlock Simulation System. Goodbye!\n" << Color::RESET;
            break;
        }

        switch (choice) {
            case 1:
                manager.displayState();
                break;
            case 2:
                manager.displayState();
                manager.runSafetyAlgorithm(true);
                break;
            case 3:
                manager.displayState();
                manager.requestResources();
                break;
            case 4:
                manager.displayState();
                manager.runDeadlockDetection(true);
                break;
            case 5:
                manager.loadDefaultPreset();
                cout << Color::GREEN << "\n[✓] Loaded Safe State Preset (P0-P4, R0-R2).\n" << Color::RESET;
                manager.displayState();
                break;
            case 6:
                manager.loadDeadlockedPreset();
                cout << Color::YELLOW << "\n[!] Loaded Deadlocked State Preset.\n" << Color::RESET;
                manager.displayState();
                break;
            case 7:
                manager.inputCustomState();
                manager.displayState();
                break;
            case 8:
                wfg.runInteractiveDemo();
                break;
            case 9:
                coffman.showMenu();
                break;
        }
    }

    return 0;
}
