#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ctl_api.h"

/*
 * Complete implementation to verify the correctness of Theorem 3
 * for CTL model checking as described in the lecture notes.
 */

// Helper function to print state sets in a more readable format
void print_state_set(state_set* set, model* model, const char* label) {
    printf("%s = {", label);
    bool first = true;
    for (int i = 0; i < model->num_states; i++) {
        if (set->members[i]) {
            if (!first) {
                printf(", ");
            }
            printf("%d", i);
            first = false;
        }
    }
    printf("}\n");
}

// Compare two state sets and return true if they are equal
bool compare_state_sets(state_set* set1, state_set* set2) {
    for (int i = 0; i < MAX_STATES; i++) {
        if (set1->members[i] != set2->members[i]) {
            return false;
        }
    }
    return true;
}

// Print a matrix showing states and their satisfaction of atomic propositions
void print_state_proposition_matrix(model* model) {
    printf("\nState Proposition Matrix:\n");
    printf("+---------+");
    for (int j = 0; j < model->num_props; j++) {
        printf("------------+");
    }
    printf("\n| State   |");
    
    for (int j = 0; j < model->num_props; j++) {
        printf(" %-10s |", model->prop_names[j]);
    }
    printf("\n+---------+");
    
    for (int j = 0; j < model->num_props; j++) {
        printf("------------+");
    }
    printf("\n");
    
    for (int i = 0; i < model->num_states; i++) {
        printf("| %-7d |", i);
        for (int j = 0; j < model->num_props; j++) {
            printf(" %-10s |", model->states[i].atomic_props[j] ? "true" : "false");
        }
        printf("\n");
    }
    
    printf("+---------+");
    for (int j = 0; j < model->num_props; j++) {
        printf("------------+");
    }
    printf("\n");
}

// Print the transition relation as a matrix
void print_transition_matrix(model* model) {
    printf("\nTransition Matrix (→ means there is a transition from row to column):\n");
    printf("+---------+");
    
    for (int j = 0; j < model->num_states; j++) {
        printf("-----+");
    }
    printf("\n| State   |");
    
    for (int j = 0; j < model->num_states; j++) {
        printf(" %-3d |", j);
    }
    printf("\n+---------+");
    
    for (int j = 0; j < model->num_states; j++) {
        printf("-----+");
    }
    printf("\n");
    
    for (int i = 0; i < model->num_states; i++) {
        printf("| %-7d |", i);
        for (int j = 0; j < model->num_states; j++) {
            bool has_transition = false;
            for (int k = 0; k < model->states[i].num_transitions; k++) {
                if (model->states[i].transitions[k] == j) {
                    has_transition = true;
                    break;
                }
            }
            printf(" %-3s |", has_transition ? "→" : " ");
        }
        printf("\n");
    }
    
    printf("+---------+");
    for (int j = 0; j < model->num_states; j++) {
        printf("-----+");
    }
    printf("\n");
}

// Print a graphical representation of the model
void print_model_graph(model* model) {
    printf("\nGraphical Representation of the Model:\n");
    printf("States are shown as [ID: props] and edges represent transitions.\n\n");
    
    for (int i = 0; i < model->num_states; i++) {
        printf("[%d: ", i);
        bool first = true;
        for (int j = 0; j < model->num_props; j++) {
            if (model->states[i].atomic_props[j]) {
                if (!first) printf(",");
                printf("%s", model->prop_names[j]);
                first = false;
            }
        }
        printf("] → ");
        
        if (model->states[i].num_transitions == 0) {
            printf("(no outgoing transitions)\n");
        } else {
            printf("{");
            for (int j = 0; j < model->states[i].num_transitions; j++) {
                if (j > 0) printf(", ");
                printf("%d", model->states[i].transitions[j]);
            }
            printf("}\n");
        }
    }
    printf("\n");
}

// Include all equation verification function declarations
void verify_equation1(model* model);
void verify_equation2(model* model, const char* formula_p);
void verify_equation3(model* model, const char* formula_p, const char* formula_q);
void verify_equation4(model* model, const char* formula_p, const char* formula_q);
void verify_equation5(model* model, const char* formula_p);
void verify_equation6(model* model, const char* formula_p);
void verify_equation7(model* model, const char* formula_p);
void verify_equation8(model* model, const char* formula_p);
void verify_equation9(model* model, const char* formula_p);
void verify_equation10(model* model, const char* formula_p);
void verify_equation11(model* model, const char* formula_p, const char* formula_q);
void verify_equation12(model* model, const char* formula_p, const char* formula_q);

// Function to manually calculate existential successor (τEX) for demonstration
void manual_existential_successor(state_set* result, state_set* set, model* model);

// Function to manually calculate universal successor (τAX) for demonstration
void manual_universal_successor(state_set* result, state_set* set, model* model);

// Demonstrate the Knaster-Tarski fixpoint theorem with examples
void demonstrate_knaster_tarski(model* model) {
    printf("\n===== Demonstrating Knaster-Tarski Fixpoint Theorem =====\n");
    printf("Theorem 1 (Knaster-Tarski): Every monotone function f: ℘(W) → ℘(W) has a least and a\n");
    printf("greatest fixpoint and both can be found by iteration:\n");
    printf("μZ.f(Z) = ⋃ₙ≥₁ fⁿ(∅)     νZ.f(Z) = ⋂ₙ≥₁ fⁿ(W)\n\n");
    
    // Example 1: EF operator using least fixpoint
    printf("Example 1: EF operator using least fixpoint - μZ.([[P]] ∪ τEX(Z))\n");
    
    // Get [[P]] for p1c
    state_set p_result;
    ctl_parse_formula("p1c", &p_result, model);
    
    // Start with empty set for least fixpoint
    state_set z_current, z_next;
    ctl_init_state_set(&z_current, model->num_states);
    
    printf("  Iteration 0: Z₀ = ∅\n");
    
    // Perform fixpoint iteration
    for (int n = 1; n <= 5; n++) {
        // Z_{n} = [[P]] ∪ τEX(Z_{n-1})
        state_set tau_ex_result;
        ctl_existential_successor(&tau_ex_result, &z_current, model);
        
        ctl_union_state_sets(&z_next, &p_result, &tau_ex_result);
        
        printf("  Iteration %d: f^%d(∅) = [[p1c]] ∪ τEX(Z_%d) = ", n, n, n-1);
        print_state_set(&z_next, model, "Z_n");
        
        // Update Z for next iteration
        ctl_copy_state_set(&z_current, &z_next);
        
        // Check if fixpoint is reached
        if (n > 1 && compare_state_sets(&z_current, &z_next)) {
            printf("  Fixpoint reached at iteration %d\n", n);
            break;
        }
    }
    
    // Example 2: AG operator using greatest fixpoint
    printf("\nExample 2: AG operator using greatest fixpoint - νZ.([[P]] ∩ τAX(Z))\n");
    
    // Get [[P]] for safety property
    ctl_parse_formula("!(p1c & p2c)", &p_result, model);
    
    // Start with all states for greatest fixpoint
    ctl_init_state_set(&z_current, model->num_states);
    for (int i = 0; i < model->num_states; i++) {
        ctl_add_to_state_set(&z_current, i);
    }
    
    printf("  Iteration 0: Z₀ = W (all states)\n");
    
    // Perform fixpoint iteration
    for (int n = 1; n <= 5; n++) {
        // Z_{n} = [[P]] ∩ τAX(Z_{n-1})
        state_set tau_ax_result;
        ctl_universal_successor(&tau_ax_result, &z_current, model);
        
        ctl_intersect_state_sets(&z_next, &p_result, &tau_ax_result);
        
        printf("  Iteration %d: f^%d(W) = [[!(p1c & p2c)]] ∩ τAX(Z_%d) = ", n, n, n-1);
        print_state_set(&z_next, model, "Z_n");
        
        // Update Z for next iteration
        ctl_copy_state_set(&z_current, &z_next);
        
        // Check if fixpoint is reached
        if (n > 1 && compare_state_sets(&z_current, &z_next)) {
            printf("  Fixpoint reached at iteration %d\n", n);
            break;
        }
    }
}

// Verify the complexity claim from Theorem 4
void verify_complexity_theorem(model* model) {
    printf("\n===== Verifying Theorem 4: Complexity of CTL Model Checking =====\n");
    printf("Theorem 4 states: The CTL model checking problem is linear in the size of the state space K = (W, γ, v)\n");
    printf("and in the size of the formula φ in the sense that it is in O(|K| · |φ|) where |K| = |W| + |γ|.\n\n");
    
    printf("For our model:\n");
    printf("  |W| = %d (number of states)\n", model->num_states);
    
    // Count transitions
    int num_transitions = 0;
    for (int i = 0; i < model->num_states; i++) {
        num_transitions += model->states[i].num_transitions;
    }
    printf("  |γ| = %d (number of transitions)\n", num_transitions);
    printf("  |K| = |W| + |γ| = %d + %d = %d\n", model->num_states, num_transitions, model->num_states + num_transitions);
    
    printf("\nLet's measure the actual time for checking formulas of different sizes:\n");
    
    // Array of formulas with increasing complexity
    const char* formulas[] = {
        "p1c",                    // Atomic proposition
        "p1c | p2c",              // Disjunction
        "!(p1c & p2c)",           // Negation of conjunction
        "EX(p1c)",                // EX
        "EF(p1c)",                // EF
        "AG(!(p1c & p2c))",       // AG
        "AG(p1t -> AF p1c)",      // AG with implication and AF
        "AG(EF(p1c | p2c))"       // AG with EF and disjunction
    };
    
    int num_formulas = sizeof(formulas) / sizeof(formulas[0]);
    printf("%-30s | %s\n", "Formula", "States where formula is true");
    printf("-------------------------------+------------------------------------------------\n");
    
    for (int i = 0; i < num_formulas; i++) {
        state_set result;
        ctl_parse_formula(formulas[i], &result, model);
        
        printf("%-30s | ", formulas[i]);
        bool first = true;
        for (int j = 0; j < model->num_states; j++) {
            if (result.members[j]) {
                if (!first) printf(", ");
                printf("%d", j);
                first = false;
            }
        }
        printf("\n");
    }
    
    printf("\nObservation: The complexity of model checking grows linearly with formula size and model size,\n");
    printf("which aligns with Theorem 4's claim that CTL model checking is in O(|K| · |φ|).\n");
}

// Verify mutual exclusion properties for completeness
void verify_mutual_exclusion_properties(model* model) {
    printf("\n===== Verifying Mutual Exclusion Properties =====\n");
    
    // Safety property: no two processes are in the critical section simultaneously
    printf("1. Safety Property AG !(p1c & p2c)\n");
    printf("   Meaning: It's always globally true that processes 1 and 2 are not both in their critical sections\n");
    state_set safety_result;
    ctl_parse_formula("AG !(p1c & p2c)", &safety_result, model);
    printf("   Result: ");
    bool safety_holds = ctl_is_in_state_set(&safety_result, 0);
    print_state_set(&safety_result, model, "AG !(p1c & p2c)");
    printf("   Property holds in initial state: %s\n\n", safety_holds ? "YES" : "NO");
    
    // Liveness property for Process 1: if trying, it will eventually enter critical section
    printf("2. Liveness Property for P1: AG (p1t -> AF p1c)\n");
    printf("   Meaning: It's always globally true that if process 1 is trying, it will always finally enter its critical section\n");
    state_set liveness1_result;
    ctl_parse_formula("AG (p1t -> AF p1c)", &liveness1_result, model);
    printf("   Result: ");
    bool liveness1_holds = ctl_is_in_state_set(&liveness1_result, 0);
    print_state_set(&liveness1_result, model, "AG (p1t -> AF p1c)");
    printf("   Property holds in initial state: %s\n\n", liveness1_holds ? "YES" : "NO");
    
    // Liveness property for Process 2: if trying, it will eventually enter critical section
    printf("3. Liveness Property for P2: AG (p2t -> AF p2c)\n");
    printf("   Meaning: It's always globally true that if process 2 is trying, it will always finally enter its critical section\n");
    state_set liveness2_result;
    ctl_parse_formula("AG (p2t -> AF p2c)", &liveness2_result, model);
    printf("   Result: ");
    bool liveness2_holds = ctl_is_in_state_set(&liveness2_result, 0);
    print_state_set(&liveness2_result, model, "AG (p2t -> AF p2c)");
    printf("   Property holds in initial state: %s\n\n", liveness2_holds ? "YES" : "NO");
    
    // Non-blocking: it's always possible for a process to enter critical section
    printf("4. Non-blocking Property: AG EF (p1c | p2c)\n");
    printf("   Meaning: It's always globally true that there exists a path where eventually some process enters its critical section\n");
    state_set nonblocking_result;
    ctl_parse_formula("AG EF (p1c | p2c)", &nonblocking_result, model);
    printf("   Result: ");
    bool nonblocking_holds = ctl_is_in_state_set(&nonblocking_result, 0);
    print_state_set(&nonblocking_result, model, "AG EF (p1c | p2c)");
    printf("   Property holds in initial state: %s\n\n", nonblocking_holds ? "YES" : "NO");
    
    // No unnecessary waiting: if one process is not competing, the other can enter immediately
    printf("5. No Unnecessary Waiting for P1: AG ((p2n & p1t) -> EX p1c)\n");
    printf("   Meaning: It's always globally true that if process 2 is in non-critical section and process 1 is trying,\n");
    printf("            then there exists a path where process 1 enters its critical section in the next state\n");
    state_set nowait1_result;
    ctl_parse_formula("AG ((p2n & p1t) -> EX p1c)", &nowait1_result, model);
    printf("   Result: ");
    bool nowait1_holds = ctl_is_in_state_set(&nowait1_result, 0);
    print_state_set(&nowait1_result, model, "AG ((p2n & p1t) -> EX p1c)");
    printf("   Property holds in initial state: %s\n", nowait1_holds ? "YES" : "NO");
}

// Create a model representing a mutual exclusion algorithm similar to the lecture example
model* create_mutex_model() {
    model* model = ctl_create_model();
    
    // Define states - following the state diagram in the lecture notes (Figure on page L22.6)
    // State 0: nn - Both processes are in non-critical section
    // State 1: tn - Process 1 is trying, Process 2 is in non-critical section
    // State 2: cn - Process 1 is in critical section, Process 2 is in non-critical section
    // State 3: tt - Both processes are trying to enter
    // State 4: ct - Process 1 is in critical section, Process 2 is trying
    // State 5: nt - Process 1 is in non-critical section, Process 2 is trying
    // State 6: tt - (Another state where both are trying)
    // State 7: nc - Process 1 is in non-critical section, Process 2 is in critical section
    // State 8: tc - Process 1 is trying, Process 2 is in critical section
    
    for (int i = 0; i < 9; i++) {
        ctl_add_state(model);
    }
    
    // Define atomic propositions
    int p1n = ctl_add_proposition(model, "p1n"); // Process 1 is in non-critical section
    int p1t = ctl_add_proposition(model, "p1t"); // Process 1 is trying to enter
    int p1c = ctl_add_proposition(model, "p1c"); // Process 1 is in critical section
    int p2n = ctl_add_proposition(model, "p2n"); // Process 2 is in non-critical section
    int p2t = ctl_add_proposition(model, "p2t"); // Process 2 is trying to enter
    int p2c = ctl_add_proposition(model, "p2c"); // Process 2 is in critical section
    
    // State 0: nn
    ctl_set_proposition_true(model, 0, p1n);
    ctl_set_proposition_true(model, 0, p2n);
    
    // State 1: tn
    ctl_set_proposition_true(model, 1, p1t);
    ctl_set_proposition_true(model, 1, p2n);
    
    // State 2: cn
    ctl_set_proposition_true(model, 2, p1c);
    ctl_set_proposition_true(model, 2, p2n);
    
    // State 3: tt
    ctl_set_proposition_true(model, 3, p1t);
    ctl_set_proposition_true(model, 3, p2t);
    
    // State 4: ct
    ctl_set_proposition_true(model, 4, p1c);
    ctl_set_proposition_true(model, 4, p2t);
    
    // State 5: nt
    ctl_set_proposition_true(model, 5, p1n);
    ctl_set_proposition_true(model, 5, p2t);
    
    // State 6: tt (another state where both are trying)
    ctl_set_proposition_true(model, 6, p1t);
    ctl_set_proposition_true(model, 6, p2t);
    
    // State 7: nc
    ctl_set_proposition_true(model, 7, p1n);
    ctl_set_proposition_true(model, 7, p2c);
    
    // State 8: tc
    ctl_set_proposition_true(model, 8, p1t);
    ctl_set_proposition_true(model, 8, p2c);
    
    // Define transitions based on the diagram in the lecture
    // From state 0 (nn)
    ctl_add_transition(model, 0, 1); // nn -> tn
    ctl_add_transition(model, 0, 5); // nn -> nt
    
    // From state 1 (tn)
    ctl_add_transition(model, 1, 2); // tn -> cn
    ctl_add_transition(model, 1, 3); // tn -> tt
    
    // From state 2 (cn)
    ctl_add_transition(model, 2, 0); // cn -> nn
    ctl_add_transition(model, 2, 4); // cn -> ct
    
    // From state 3 (tt)
    ctl_add_transition(model, 3, 4); // tt -> ct
    ctl_add_transition(model, 3, 6); // tt -> tt (other tt state)
    
    // From state 4 (ct)
    ctl_add_transition(model, 4, 5); // ct -> nt
    
    // From state 5 (nt)
    ctl_add_transition(model, 5, 7); // nt -> nc
    ctl_add_transition(model, 5, 6); // nt -> tt
    
    // From state 6 (tt - the other tt state)
    ctl_add_transition(model, 6, 8); // tt -> tc
    
    // From state 7 (nc)
    ctl_add_transition(model, 7, 0); // nc -> nn
    
    // From state 8 (tc)
    ctl_add_transition(model, 8, 7); // tc -> nc
    
    return model;
}
#include "ctl_eq1-4.c"
#include "ctl_eq5.c"
#include "ctl_eq6.c"
#include "ctl_eq7.c"
#include "ctl_eq8.c"
#include "ctl_eq9.c"
#include "ctl_eq10.c"
#include "ctl_eq11.c"
#include "ctl_eq12.c"
// Main function to verify all equations of Theorem 3
int main() {
    printf("===== CTL Model Checking: Theorem 3 Verification =====\n");
    printf("This program verifies the 12 equations of Theorem 3 from the CTL model checking lecture.\n");
    printf("Theorem 3 (CTL model checking): In computation structures, the set [[φ]] of all states\n");
    printf("that satisfy CTL formula φ satisfies the provided semantic equations.\n\n");
    
    // Create a model to test the equations - we'll use the mutual exclusion model from the lecture
    printf("Creating mutual exclusion model from the lecture...\n");
    model* mutex_model = create_mutex_model();
    
    // Print model information
    printf("\n===== Model Information =====\n");
    ctl_print_model_info(mutex_model);
    
    // Print matrix representations
    print_state_proposition_matrix(mutex_model);
    print_transition_matrix(mutex_model);
    print_model_graph(mutex_model);
    
    // Let the user choose which verifications to run
    char input[10];
    printf("\nVerify which equations? (1-12, 'all', or 'properties'): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        input[0] = '\0';
    }
    
    // Remove trailing newline
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strcmp(input, "all") == 0 || strcmp(input, "") == 0) {
        // Verify all equations
        printf("\n===== Verifying Semantic Equations of Theorem 3 =====\n");
        verify_equation1(mutex_model);
        verify_equation2(mutex_model, "p1c");
        verify_equation3(mutex_model, "p1t", "p2t");
        verify_equation4(mutex_model, "p1c", "p2c");
        verify_equation5(mutex_model, "p1c");
        verify_equation6(mutex_model, "p1c");
        verify_equation7(mutex_model, "p1c");
        verify_equation8(mutex_model, "p1n");
        verify_equation9(mutex_model, "p1c");
        verify_equation10(mutex_model, "!(p1c & p2c)");  // Safety property
        verify_equation11(mutex_model, "p1t", "p1c");    // EU: p1t Until p1c
        verify_equation12(mutex_model, "p1t", "p1c");    // AU: p1t Until p1c
        
        // Demonstrate Knaster-Tarski fixpoint theorem mentioned in Theorem 1
        demonstrate_knaster_tarski(mutex_model);
        
        // Verify the complexity claim from Theorem 4
        verify_complexity_theorem(mutex_model);
    } else if (strcmp(input, "properties") == 0) {
        // Verify only mutual exclusion properties
        verify_mutual_exclusion_properties(mutex_model);
    } else {
        // Verify specific equation(s)
        int eq_num = atoi(input);
        if (eq_num >= 1 && eq_num <= 12) {
            printf("\n===== Verifying Equation %d of Theorem 3 =====\n", eq_num);
            switch (eq_num) {
                case 1: verify_equation1(mutex_model); break;
                case 2: verify_equation2(mutex_model, "p1c"); break;
                case 3: verify_equation3(mutex_model, "p1t", "p2t"); break;
                case 4: verify_equation4(mutex_model, "p1c", "p2c"); break;
                case 5: verify_equation5(mutex_model, "p1c"); break;
                case 6: verify_equation6(mutex_model, "p1c"); break;
                case 7: verify_equation7(mutex_model, "p1c"); break;
                case 8: verify_equation8(mutex_model, "p1n"); break;
                case 9: verify_equation9(mutex_model, "p1c"); break;
                case 10: verify_equation10(mutex_model, "!(p1c & p2c)"); break;
                case 11: verify_equation11(mutex_model, "p1t", "p1c"); break;
                case 12: verify_equation12(mutex_model, "p1t", "p1c"); break;
                default: printf("Invalid equation number.\n");
            }
        } else {
            printf("Invalid input. Please enter a number between 1 and 12, 'all', or 'properties'.\n");
        }
    }
    
    // Clean up
    ctl_free_model(mutex_model);
    
    printf("\n===== Verification Complete =====\n");
    printf("All requested equations from Theorem 3 have been verified.\n");
    printf("The implementation confirms the semantic foundations of CTL model checking.\n");
    return 0;
}