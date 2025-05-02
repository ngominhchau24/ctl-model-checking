// Function to manually calculate existential successor (τEX) for demonstration
void manual_existential_successor(state_set* result, state_set* set, model* model) {
  ctl_init_state_set(result, model->num_states);
  
  printf("  Computing τEX manually for set ");
  print_state_set(set, model, "Z");
  printf("  τEX(Z) = {s ∈ W : t ∈ Z for some state t with s y t}\n");
  
  for (int i = 0; i < model->num_states; i++) {
      state* current_state = &model->states[i];
      
      printf("    Checking state %d. Successors: {", i);
      for (int j = 0; j < current_state->num_transitions; j++) {
          if (j > 0) printf(", ");
          printf("%d", current_state->transitions[j]);
      }
      printf("}\n");
      
      // Check if any successor of state i is in the input set
      bool found = false;
      for (int j = 0; j < current_state->num_transitions; j++) {
          int successor_id = current_state->transitions[j];
          if (ctl_is_in_state_set(set, successor_id)) {
              found = true;
              printf("      Successor %d is in Z, so state %d is in τEX(Z)\n", successor_id, i);
              ctl_add_to_state_set(result, i);
              break;
          }
      }
      
      if (!found) {
          printf("      No successors in Z, so state %d is NOT in τEX(Z)\n", i);
      }
  }
}

// Function to verify equation 5: [[EX P]] = τEX([[P]])
void verify_equation5(model* model, const char* formula_p) {
  printf("\n5. Verifying [[EX P]] = τEX([[P]]) where P = %s:\n", formula_p);
  printf("   This equation defines the semantics of the existential next operator in CTL.\n");
  printf("   τEX(Z) = {s ∈ W : t ∈ Z for some state t with s y t}\n");
  
  // Get [[P]]
  state_set p_result;
  ctl_parse_formula(formula_p, &p_result, model);
  
  // Get [[EX P]] using API
  char ex_p_formula[MAX_FORMULA_LEN];
  snprintf(ex_p_formula, MAX_FORMULA_LEN, "EX(%s)", formula_p);
  state_set ex_p_result;
  ctl_parse_formula(ex_p_formula, &ex_p_result, model);
  
  // Calculate τEX([[P]]) manually
  state_set tau_ex_result;
  manual_existential_successor(&tau_ex_result, &p_result, model);
  
  // Compare results
  bool equal = compare_state_sets(&ex_p_result, &tau_ex_result);
  printf("  CTL API result [[EX P]] = ");
  print_state_set(&ex_p_result, model, ex_p_formula);
  printf("  Manually computed τEX([[P]]) = ");
  print_state_set(&tau_ex_result, model, "τEX");
  printf("  Equation holds: %s\n", equal ? "YES" : "NO");
}