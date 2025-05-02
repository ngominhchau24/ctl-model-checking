#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ctl_api.h"

// Enum for token types in the formula parser
typedef enum {
    TOKEN_PROP,    // Atomic proposition
    TOKEN_NOT,     // Negation (¬ or !)
    TOKEN_AND,     // Conjunction (∧ or &)
    TOKEN_OR,      // Disjunction (∨ or |)
    TOKEN_EX,      // Exists Next
    TOKEN_EF,      // Exists Finally
    TOKEN_EG,      // Exists Globally
    TOKEN_EU,      // Exists Until
    TOKEN_AX,      // Always Next
    TOKEN_AF,      // Always Finally
    TOKEN_AG,      // Always Globally
    TOKEN_AU,      // Always Until
    TOKEN_LPAREN,  // Left parenthesis
    TOKEN_RPAREN,  // Right parenthesis
    TOKEN_COMMA,   // Comma
    TOKEN_EOF      // End of formula
} token_type;

// Structure for a token
typedef struct {
    token_type type;
    char value[MAX_TOKEN_LEN];
} token;

// Structure for a lexer
typedef struct {
    char formula[MAX_FORMULA_LEN];
    int position;
} lexer;

// Forward declarations for recursive descent parser
state_set* parse_expression(lexer* lexer, model* model);
state_set* parse_term(lexer* lexer, model* model);
state_set* parse_factor(lexer* lexer, model* model);

// Initialize the lexer with a formula
void init_lexer(lexer* lexer, const char* formula) {
    strncpy(lexer->formula, formula, MAX_FORMULA_LEN - 1);
    lexer->formula[MAX_FORMULA_LEN - 1] = '\0';
    lexer->position = 0;
}

// Skip whitespace in the formula
void skip_whitespace(lexer* lexer) {
    while (lexer->formula[lexer->position] != '\0' && 
           isspace(lexer->formula[lexer->position])) {
        lexer->position++;
    }
}

// Get the next token from the formula
token get_next_token(lexer* lexer) {
    token token;
    
    skip_whitespace(lexer);
    
    if (lexer->formula[lexer->position] == '\0') {
        token.type = TOKEN_EOF;
        token.value[0] = '\0';
        return token;
    }
    
    // Check for special tokens
    switch (lexer->formula[lexer->position]) {
        case '!':
        case '~':
            token.type = TOKEN_NOT;
            token.value[0] = lexer->formula[lexer->position];
            token.value[1] = '\0';
            lexer->position++;
            return token;
        case '&':
        case '^':
            token.type = TOKEN_AND;
            token.value[0] = lexer->formula[lexer->position];
            token.value[1] = '\0';
            lexer->position++;
            return token;
        case '|':
        case '+':
            token.type = TOKEN_OR;
            token.value[0] = lexer->formula[lexer->position];
            token.value[1] = '\0';
            lexer->position++;
            return token;
        case '(':
            token.type = TOKEN_LPAREN;
            token.value[0] = '(';
            token.value[1] = '\0';
            lexer->position++;
            return token;
        case ')':
            token.type = TOKEN_RPAREN;
            token.value[0] = ')';
            token.value[1] = '\0';
            lexer->position++;
            return token;
        case ',':
            token.type = TOKEN_COMMA;
            token.value[0] = ',';
            token.value[1] = '\0';
            lexer->position++;
            return token;
    }
    
    // Check for CTL operators (EX, EF, EG, EU, AX, AF, AG, AU)
    if (lexer->formula[lexer->position] == 'E' || lexer->formula[lexer->position] == 'A') {
        char first_char = lexer->formula[lexer->position];
        lexer->position++;
        
        if (lexer->formula[lexer->position] == 'X') {
            token.type = (first_char == 'E') ? TOKEN_EX : TOKEN_AX;
            sprintf(token.value, "%cX", first_char);
            lexer->position++;
            return token;
        } else if (lexer->formula[lexer->position] == 'F') {
            token.type = (first_char == 'E') ? TOKEN_EF : TOKEN_AF;
            sprintf(token.value, "%cF", first_char);
            lexer->position++;
            return token;
        } else if (lexer->formula[lexer->position] == 'G') {
            token.type = (first_char == 'E') ? TOKEN_EG : TOKEN_AG;
            sprintf(token.value, "%cG", first_char);
            lexer->position++;
            return token;
        } else if (lexer->formula[lexer->position] == 'U') {
            token.type = (first_char == 'E') ? TOKEN_EU : TOKEN_AU;
            sprintf(token.value, "%cU", first_char);
            lexer->position++;
            return token;
        } else {
            // Invalid operator, treat as an atomic proposition
            lexer->position--;
        }
    }
    
    // Atomic proposition
    token.type = TOKEN_PROP;
    int i = 0;
    while (lexer->formula[lexer->position] != '\0' && 
           !isspace(lexer->formula[lexer->position]) &&
           lexer->formula[lexer->position] != '(' && 
           lexer->formula[lexer->position] != ')' &&
           lexer->formula[lexer->position] != ',' &&
           lexer->formula[lexer->position] != '&' &&
           lexer->formula[lexer->position] != '|' &&
           lexer->formula[lexer->position] != '!' &&
           lexer->formula[lexer->position] != '~') {
        
        token.value[i++] = lexer->formula[lexer->position++];
        if (i >= MAX_TOKEN_LEN - 1) break;
    }
    token.value[i] = '\0';
    return token;
}

/* State Set Operations */

// Initialize an empty state set
void ctl_init_state_set(state_set* set, int num_states) {
    set->size = 0;
    for (int i = 0; i < num_states; i++) {
        set->members[i] = false;
    }
}

// Add a state to a state set
void ctl_add_to_state_set(state_set* set, int state_id) {
    if (!set->members[state_id]) {
        set->members[state_id] = true;
        set->size++;
    }
}

// Check if a state is in a state set
bool ctl_is_in_state_set(state_set* set, int state_id) {
    return set->members[state_id];
}

// Set complement (W \ X)
void ctl_complement_state_set(state_set* result, state_set* set, int num_states) {
    ctl_init_state_set(result, num_states);
    for (int i = 0; i < num_states; i++) {
        if (!set->members[i]) {
            ctl_add_to_state_set(result, i);
        }
    }
}

// Set intersection (X ∩ Y)
void ctl_intersect_state_sets(state_set* result, state_set* set1, state_set* set2) {
    result->size = 0;
    for (int i = 0; i < MAX_STATES; i++) {
        result->members[i] = set1->members[i] && set2->members[i];
        if (result->members[i]) {
            result->size++;
        }
    }
}

// Set union (X ∪ Y)
void ctl_union_state_sets(state_set* result, state_set* set1, state_set* set2) {
    result->size = 0;
    for (int i = 0; i < MAX_STATES; i++) {
        result->members[i] = set1->members[i] || set2->members[i];
        if (result->members[i]) {
            result->size++;
        }
    }
}

// Copy state set
void ctl_copy_state_set(state_set* dest, state_set* src) {
    dest->size = src->size;
    for (int i = 0; i < MAX_STATES; i++) {
        dest->members[i] = src->members[i];
    }
}

/* Model Operations */

// Create a new empty model
model* ctl_create_model() {
    model* m = (model*)malloc(sizeof(model));
    if (m == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for model\n");
        return NULL;
    }
    
    m->num_states = 0;
    m->num_props = 0;
    
    return m;
}

// Free a model
void ctl_free_model(model* model) {
    if (model == NULL) return;
    
    for (int i = 0; i < model->num_props; i++) {
        free(model->prop_names[i]);
    }
    
    free(model);
}

// Add a new state to the model
int ctl_add_state(model* model) {
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
bool ctl_add_transition(model* model, int from_state, int to_state) {
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
int ctl_add_proposition(model* model, const char* prop_name) {
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
void ctl_set_proposition_true(model* model, int state_id, int prop_id) {
    if (state_id < 0 || state_id >= model->num_states || 
        prop_id < 0 || prop_id >= model->num_props) {
        fprintf(stderr, "Error: Invalid state or proposition ID\n");
        return;
    }
    
    model->states[state_id].atomic_props[prop_id] = true;
}

// Set a proposition to false for a state
void ctl_set_proposition_false(model* model, int state_id, int prop_id) {
    if (state_id < 0 || state_id >= model->num_states || 
        prop_id < 0 || prop_id >= model->num_props) {
        fprintf(stderr, "Error: Invalid state or proposition ID\n");
        return;
    }
    
    model->states[state_id].atomic_props[prop_id] = false;
}

// Get the ID of a proposition by name
int ctl_get_proposition_id(model* model, const char* prop_name) {
    for (int i = 0; i < model->num_props; i++) {
        if (strcmp(model->prop_names[i], prop_name) == 0) {
            return i;
        }
    }
    
    return -1;  // Proposition not found
}

/* CTL Operators */

// Evaluate an atomic proposition
void ctl_eval_atomic_prop(state_set* result, model* model, int prop_id) {
    ctl_init_state_set(result, model->num_states);
    
    if (prop_id < 0 || prop_id >= model->num_props) {
        fprintf(stderr, "Error: Invalid proposition ID %d\n", prop_id);
        return;
    }
    
    for (int i = 0; i < model->num_states; i++) {
        if (model->states[i].atomic_props[prop_id]) {
            ctl_add_to_state_set(result, i);
        }
    }
}

// Evaluate an atomic proposition by name
void ctl_eval_atomic_prop_by_name(state_set* result, model* model, const char* prop_name) {
    int prop_id = ctl_get_proposition_id(model, prop_name);
    if (prop_id >= 0) {
        ctl_eval_atomic_prop(result, model, prop_id);
    } else {
        ctl_init_state_set(result, model->num_states);
        fprintf(stderr, "Error: Proposition '%s' not found\n", prop_name);
    }
}

// Find states that have a successor in the given set (EX)
void ctl_existential_successor(state_set* result, state_set* set, model* model) {
    ctl_init_state_set(result, model->num_states);
    
    for (int i = 0; i < model->num_states; i++) {
        state* current_state = &model->states[i];
        
        // Check if any successor of state i is in the input set
        for (int j = 0; j < current_state->num_transitions; j++) {
            int successor_id = current_state->transitions[j];
            if (ctl_is_in_state_set(set, successor_id)) {
                ctl_add_to_state_set(result, i);
                break;
            }
        }
    }
}

// Find states such that all successors are in the given set (AX)
void ctl_universal_successor(state_set* result, state_set* set, model* model) {
    ctl_init_state_set(result, model->num_states);
    
    for (int i = 0; i < model->num_states; i++) {
        state* current_state = &model->states[i];
        bool all_in_set = true;
        
        // Check if all successors of state i are in the input set
        for (int j = 0; j < current_state->num_transitions; j++) {
            int successor_id = current_state->transitions[j];
            if (!ctl_is_in_state_set(set, successor_id)) {
                all_in_set = false;
                break;
            }
        }
        
        if (all_in_set && current_state->num_transitions > 0) {
            ctl_add_to_state_set(result, i);
        }
    }
}

// Evaluate EF P (exists finally) - states that can reach a state where P is true
void ctl_eval_ef(state_set* result, state_set* prop_p, model* model) {
    state_set current, next;
    
    // Initialize with states where P is true
    ctl_init_state_set(&current, model->num_states);
    ctl_copy_state_set(&current, prop_p);
    
    bool changed = true;
    while (changed) {
        changed = false;
        
        // Find states that can reach the current set in one step
        ctl_existential_successor(&next, &current, model);
        
        // Add these states to the current set
        for (int i = 0; i < model->num_states; i++) {
            if (next.members[i] && !current.members[i]) {
                ctl_add_to_state_set(&current, i);
                changed = true;
            }
        }
    }
    
    ctl_copy_state_set(result, &current);
}

// Evaluate EG P (exists globally) - states where there exists a path where P is always true
void ctl_eval_eg(state_set* result, state_set* prop_p, model* model) {
    state_set current;
    
    // Initialize with all states where P is true
    ctl_copy_state_set(&current, prop_p);
    
    bool changed = true;
    while (changed) {
        changed = false;
        
        for (int i = 0; i < model->num_states; i++) {
            if (current.members[i]) {
                bool has_valid_successor = false;
                state* current_state = &model->states[i];
                
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
    
    ctl_copy_state_set(result, &current);
}

// Evaluate AF P (always finally) - states where all paths eventually reach a state where P is true
void ctl_eval_af(state_set* result, state_set* prop_p, model* model) {
    state_set not_p, eg_not_p;
    
    // Calculate ¬P
    ctl_complement_state_set(&not_p, prop_p, model->num_states);
    
    // Calculate EG ¬P
    ctl_eval_eg(&eg_not_p, &not_p, model);
    
    // AF P = ¬EG ¬P
    ctl_complement_state_set(result, &eg_not_p, model->num_states);
}

// Evaluate AG P (always globally) - states where P is true in all future paths
void ctl_eval_ag(state_set* result, state_set* prop_p, model* model) {
    state_set not_p, ef_not_p;
    
    // Calculate ¬P
    ctl_complement_state_set(&not_p, prop_p, model->num_states);
    
    // Calculate EF ¬P
    ctl_eval_ef(&ef_not_p, &not_p, model);
    
    // AG P = ¬EF ¬P
    ctl_complement_state_set(result, &ef_not_p, model->num_states);
}

// Evaluate EU P Q (exists until) - states that can reach Q through P states
void ctl_eval_eu(state_set* result, state_set* prop_p, state_set* prop_q, model* model) {
    state_set current, next, temp;
    
    // Initialize with states where Q is true
    ctl_copy_state_set(&current, prop_q);
    
    bool changed = true;
    while (changed) {
        changed = false;
        
        // Find states that can reach the current set in one step and satisfy P
        ctl_existential_successor(&next, &current, model);
        ctl_intersect_state_sets(&temp, &next, prop_p);
        
        // Add these states to the current set
        for (int i = 0; i < model->num_states; i++) {
            if (temp.members[i] && !current.members[i]) {
                ctl_add_to_state_set(&current, i);
                changed = true;
            }
        }
    }
    
    ctl_copy_state_set(result, &current);
}

// Evaluate AU P Q (always until) - states where all paths reach Q through P states
void ctl_eval_au(state_set* result, state_set* prop_p, state_set* prop_q, model* model) {
    state_set not_p, not_q, eu_not_p_not_q, eg_not_q;
    
    // Calculate ¬P and ¬Q
    ctl_complement_state_set(&not_p, prop_p, model->num_states);
    ctl_complement_state_set(&not_q, prop_q, model->num_states);
    
    // Calculate E[¬P U (¬P ∧ ¬Q)]
    ctl_intersect_state_sets(&eu_not_p_not_q, &not_p, &not_q);
    ctl_eval_eu(&eu_not_p_not_q, &not_p, &eu_not_p_not_q, model);
    
    // Calculate EG ¬Q
    ctl_eval_eg(&eg_not_q, &not_q, model);
    
    // A[P U Q] = ¬(E[¬P U (¬P ∧ ¬Q)] ∨ EG ¬Q)
    state_set temp;
    ctl_union_state_sets(&temp, &eu_not_p_not_q, &eg_not_q);
    ctl_complement_state_set(result, &temp, model->num_states);
}

// Parse a formula
void ctl_parse_formula(const char* formula, state_set* result, model* model) {
    lexer lex;
    init_lexer(&lex, formula);
    
    state_set* temp_result = parse_expression(&lex, model);
    
    // Copy the result to the output parameter
    ctl_copy_state_set(result, temp_result);
    
    // Free the temporary result
    free(temp_result);
    
    // Check if we've consumed the whole formula
    token current_token = get_next_token(&lex);
    if (current_token.type != TOKEN_EOF) {
        fprintf(stderr, "Warning: Unconsumed input at the end of formula: %s\n", current_token.value);
    }
}

// Parse an expression (lowest precedence: OR)
state_set* parse_expression(lexer* lexer, model* model) {
    state_set* left = parse_term(lexer, model);
    token current_token = get_next_token(lexer);
    
    while (current_token.type == TOKEN_OR) {
        state_set* right = parse_term(lexer, model);
        state_set* result = malloc(sizeof(state_set));
        ctl_init_state_set(result, model->num_states);
        ctl_union_state_sets(result, left, right);
        
        free(left);
        free(right);
        left = result;
        
        current_token = get_next_token(lexer);
    }
    
    // Put back the token that's not part of this expression
    lexer->position -= strlen(current_token.value);
    
    return left;
}

// Parse a term (AND has higher precedence than OR)
state_set* parse_term(lexer* lexer, model* model) {
    state_set* left = parse_factor(lexer, model);
    token current_token = get_next_token(lexer);
    
    while (current_token.type == TOKEN_AND) {
        state_set* right = parse_factor(lexer, model);
        state_set* result = malloc(sizeof(state_set));
        ctl_init_state_set(result, model->num_states);
        ctl_intersect_state_sets(result, left, right);
        
        free(left);
        free(right);
        left = result;
        
        current_token = get_next_token(lexer);
    }
    
    // Put back the token that's not part of this term
    lexer->position -= strlen(current_token.value);
    
    return left;
}

// Parse a factor (highest precedence: NOT, EX, AX, etc.)
state_set* parse_factor(lexer* lexer, model* model) {
    token current_token = get_next_token(lexer);
    
    if (current_token.type == TOKEN_LPAREN) {
        // Handle parenthesized expressions
        state_set* result = parse_expression(lexer, model);
        current_token = get_next_token(lexer);
        
        if (current_token.type != TOKEN_RPAREN) {
            fprintf(stderr, "Error: Expected closing parenthesis, got %s\n", current_token.value);
            free(result);
            exit(1);
        }
        
        return result;
    } else if (current_token.type == TOKEN_NOT) {
        // Handle negation
        state_set* operand = parse_factor(lexer, model);
        state_set* result = malloc(sizeof(state_set));
        ctl_init_state_set(result, model->num_states);
        ctl_complement_state_set(result, operand, model->num_states);
        
        free(operand);
        return result;
    } else if (current_token.type == TOKEN_EX || current_token.type == TOKEN_AX ||
               current_token.type == TOKEN_EF || current_token.type == TOKEN_AF ||
               current_token.type == TOKEN_EG || current_token.type == TOKEN_AG) {
        // Handle unary CTL operators
        state_set* operand = parse_factor(lexer, model);
        state_set* result = malloc(sizeof(state_set));
        ctl_init_state_set(result, model->num_states);
        
        switch (current_token.type) {
            case TOKEN_EX:
                ctl_existential_successor(result, operand, model);
                break;
            case TOKEN_AX:
                ctl_universal_successor(result, operand, model);
                break;
            case TOKEN_EF:
                ctl_eval_ef(result, operand, model);
                break;
            case TOKEN_AF:
                ctl_eval_af(result, operand, model);
                break;
            case TOKEN_EG:
                ctl_eval_eg(result, operand, model);
                break;
            case TOKEN_AG:
                ctl_eval_ag(result, operand, model);
                break;
            default:
                fprintf(stderr, "Error: Unhandled unary operator: %s\n", current_token.value);
                exit(1);
        }
        
        free(operand);
        return result;
    } else if (current_token.type == TOKEN_EU || current_token.type == TOKEN_AU) {
        // Handle binary CTL operators
        token op_token = current_token;
        
        // Check for opening parenthesis
        current_token = get_next_token(lexer);
        if (current_token.type != TOKEN_LPAREN) {
            fprintf(stderr, "Error: Expected opening parenthesis after %s, got %s\n", op_token.value, current_token.value);
            exit(1);
        }
        
        // Parse first operand
        state_set* left = parse_expression(lexer, model);
        
        // Check for comma
        current_token = get_next_token(lexer);
        if (current_token.type != TOKEN_COMMA) {
            fprintf(stderr, "Error: Expected comma in %s operator, got %s\n", op_token.value, current_token.value);
            free(left);
            exit(1);
        }
        
        // Parse second operand
        state_set* right = parse_expression(lexer, model);
        
        // Check for closing parenthesis
        current_token = get_next_token(lexer);
        if (current_token.type != TOKEN_RPAREN) {
            fprintf(stderr, "Error: Expected closing parenthesis after %s operands, got %s\n", op_token.value, current_token.value);
            free(left);
            free(right);
            exit(1);
        }
        
        // Evaluate the binary operator
        state_set* result = malloc(sizeof(state_set));
        ctl_init_state_set(result, model->num_states);
        
        if (op_token.type == TOKEN_EU) {
            ctl_eval_eu(result, left, right, model);
        } else { // TOKEN_AU
            ctl_eval_au(result, left, right, model);
        }
        
        free(left);
        free(right);
        return result;
    } else if (current_token.type == TOKEN_PROP) {
        // Handle atomic propositions
        state_set* result = malloc(sizeof(state_set));
        ctl_init_state_set(result, model->num_states);
        ctl_eval_atomic_prop_by_name(result, model, current_token.value);
        return result;
    } else {
        fprintf(stderr, "Error: Unexpected token: %s\n", current_token.value);
        exit(1);
    }
}

// Check if a formula is satisfied at a specific state
bool ctl_check_formula_at_state(const char* formula, model* model, int state_id) {
    if (state_id < 0 || state_id >= model->num_states) {
        fprintf(stderr, "Error: Invalid state ID: %d\n", state_id);
        return false;
    }
    
    state_set result;
    ctl_parse_formula(formula, &result, model);
    
    return ctl_is_in_state_set(&result, state_id);
}

// Check if a state is in a state set
bool ctl_check_state_in_set(int state_id, state_set* set) {
  return ctl_is_in_state_set(set, state_id);
}

// Print information about a model
void ctl_print_model_info(model* model) {
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
      ctl_print_state_info(model, i);
  }
  printf("\n");
}

// Print information about a specific state
void ctl_print_state_info(model* model, int state_id) {
  if (state_id < 0 || state_id >= model->num_states) {
      fprintf(stderr, "Error: Invalid state ID: %d\n", state_id);
      return;
  }
  
  printf("State %d: ", state_id);
  
  // Print atomic propositions
  printf("(");
  for (int j = 0; j < model->num_props; j++) {
      if (j > 0) printf(", ");
      printf("%s=%s", model->prop_names[j], model->states[state_id].atomic_props[j] ? "true" : "false");
  }
  printf(") -> ");
  
  // Print transitions
  for (int j = 0; j < model->states[state_id].num_transitions; j++) {
      if (j > 0) printf(", ");
      printf("%d", model->states[state_id].transitions[j]);
  }
  printf("\n");
}

// Print the states in a state set
void ctl_print_state_set(state_set* set, model* model) {
  printf("{");
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