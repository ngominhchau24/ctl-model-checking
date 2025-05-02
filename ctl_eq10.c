// Function to verify equation 10: [[AG P]] = νZ.([[P]] ∩ τAX(Z))
void verify_equation10(model* model, const char* formula_p) {
  printf("\n10. Verifying [[AG P]] = νZ.([[P]] ∩ τAX(Z)) where P = %s:\n", formula_p);
  printf("    This equation defines the semantics of the always globally operator using a greatest fixpoint.\n");
  printf("    νZ.f(Z) denotes the greatest fixpoint of the operation f(Z).\n");
  
  // Get [[P]]
  state_set p_result;
  ctl_parse_formula(formula_p, &p_result, model);
  
  // Get [[AG P]] using API
  char ag_p_formula[MAX_FORMULA_LEN];
  snprintf(ag_p_formula, MAX_FORMULA_LEN, "AG(%s)", formula_p);
  state_set ag_p_result;
  ctl_parse_formula(ag_p_formula, &ag_p_result, model);
  
  // Calculate νZ.([[P]] ∩ τAX(Z)) manually using fixed-point iteration
  state_set z_current, z_next;
  
  // Initialize Z with all states (W)
  ctl_init_state_set(&z_current, model->num_states);
  for (int i = 0; i < model->num_states; i++) {
      ctl_add_to_state_set(&z_current, i);
  }
  
  bool changed = true;
  int iteration = 0;
  
  printf("  Computing νZ.([[P]] ∩ τAX(Z)) as a greatest fixpoint:\n");
  printf("  According to Knaster-Tarski theorem: νZ.f(Z) = ⋂ₙ≥₁ fⁿ(W)\n");
  printf("  Starting with Z₀ = W (all states) and iterating:\n");
  
  while (changed) {
      printf("    Iteration %d: Z_%d = ", iteration, iteration);
      print_state_set(&z_current, model, "current Z");
      
      // Calculate τAX(Z_current)
      state_set tau_ax_result;
      ctl_universal_successor(&tau_ax_result, &z_current, model);
      printf("    τAX(Z_%d) = ", iteration);
      print_state_set(&tau_ax_result, model, "τAX(Z)");
      
      // Calculate Z_next = [[P]] ∩ τAX(Z_current)
      ctl_intersect_state_sets(&z_next, &p_result, &tau_ax_result);
      printf("    f(Z_%d) = [[P]] ∩ τAX(Z_%d) = ", iteration, iteration);
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
  printf("  Explanation of AG %s:\n", formula_p);
  printf("  AG P means 'on all paths, P is always true'\n");
  printf("  The fixpoint calculation starts with all states and iteratively removes states:\n");
  printf("  1. Start with all states\n");
  printf("  2. Keep only states where P is true AND where all successors are in the current set\n");
  printf("  3. Continue until no more states are removed\n");
  
  // Alternative calculation: AG P = ¬EF ¬P
  printf("  Alternative calculation: AG P = ¬EF ¬P\n");
  
  // Calculate ¬P
  state_set not_p_result;
  ctl_complement_state_set(&not_p_result, &p_result, model->num_states);
  printf("  ¬P = ");
  print_state_set(&not_p_result, model, "¬P");
  
  // Calculate EF ¬P
  state_set ef_not_p_result;
  ctl_eval_ef(&ef_not_p_result, &not_p_result, model);
  printf("  EF ¬P = ");
  print_state_set(&ef_not_p_result, model, "EF ¬P");
  
  // Calculate ¬EF ¬P
  state_set not_ef_not_p_result;
  ctl_complement_state_set(&not_ef_not_p_result, &ef_not_p_result, model->num_states);
  printf("  ¬EF ¬P = ");
  print_state_set(&not_ef_not_p_result, model, "¬EF ¬P");
  
  // Compare with AG P
  bool dual_equal = compare_state_sets(&ag_p_result, &not_ef_not_p_result);
  printf("  AG P = ¬EF ¬P: %s\n", dual_equal ? "YES" : "NO");
  
  // Compare results
  bool equal = compare_state_sets(&ag_p_result, &z_current);
  printf("  CTL API result [[AG P]] = ");
  print_state_set(&ag_p_result, model, ag_p_formula);
  printf("  Manually computed νZ.([[P]] ∩ τAX(Z)) = ");
  print_state_set(&z_current, model, "fixed point");
  printf("  Equation holds: %s\n", equal ? "YES" : "NO");
}