# Deadlock Detection and Prevention System

An interactive Operating Systems simulation written in **C++17** that models multi-process resource allocation and implements algorithms for **Deadlock Avoidance**, **Deadlock Detection**, and **Deadlock Prevention**.

---

## Concepts Covered

### 1. Deadlock Conditions (Coffman's Four Conditions) & Prevention Strategies
A system enters a deadlock only when all four Coffman conditions hold simultaneously. This simulator demonstrates how deadlocks arise and how each condition can be negated:
- **Mutual Exclusion**: Compares exclusive hardware locks against **Spooling / Virtualization** for shareable access.
- **Hold and Wait**: Demonstrates **All-or-Nothing Atomic Allocation** and **Release-Before-Request** protocols.
- **No Preemption**: Simulates resource preemption when a process holding resources requests an unavailable resource.
- **Circular Wait**: Enforces a **Strict Resource Hierarchy ($F(R_i)$)** and includes an interactive **Lock-Order Validator** that rejects out-of-order lock acquisitions.

### 2. Deadlock Avoidance — Banker's Algorithm
- **Safety Algorithm**: Evaluates `Available`, `Allocation`, `Max`, and `Need` ($Need = Max - Allocation$) matrices to verify whether a safe execution sequence $<P_{i_1}, P_{i_2}, \dots, P_{i_n}>$ exists.
- **Resource-Request Algorithm**: Dynamically validates process requests against `Need` and `Available`, tentatively allocates resources, verifies system safety, and rolls back unsafe allocations.

### 3. Deadlock Detection Algorithms
- **Multi-Instance Matrix Deadlock Detection**: Identifies deadlocked processes when processes hold and request multi-instance resource types without predetermined maximum claims.
- **Single-Instance Wait-For Graph (WFG) Cycle Detection**: Uses Depth-First Search (DFS) with recursion-stack tracking to detect directed cycles ($P_i \to P_j \to \dots \to P_i$) and reconstruct the exact deadlock cycle.

---

## Repository Structure

```text
deadlock-detection-prevention/
├── Makefile                        # Build, run, and test automation
├── README.md                       # Project documentation and usage guide
├── include/
│   ├── CoffmanPrevention.hpp       # Coffman's 4 Conditions & Lock Order Validator
│   ├── ResourceManager.hpp         # Banker's Algorithm & Matrix Detection
│   ├── Utils.hpp                   # CLI formatting and input validation utilities
│   └── WaitForGraph.hpp            # Wait-For Graph DFS cycle detector
├── src/
│   ├── CoffmanPrevention.cpp       # Implementation of prevention strategies
│   ├── ResourceManager.cpp         # Implementation of Banker's & Matrix detection
│   ├── WaitForGraph.cpp            # Implementation of WFG cycle detection
│   └── main.cpp                    # Interactive menu-driven CLI entry point
└── tests/
    └── test_suite.cpp              # Automated unit tests for all algorithms
```

---

## How to Build and Run

### Prerequisites
- Any C++17 compatible compiler (`g++` or `clang++`)
- `make` (optional)

### Option A: Using `make` (Recommended)

```bash
# 1. Build the interactive simulator
make

# 2. Run the interactive simulator
make run

# 3. Build and run the automated unit test suite
make test

# 4. Clean compiled binaries
make clean
```

### Option B: Direct Compilation with `g++`

```bash
# Compile interactive simulator
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude src/main.cpp src/ResourceManager.cpp src/WaitForGraph.cpp src/CoffmanPrevention.cpp -o deadlock_sim

# Run interactive simulator
./deadlock_sim
```

---

## Sample Walkthrough

1. **Verify Safe State (Option 2)**:
   - Uses the default 5-process (`P0..P4`), 3-resource (`R0..R2`) state.
   - Outputs the step-by-step `Work` vector progression and safe sequence: `< P1 -> P3 -> P4 -> P0 -> P2 >`.
2. **Test Dynamic Resource Request (Option 3)**:
   - Select Process `1` (`P1`) and request `1 0 2`.
   - The Banker's Resource-Request Algorithm verifies `Request <= Need` and `Request <= Available`, confirms the resulting state is safe, and grants the request.
3. **Detect Active Deadlock (Option 6, then Option 4)**:
   - Load the Deadlocked State Preset (`Option 6`), then run Matrix Deadlock Detection (`Option 4`) to identify the deadlocked processes (`P1 P2 P3 P4`).
4. **Wait-For Graph Cycle Detection (Option 8)**:
   - Run the preset graph or enter custom edges to detect directed circular-wait cycles (`P0 -> P1 -> P2 -> P0`).
5. **Coffman Conditions & Lock-Order Validator (Option 9)**:
   - Explore each of the 4 Coffman conditions or test custom lock acquisition sequences against the global resource rank hierarchy.
