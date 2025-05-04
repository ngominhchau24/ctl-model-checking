// Function to verify equation 8: [[EG P]] = νZ.([[P]] ∩ τEX(Z))
void verify_equation8(model* model, const char* formula_p) {
  printf("\n8. Verifying [[EG P]] = νZ.([[P]] ∩ τEX(Z)) where P = %s:\n", formula_p);
  printf("   This equation defines the semantics of the existential globally operator using a greatest fixpoint.\n");
  printf("   νZ.f(Z) denotes the greatest fixpoint of the operation f(Z).\n");
  
  // Get [[P]]
  state_set p_result;
  ctl_parse_formula(formula_p, &p_result, model);
  
  // Get [[EG P]] using API
  char eg_p_formula[MAX_FORMULA_LEN];
  snprintf(eg_p_formula, MAX_FORMULA_LEN, "EG(%s)", formula_p);
  state_set eg_p_result;
  ctl_parse_formula(eg_p_formula, &eg_p_result, model);
  
  // Calculate νZ.([[P]] ∩ τEX(Z)) manually using fixed-point iteration
  state_set z_current, z_next;
  
  // Initialize Z with all states (W)
  ctl_init_state_set(&z_current, model->num_states);
  for (int i = 0; i < model->num_states; i++) {
      ctl_add_to_state_set(&z_current, i);
  }
  
  bool changed = true;
  int iteration = 0;
  
  printf("  Computing νZ.([[P]] ∩ τEX(Z)) as a greatest fixpoint:\n");
  printf("  According to Knaster-Tarski theorem: νZ.f(Z) = ⋂ₙ≥₁ fⁿ(W)\n");
  printf("  Starting with Z₀ = W (all states) and iterating:\n");
  
  while (changed) {
      printf("    Iteration %d: Z_%d = ", iteration, iteration);
      print_state_set(&z_current, model, "current Z");
      
      // Calculate τEX(Z_current)
      state_set tau_ex_result;
      ctl_existential_successor(&tau_ex_result, &z_current, model);
      printf("    τEX(Z_%d) = ", iteration);
      print_state_set(&tau_ex_result, model, "τEX(Z)");
      
      // Calculate Z_next = [[P]] ∩ τEX(Z_current)
      ctl_intersect_state_sets(&z_next, &p_result, &tau_ex_result);
      printf("    f(Z_%d) = [[P]] ∩ τEX(Z_%d) = ", iteration, iteration);
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
  printf("  Explanation of EG %s:\n", formula_p);
  printf("  EG P means 'there exists a path where P is always true'\n");
  printf("  The fixpoint calculation starts with all states and iteratively removes states:\n");
  printf("  1. Start with all states\n");
  printf("  2. Keep only states where P is true AND that have a successor in the current set\n");
  printf("  3. Continue until no more states are removed\n");
  
  // Compare results
  bool equal = compare_state_sets(&eg_p_result, &z_current);
  printf("  CTL API result [[EG P]] = ");
  print_state_set(&eg_p_result, model, eg_p_formula);
  printf("  Manually computed νZ.([[P]] ∩ τEX(Z)) = ");
  print_state_set(&z_current, model, "fixed point");
  printf("  Equation holds: %s\n", equal ? "YES" : "NO");
}