// Function to verify equation 12: [[AP UQ]] = μZ.([[Q]] ∪ ([[P]] ∩ τAX(Z)))
void verify_equation12(model* model, const char* formula_p, const char* formula_q) {
  printf("\n12. Verifying [[AP UQ]] = μZ.([[Q]] ∪ ([[P]] ∩ τAX(Z))) where P = %s, Q = %s:\n", 
         formula_p, formula_q);
  printf("    This equation defines the semantics of the universal until operator using a least fixpoint.\n");
  printf("    μZ.f(Z) denotes the least fixpoint of the operation f(Z).\n");
  
  // Get [[P]]
  state_set p_result;
  ctl_parse_formula(formula_p, &p_result, model);
  
  // Get [[Q]]
  state_set q_result;
  ctl_parse_formula(formula_q, &q_result, model);
  
  // Get [[AP UQ]] using API
  char au_formula[MAX_FORMULA_LEN];
  snprintf(au_formula, MAX_FORMULA_LEN, "AU(%s,%s)", formula_p, formula_q);
  state_set au_result;
  ctl_parse_formula(au_formula, &au_result, model);
  
  // Calculate μZ.([[Q]] ∪ ([[P]] ∩ τAX(Z))) manually using fixed-point iteration
  state_set z_current, z_next;
  ctl_init_state_set(&z_current, model->num_states);
  
  bool changed = true;
  int iteration = 0;
  
  printf("  Computing μZ.([[Q]] ∪ ([[P]] ∩ τAX(Z))) as a least fixpoint:\n");
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
      
      // Calculate [[P]] ∩ τAX(Z_current)
      state_set p_and_tau;
      ctl_intersect_state_sets(&p_and_tau, &p_result, &tau_ax_result);
      printf("    [[P]] ∩ τAX(Z_%d) = ", iteration);
      print_state_set(&p_and_tau, model, "P ∩ τAX(Z)");
      
      // Calculate Z_next = [[Q]] ∪ ([[P]] ∩ τAX(Z_current))
      ctl_union_state_sets(&z_next, &q_result, &p_and_tau);
      printf("    f(Z_%d) = [[Q]] ∪ ([[P]] ∩ τAX(Z_%d)) = ", iteration, iteration);
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
  printf("  Explanation of A[%s U %s]:\n", formula_p, formula_q);
  printf("  A[P U Q] means 'on all paths, P holds until Q becomes true'\n");
  printf("  The fixpoint calculation builds the set of states where all paths satisfy P U Q:\n");
  printf("  1. Start with states where Q is already true\n");
  printf("  2. Add states where P is true and all successors are in the current set\n");
  printf("  3. Continue until no new states are added\n");
  
  // Alternative calculation: A[P U Q] = ¬(E[¬Q U (¬P ∧ ¬Q)] ∨ EG ¬Q)
  printf("  Alternative calculation: A[P U Q] = ¬(E[¬Q U (¬P ∧ ¬Q)] ∨ EG ¬Q)\n");
  
  // Calculate ¬P and ¬Q
  state_set not_p, not_q;
  ctl_complement_state_set(&not_p, &p_result, model->num_states);
  ctl_complement_state_set(&not_q, &q_result, model->num_states);
  printf("  ¬P = ");
  print_state_set(&not_p, model, "¬P");
  printf("  ¬Q = ");
  print_state_set(&not_q, model, "¬Q");
  
  // Calculate ¬P ∧ ¬Q
  state_set not_p_and_not_q;
  ctl_intersect_state_sets(&not_p_and_not_q, &not_p, &not_q);
  printf("  ¬P ∧ ¬Q = ");
  print_state_set(&not_p_and_not_q, model, "¬P ∧ ¬Q");
  
  // Calculate E[¬Q U (¬P ∧ ¬Q)]
  state_set e_not_q_u_not_p_and_not_q;
  ctl_eval_eu(&e_not_q_u_not_p_and_not_q, &not_q, &not_p_and_not_q, model);
  printf("  E[¬Q U (¬P ∧ ¬Q)] = ");
  print_state_set(&e_not_q_u_not_p_and_not_q, model, "E[¬Q U (¬P ∧ ¬Q)]");
  
  // Calculate EG ¬Q
  state_set eg_not_q;
  ctl_eval_eg(&eg_not_q, &not_q, model);
  printf("  EG ¬Q = ");
  print_state_set(&eg_not_q, model, "EG ¬Q");
  
  // Calculate E[¬Q U (¬P ∧ ¬Q)] ∨ EG ¬Q
  state_set disjunction;
  ctl_union_state_sets(&disjunction, &e_not_q_u_not_p_and_not_q, &eg_not_q);
  printf("  E[¬Q U (¬P ∧ ¬Q)] ∨ EG ¬Q = ");
  print_state_set(&disjunction, model, "disjunction");
  
  // Calculate ¬(E[¬Q U (¬P ∧ ¬Q)] ∨ EG ¬Q)
  state_set negation;
  ctl_complement_state_set(&negation, &disjunction, model->num_states);
  printf("  ¬(E[¬Q U (¬P ∧ ¬Q)] ∨ EG ¬Q) = ");
  print_state_set(&negation, model, "negation");
  
  // Compare with A[P U Q]
  bool alt_equal = compare_state_sets(&au_result, &negation);
  printf("  A[P U Q] = ¬(E[¬Q U (¬P ∧ ¬Q)] ∨ EG ¬Q): %s\n", alt_equal ? "YES" : "NO");
  
  // Compare results
  bool equal = compare_state_sets(&au_result, &z_current);
  printf("  CTL API result [[AP UQ]] = ");
  print_state_set(&au_result, model, au_formula);
  printf("  Manually computed μZ.([[Q]] ∪ ([[P]] ∩ τAX(Z))) = ");
  print_state_set(&z_current, model, "fixed point");
  printf("  Equation holds: %s\n", equal ? "YES" : "NO");
  
  // Verify the expansion law for AU
  printf("  Verifying expansion law: AP UQ ↔ Q ∨ P ∧ AXAP UQ\n");
  
  // Calculate AXAP UQ
  state_set ax_au_result;
  ctl_universal_successor(&ax_au_result, &au_result, model);
  printf("  AXAP UQ = ");
  print_state_set(&ax_au_result, model, "AXAP UQ");
  
  // Calculate P ∧ AXAP UQ
  state_set p_and_ax_au;
  ctl_intersect_state_sets(&p_and_ax_au, &p_result, &ax_au_result);
  printf("  P ∧ AXAP UQ = ");
  print_state_set(&p_and_ax_au, model, "P ∧ AXAP UQ");
  
  // Calculate Q ∨ (P ∧ AXAP UQ)
  state_set expansion_law_result;
  ctl_union_state_sets(&expansion_law_result, &q_result, &p_and_ax_au);
  printf("  Q ∨ (P ∧ AXAP UQ) = ");
  print_state_set(&expansion_law_result, model, "expansion");
  
  // Compare with AP UQ
  bool expansion_equal = compare_state_sets(&au_result, &expansion_law_result);
  printf("  Expansion law holds: %s\n", expansion_equal ? "YES" : "NO");
}