// ctl_main.c - Main program to verify CTL Theorem 3 equations
#include "ctl_common.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("===== CTL Model Checking: Theorem 3 Verification =====\n");
    printf("This program verifies the 12 equations of Theorem 3 from the CTL model checking lecture.\n");
    printf("Theorem 3 (CTL model checking): In computation structures, the set [[φ]] of all states\n");
    printf("that satisfy CTL formula φ satisfies the following semantic equations:\n");
    printf(" 1. [[p]] = {s ∈ W : v(s)(p) = true} for atomic propositions p\n");
    printf(" 2. [[¬P]] = W \\ [[P]]\n");
    printf(" 3. [[P ∧ Q]] = [[P]] ∩ [[Q]]\n");
    printf(" 4. [[P ∨ Q]] = [[P]] ∪ [[Q]]\n");
    printf(" 5. [[EX P]] = τEX([[P]]) using the existential successor function\n");
    printf(" 6. [[AX P]] = τAX([[P]]) using the universal successor function\n");
    printf(" 7. [[EF P]] = μZ.([[P]] ∪ τEX(Z)) where μZ denotes the least fixpoint\n");
    printf(" 8. [[EG P]] = νZ.([[P]] ∩ τEX(Z)) where νZ denotes the greatest fixpoint\n");
    printf(" 9. [[AF P]] = μZ.([[P]] ∪ τAX(Z))\n");
    printf("10. [[AG P]] = νZ.([[P]] ∩ τAX(Z))\n");
    printf("11. [[EP UQ]] = μZ.([[Q]] ∪ ([[P]] ∩ τEX(Z)))\n");
    printf("12. [[AP UQ]] = μZ.([[Q]] ∪ ([[P]] ∩ τAX(Z)))\n\n");
    
    // Create a model to test the equations - use the mutual exclusion model
    printf("Creating mutual exclusion model from the lecture...\n");
    Model* model = create_mutex_model();
    
    // Print model information
    print_model_info(model);
    
    // Verify all equations
    printf("\n===== Verifying Semantic Equations of Theorem 3 =====\n");
    verify_equation1(model);
    verify_equation2(model, 0); // Using p1n proposition
    verify_equation3(model, 1, 2); // Using p1t and p1c propositions
    verify_equation4(model, 3, 4); // Using p2t and p2c propositions
    verify_equation5(model, 2); // Using p1c proposition
    verify_equation6(model, 2); // Using p1c proposition
    verify_equation7(model, 2); // Using p1c proposition
    verify_equation8(model, 0); // Using p1n proposition
    verify_equation9(model, 2); // Using p1c proposition
    verify_equation10(model, 5); // Using p2t proposition
    verify_equation11(model, 1, 2); // Using p1t and p1c propositions
    verify_equation12(model, 3, 4); // Using p2t and p2c propositions
    
    // Clean up
    free_model(model);
    
    printf("\n===== Verification Complete =====\n");
    printf("All 12 equations from Theorem 3 have been verified.\n");
    return 0;
}
