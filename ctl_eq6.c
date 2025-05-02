// Function to manually calculate universal successor (τAX) for demonstration
void manual_universal_successor(state_set* result, state_set* set, model* model) {
  ctl_init_state_set(result, model->num_states);
  
  printf("  Computing τAX manually for set ");
  print_state_set(set, model, "Z");
  printf("  τAX(Z) = {s ∈ W : t ∈ Z for all states t with s y t}\n");
  
  for (int i = 0; i < model->num_states; i++) {
      state* current_state = &model->states[i];
      
      printf("    Checking state %d. Successors: {", i);
      for (int j = 0; j < current_state->num_transitions; j++) {
          if (j > 0) printf(", ");
          printf("%d", current_state->transitions[j]);
      }
      printf("}\n");
      
      // Check if all successors of state i are in the input set
      bool all_in_set = true;
      
      if (current_state->num_transitions == 0) {
          printf("      No successors, so vacuously true. State %d is in τAX(Z)\n", i);
          ctl_add_to_state_set(result, i);
          continue;
      }
      
      for (int j = 0; j < current_state->num_transitions; j++) {
          int successor_id = current_state->transitions[j];
          if (!ctl_is_in_state_set(set, successor_id)) {
              all_in_set = false;
              printf("      Successor %d is NOT in Z, so state %d is NOT in τAX(Z)\n", successor_id, i);
              break;
          }
      }
      
      if (all_in_set) {
          printf("      All successors are in Z, so state %d is in τAX(Z)\n", i);
          ctl_add_to_state_set(result, i);
      }
  }
}

// Function to verify equation 6: [[AX P]] = τAX([[P]])
void verify_equation6(model* model, const char* formula_p) {
  printf("\n6. Verifying [[AX P]] = τAX([[P]]) where P = %s:\n", formula_p);
  printf("   This equation defines the semantics of the universal next operator in CTL.\n");
  printf("   τAX(Z) = {s ∈ W : t ∈ Z for all states t with s y t}\n");
  
  // Get [[P]]
  state_set p_result;
  ctl_parse_formula(formula_p, &p_result, model);
  
  // Get [[AX P]] using API
  char ax_p_formula[MAX_FORMULA_LEN];
  snprintf(ax_p_formula, MAX_FORMULA_LEN, "AX(%s)", formula_p);
  state_set ax_p_result;
  ctl_parse_formula(ax_p_formula, &ax_p_result, model);
  
  // Calculate τAX([[P]]) manually
  state_set tau_ax_result;
  manual_universal_successor(&tau_ax_result, &p_result, model);
  
  // Compare results
  bool equal = compare_state_sets(&ax_p_result, &tau_ax_result);
  printf("  CTL API result [[AX P]] = ");
  print_state_set(&ax_p_result, model, ax_p_formula);
  printf("  Manually computed τAX([[P]]) = ");
  print_state_set(&tau_ax_result, model, "τAX");
  printf("  Equation holds: %s\n", equal ? "YES" : "NO");
}