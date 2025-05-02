// Function to verify equation 11: [[EP UQ]] = μZ.([[Q]] ∪ ([[P]] ∩ τEX(Z)))
void verify_equation11(model* model, const char* formula_p, const char* formula_q) {
  printf("\n11. Verifying [[EP UQ]] = μZ.([[Q]] ∪ ([[P]] ∩ τEX(Z))) where P = %s, Q = %s:\n", 
         formula_p, formula_q);
  printf("    This equation defines the semantics of the existential until operator using a least fixpoint.\n");
  printf("    μZ.f(Z) denotes the least fixpoint of the operation f(Z).\n");
  
  // Get [[P]]
  state_set p_result;
  ctl_parse_formula(formula_p, &p_result, model);
  
  // Get [[Q]]
  state_set q_result;
  ctl_parse_formula(formula_q, &q_result, model);
  
  // Get [[EP UQ]] using API
  char eu_formula[MAX_FORMULA_LEN];
  snprintf(eu_formula, MAX_FORMULA_LEN, "EU(%s,%s)", formula_p, formula_q);
  state_set eu_result;
  ctl_parse_formula(eu_formula, &eu_result, model);
  
  // Calculate μZ.([[Q]] ∪ ([[P]] ∩ τEX(Z))) manually using fixed-point iteration
  state_set z_current, z_next;
  ctl_init_state_set(&z_current, model->num_states);
  
  bool changed = true;
  int iteration = 0;
  
  printf("  Computing μZ.([[Q]] ∪ ([[P]] ∩ τEX(Z))) as a least fixpoint:\n");
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
      
      // Calculate [[P]] ∩ τEX(Z_current)
      state_set p_and_tau;
      ctl_intersect_state_sets(&p_and_tau, &p_result, &tau_ex_result);
      printf("    [[P]] ∩ τEX(Z_%d) = ", iteration);
      print_state_set(&p_and_tau, model, "P ∩ τEX(Z)");
      
      // Calculate Z_next = [[Q]] ∪ ([[P]] ∩ τEX(Z_current))
      ctl_union_state_sets(&z_next, &q_result, &p_and_tau);
      printf("    f(Z_%d) = [[Q]] ∪ ([[P]] ∩ τEX(Z_%d)) = ", iteration, iteration);
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
  printf("  Explanation of E[%s U %s]:\n", formula_p, formula_q);
  printf("  E[P U Q] means 'there exists a path where P holds until Q becomes true'\n");
  printf("  The fixpoint calculation builds the set of states where such a path exists:\n");
  printf("  1. Start with states where Q is already true\n");
  printf("  2. Add states where P is true and that can reach the current set in one step\n");
  printf("  3. Continue until no new states are added\n");
  
  // Compare results
  bool equal = compare_state_sets(&eu_result, &z_current);
  printf("  CTL API result [[EP UQ]] = ");
  print_state_set(&eu_result, model, eu_formula);
  printf("  Manually computed μZ.([[Q]] ∪ ([[P]] ∩ τEX(Z))) = ");
  print_state_set(&z_current, model, "fixed point");
  printf("  Equation holds: %s\n", equal ? "YES" : "NO");
  
  // Verify the expansion law for EU
  printf("  Verifying expansion law: EP UQ ↔ Q ∨ P ∧ EXEP UQ\n");
  
  // Calculate Q ∨ P ∧ EXEP UQ
  // First, calculate EXEP UQ
  state_set ex_eu_result;
  ctl_existential_successor(&ex_eu_result, &eu_result, model);
  printf("  EXEP UQ = ");
  print_state_set(&ex_eu_result, model, "EXEP UQ");
  
  // Calculate P ∧ EXEP UQ
  state_set p_and_ex_eu;
  ctl_intersect_state_sets(&p_and_ex_eu, &p_result, &ex_eu_result);
  printf("  P ∧ EXEP UQ = ");
  print_state_set(&p_and_ex_eu, model, "P ∧ EXEP UQ");
  
  // Calculate Q ∨ (P ∧ EXEP UQ)
  state_set expansion_law_result;
  ctl_union_state_sets(&expansion_law_result, &q_result, &p_and_ex_eu);
  printf("  Q ∨ (P ∧ EXEP UQ) = ");
  print_state_set(&expansion_law_result, model, "expansion");
  
  // Compare with EP UQ
  bool expansion_equal = compare_state_sets(&eu_result, &expansion_law_result);
  printf("  Expansion law holds: %s\n", expansion_equal ? "YES" : "NO");
}