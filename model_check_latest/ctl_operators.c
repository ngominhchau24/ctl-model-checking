// ctl_operators.c - Implementation of CTL operators
#include "ctl_common.h"
#include <stdio.h>
#include <stdlib.h>

/* CTL Operators */

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
