// Function to verify equation 1: [[p]] = {s ∈ W : v(s)(p) = true}
void verify_equation1(model* model) {
  printf("\n1. Verifying [[p]] = {s ∈ W : v(s)(p) = true}:\n");
  printf("   This equation defines the semantics of atomic propositions in CTL.\n");
  
  for (int p = 0; p < model->num_props; p++) {
      // Get set of states where p is true using API
      state_set api_result;
      ctl_eval_atomic_prop(&api_result, model, p);
      
      // Calculate set of states where p is true manually
      state_set manual_result;
      ctl_init_state_set(&manual_result, model->num_states);
      
      printf("  For proposition '%s':\n", model->prop_names[p]);
      printf("  Formula semantics: {s ∈ W : v(s)(%s) = true}\n", model->prop_names[p]);
      printf("  Evaluating manually:\n");
      
      for (int s = 0; s < model->num_states; s++) {
          printf("    State %d: %s is %s\n", s, model->prop_names[p], 
                 model->states[s].atomic_props[p] ? "true" : "false");
          
          if (model->states[s].atomic_props[p]) {
              ctl_add_to_state_set(&manual_result, s);
          }
      }
      
      // Compare results
      bool equal = compare_state_sets(&api_result, &manual_result);
      printf("  CTL API result [[%s]] = ", model->prop_names[p]);
      print_state_set(&api_result, model, "api result");
      printf("  Manually computed {s ∈ W : v(s)(%s) = true} = ", model->prop_names[p]);
      print_state_set(&manual_result, model, "manual result");
      printf("  Equation holds: %s\n\n", equal ? "YES" : "NO");
  }
}

// Function to verify equation 2: [[¬P]] = W \ [[P]]
void verify_equation2(model* model, const char* formula_p) {
  printf("\n2. Verifying [[¬P]] = W \\ [[P]] where P = %s:\n", formula_p);
  printf("   This equation defines the semantics of negation in CTL.\n");
  
  // Get [[P]]
  state_set p_result;
  ctl_parse_formula(formula_p, &p_result, model);
  
  // Get [[¬P]] using API
  char not_p_formula[MAX_FORMULA_LEN];
  snprintf(not_p_formula, MAX_FORMULA_LEN, "!(%s)", formula_p);
  state_set not_p_result;
  ctl_parse_formula(not_p_formula, &not_p_result, model);
  
  // Calculate W \ [[P]] manually
  state_set complement_result;
  ctl_init_state_set(&complement_result, model->num_states);
  
  printf("  Computing W \\ [[P]] manually:\n");
  printf("  W = {0, 1, 2, ..., %d} (all states)\n", model->num_states - 1);
  printf("  [[P]] = ");
  print_state_set(&p_result, model, formula_p);
  printf("  W \\ [[P]] = states in W that are not in [[P]]\n");
  
  for (int i = 0; i < model->num_states; i++) {
      if (!p_result.members[i]) {
          printf("    State %d is not in [[P]], so it's in W \\ [[P]]\n", i);
          ctl_add_to_state_set(&complement_result, i);
      } else {
          printf("    State %d is in [[P]], so it's not in W \\ [[P]]\n", i);
      }
  }
  
  // Compare results
  bool equal = compare_state_sets(&not_p_result, &complement_result);
  printf("  CTL API result [[¬P]] = ");
  print_state_set(&not_p_result, model, not_p_formula);
  printf("  Manually computed W \\ [[P]] = ");
  print_state_set(&complement_result, model, "complement");
  printf("  Equation holds: %s\n", equal ? "YES" : "NO");
}

// Function to verify equation 3: [[P ∧ Q]] = [[P]] ∩ [[Q]]
void verify_equation3(model* model, const char* formula_p, const char* formula_q) {
  printf("\n3. Verifying [[P ∧ Q]] = [[P]] ∩ [[Q]] where P = %s, Q = %s:\n", formula_p, formula_q);
  printf("   This equation defines the semantics of conjunction in CTL.\n");
  
  // Get [[P]]
  state_set p_result;
  ctl_parse_formula(formula_p, &p_result, model);
  
  // Get [[Q]]
  state_set q_result;
  ctl_parse_formula(formula_q, &q_result, model);
  
  // Get [[P ∧ Q]] using API
  char p_and_q_formula[MAX_FORMULA_LEN];
  snprintf(p_and_q_formula, MAX_FORMULA_LEN, "(%s) & (%s)", formula_p, formula_q);
  state_set p_and_q_result;
  ctl_parse_formula(p_and_q_formula, &p_and_q_result, model);
  
  // Calculate [[P]] ∩ [[Q]] manually
  state_set intersection_result;
  ctl_init_state_set(&intersection_result, model->num_states);
  
  printf("  Computing [[P]] ∩ [[Q]] manually:\n");
  printf("  [[P]] = ");
  print_state_set(&p_result, model, formula_p);
  printf("  [[Q]] = ");
  print_state_set(&q_result, model, formula_q);
  printf("  [[P]] ∩ [[Q]] = states that are in both [[P]] and [[Q]]\n");
  
  for (int i = 0; i < model->num_states; i++) {
      if (p_result.members[i] && q_result.members[i]) {
          printf("    State %d is in both [[P]] and [[Q]], so it's in the intersection\n", i);
          ctl_add_to_state_set(&intersection_result, i);
      } else if (p_result.members[i]) {
          printf("    State %d is in [[P]] but not in [[Q]], so it's not in the intersection\n", i);
      } else if (q_result.members[i]) {
          printf("    State %d is in [[Q]] but not in [[P]], so it's not in the intersection\n", i);
      } else {
          printf("    State %d is neither in [[P]] nor [[Q]], so it's not in the intersection\n", i);
      }
  }
  
  // Compare results
  bool equal = compare_state_sets(&p_and_q_result, &intersection_result);
  printf("  CTL API result [[P ∧ Q]] = ");
  print_state_set(&p_and_q_result, model, p_and_q_formula);
  printf("  Manually computed [[P]] ∩ [[Q]] = ");
  print_state_set(&intersection_result, model, "intersection");
  printf("  Equation holds: %s\n", equal ? "YES" : "NO");
}

// Function to verify equation 4: [[P ∨ Q]] = [[P]] ∪ [[Q]]
void verify_equation4(model* model, const char* formula_p, const char* formula_q) {
  printf("\n4. Verifying [[P ∨ Q]] = [[P]] ∪ [[Q]] where P = %s, Q = %s:\n", formula_p, formula_q);
  printf("   This equation defines the semantics of disjunction in CTL.\n");
  
  // Get [[P]]
  state_set p_result;
  ctl_parse_formula(formula_p, &p_result, model);
  
  // Get [[Q]]
  state_set q_result;
  ctl_parse_formula(formula_q, &q_result, model);
  
  // Get [[P ∨ Q]] using API
  char p_or_q_formula[MAX_FORMULA_LEN];
  snprintf(p_or_q_formula, MAX_FORMULA_LEN, "(%s) | (%s)", formula_p, formula_q);
  state_set p_or_q_result;
  ctl_parse_formula(p_or_q_formula, &p_or_q_result, model);
  
  // Calculate [[P]] ∪ [[Q]] manually
  state_set union_result;
  ctl_init_state_set(&union_result, model->num_states);
  
  printf("  Computing [[P]] ∪ [[Q]] manually:\n");
  printf("  [[P]] = ");
  print_state_set(&p_result, model, formula_p);
  printf("  [[Q]] = ");
  print_state_set(&q_result, model, formula_q);
  printf("  [[P]] ∪ [[Q]] = states that are in either [[P]] or [[Q]] or both\n");
  
  for (int i = 0; i < model->num_states; i++) {
      if (p_result.members[i] && q_result.members[i]) {
          printf("    State %d is in both [[P]] and [[Q]], so it's in the union\n", i);
          ctl_add_to_state_set(&union_result, i);
      } else if (p_result.members[i]) {
          printf("    State %d is in [[P]], so it's in the union\n", i);
          ctl_add_to_state_set(&union_result, i);
      } else if (q_result.members[i]) {
          printf("    State %d is in [[Q]], so it's in the union\n", i);
          ctl_add_to_state_set(&union_result, i);
      } else {
          printf("    State %d is neither in [[P]] nor [[Q]], so it's not in the union\n", i);
      }
  }
  
  // Compare results
  bool equal = compare_state_sets(&p_or_q_result, &union_result);
  printf("  CTL API result [[P ∨ Q]] = ");
  print_state_set(&p_or_q_result, model, p_or_q_formula);
  printf("  Manually computed [[P]] ∪ [[Q]] = ");
  print_state_set(&union_result, model, "union");
  printf("  Equation holds: %s\n", equal ? "YES" : "NO");
}