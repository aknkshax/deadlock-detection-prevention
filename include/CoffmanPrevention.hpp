#ifndef COFFMAN_PREVENTION_HPP
#define COFFMAN_PREVENTION_HPP

#include <vector>

/**
 * @class CoffmanPreventionSimulator
 * @brief Demonstrates and validates deadlock prevention strategies by
 *        negating each of Coffman's Four Necessary Conditions.
 */
class CoffmanPreventionSimulator {
public:
    void showMenu();
    void simulateMutualExclusion() const;
    void simulateHoldAndWait() const;
    void simulateNoPreemption() const;
    void simulateCircularWait() const;
    void interactiveLockOrderValidator() const;
    bool validateLockOrder(const std::vector<int>& resourceSequence, bool verbose = true) const;
};

#endif // COFFMAN_PREVENTION_HPP
