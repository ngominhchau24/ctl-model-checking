// ctl_main.c - Main program to verify CTL Theorem 3 equations
#include "ctl_common.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    p1n = 0,
    p1t = 1,
    p1c = 2,
    p2n = 3,
    p2t = 4,
    p2c = 5
} proposition_t;

const char* proposition_names[] = {
    "p1n",
    "p1t",
    "p1c",
    "p2n",
    "p2t",
    "p2c"
};

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
    
    
    printf("Creating mutual exclusion model from the lecture...\n");
    Model* model = create_mutex_model();

    // Print model information
    print_model_info(model);
    
    // Verify all equations
    printf("\n===== Verifying Semantic Equations of Theorem 3 =====\n");
    // verify_equation1 (model);
    // verify_equation2 (model, p1n); // Using p1n proposition
    // verify_equation3 (model, 2, 1); // Using p1t and p1c propositions
    // verify_equation4 (model, 3, 4); // Using p2t and p2c propositions
    // verify_equation5 (model, 2); // Using p1c proposition
    // verify_equation6 (model, 2); // Using p1c proposition
    // verify_equation7 (model, 2); // Using p1c proposition
    // verify_equation8 (model, 0); // Using p1n proposition
    // verify_equation9 (model, 2); // Using p1c proposition
    // verify_equation10(model, 5); // Using p2t proposition
    // verify_equation11(model, 1, 2); // Using p1t and p1c propositions
    // verify_equation12(model, 3, 4); // Using p2t and p2c propositions

    // Loop for equations that use 1 proposition input (from p1n to p2n)
    for (int i = p1n; i <= p2c; i++) {
        printf("----------------------------------------\n");
        verify_equation1(model, i);
        printf("----------------------------------------\n");
        verify_equation2(model, i);
        printf("----------------------------------------\n");
        verify_equation5(model, i);
        printf("----------------------------------------\n");
        verify_equation6(model, i);
        printf("----------------------------------------\n");
        verify_equation7(model, i);
        printf("----------------------------------------\n");
        verify_equation8(model, i);
        printf("----------------------------------------\n");
        verify_equation9(model, i);
        printf("----------------------------------------\n");
        verify_equation10(model, i);
    }
    
    for (int i = p1n; i <= p1c; i++) {
        for (int j = p2n; j <= p2c; j++) {
            printf("----------------------------------------\n");
            verify_equation3(model, i, j);
            printf("----------------------------------------\n");
            verify_equation4(model, i, j);
            printf("----------------------------------------\n");
            verify_equation11(model, i, j);
            printf("----------------------------------------\n");
            verify_equation12(model, i, j);
            }
    }

        
    // for (int i = p1n; i <= p2c; i++) {
    //     printf("Test #%d with input proposition is %s ", i, proposition_names[i]);
    //     verify_equation1(model);
    //     verify_equation2(model, i);
    //     verify_equation5(model, i);
    //     verify_equation6(model, i);
    //     verify_equation7(model, i);
    //     verify_equation8(model, i);
    // }

    // Clean up
    free_model(model);
    printf("\n===== Verification Complete =====\n");
    printf("All 12 equations from Theorem 3 have been verified.\n");
    return 0;
}