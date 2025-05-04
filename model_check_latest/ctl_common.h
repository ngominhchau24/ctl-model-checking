// ctl_common.h - Common definitions for CTL Verifier and Visualizer
#ifndef CTL_COMMON_H
#define CTL_COMMON_H

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
void set_proposition_false(Model* model, int state_id, int prop_id);
int get_proposition_id(Model* model, const char* prop_name);

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

// Helper functions to access model internals
int get_num_states(Model* model);
int get_num_props(Model* model);
char* get_prop_name(Model* model, int prop_id);
int get_num_transitions(Model* model, int state_id);
int get_transition(Model* model, int state_id, int transition_index);
bool is_prop_true(Model* model, int state_id, int prop_id);

// Equation verification functions
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

// Utility functions
void print_model_info(Model* model);
void print_state_set(StateSet* set, Model* model, const char* label);

// Create a mutex model
Model* create_mutex_model();

#endif /* CTL_COMMON_H */// ctl_common.h - Common definitions for CTL Verifier and Visualizer
#ifndef CTL_COMMON_H
#define CTL_COMMON_H

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

// Basic CTL operators
void eval_atomic_prop(StateSet* result, Model* model, int prop_id);

// Helper functions to access model internals
int get_num_states(Model* model);
int get_num_props(Model* model);
char* get_prop_name(Model* model, int prop_id);
int get_num_transitions(Model* model, int state_id);
int get_transition(Model* model, int state_id, int transition_index);
bool is_prop_true(Model* model, int state_id, int prop_id);

// Create a mutex model
Model* create_mutex_model();

#endif /* CTL_COMMON_H */
