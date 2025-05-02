# CTL Theorem 3 Verifier

This project provides a comprehensive implementation to verify and demonstrate the correctness of Theorem 3 from the lecture notes on CTL (Computation Tree Logic) model checking.

## Theorem 3 (CTL model checking)

In computation structures, the set [[φ]] of all states that satisfy CTL formula φ satisfies the following equations:

1. [[p]] = {s ∈ W : v(s)(p) = true} for atomic propositions p
2. [[¬P]] = W \ [[P]]
3. [[P ∧ Q]] = [[P]] ∩ [[Q]]
4. [[P ∨ Q]] = [[P]] ∪ [[Q]]
5. [[EX P]] = τEX([[P]]) using the existential successor function τEX()
6. [[AX P]] = τAX([[P]]) using the universal successor function τAX()
7. [[EF P]] = μZ.([[P]] ∪ τEX(Z))
8. [[EG P]] = νZ.([[P]] ∩ τEX(Z))
9. [[AF P]] = μZ.([[P]] ∪ τAX(Z))
10. [[AG P]] = νZ.([[P]] ∩ τAX(Z))
11. [[EP UQ]] = μZ.([[Q]] ∪ ([[P]] ∩ τEX(Z)))
12. [[AP UQ]] = μZ.([[Q]] ∪ ([[P]] ∩ τAX(Z)))

Where:
- μZ.f(Z) denotes the least fixpoint of operation f(Z)
- νZ.f(Z) denotes the greatest fixpoint of operation f(Z)

## Project Structure

This project is organized to provide a clear and modular verification of each equation:

- `ctl_api.h` - Header file with API declarations
- `ctl_implementation.c` - Implementation of CTL operations and formula parser
- `ctl_main.c` - Main program that orchestrates the verification process
- `ctl_eq1-4.c` - Verification of equations 1-4 (basic CTL operators)
- `ctl_eq5.c` - Verification of equation 5 (EX operator)
- `ctl_eq6.c` - Verification of equation 6 (AX operator)
- `ctl_eq7.c` - Verification of equation 7 (EF operator)
- `ctl_eq8.c` - Verification of equation 8 (EG operator)
- `ctl_eq9.c` - Verification of equation 9 (AF operator)
- `ctl_eq10.c` - Verification of equation 10 (AG operator)
- `ctl_eq11.c` - Verification of equation 11 (EU operator)
- `ctl_eq12.c` - Verification of equation 12 (AU operator)
- `Makefile` - Build script for the project

## Building the Project

To build the project, simply run:

```bash
make
```

This will compile all the source files and create the executable `theorem3_verifier`.

## Running the Verifier

To run the CTL Theorem 3 verifier:

```bash
./theorem3_verifier
```

The program will prompt you to choose which equations to verify:
- Enter a number from 1-12 to verify a specific equation
- Enter "all" to verify all equations (default if you just press Enter)
- Enter "properties" to verify mutual exclusion properties

## Features

### 1. Detailed Verification of Each Equation

For each equation in Theorem 3, the verifier:
- Computes the left-hand side using the CTL API
- Independently calculates the right-hand side using the semantic definition
- Shows step-by-step calculations, particularly for fixpoint iterations
- Compares the results to verify equality

### 2. Fixpoint Calculations

For operators that involve fixpoints (EF, EG, AF, AG, EU, AU), the verifier:
- Shows the complete iteration process starting from empty set (for least fixpoints) or all states (for greatest fixpoints)
- Displays the state sets at each iteration
- Demonstrates convergence to the fixpoint
- Explains the semantic meaning of each operator

### 3. Model Visualization

The verifier provides multiple ways to visualize the model:
- State-proposition matrix showing atomic propositions true in each state
- Transition matrix showing the transition relation
- Graphical representation of the state transition system

### 4. Knaster-Tarski Theorem Demonstration

The verifier includes a demonstration of the Knaster-Tarski fixpoint theorem:
- Shows how least fixpoints are computed as unions of iterates starting from empty set
- Shows how greatest fixpoints are computed as intersections of iterates starting from all states

### 5. Verification of Mutual Exclusion Properties

The verifier checks important properties of the mutual exclusion algorithm:
- Safety: Two processes are never in their critical sections simultaneously
- Liveness: If a process is trying to enter, it will eventually succeed
- Non-blocking: It's always possible for some process to enter the critical section
- No unnecessary waiting: If one process is not competing, the other can enter immediately

## Example Output

For each equation, the verifier provides detailed output like this:

```
7. Verifying [[EF P]] = μZ.([[P]] ∪ τEX(Z)) where P = p1c:
   This equation defines the semantics of the existential finally operator using a least fixpoint.
   μZ.f(Z) denotes the least fixpoint of the operation f(Z).

  Computing μZ.([[P]] ∪ τEX(Z)) as a least fixpoint:
  According to Knaster-Tarski theorem: μZ.f(Z) = ⋃ₙ≥₁ fⁿ(∅)
  Starting with Z₀ = ∅ (empty set) and iterating:
  
    Iteration 0: Z_0 = {}
    τEX(Z_0) = {}
    f(Z_0) = [[P]] ∪ τEX(Z_0) = {2, 4}
    
    Iteration 1: Z_1 = {2, 4}
    τEX(Z_1) = {1, 3, 6}
    f(Z_1) = [[P]] ∪ τEX(Z_1) = {1, 2, 3, 4, 6}
    
    ...
    
    Fixed point reached! Z_4 = Z_3
    
  Explanation of EF p1c:
  EF P means 'there exists a path where P eventually holds'
  ...
  
  CTL API result [[EF P]] = {0, 1, 2, 3, 4, 5, 6, 7, 8}
  Manually computed μZ.([[P]] ∪ τEX(Z)) = {0, 1, 2, 3, 4, 5, 6, 7, 8}
  Equation holds: YES
```

## References

- Lecture 22: CTL model checking
- Baier, C., & Katoen, J. P. (2008). Principles of Model Checking. MIT Press.
- Clarke, E. M., Grumberg, O., & Peled, D. A. (1999). Model Checking. MIT Press.
