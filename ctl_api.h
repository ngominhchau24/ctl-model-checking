#ifndef CTL_API_H
#define CTL_API_H

#include <stdbool.h>

// Maximum number of states and atomic propositions
#define MAX_STATES 1000
#define MAX_PROPS 100
#define MAX_TRANSITIONS 50
#define MAX_FORMULA_LEN 1024
#define MAX_TOKEN_LEN 64

// Structure for representing a state
typedef struct {
    int id;
    bool atomic_props[MAX_PROPS];  // Truth values of atomic propositions
    int num_transitions;
    int transitions[MAX_TRANSITIONS];  // IDs of successor states
} state;

// Structure for representing a Kripke structure (model)
typedef struct {
    state states[MAX_STATES];
    int num_states;
    int num_props;
    char* prop_names[MAX_PROPS];  // Names of atomic propositions
} model;

// Structure for representing a set of states
typedef struct {
    bool members[MAX_STATES];
    int size;
} state_set;

// API Functions

// Model creation and manipulation
model* ctl_create_model();
void ctl_free_model(model* model);
int ctl_add_state(model* model);
bool ctl_add_transition(model* model, int from_state, int to_state);
int ctl_add_proposition(model* model, const char* prop_name);
void ctl_set_proposition_true(model* model, int state_id, int prop_id);
void ctl_set_proposition_false(model* model, int state_id, int prop_id);
int ctl_get_proposition_id(model* model, const char* prop_name);

// State set operations
void ctl_init_state_set(state_set* set, int num_states);
void ctl_add_to_state_set(state_set* set, int state_id);
bool ctl_is_in_state_set(state_set* set, int state_id);
void ctl_complement_state_set(state_set* result, state_set* set, int num_states);
void ctl_intersect_state_sets(state_set* result, state_set* set1, state_set* set2);
void ctl_union_state_sets(state_set* result, state_set* set1, state_set* set2);
void ctl_copy_state_set(state_set* dest, state_set* src);

// Basic CTL operators
void ctl_eval_atomic_prop(state_set* result, model* model, int prop_id);
void ctl_eval_atomic_prop_by_name(state_set* result, model* model, const char* prop_name);
void ctl_existential_successor(state_set* result, state_set* set, model* model);  // EX
void ctl_universal_successor(state_set* result, state_set* set, model* model);    // AX
void ctl_eval_ef(state_set* result, state_set* prop_p, model* model);             // EF
void ctl_eval_af(state_set* result, state_set* prop_p, model* model);             // AF
void ctl_eval_eg(state_set* result, state_set* prop_p, model* model);             // EG
void ctl_eval_ag(state_set* result, state_set* prop_p, model* model);             // AG
void ctl_eval_eu(state_set* result, state_set* prop_p, state_set* prop_q, model* model); // EU
void ctl_eval_au(state_set* result, state_set* prop_p, state_set* prop_q, model* model); // AU

// Formula parsing
void ctl_parse_formula(const char* formula, state_set* result, model* model);
bool ctl_check_formula_at_state(const char* formula, model* model, int state_id);
bool ctl_check_state_in_set(int state_id, state_set* set);

// Utility functions
void ctl_print_model_info(model* model);
void ctl_print_state_set(state_set* set, model* model);
void ctl_print_state_info(model* model, int state_id);

#endif /* CTL_API_H */