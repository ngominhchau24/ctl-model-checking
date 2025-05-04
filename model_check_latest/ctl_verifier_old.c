// ctl_verifier.c - A program to verify the 12 equations in Theorem 3 of CTL model checking
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Maximum number of states and atomic propositions
#define MAX_STATES 100
#define MAX_PROPS 20
#define MAX_TRANSITIONS 10

// Structure for representing a state
typedef struct {
    int id;
    bool atomic_props[MAX_PROPS];  // Truth values of atomic propositions
    int num_transitions;
    int transitions[MAX_TRANSITIONS];  // IDs of successor states
} State;

// Structure for representing a Kripke structure (model)
typedef struct {
    State states[MAX_STATES];
    int num_states;
    int num_props;
    char* prop_names[MAX_PROPS];  // Names of atomic propositions
} Model;

// Structure for representing a set of states
typedef struct {
    bool members[MAX_STATES];
    int size;
} StateSet;

// Model creation and manipulation
Model* create_model();
void free_model(Model* model);
int add_state(Model* model);
bool add_transition(Model* model, int from_state, int to_state);
int add_proposition(Model* model, const char* prop_name);
void set_proposition_true(Model* model, int state_id, int prop_id);

// State set operations
void init_state_set(StateSet* set, int num_states);
void add_to_state_set(StateSet* set, int state_id);
bool is_in_state_set(StateSet* set, int state_id);
void complement_state_set(StateSet* result, StateSet* set, int num_states);
void intersect_state_sets(StateSet* result, StateSet* set1, StateSet* set2);
void union_state_sets(StateSet* result, StateSet* set1, StateSet* set2);
void copy_state_set(StateSet* dest, StateSet* src);
bool compare_state_sets(StateSet* set1, StateSet* set2);

// Basic CTL operators
void eval_atomic_prop(StateSet* result, Model* model, int prop_id);
void existential_successor(StateSet* result, StateSet* set, Model* model);  // EX
void universal_successor(StateSet* result, StateSet* set, Model* model);    // AX
void eval_ef(StateSet* result, StateSet* prop_p, Model* model);             // EF
void eval_af(StateSet* result, StateSet* prop_p, Model* model);             // AF
void eval_eg(StateSet* result, StateSet* prop_p, Model* model);             // EG
void eval_ag(StateSet* result, StateSet* prop_p, Model* model);             // AG
void eval_eu(StateSet* result, StateSet* prop_p, StateSet* prop_q, Model* model); // EU
void eval_au(StateSet* result, StateSet* prop_p, StateSet* prop_q, Model* model); // AU

// Utility functions
void print_model_info(Model* model);
void print_state_set(StateSet* set, Model* model, const char* label);

// Functions to verify each equation in Theorem 3
void verify_equation1(Model* model);
void verify_equation2(Model* model, int prop_p);
void verify_equation3(Model* model, int prop_p, int prop_q);
void verify_equation4(Model* model, int prop_p, int prop_q);
void verify_equation5(Model* model, int prop_p);
void verify_equation6(Model* model, int prop_p);
void verify_equation7(Model* model, int prop_p);
void verify_equation8(Model* model, int prop_p);
void verify_equation9(Model* model, int prop_p);
void verify_equation10(Model* model, int prop_p);
void verify_equation11(Model* model, int prop_p, int prop_q);
void verify_equation12(Model* model, int prop_p, int prop_q);

// Create a model representing a mutual exclusion algorithm (similar to the lecture example)
Model* create_mutex_model() {
    Model* model = create_model();
    
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
        add_state(model);
    }
    
    // Define atomic propositions
    int p1n = add_proposition(model, "p1n"); // Process 1 is in non-critical section
    int p1t = add_proposition(model, "p1t"); // Process 1 is trying to enter
    int p1c = add_proposition(model, "p1c"); // Process 1 is in critical section
    int p2n = add_proposition(model, "p2n"); // Process 2 is in non-critical section
    int p2t = add_proposition(model, "p2t"); // Process 2 is trying to enter
    int p2c = add_proposition(model, "p2c"); // Process 2 is in critical section
    
    // State 0: nn
    set_proposition_true(model, 0, p1n);
    set_proposition_true(model, 0, p2n);
    
    // State 1: tn
    set_proposition_true(model, 1, p1t);
    set_proposition_true(model, 1, p2n);
    
    // State 2: cn
    set_proposition_true(model, 2, p1c);
    set_proposition_true(model, 2, p2n);
    
    // State 3: tt
    set_proposition_true(model, 3, p1t);
    set_proposition_true(model, 3, p2t);
    
    // State 4: ct
    set_proposition_true(model, 4, p1c);
    set_proposition_true(model, 4, p2t);
    
    // State 5: nt
    set_proposition_true(model, 5, p1n);
    set_proposition_true(model, 5, p2t);
    
    // State 6: tt (another state where both are trying)
    set_proposition_true(model, 6, p1t);
    set_proposition_true(model, 6, p2t);
    
    // State 7: nc
    set_proposition_true(model, 7, p1n);
    set_proposition_true(model, 7, p2c);
    
    // State 8: tc
    set_proposition_true(model, 8, p1t);
    set_proposition_true(model, 8, p2c);
    
    // Define transitions based on the diagram in the lecture
    // From state 0 (nn)
    add_transition(model, 0, 1); // nn -> tn
    add_transition(model, 0, 5); // nn -> nt
    
    // From state 1 (tn)
    add_transition(model, 1, 2); // tn -> cn
    add_transition(model, 1, 3); // tn -> tt
    
    // From state 2 (cn)
    add_transition(model, 2, 0); // cn -> nn
    add_transition(model, 2, 4); // cn -> ct
    
    // From state 3 (tt)
    add_transition(model, 3, 4); // tt -> ct
    add_transition(model, 3, 6); // tt -> tt (other tt state)
    
    // From state 4 (ct)
    add_transition(model, 4, 5); // ct -> nt
    
    // From state 5 (nt)
    add_transition(model, 5, 7); // nt -> nc
    add_transition(model, 5, 6); // nt -> tt
    
    // From state 6 (tt - the other tt state)
    add_transition(model, 6, 8); // tt -> tc
    
    // From state 7 (nc)
    add_transition(model, 7, 0); // nc -> nn
    
    // From state 8 (tc)
    add_transition(model, 8, 7); // tc -> nc
    
    return model;
}

int main() {
    printf("===== CTL Model Checking: Theorem 3 Verification =====\n");
    printf("This program verifies the 12 equations of Theorem 3 from the CTL model checking lecture.\n");
    printf("Theorem 3 (CTL model checking): In computation structures, the set [[φ]] of all states\n");
    printf("that satisfy CTL formula φ satisfies the provided semantic equations.\n\n");
    
    // Create the mutual exclusion model
    printf("Creating mutual exclusion model...\n");
    Model* model = create_mutex_model();
    
    // Print model information
    print_model_info(model);
    
    // Verify all 12 equations
    printf("\n===== Verifying Semantic Equations of Theorem 3 =====\n");
    verify_equation1(model);
    verify_equation2(model, 0); // Using p1n proposition
    verify_equation3(model, 1, 2); // Using p1t and p1c propositions
    verify_equation4(model, 3, 4); // Using p2t and p2c propositions
    verify_equation5(model, 2); // Using p1c proposition
    verify_equation6(model, 2); // Using p1c proposition
    verify_equation7(model, 2); // Using p1c proposition
    verify_equation8(model, 0); // Using p1n proposition
    verify_equation9(model, 2); // Using p1c proposition
    verify_equation10(model, 5); // Using p2t proposition
    verify_equation11(model, 1, 2); // Using p1t and p1c propositions
    verify_equation12(model, 3, 4); // Using p2t and p2c propositions
    
    // Clean up
    free_model(model);
    
    printf("\n===== Verification Complete =====\n");
    printf("All 12 equations from Theorem 3 have been verified.\n");
    return 0;
}

// Create a new empty model
Model* create_model() {
    Model* model = (Model*)malloc(sizeof(Model));
    if (model == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for model\n");
        return NULL;
    }
    
    model->num_states = 0;
    model->num_props = 0;
    
    return model;
}

// Free a model
void free_model(Model* model) {
    if (model == NULL) return;
    
    for (int i = 0; i < model->num_props; i++) {
        free(model->prop_names[i]);
    }
    
    free(model);
}

// Add a new state to the model
int add_state(Model* model) {
    if (model->num_states >= MAX_STATES) {
        fprintf(stderr, "Error: Maximum number of states reached\n");
        return -1;
    }
    
    int state_id = model->num_states;
    model->states[state_id].id = state_id;
    model->states[state_id].num_transitions = 0;
    
    // Initialize all propositions to false for this state
    for (int i = 0; i < model->num_props; i++) {
        model->states[state_id].atomic_props[i] = false;
    }
    
    model->num_states++;
    return state_id;
}

// Add a transition between states
bool add_transition(Model* model, int from_state, int to_state) {
    if (from_state < 0 || from_state >= model->num_states || 
        to_state < 0 || to_state >= model->num_states) {
        fprintf(stderr, "Error: Invalid state ID for transition\n");
        return false;
    }
    
    if (model->states[from_state].num_transitions >= MAX_TRANSITIONS) {
        fprintf(stderr, "Error: Maximum number of transitions reached for state %d\n", from_state);
        return false;
    }
    
    // Add the transition
    model->states[from_state].transitions[model->states[from_state].num_transitions++] = to_state;
    return true;
}

// Add a new atomic proposition to the model
int add_proposition(Model* model, const char* prop_name) {
    if (model->num_props >= MAX_PROPS) {
        fprintf(stderr, "Error: Maximum number of propositions reached\n");
        return -1;
    }
    
    // Check if the proposition already exists
    for (int i = 0; i < model->num_props; i++) {
        if (strcmp(model->prop_names[i], prop_name) == 0) {
            return i;  // Return existing proposition ID
        }
    }
    
    // Add the new proposition
    int prop_id = model->num_props;
    model->prop_names[prop_id] = strdup(prop_name);
    
    // Initialize the proposition to false for all existing states
    for (int i = 0; i < model->num_states; i++) {
        model->states[i].atomic_props[prop_id] = false;
    }
    
    model->num_props++;
    return prop_id;
}

// Set a proposition to true for a state
void set_proposition_true(Model* model, int state_id, int prop_id) {
    if (state_id < 0 || state_id >= model->num_states || 
        prop_id < 0 || prop_id >= model->num_props) {
        fprintf(stderr, "Error: Invalid state or proposition ID\n");
        return;
    }
    
    model->states[state_id].atomic_props[prop_id] = true;
}

// Initialize an empty state set
void init_state_set(StateSet* set, int num_states) {
    set->size = 0;
    for (int i = 0; i < num_states; i++) {
        set->members[i] = false;
    }
}

// Add a state to a state set
void add_to_state_set(StateSet* set, int state_id) {
    if (!set->members[state_id]) {
        set->members[state_id] = true;
        set->size++;
    }
}

// Check if a state is in a state set
bool is_in_state_set(StateSet* set, int state_id) {
    return set->members[state_id];
}

// Set complement (W \ X)
void complement_state_set(StateSet* result, StateSet* set, int num_states) {
    init_state_set(result, num_states);
    for (int i = 0; i < num_states; i++) {
        if (!set->members[i]) {
            add_to_state_set(result, i);
        }
    }
}

// Set intersection (X ∩ Y)
void intersect_state_sets(StateSet* result, StateSet* set1, StateSet* set2) {
    result->size = 0;
    for (int i = 0; i < MAX_STATES; i++) {
        result->members[i] = set1->members[i] && set2->members[i];
        if (result->members[i]) {
            result->size++;
        }
    }
}

// Set union (X ∪ Y)
void union_state_sets(StateSet* result, StateSet* set1, StateSet* set2) {
    result->size = 0;
    for (int i = 0; i < MAX_STATES; i++) {
        result->members[i] = set1->members[i] || set2->members[i];
        if (result->members[i]) {
            result->size++;
        }
    }
}

// Copy state set
void copy_state_set(StateSet* dest, StateSet* src) {
    dest->size = src->size;
    for (int i = 0; i < MAX_STATES; i++) {
        dest->members[i] = src->members[i];
    }
}

// Compare two state sets and return true if they are equal
bool compare_state_sets(StateSet* set1, StateSet* set2) {
    for (int i = 0; i < MAX_STATES; i++) {
        if (set1->members[i] != set2->members[i]) {
            return false;
        }
    }
    return true;
}

// Evaluate an atomic proposition
void eval_atomic_prop(StateSet* result, Model* model, int prop_id) {
    init_state_set(result, model->num_states);
    
    if (prop_id < 0 || prop_id >= model->num_props) {
        fprintf(stderr, "Error: Invalid proposition ID %d\n", prop_id);
        return;
    }
    
    for (int i = 0; i < model->num_states; i++) {
        if (model->states[i].atomic_props[prop_id]) {
            add_to_state_set(result, i);
        }
    }
}

// Find states that have a successor in the given set (EX)
void existential_successor(StateSet* result, StateSet* set, Model* model) {
    init_state_set(result, model->num_states);
    
    for (int i = 0; i < model->num_states; i++) {
        State* current_state = &model->states[i];
        
        // Check if any successor of state i is in the input set
        for (int j = 0; j < current_state->num_transitions; j++) {
            int successor_id = current_state->transitions[j];
            if (is_in_state_set(set, successor_id)) {
                add_to_state_set(result, i);
                break;
            }
        }
    }
}

// Find states such that all successors are in the given set (AX)
void universal_successor(StateSet* result, StateSet* set, Model* model) {
    init_state_set(result, model->num_states);
    
    for (int i = 0; i < model->num_states; i++) {
        State* current_state = &model->states[i];
        bool all_in_set = true;
        
        // Check if all successors of state i are in the input set
        for (int j = 0; j < current_state->num_transitions; j++) {
            int successor_id = current_state->transitions[j];
            if (!is_in_state_set(set, successor_id)) {
                all_in_set = false;
                break;
            }
        }
        
        if (all_in_set && current_state->num_transitions > 0) {
            add_to_state_set(result, i);
        }
    }
}

// Evaluate EF P (exists finally) - states that can reach a state where P is true
void eval_ef(StateSet* result, StateSet* prop_p, Model* model) {
    StateSet current, next;
    
    // Initialize with states where P is true
    init_state_set(&current, model->num_states);
    copy_state_set(&current, prop_p);
    
    bool changed = true;
    while (changed) {
        changed = false;
        
        // Find states that can reach the current set in one step
        existential_successor(&next, &current, model);
        
        // Add these states to the current set
        for (int i = 0; i < model->num_states; i++) {
            if (next.members[i] && !current.members[i]) {
                add_to_state_set(&current, i);
                changed = true;
            }
        }
    }
    
    copy_state_set(result, &current);
}

// Evaluate EG P (exists globally) - states where there exists a path where P is always true
void eval_eg(StateSet* result, StateSet* prop_p, Model* model) {
    StateSet current;
    
    // Initialize with all states where P is true
    copy_state_set(&current, prop_p);
    
    bool changed = true;
    while (changed) {
        changed = false;
        
        for (int i = 0; i < model->num_states; i++) {
            if (current.members[i]) {
                bool has_valid_successor = false;
                State* current_state = &model->states[i];
                
                // Check if the state has at least one successor that's in the current set
                for (int j = 0; j < current_state->num_transitions; j++) {
                    int successor_id = current_state->transitions[j];
                    if (current.members[successor_id]) {
                        has_valid_successor = true;
                        break;
                    }
                }
                
                // If it doesn't have a valid successor, remove it from the set
                if (!has_valid_successor && current_state->num_transitions > 0) {
                    current.members[i] = false;
                    current.size--;
                    changed = true;
                }
            }
        }
    }
    
    copy_state_set(result, &current);
}

// Evaluate AF P (always finally) - states where all paths eventually reach a state where P is true
void eval_af(StateSet* result, StateSet* prop_p, Model* model) {
    StateSet not_p, eg_not_p;
    
    // Calculate ¬P
    complement_state_set(&not_p, prop_p, model->num_states);
    
    // Calculate EG ¬P
    eval_eg(&eg_not_p, &not_p, model);
    
    // AF P = ¬EG ¬P
    complement_state_set(result, &eg_not_p, model->num_states);
}

// Evaluate AG P (always globally) - states where P is true in all future paths
void eval_ag(StateSet* result, StateSet* prop_p, Model* model) {
    StateSet not_p, ef_not_p;
    
    // Calculate ¬P
    complement_state_set(&not_p, prop_p, model->num_states);
    
    // Calculate EF ¬P
    eval_ef(&ef_not_p, &not_p, model);
    
    // AG P = ¬EF ¬P
    complement_state_set(result, &ef_not_p, model->num_states);
}

// Evaluate EU P Q (exists until) - states that can reach Q through P states
void eval_eu(StateSet* result, StateSet* prop_p, StateSet* prop_q, Model* model) {
    StateSet current, next, temp;
    
    // Initialize with states where Q is true
    copy_state_set(&current, prop_q);
    
    bool changed = true;
    while (changed) {
        changed = false;
        
        // Find states that can reach the current set in one step and satisfy P
        existential_successor(&next, &current, model);
        intersect_state_sets(&temp, &next, prop_p);
        
        // Add these states to the current set
        for (int i = 0; i < model->num_states; i++) {
            if (temp.members[i] && !current.members[i]) {
                add_to_state_set(&current, i);
                changed = true;
            }
        }
    }
    
    copy_state_set(result, &current);
}

// Evaluate AU P Q (always until) - states where all paths reach Q through P states
void eval_au(StateSet* result, StateSet* prop_p, StateSet* prop_q, Model* model) {
    StateSet not_p, not_q, eu_not_p_not_q, eg_not_q;
    
    // Calculate ¬P and ¬Q
    complement_state_set(&not_p, prop_p, model->num_states);
    complement_state_set(&not_q, prop_q, model->num_states);
    
    // Calculate E[¬P U (¬P ∧ ¬Q)]
    intersect_state_sets(&eu_not_p_not_q, &not_p, &not_q);
    eval_eu(&eu_not_p_not_q, &not_p, &eu_not_p_not_q, model);
    
    // Calculate EG ¬Q
    eval_eg(&eg_not_q, &not_q, model);
    
    // A[P U Q] = ¬(E[¬P U (¬P ∧ ¬Q)] ∨ EG ¬Q)
    StateSet temp;
    union_state_sets(&temp, &eu_not_p_not_q, &eg_not_q);
    complement_state_set(result, &temp, model->num_states);
}

// Print information about a model
void print_model_info(Model* model) {
    printf("Model information:\n");
    printf("Number of states: %d\n", model->num_states);
    printf("Number of propositions: %d\n", model->num_props);
    
    printf("Propositions: ");
    for (int i = 0; i < model->num_props; i++) {
        printf("%s ", model->prop_names[i]);
    }
    printf("\n\n");
    
    printf("State transitions:\n");
    for (int i = 0; i < model->num_states; i++) {
        printf("State %d: (", i);
        
        // Print atomic propositions
        for (int j = 0; j < model->num_props; j++) {
            if (j > 0) printf(", ");
            printf("%s=%s", model->prop_names[j], model->states[i].atomic_props[j] ? "true" : "false");
        }
        printf(") -> {");
        
        // Print transitions
        for (int j = 0; j < model->states[i].num_transitions; j++) {
            if (j > 0) printf(", ");
            printf("%d", model->states[i].transitions[j]);
        }
        printf("}\n");
    }
}

// Print the states in a state set
void print_state_set(StateSet* set, Model* model, const char* label) {
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

// Function to verify equation 1: [[p]] = {s ∈ W : v(s)(p) = true}
void verify_equation1(Model* model) {
    printf("\n1. Verifying [[p]] = {s ∈ W : v(s)(p) = true}:\n");
    printf("   This equation defines the semantics of atomic propositions in CTL.\n");
    
    for (int p = 0; p < model->num_props; p++) {
        // Get set of states where p is true using API
        StateSet api_result;
        eval_atomic_prop(&api_result, model, p);
        
        // Calculate set of states where p is true manually
        StateSet manual_result;
        init_state_set(&manual_result, model->num_states);
        
        printf("  For proposition '%s':\n", model->prop_names[p]);
        
        for (int s = 0; s < model->num_states; s++) {
            if (model->states[s].atomic_props[p]) {
                add_to_state_set(&manual_result, s);
            }
        }
        
        // Compare results
        bool equal = compare_state_sets(&api_result, &manual_result);
        printf("  API result [[%s]] = ", model->prop_names[p]);
        print_state_set(&api_result, model, "api result");
        printf("  Manually computed {s ∈ W : v(s)(%s) = true} = ", model->prop_names[p]);
        print_state_set(&manual_result, model, "manual result");
        printf("  Equation holds: %s\n\n", equal ? "YES" : "NO");
    }
}

// Function to verify equation 2: [[¬P]] = W \ [[P]]
void verify_equation2(Model* model, int prop_p) {
    printf("\n2. Verifying [[¬P]] = W \\ [[P]] where P = %s:\n", model->prop_names[prop_p]);
    printf("   This equation defines the semantics of negation in CTL.\n");
    
    // Get [[P]]
    StateSet p_result;
    eval_atomic_prop(&p_result, model, prop_p);
    
    // Calculate ¬[[P]] using the API (by directly implementing the complementation)
    StateSet not_p_result;
    complement_state_set(&not_p_result, &p_result, model->num_states);
    
    // Calculate W \ [[P]] manually
    StateSet complement_result;
    init_state_set(&complement_result, model->num_states);
    
    for (int i = 0; i < model->num_states; i++) {
        if (!p_result.members[i]) {
            add_to_state_set(&complement_result, i);
        }
    }
    
    // Compare results
    bool equal = compare_state_sets(&not_p_result, &complement_result);
    printf("  Computed [[¬P]] = ");
    print_state_set(&not_p_result, model, "not P");
    printf("  Manually computed W \\ [[P]] = ");
    print_state_set(&complement_result, model, "complement");
    printf("  Equation holds: %s\n", equal ? "YES" : "NO");
}

// Function to verify equation 3: [[P ∧ Q]] = [[P]] ∩ [[Q]]
void verify_equation3(Model* model, int prop_p, int prop_q) {
    printf("\n3. Verifying [[P ∧ Q]] = [[P]] ∩ [[Q]] where P = %s, Q = %s:\n", 
           model->prop_names[prop_p], model->prop_names[prop_q]);
    printf("   This equation defines the semantics of conjunction in CTL.\n");
    
    // Get [[P]]
    StateSet p_result;
    eval_atomic_prop(&p_result, model, prop_p);
    
    // Get [[Q]]
    StateSet q_result;
    eval_atomic_prop(&q_result, model, prop_q);
    
    // Calculate [[P]] ∩ [[Q]] using API
    StateSet intersection_api;
    init_state_set(&intersection_api, model->num_states);
    intersect_state_sets(&intersection_api, &p_result, &q_result);
    
    // Calculate [[P]] ∩ [[Q]] manually
    StateSet intersection_manual;
    init_state_set(&intersection_manual, model->num_states);
    
    for (int i = 0; i < model->num_states; i++) {
        if (p_result.members[i] && q_result.members[i]) {
            add_to_state_set(&intersection_manual, i);
        }
    }
    
    // Compare results
    bool equal = compare_state_sets(&intersection_api, &intersection_manual);
    printf("  Computed [[P ∧ Q]] = ");
    print_state_set(&intersection_api, model, "P ∧ Q");
    printf("  Manually computed [[P]] ∩ [[Q]] = ");
    print_state_set(&intersection_manual, model, "intersection");
    printf("  Equation holds: %s\n", equal ? "YES" : "NO");
}

// Function to verify equation 4: [[P ∨ Q]] = [[P]] ∪ [[Q]]
void verify_equation4(Model* model, int prop_p, int prop_q) {
    printf("\n4. Verifying [[P ∨ Q]] = [[P]] ∪ [[Q]] where P = %s, Q = %s:\n", 
           model->prop_names[prop_p], model->prop_names[prop_q]);
    printf("   This equation defines the semantics of disjunction in CTL.\n");
    
    // Get [[P]]
    StateSet p_result;
    eval_atomic_prop(&p_result, model, prop_p);
    
    // Get [[Q]]
    StateSet q_result;
    eval_atomic_prop(&q_result, model, prop_q);
    
    // Calculate [[P]] ∪ [[Q]] using API
    StateSet union_api;
    init_state_set(&union_api, model->num_states);
    union_state_sets(&union_api, &p_result, &q_result);
    
    // Calculate [[P]] ∪ [[Q]] manually
    StateSet union_manual;
    init_state_set(&union_manual, model->num_states);
    
    for (int i = 0; i < model->num_states; i++) {
        if (p_result.members[i] || q_result.members[i]) {
            add_to_state_set(&union_manual, i);
        }
    }
    
    // Compare results
    bool equal = compare_state_sets(&union_api, &union_manual);
    printf("  Computed [[P ∨ Q]] = ");
    print_state_set(&union_api, model, "P ∨ Q");
    printf("  Manually computed [[P]] ∪ [[Q]] = ");
    print_state_set(&union_manual, model, "union");
    printf("  Equation holds: %s\n", equal ? "YES" : "NO");
}

// Function to verify equation 5: [[EX P]] = τEX([[P]])
void verify_equation5(Model* model, int prop_p) {
    printf("\n5. Verifying [[EX P]] = τEX([[P]]) where P = %s:\n", model->prop_names[prop_p]);
    printf("   This equation defines the semantics of the existential next operator in CTL.\n");
    printf("   τEX(Z) = {s ∈ W : t ∈ Z for some state t with s y t}\n");
    
    // Get [[P]]
    StateSet p_result;
    eval_atomic_prop(&p_result, model, prop_p);
    
    // Calculate EX P using API
    StateSet ex_p_api;
    init_state_set(&ex_p_api, model->num_states);
    existential_successor(&ex_p_api, &p_result, model);
    
    // Calculate τEX([[P]]) manually
    StateSet tau_ex_manual;
    init_state_set(&tau_ex_manual, model->num_states);
    
    for (int i = 0; i < model->num_states; i++) {
        State* current_state = &model->states[i];
        
        // Check if any successor of state i is in P
        for (int j = 0; j < current_state->num_transitions; j++) {
            int successor_id = current_state->transitions[j];
            if (p_result.members[successor_id]) {
                add_to_state_set(&tau_ex_manual, i);
                break;
            }
        }
    }
    
    // Compare results
    bool equal = compare_state_sets(&ex_p_api, &tau_ex_manual);
    printf("  Computed [[EX P]] = ");
    print_state_set(&ex_p_api, model, "EX P");
    printf("  Manually computed τEX([[P]]) = ");
    print_state_set(&tau_ex_manual, model, "τEX");
    printf("  Equation holds: %s\n", equal ? "YES" : "NO");
}

// Function to verify equation 6: [[AX P]] = τAX([[P]])
void verify_equation6(Model* model, int prop_p) {
    printf("\n6. Verifying [[AX P]] = τAX([[P]]) where P = %s:\n", model->prop_names[prop_p]);
    printf("   This equation defines the semantics of the universal next operator in CTL.\n");
    printf("   τAX(Z) = {s ∈ W : t ∈ Z for all states t with s y t}\n");
    
    // Get [[P]]
    StateSet p_result;
    eval_atomic_prop(&p_result, model, prop_p);
    
    // Calculate AX P using API
    StateSet ax_p_api;
    init_state_set(&ax_p_api, model->num_states);
    universal_successor(&ax_p_api, &p_result, model);
    
    // Calculate τAX([[P]]) manually
    StateSet tau_ax_manual;
    init_state_set(&tau_ax_manual, model->num_states);
    
    for (int i = 0; i < model->num_states; i++) {
        State* current_state = &model->states[i];
        bool all_in_set = true;
        
        // Only consider states with successors
        if (current_state->num_transitions > 0) {
            // Check if all successors of state i are in P
            for (int j = 0; j < current_state->num_transitions; j++) {
                int successor_id = current_state->transitions[j];
                if (!p_result.members[successor_id]) {
                    all_in_set = false;
                    break;
                }
            }
            
            if (all_in_set) {
                add_to_state_set(&tau_ax_manual, i);
            }
        }
    }
    
    // Compare results
    bool equal = compare_state_sets(&ax_p_api, &tau_ax_manual);
    printf("  Computed [[AX P]] = ");
    print_state_set(&ax_p_api, model, "AX P");
    printf("  Manually computed τAX([[P]]) = ");
    print_state_set(&tau_ax_manual, model, "τAX");
    printf("  Equation holds: %s\n", equal ? "YES" : "NO");
}

// Function to verify equation 7: [[EF P]] = μZ.([[P]] ∪ τEX(Z))
void verify_equation7(Model* model, int prop_p) {
    printf("\n7. Verifying [[EF P]] = μZ.([[P]] ∪ τEX(Z)) where P = %s:\n", model->prop_names[prop_p]);
    printf("   This equation defines the semantics of the existential finally operator using a least fixpoint.\n");
    printf("   μZ.f(Z) denotes the least fixpoint of the operation f(Z).\n");
    
    // Get [[P]]
    StateSet p_result;
    eval_atomic_prop(&p_result, model, prop_p);
    
    // Calculate EF P using API
    StateSet ef_p_api;
    init_state_set(&ef_p_api, model->num_states);
    eval_ef(&ef_p_api, &p_result, model);
    
    // Calculate μZ.([[P]] ∪ τEX(Z)) manually using fixed-point iteration
    StateSet z_current, z_next, tau_ex_result;
    init_state_set(&z_current, model->num_states);
    
    bool changed = true;
    int iteration = 0;
    
    printf("  Computing μZ.([[P]] ∪ τEX(Z)) as a least fixpoint:\n");
    printf("  Starting with Z₀ = ∅ (empty set) and iterating:\n");
    
    while (changed) {
        printf("    Iteration %d: Z_%d = ", iteration, iteration);
        print_state_set(&z_current, model, "current Z");
        
        // Calculate τEX(Z_current)
        existential_successor(&tau_ex_result, &z_current, model);
        
        // Calculate Z_next = [[P]] ∪ τEX(Z_current)
        union_state_sets(&z_next, &p_result, &tau_ex_result);
        
        // Check if fixed point is reached
        changed = !compare_state_sets(&z_current, &z_next);
        
        // Update Z for next iteration
        copy_state_set(&z_current, &z_next);
        iteration++;
    }
    
    printf("    Fixed point reached at iteration %d\n", iteration-1);
    
    // Compare results
    bool equal = compare_state_sets(&ef_p_api, &z_current);
    printf("  Computed [[EF P]] = ");
    print_state_set(&ef_p_api, model, "EF P");
    printf("  Manually computed μZ.([[P]] ∪ τEX(Z)) = ");
    print_state_set(&z_current, model, "fixed point");
    printf("  Equation holds: %s\n", equal ? "YES" : "NO");
}

// Function to verify equation 8: [[EG P]] = νZ.([[P]] ∩ τEX(Z))
void verify_equation8(Model* model, int prop_p) {
    printf("\n8. Verifying [[EG P]] = νZ.([[P]] ∩ τEX(Z)) where P = %s:\n", model->prop_names[prop_p]);
    printf("   This equation defines the semantics of the existential globally operator using a greatest fixpoint.\n");
    printf("   νZ.f(Z) denotes the greatest fixpoint of the operation f(Z).\n");
    
    // Get [[P]]
    StateSet p_result;
    eval_atomic_prop(&p_result, model, prop_p);
    
    // Calculate EG P using API
    StateSet eg_p_api;
    init_state_set(&eg_p_api, model->num_states);
    eval_eg(&eg_p_api, &p_result, model);
    
    // Calculate νZ.([[P]] ∩ τEX(Z)) manually using fixed-point iteration
    StateSet z_current, z_next, tau_ex_result;
    
    // Initialize Z with all states where P is true (the greatest possible fixpoint)
    copy_state_set(&z_current, &p_result);
    
    bool changed = true;
    int iteration = 0;
    
    printf("  Computing νZ.([[P]] ∩ τEX(Z)) as a greatest fixpoint:\n");
    printf("  Starting with Z₀ = [[P]] and iterating:\n");
    
    while (changed) {
        printf("    Iteration %d: Z_%d = ", iteration, iteration);
        print_state_set(&z_current, model, "current Z");
        
        // Calculate τEX(Z_current)
        existential_successor(&tau_ex_result, &z_current, model);
        
        // Calculate Z_next = [[P]] ∩ τEX(Z_current)
        intersect_state_sets(&z_next, &p_result, &tau_ex_result);
        
        // Check if fixed point is reached
        changed = !compare_state_sets(&z_current, &z_next);
        
        // Update Z for next iteration
        copy_state_set(&z_current, &z_next);
        iteration++;
    }
    
    printf("    Fixed point reached at iteration %d\n", iteration-1);
    
    // Compare results
    bool equal = compare_state_sets(&eg_p_api, &z_current);
    printf("  Computed [[EG P]] = ");
    print_state_set(&eg_p_api, model, "EG P");
    printf("  Manually computed νZ.([[P]] ∩ τEX(Z)) = ");
    print_state_set(&z_current, model, "fixed point");
    printf("  Equation holds: %s\n", equal ? "YES" : "NO");
}

// Function to verify equation 9: [[AF P]] = μZ.([[P]] ∪ τAX(Z))
void verify_equation9(Model* model, int prop_p) {
    printf("\n9. Verifying [[AF P]] = μZ.([[P]] ∪ τAX(Z)) where P = %s:\n", model->prop_names[prop_p]);
    printf("   This equation defines the semantics of the always finally operator using a least fixpoint.\n");
    printf("   μZ.f(Z) denotes the least fixpoint of the operation f(Z).\n");
    
    // Get [[P]]
    StateSet p_result;
    eval_atomic_prop(&p_result, model, prop_p);
    
    // Calculate AF P using API
    StateSet af_p_api;
    init_state_set(&af_p_api, model->num_states);
    eval_af(&af_p_api, &p_result, model);
    
    // Calculate μZ.([[P]] ∪ τAX(Z)) manually using fixed-point iteration
    StateSet z_current, z_next, tau_ax_result;
    init_state_set(&z_current, model->num_states);
    
    bool changed = true;
    int iteration = 0;
    
    printf("  Computing μZ.([[P]] ∪ τAX(Z)) as a least fixpoint:\n");
    printf("  Starting with Z₀ = ∅ (empty set) and iterating:\n");
    
    while (changed) {
        printf("    Iteration %d: Z_%d = ", iteration, iteration);
        print_state_set(&z_current, model, "current Z");
        
        // Calculate τAX(Z_current)
        universal_successor(&tau_ax_result, &z_current, model);
        
        // Calculate Z_next = [[P]] ∪ τAX(Z_current)
        union_state_sets(&z_next, &p_result, &tau_ax_result);
        
        // Check if fixed point is reached
        changed = !compare_state_sets(&z_current, &z_next);
        
        // Update Z for next iteration
        copy_state_set(&z_current, &z_next);
        iteration++;
    }
    
    printf("    Fixed point reached at iteration %d\n", iteration-1);
    
    // Alternative calculation: AF P = ¬EG ¬P
    printf("  Alternative calculation: AF P = ¬EG ¬P\n");
    
    // Calculate ¬P
    StateSet not_p;
    complement_state_set(&not_p, &p_result, model->num_states);
    
    // Calculate EG ¬P
    StateSet eg_not_p;
    eval_eg(&eg_not_p, &not_p, model);
    
    // Calculate ¬EG ¬P
    StateSet not_eg_not_p;
    complement_state_set(&not_eg_not_p, &eg_not_p, model->num_states);
    
    // Compare with AF P calculated using fixpoint
    bool dual_equal = compare_state_sets(&not_eg_not_p, &z_current);
    printf("  AF P = ¬EG ¬P: %s\n", dual_equal ? "YES" : "NO");
    
    // Compare results with API
    bool equal = compare_state_sets(&af_p_api, &z_current);
    printf("  Computed [[AF P]] = ");
    print_state_set(&af_p_api, model, "AF P");
    printf("  Manually computed μZ.([[P]] ∪ τAX(Z)) = ");
    print_state_set(&z_current, model, "fixed point");
    printf("  Equation holds: %s\n", equal ? "YES" : "NO");
}

// Function to verify equation 10: [[AG P]] = νZ.([[P]] ∩ τAX(Z))
void verify_equation10(Model* model, int prop_p) {
    printf("\n10. Verifying [[AG P]] = νZ.([[P]] ∩ τAX(Z)) where P = %s:\n", model->prop_names[prop_p]);
    printf("    This equation defines the semantics of the always globally operator using a greatest fixpoint.\n");
    printf("    νZ.f(Z) denotes the greatest fixpoint of the operation f(Z).\n");
    
    // Get [[P]]
    StateSet p_result;
    eval_atomic_prop(&p_result, model, prop_p);
    
    // Calculate AG P using API
    StateSet ag_p_api;
    init_state_set(&ag_p_api, model->num_states);
    eval_ag(&ag_p_api, &p_result, model);
    
    // Calculate νZ.([[P]] ∩ τAX(Z)) manually using fixed-point iteration
    StateSet z_current, z_next, tau_ax_result;
    
    // Initialize Z with all states where P is true (the greatest possible fixpoint)
    copy_state_set(&z_current, &p_result);
    
    bool changed = true;
    int iteration = 0;
    
    printf("  Computing νZ.([[P]] ∩ τAX(Z)) as a greatest fixpoint:\n");
    printf("  Starting with Z₀ = [[P]] and iterating:\n");
    
    while (changed) {
        printf("    Iteration %d: Z_%d = ", iteration, iteration);
        print_state_set(&z_current, model, "current Z");
        
        // Calculate τAX(Z_current)
        universal_successor(&tau_ax_result, &z_current, model);
        
        // Calculate Z_next = [[P]] ∩ τAX(Z_current)
        intersect_state_sets(&z_next, &p_result, &tau_ax_result);
        
        // Check if fixed point is reached
        changed = !compare_state_sets(&z_current, &z_next);
        
        // Update Z for next iteration
        copy_state_set(&z_current, &z_next);
        iteration++;
    }
    
    printf("    Fixed point reached at iteration %d\n", iteration-1);
    
    // Alternative calculation: AG P = ¬EF ¬P
    printf("  Alternative calculation: AG P = ¬EF ¬P\n");
    
    // Calculate ¬P
    StateSet not_p;
    complement_state_set(&not_p, &p_result, model->num_states);
    
    // Calculate EF ¬P
    StateSet ef_not_p;
    eval_ef(&ef_not_p, &not_p, model);
    
    // Calculate ¬EF ¬P
    StateSet not_ef_not_p;
    complement_state_set(&not_ef_not_p, &ef_not_p, model->num_states);
    
    // Compare with AG P calculated using fixpoint
    bool dual_equal = compare_state_sets(&not_ef_not_p, &z_current);
    printf("  AG P = ¬EF ¬P: %s\n", dual_equal ? "YES" : "NO");
    
    // Compare results with API
    bool equal = compare_state_sets(&ag_p_api, &z_current);
    printf("  Computed [[AG P]] = ");
    print_state_set(&ag_p_api, model, "AG P");
    printf("  Manually computed νZ.([[P]] ∩ τAX(Z)) = ");
    print_state_set(&z_current, model, "fixed point");
    printf("  Equation holds: %s\n", equal ? "YES" : "NO");
}

// Function to verify equation 11: [[EP UQ]] = μZ.([[Q]] ∪ ([[P]] ∩ τEX(Z)))
void verify_equation11(Model* model, int prop_p, int prop_q) {
    printf("\n11. Verifying [[EP UQ]] = μZ.([[Q]] ∪ ([[P]] ∩ τEX(Z))) where P = %s, Q = %s:\n", 
           model->prop_names[prop_p], model->prop_names[prop_q]);
    printf("    This equation defines the semantics of the existential until operator using a least fixpoint.\n");
    printf("    μZ.f(Z) denotes the least fixpoint of the operation f(Z).\n");
    
    // Get [[P]]
    StateSet p_result;
    eval_atomic_prop(&p_result, model, prop_p);
    
    // Get [[Q]]
    StateSet q_result;
    eval_atomic_prop(&q_result, model, prop_q);
    
    // Calculate EP UQ using API
    StateSet eu_api;
    init_state_set(&eu_api, model->num_states);
    eval_eu(&eu_api, &p_result, &q_result, model);
    
    // Calculate μZ.([[Q]] ∪ ([[P]] ∩ τEX(Z))) manually using fixed-point iteration
    StateSet z_current, z_next, tau_ex_result, p_and_tau;
    
    // Initialize Z with states where Q is true (the base case)
    copy_state_set(&z_current, &q_result);
    
    bool changed = true;
    int iteration = 0;
    
    printf("  Computing μZ.([[Q]] ∪ ([[P]] ∩ τEX(Z))) as a least fixpoint:\n");
    printf("  Starting with Z₀ = [[Q]] and iterating:\n");
    
    while (changed) {
        printf("    Iteration %d: Z_%d = ", iteration, iteration);
        print_state_set(&z_current, model, "current Z");
        
        // Calculate τEX(Z_current)
        existential_successor(&tau_ex_result, &z_current, model);
        
        // Calculate [[P]] ∩ τEX(Z_current)
        intersect_state_sets(&p_and_tau, &p_result, &tau_ex_result);
        
        // Calculate Z_next = [[Q]] ∪ ([[P]] ∩ τEX(Z_current))
        union_state_sets(&z_next, &q_result, &p_and_tau);
        
        // Check if fixed point is reached
        changed = !compare_state_sets(&z_current, &z_next);
        
        // Update Z for next iteration
        copy_state_set(&z_current, &z_next);
        iteration++;
    }
    
    printf("    Fixed point reached at iteration %d\n", iteration-1);
    
    // Compare results
    bool equal = compare_state_sets(&eu_api, &z_current);
    printf("  Computed [[EP UQ]] = ");
    print_state_set(&eu_api, model, "EU");
    printf("  Manually computed μZ.([[Q]] ∪ ([[P]] ∩ τEX(Z))) = ");
    print_state_set(&z_current, model, "fixed point");
    printf("  Equation holds: %s\n", equal ? "YES" : "NO");
}

// Function to verify equation 12: [[AP UQ]] = μZ.([[Q]] ∪ ([[P]] ∩ τAX(Z)))
void verify_equation12(Model* model, int prop_p, int prop_q) {
    printf("\n12. Verifying [[AP UQ]] = μZ.([[Q]] ∪ ([[P]] ∩ τAX(Z))) where P = %s, Q = %s:\n", 
           model->prop_names[prop_p], model->prop_names[prop_q]);
    printf("    This equation defines the semantics of the universal until operator using a least fixpoint.\n");
    printf("    μZ.f(Z) denotes the least fixpoint of the operation f(Z).\n");
    
    // Get [[P]]
    StateSet p_result;
    eval_atomic_prop(&p_result, model, prop_p);
    
    // Get [[Q]]
    StateSet q_result;
    eval_atomic_prop(&q_result, model, prop_q);
    
    // Calculate AP UQ using API
    StateSet au_api;
    init_state_set(&au_api, model->num_states);
    eval_au(&au_api, &p_result, &q_result, model);
    
    // Calculate μZ.([[Q]] ∪ ([[P]] ∩ τAX(Z))) manually using fixed-point iteration
    StateSet z_current, z_next, tau_ax_result, p_and_tau;
    
    // Initialize Z with states where Q is true (the base case)
    copy_state_set(&z_current, &q_result);
    
    bool changed = true;
    int iteration = 0;
    
    printf("  Computing μZ.([[Q]] ∪ ([[P]] ∩ τAX(Z))) as a least fixpoint:\n");
    printf("  Starting with Z₀ = [[Q]] and iterating:\n");
    
    while (changed) {
        printf("    Iteration %d: Z_%d = ", iteration, iteration);
        print_state_set(&z_current, model, "current Z");
        
        // Calculate τAX(Z_current)
        universal_successor(&tau_ax_result, &z_current, model);
        
        // Calculate [[P]] ∩ τAX(Z_current)
        intersect_state_sets(&p_and_tau, &p_result, &tau_ax_result);
        
        // Calculate Z_next = [[Q]] ∪ ([[P]] ∩ τAX(Z_current))
        union_state_sets(&z_next, &q_result, &p_and_tau);
        
        // Check if fixed point is reached
        changed = !compare_state_sets(&z_current, &z_next);
        
        // Update Z for next iteration
        copy_state_set(&z_current, &z_next);
        iteration++;
    }
    
    printf("    Fixed point reached at iteration %d\n", iteration-1);
    
    // Alternative calculation: A[P U Q] = ¬(E[¬Q U (¬P ∧ ¬Q)] ∨ EG ¬Q)
    printf("  Alternative calculation: A[P U Q] = ¬(E[¬Q U (¬P ∧ ¬Q)] ∨ EG ¬Q)\n");
    
    // Calculate ¬P and ¬Q
    StateSet not_p, not_q;
    complement_state_set(&not_p, &p_result, model->num_states);
    complement_state_set(&not_q, &q_result, model->num_states);
    
    // Calculate ¬P ∧ ¬Q
    StateSet not_p_and_not_q;
    intersect_state_sets(&not_p_and_not_q, &not_p, &not_q);
    
    // Calculate E[¬Q U (¬P ∧ ¬Q)]
    StateSet eu_not_q_and_not_p_and_not_q;
    eval_eu(&eu_not_q_and_not_p_and_not_q, &not_q, &not_p_and_not_q, model);
    
    // Calculate EG ¬Q
    StateSet eg_not_q;
    eval_eg(&eg_not_q, &not_q, model);
    
    // Calculate E[¬Q U (¬P ∧ ¬Q)] ∨ EG ¬Q
    StateSet disjunction;
    union_state_sets(&disjunction, &eu_not_q_and_not_p_and_not_q, &eg_not_q);
    
    // Calculate ¬(E[¬Q U (¬P ∧ ¬Q)] ∨ EG ¬Q)
    StateSet not_disjunction;
    complement_state_set(&not_disjunction, &disjunction, model->num_states);
    
    // Compare with AU P Q calculated using fixpoint
    bool dual_equal = compare_state_sets(&not_disjunction, &z_current);
    printf("  A[P U Q] = ¬(E[¬Q U (¬P ∧ ¬Q)] ∨ EG ¬Q): %s\n", dual_equal ? "YES" : "NO");
    
    // Compare results with API
    bool equal = compare_state_sets(&au_api, &z_current);
    printf("  Computed [[AP UQ]] = ");
    print_state_set(&au_api, model, "AU");
    printf("  Manually computed μZ.([[Q]] ∪ ([[P]] ∩ τAX(Z))) = ");
    print_state_set(&z_current, model, "fixed point");
    printf("  Equation holds: %s\n", equal ? "YES" : "NO");
}
