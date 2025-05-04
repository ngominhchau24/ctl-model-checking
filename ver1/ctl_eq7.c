// Function to verify equation 7: [[EF P]] = μZ.([[P]] ∪ τEX(Z))
void verify_equation7(model* model, const char* formula_p) {
  printf("\n7. Verifying [[EF P]] = μZ.([[P]] ∪ τEX(Z)) where P = %s:\n", formula_p);
  printf("   This equation defines the semantics of the existential finally operator using a least fixpoint.\n");
  printf("   μZ.f(Z) denotes the least fixpoint of the operation f(Z).\n");
  
  // Get [[P]]
  state_set p_result;
  ctl_parse_formula(formula_p, &p_result, model);
  
  // Get [[EF P]] using API
  char ef_p_formula[MAX_FORMULA_LEN];
  snprintf(ef_p_formula, MAX_FORMULA_LEN, "EF(%s)", formula_p);
  state_set ef_p_result;
  ctl_parse_formula(ef_p_formula, &ef_p_result, model);
  
  // Calculate μZ.([[P]] ∪ τEX(Z)) manually using fixed-point iteration
  state_set z_current, z_next;
  ctl_init_state_set(&z_current, model->num_states);
  
  bool changed = true;
  int iteration = 0;
  
  printf("  Computing μZ.([[P]] ∪ τEX(Z)) as a least fixpoint:\n");
  printf("  According to Knaster-Tarski theorem: μZ.f(Z) = ⋃ₙ≥₁ fⁿ(∅)\n");
  printf("  Starting with Z₀ = ∅ (empty set) and iterating:\n");
  
  while (changed) {
      printf("    Iteration %d: Z_%d = ", iteration, iteration);
      print_state_set(&z_current, model, "current Z");
      
      // Calculate τEX(Z_current)
      state_set tau_ex_result;
      ctl_existential_successor(&tau_ex_result, &z_current, model);
      printf("    τEX(Z_%d) = ", iteration);
      print_state_set(&tau_ex_result, model, "τEX(Z)");
      
      // Calculate Z_next = [[P]] ∪ τEX(Z_current)
      ctl_union_state_sets(&z_next, &p_result, &tau_ex_result);
      printf("    f(Z_%d) = [[P]] ∪ τEX(Z_%d) = ", iteration, iteration);
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
  printf("  Explanation of EF %s:\n", formula_p);
  printf("  EF P means 'there exists a path where P eventually holds'\n");
  printf("  The fixpoint calculation builds the set of states from which such a path exists:\n");
  printf("  1. Start with states where P is already true\n");
  printf("  2. Add states that can reach these states in one step\n");
  printf("  3. Continue until no new states are added\n");
  
  // Compare results
  bool equal = compare_state_sets(&ef_p_result, &z_current);
  printf("  CTL API result [[EF P]] = ");
  print_state_set(&ef_p_result, model, ef_p_formula);
  printf("  Manually computed μZ.([[P]] ∪ τEX(Z)) = ");
  print_state_set(&z_current, model, "fixed point");
  printf("  Equation holds: %s\n", equal ? "YES" : "NO");
}