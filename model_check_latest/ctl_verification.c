// ctl_verification.c - Implementation of CTL verification functions
#include "ctl_common.h"
#include <stdio.h>
#include <stdlib.h>

/* Equation Verification Functions */

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
    
    // Calculate ¬[[P]]
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
    
    // Calculate [[EX P]] using existential_successor
    StateSet ex_p_api;
    init_state_set(&ex_p_api, model->num_states);
    existential_successor(&ex_p_api, &p_result, model);
    
    // Calculate τEX([[P]]) manually
    StateSet tau_ex_manual;
    init_state_set(&tau_ex_manual, model->num_states);
    
    printf("  Computing τEX manually:\n");
    
    for (int i = 0; i < model->num_states; i++) {
        State* current_state = &model->states[i];
        
        // Check if any successor of state i is in P
        for (int j = 0; j < current_state->num_transitions; j++) {
            int successor_id = current_state->transitions[j];
            if (is_in_state_set(&p_result, successor_id)) {
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
    
    // Calculate [[AX P]] using universal_successor
    StateSet ax_p_api;
    init_state_set(&ax_p_api, model->num_states);
    universal_successor(&ax_p_api, &p_result, model);
    
    // Calculate τAX([[P]]) manually
    StateSet tau_ax_manual;
    init_state_set(&tau_ax_manual, model->num_states);
    
    printf("  Computing τAX manually:\n");
    
    for (int i = 0; i < model->num_states; i++) {
        State* current_state = &model->states[i];
        bool all_in_set = true;
        
        // Only consider states with successors
        if (current_state->num_transitions > 0) {
            // Check if all successors of state i are in P
            for (int j = 0; j < current_state->num_transitions; j++) {
                int successor_id = current_state->transitions[j];
                if (!is_in_state_set(&p_result, successor_id)) {
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
    
    // Calculate [[EF P]] using eval_ef
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
    
    // Calculate [[EG P]] using eval_eg
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
    
    // Calculate [[AF P]] using eval_af
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
    
    // Calculate [[AG P]] using eval_ag
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
    
    // Compare results
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
    
    // Calculate [[EP UQ]] using eval_eu
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
        
        // Calculate Z_next = [[Q]] ∪ ([[P]] ∩ τAX(Z_current))
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
    
    // Calculate [[AP UQ]] using eval_au
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
        
        // Calculate Z_next = [[Q// ctl_verification.c - Implementation of CTL verification functions
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
