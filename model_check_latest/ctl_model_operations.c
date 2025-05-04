// ctl_model_operations.c - Implementation of model operations for CTL
#include "ctl_common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Model Operations */

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

// Set a proposition to false for a state
void set_proposition_false(Model* model, int state_id, int prop_id) {
    if (state_id < 0 || state_id >= model->num_states || 
        prop_id < 0 || prop_id >= model->num_props) {
        fprintf(stderr, "Error: Invalid state or proposition ID\n");
        return;
    }
    
    model->states[state_id].atomic_props[prop_id] = false;
}

// Get the ID of a proposition by name
int get_proposition_id(Model* model, const char* prop_name) {
    for (int i = 0; i < model->num_props; i++) {
        if (strcmp(model->prop_names[i], prop_name) == 0) {
            return i;
        }
    }
    
    return -1;  // Proposition not found
}

/* State Set Operations */

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

/* Helper Functions */

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

// Helper functions to access model internals
int get_num_states(Model* model) {
    return model->num_states;
}

int get_num_props(Model* model) {
    return model->num_props;
}

char* get_prop_name(Model* model, int prop_id) {
    if (prop_id < 0 || prop_id >= model->num_props) {
        return "Invalid";
    }
    return model->prop_names[prop_id];
}

int get_num_transitions(Model* model, int state_id) {
    if (state_id < 0 || state_id >= model->num_states) {
        return 0;
    }
    return model->states[state_id].num_transitions;
}

int get_transition(Model* model, int state_id, int transition_index) {
    if (state_id < 0 || state_id >= model->num_states || 
        transition_index < 0 || transition_index >= model->states[state_id].num_transitions) {
        return -1;
    }
    return model->states[state_id].transitions[transition_index];
}

bool is_prop_true(Model* model, int state_id, int prop_id) {
    if (state_id < 0 || state_id >= model->num_states || 
        prop_id < 0 || prop_id >= model->num_props) {
        return false;
    }
    return model->states[state_id].atomic_props[prop_id];
}

/* Model Creation */

// Create a model representing a mutual exclusion algorithm
Model* create_mutex_model() {
    Model* model = create_model();
    
    // Define states - following the state diagram in the lecture notes
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
