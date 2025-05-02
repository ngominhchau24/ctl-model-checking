// Function to verify equation 9: [[AF P]] = μZ.([[P]] ∪ τAX(Z))
void verify_equation9(model* model, const char* formula_p) {
  printf("\n9. Verifying [[AF P]] = μZ.([[P]] ∪ τAX(Z)) where P = %s:\n", formula_p);
  printf("   This equation defines the semantics of the always finally operator using a least fixpoint.\n");
  printf("   μZ.f(Z) denotes the least fixpoint of the operation f(Z).\n");
  
  // Get [[P]]
  state_set p_result;
  ctl_parse_formula(formula_p, &p_result, model);
  
  // Get [[AF P]] using API
  char af_p_formula[MAX_FORMULA_LEN];
  snprintf(af_p_formula, MAX_FORMULA_LEN, "AF(%s)", formula_p);
  state_set af_p_result;
  ctl_parse_formula(af_p_formula, &af_p_result, model);
  
  // Calculate μZ.([[P]] ∪ τAX(Z)) manually using fixed-point iteration
  state_set z_current, z_next;
  ctl_init_state_set(&z_current, model->num_states);
  
  bool changed = true;
  int iteration = 0;
  
  printf("  Computing μZ.([[P]] ∪ τAX(Z)) as a least fixpoint:\n");
  printf("  According to Knaster-Tarski theorem: μZ.f(Z) = ⋃ₙ≥₁ fⁿ(∅)\n");
  printf("  Starting with Z₀ = ∅ (empty set) and iterating:\n");
  
  while (changed) {
      printf("    Iteration %d: Z_%d = ", iteration, iteration);
      print_state_set(&z_current, model, "current Z");
      
      // Calculate τAX(Z_current)
      state_set tau_ax_result;
      ctl_universal_successor(&tau_ax_result, &z_current, model);
      printf("    τAX(Z_%d) = ", iteration);
      print_state_set(&tau_ax_result, model, "τAX(Z)");
      
      // Calculate Z_next = [[P]] ∪ τAX(Z_current)
      ctl_union_state_sets(&z_next, &p_result, &tau_ax_result);
      printf("    f(Z_%d) = [[P]] ∪ τAX(Z_%d) = ", iteration, iteration);
      print_state_set(&z_next, model, "next Z");
      
      // Check if fixed point is reached
      changed = !compare_state_sets(&z_current, &z_next);
      if (!changed) {
          printf("    Fixed point reached! Z_%d = Z_%d\n", iteration + 1, iteration);
      }
      
      // Update Z for next iteration
      ctl_copy_state_set(&z_current, &z_next);
      iteration++;
  }
  
  // Explanation of the result
  printf("  Explanation of AF %s:\n", formula_p);
  printf("  AF P means 'on all paths, P eventually holds'\n");
  printf("  The fixpoint calculation builds the set of states where all paths eventually reach P:\n");
  printf("  1. Start with states where P is already true\n");
  printf("  2. Add states where all successors are in the current set\n");
  printf("  3. Continue until no new states are added\n");
  
  // Alternative calculation: AF P = ¬EG ¬P
  printf("  Alternative calculation: AF P = ¬EG ¬P\n");
  
  // Calculate ¬P
  state_set not_p_result;
  ctl_complement_state_set(&not_p_result, &p_result, model->num_states);
  printf("  ¬P = ");
  print_state_set(&not_p_result, model, "¬P");
  
  // Calculate EG ¬P
  state_set eg_not_p_result;
  ctl_eval_eg(&eg_not_p_result, &not_p_result, model);
  printf("  EG ¬P = ");
  print_state_set(&eg_not_p_result, model, "EG ¬P");
  
  // Calculate ¬EG ¬P
  state_set not_eg_not_p_result;
  ctl_complement_state_set(&not_eg_not_p_result, &eg_not_p_result, model->num_states);
  printf("  ¬EG ¬P = ");
  print_state_set(&not_eg_not_p_result, model, "¬EG ¬P");
  
  // Compare with AF P
  bool dual_equal = compare_state_sets(&af_p_result, &not_eg_not_p_result);
  printf("  AF P = ¬EG ¬P: %s\n", dual_equal ? "YES" : "NO");
  
  // Compare results
  bool equal = compare_state_sets(&af_p_result, &z_current);
  printf("  CTL API result [[AF P]] = ");
  print_state_set(&af_p_result, model, af_p_formula);
  printf("  Manually computed μZ.([[P]] ∪ τAX(Z)) = ");
  print_state_set(&z_current, model, "fixed point");
  printf("  Equation holds: %s\n", equal ? "YES" : "NO");
}