// ctl_visualizer.c - A program to visualize the CTL model and verify equations 1-4
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ctl_common.h"

// Visualization functions
void generate_dot_file(Model* model, const char* filename);
void visualize_equation1(Model* model, int prop_id, const char* filename);
void visualize_equation2(Model* model, int prop_id, const char* filename);
void visualize_equation3(Model* model, int prop_p, int prop_q, const char* filename);
void visualize_equation4(Model* model, int prop_p, int prop_q, const char* filename);

int main() {
    printf("===== CTL Model Visualization =====\n");
    printf("This program creates visualizations for the mutex model and the first 4 equations in Theorem 3.\n");
    
    // Create the mutual exclusion model
    printf("Creating mutual exclusion model...\n");
    Model* model = create_mutex_model();
    
    // Generate DOT file for the model
    generate_dot_file(model, "mutex_model.dot");
    printf("Generated mutex_model.dot. To create a visual graph, run: \n");
    printf("  dot -Tpng mutex_model.dot -o mutex_model.png\n\n");
    
    // Visualize equations 1-4
    int p1c = 2; // p1c proposition (Process 1 in critical section)
    int p2t = 4; // p2t proposition (Process 2 trying to enter)
    
    visualize_equation1(model, p1c, "equation1.dot");
    printf("Generated equation1.dot. To create a visual graph, run: \n");
    printf("  dot -Tpng equation1.dot -o equation1.png\n\n");
    
    visualize_equation2(model, p1c, "equation2.dot");
    printf("Generated equation2.dot. To create a visual graph, run: \n");
    printf("  dot -Tpng equation2.dot -o equation2.png\n\n");
    
    visualize_equation3(model, p1c, p2t, "equation3.dot");
    printf("Generated equation3.dot. To create a visual graph, run: \n");
    printf("  dot -Tpng equation3.dot -o equation3.png\n\n");
    
    visualize_equation4(model, p1c, p2t, "equation4.dot");
    printf("Generated equation4.dot. To create a visual graph, run: \n");
    printf("  dot -Tpng equation4.dot -o equation4.png\n\n");
    
    // Clean up
    free_model(model);
    
    printf("Visualization files generated successfully.\n");
    return 0;
}

// Generate a DOT file for the model
void generate_dot_file(Model* model, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s for writing\n", filename);
        return;
    }
    
    // Start the digraph
    fprintf(file, "digraph MutexModel {\n");
    fprintf(file, "  rankdir=LR;\n");
    fprintf(file, "  node [shape=circle, fontname=\"Arial\", fontsize=10];\n");
    
    // Define nodes (states)
    for (int i = 0; i < model->num_states; i++) {
        fprintf(file, "  s%d [label=\"%d\\n", i, i);
        
        // Add atomic propositions
        bool first = true;
        for (int j = 0; j < model->num_props; j++) {
            if (model->states[i].atomic_props[j]) {
                if (!first) fprintf(file, ", ");
                fprintf(file, "%s", model->prop_names[j]);
                first = false;
            }
        }
        fprintf(file, "\"];\n");
    }
    
    // Define edges (transitions)
    for (int i = 0; i < model->num_states; i++) {
        for (int j = 0; j < model->states[i].num_transitions; j++) {
            int target = model->states[i].transitions[j];
            fprintf(file, "  s%d -> s%d;\n", i, target);
        }
    }
    
    // End the digraph
    fprintf(file, "}\n");
    
    fclose(file);
}

// Visualize equation 1: [[p]] = {s ∈ W : v(s)(p) = true}
void visualize_equation1(Model* model, int prop_id, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s for writing\n", filename);
        return;
    }
    
    // Get set of states where p is true
    StateSet p_result;
    eval_atomic_prop(&p_result, model, prop_id);
    
    // Start the digraph
    fprintf(file, "digraph Equation1 {\n");
    fprintf(file, "  rankdir=LR;\n");
    fprintf(file, "  label=\"Equation 1: [[%s]] = {s ∈ W : v(s)(%s) = true}\";\n", 
            model->prop_names[prop_id], model->prop_names[prop_id]);
    fprintf(file, "  fontname=\"Arial\";\n");
    fprintf(file, "  fontsize=12;\n");
    fprintf(file, "  node [shape=circle, fontname=\"Arial\", fontsize=10];\n");
    
    // Define nodes (states)
    for (int i = 0; i < model->num_states; i++) {
        fprintf(file, "  s%d [label=\"%d\\n", i, i);
        
        // Add atomic propositions
        bool first = true;
        for (int j = 0; j < model->num_props; j++) {
            if (model->states[i].atomic_props[j]) {
                if (!first) fprintf(file, ", ");
                fprintf(file, "%s", model->prop_names[j]);
                first = false;
            }
        }
        
        // Highlight states where p is true
        if (is_in_state_set(&p_result, i)) {
            fprintf(file, "\", style=filled, fillcolor=lightblue];\n");
        } else {
            fprintf(file, "\"];\n");
        }
    }
    
    // Define edges (transitions)
    for (int i = 0; i < model->num_states; i++) {
        for (int j = 0; j < model->states[i].num_transitions; j++) {
            int target = model->states[i].transitions[j];
            fprintf(file, "  s%d -> s%d;\n", i, target);
        }
    }
    
    // Add a legend
    fprintf(file, "  subgraph cluster_legend {\n");
    fprintf(file, "    label=\"Legend\";\n");
    fprintf(file, "    node [shape=plaintext];\n");
    fprintf(file, "    legend [label=<\n");
    fprintf(file, "      <table border=\"0\" cellpadding=\"2\" cellspacing=\"0\" cellborder=\"0\">\n");
    fprintf(file, "        <tr><td align=\"left\" port=\"i1\">States where %s is true</td></tr>\n", model->prop_names[prop_id]);
    fprintf(file, "      </table>\n");
    fprintf(file, "    >];\n");
    fprintf(file, "    node [shape=circle, style=filled, fillcolor=lightblue];\n");
    fprintf(file, "    n1 [label=\"\"];\n");
    fprintf(file, "    legend:i1 -> n1 [style=invis];\n");
    fprintf(file, "  }\n");
    
    // End the digraph
    fprintf(file, "}\n");
    
    fclose(file);
}

// Visualize equation 2: [[¬P]] = W \ [[P]]
void visualize_equation2(Model* model, int prop_id, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s for writing\n", filename);
        return;
    }
    
    // Get [[P]]
    StateSet p_result;
    eval_atomic_prop(&p_result, model, prop_id);
    
    // Calculate ¬[[P]]
    StateSet not_p_result;
    complement_state_set(&not_p_result, &p_result, model->num_states);
    
    // Start the digraph
    fprintf(file, "digraph Equation2 {\n");
    fprintf(file, "  rankdir=LR;\n");
    fprintf(file, "  label=\"Equation 2: [[¬%s]] = W \\ [[%s]]\";\n", 
            model->prop_names[prop_id], model->prop_names[prop_id]);
    fprintf(file, "  fontname=\"Arial\";\n");
    fprintf(file, "  fontsize=12;\n");
    fprintf(file, "  node [shape=circle, fontname=\"Arial\", fontsize=10];\n");
    
    // Define nodes (states)
    for (int i = 0; i < model->num_states; i++) {
        fprintf(file, "  s%d [label=\"%d\\n", i, i);
        
        // Add atomic propositions
        bool first = true;
        for (int j = 0; j < model->num_props; j++) {
            if (model->states[i].atomic_props[j]) {
                if (!first) fprintf(file, ", ");
                fprintf(file, "%s", model->prop_names[j]);
                first = false;
            }
        }
        
        // Highlight states based on the formula
        if (is_in_state_set(&p_result, i)) {
            fprintf(file, "\", style=filled, fillcolor=lightblue];\n");
        } else if (is_in_state_set(&not_p_result, i)) {
            fprintf(file, "\", style=filled, fillcolor=lightgreen];\n");
        } else {
            fprintf(file, "\"];\n");
        }
    }
    
    // Define edges (transitions)
    for (int i = 0; i < model->num_states; i++) {
        for (int j = 0; j < model->states[i].num_transitions; j++) {
            int target = model->states[i].transitions[j];
            fprintf(file, "  s%d -> s%d;\n", i, target);
        }
    }
    
    // Add a legend
    fprintf(file, "  subgraph cluster_legend {\n");
    fprintf(file, "    label=\"Legend\";\n");
    fprintf(file, "    node [shape=plaintext];\n");
    fprintf(file, "    legend [label=<\n");
    fprintf(file, "      <table border=\"0\" cellpadding=\"2\" cellspacing=\"0\" cellborder=\"0\">\n");
    fprintf(file, "        <tr><td align=\"left\" port=\"i1\">States where %s is true ([[%s]])</td></tr>\n", 
            model->prop_names[prop_id], model->prop_names[prop_id]);
    fprintf(file, "        <tr><td align=\"left\" port=\"i2\">States where %s is false ([[¬%s]] = W \\ [[%s]])</td></tr>\n", 
            model->prop_names[prop_id], model->prop_names[prop_id], model->prop_names[prop_id]);
    fprintf(file, "      </table>\n");
    fprintf(file, "    >];\n");
    fprintf(file, "    node [shape=circle];\n");
    fprintf(file, "    n1 [style=filled, fillcolor=lightblue];\n");
    fprintf(file, "    n2 [style=filled, fillcolor=lightgreen];\n");
    fprintf(file, "    legend:i1 -> n1 [style=invis];\n");
    fprintf(file, "    legend:i2 -> n2 [style=invis];\n");
    fprintf(file, "  }\n");
    
    // End the digraph
    fprintf(file, "}\n");
    
    fclose(file);
}

// Visualize equation 3: [[P ∧ Q]] = [[P]] ∩ [[Q]]
void visualize_equation3(Model* model, int prop_p, int prop_q, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s for writing\n", filename);
        return;
    }
    
    // Get [[P]]
    StateSet p_result;
    eval_atomic_prop(&p_result, model, prop_p);
    
    // Get [[Q]]
    StateSet q_result;
    eval_atomic_prop(&q_result, model, prop_q);
    
    // Calculate [[P]] ∩ [[Q]]
    StateSet intersection;
    init_state_set(&intersection, model->num_states);
    intersect_state_sets(&intersection, &p_result, &q_result);
    
    // Start the digraph
    fprintf(file, "digraph Equation3 {\n");
    fprintf(file, "  rankdir=LR;\n");
    fprintf(file, "  label=\"Equation 3: [[%s ∧ %s]] = [[%s]] ∩ [[%s]]\";\n", 
            model->prop_names[prop_p], model->prop_names[prop_q], 
            model->prop_names[prop_p], model->prop_names[prop_q]);
    fprintf(file, "  fontname=\"Arial\";\n");
    fprintf(file, "  fontsize=12;\n");
    fprintf(file, "  node [shape=circle, fontname=\"Arial\", fontsize=10];\n");
    
    // Define nodes (states)
    for (int i = 0; i < model->num_states; i++) {
        fprintf(file, "  s%d [label=\"%d\\n", i, i);
        
        // Add atomic propositions
        bool first = true;
        for (int j = 0; j < model->num_props; j++) {
            if (model->states[i].atomic_props[j]) {
                if (!first) fprintf(file, ", ");
                fprintf(file, "%s", model->prop_names[j]);
                first = false;
            }
        }
        
        // Highlight states based on the formula
        if (is_in_state_set(&intersection, i)) {
            fprintf(file, "\", style=filled, fillcolor=purple];\n");
        } else if (is_in_state_set(&p_result, i)) {
            fprintf(file, "\", style=filled, fillcolor=lightblue];\n");
        } else if (is_in_state_set(&q_result, i)) {
            fprintf(file, "\", style=filled, fillcolor=lightgreen];\n");
        } else {
            fprintf(file, "\"];\n");
        }
    }
    
    // Define edges (transitions)
    for (int i = 0; i < model->num_states; i++) {
        for (int j = 0; j < model->states[i].num_transitions; j++) {
            int target = model->states[i].transitions[j];
            fprintf(file, "  s%d -> s%d;\n", i, target);
        }
    }
    
    // Add a legend
    fprintf(file, "  subgraph cluster_legend {\n");
    fprintf(file, "    label=\"Legend\";\n");
    fprintf(file, "    node [shape=plaintext];\n");
    fprintf(file, "    legend [label=<\n");
    fprintf(file, "      <table border=\"0\" cellpadding=\"2\" cellspacing=\"0\" cellborder=\"0\">\n");
    fprintf(file, "        <tr><td align=\"left\" port=\"i1\">States where %s is true ([[%s]])</td></tr>\n", 
            model->prop_names[prop_p], model->prop_names[prop_p]);
    fprintf(file, "        <tr><td align=\"left\" port=\"i2\">States where %s is true ([[%s]])</td></tr>\n", 
            model->prop_names[prop_q], model->prop_names[prop_q]);
    fprintf(file, "        <tr><td align=\"left\" port=\"i3\">States where both are true ([[%s]] ∩ [[%s]])</td></tr>\n", 
            model->prop_names[prop_p], model->prop_names[prop_q]);
    fprintf(file, "      </table>\n");
    fprintf(file, "    >];\n");
    fprintf(file, "    node [shape=circle];\n");
    fprintf(file, "    n1 [style=filled, fillcolor=lightblue];\n");
    fprintf(file, "    n2 [style=filled, fillcolor=lightgreen];\n");
    fprintf(file, "    n3 [style=filled, fillcolor=purple];\n");
    fprintf(file, "    legend:i1 -> n1 [style=invis];\n");
    fprintf(file, "    legend:i2 -> n2 [style=invis];\n");
    fprintf(file, "    legend:i3 -> n3 [style=invis];\n");
    fprintf(file, "  }\n");
    
    // End the digraph
    fprintf(file, "}\n");
    
    fclose(file);
}

// Visualize equation 4: [[P ∨ Q]] = [[P]] ∪ [[Q]]
void visualize_equation4(Model* model, int prop_p, int prop_q, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s for writing\n", filename);
        return;
    }
    
    // Get [[P]]
    StateSet p_result;
    eval_atomic_prop(&p_result, model, prop_p);
    
    // Get [[Q]]
    StateSet q_result;
    eval_atomic_prop(&q_result, model, prop_q);
    
    // Calculate [[P]] ∪ [[Q]]
    StateSet union_result;
    init_state_set(&union_result, model->num_states);
    union_state_sets(&union_result, &p_result, &q_result);
    
    // Start the digraph
    fprintf(file, "digraph Equation4 {\n");
    fprintf(file, "  rankdir=LR;\n");
    fprintf(file, "  label=\"Equation 4: [[%s ∨ %s]] = [[%s]] ∪ [[%s]]\";\n", 
            model->prop_names[prop_p], model->prop_names[prop_q], 
            model->prop_names[prop_p], model->prop_names[prop_q]);
    fprintf(file, "  fontname=\"Arial\";\n");
    fprintf(file, "  fontsize=12;\n");
    fprintf(file, "  node [shape=circle, fontname=\"Arial\", fontsize=10];\n");
    
    // Define nodes (states)
    for (int i = 0; i < model->num_states; i++) {
        fprintf(file, "  s%d [label=\"%d\\n", i, i);
        
        // Add atomic propositions
        bool first = true;
        for (int j = 0; j < model->num_props; j++) {
            if (model->states[i].atomic_props[j]) {
                if (!first) fprintf(file, ", ");
                fprintf(file, "%s", model->prop_names[j]);
                first = false;
            }
        }
        
        // Highlight states based on the formula
        if (is_in_state_set(&p_result, i) && is_in_state_set(&q_result, i)) {
            fprintf(file, "\", style=filled, fillcolor=purple];\n");
        } else if (is_in_state_set(&p_result, i)) {
            fprintf(file, "\", style=filled, fillcolor=lightblue];\n");
        } else if (is_in_state_set(&q_result, i)) {
            fprintf(file, "\", style=filled, fillcolor=lightgreen];\n");
        } else {
            fprintf(file, "\"];\n");
        }
    }
    
    // Define edges (transitions)
    for (int i = 0; i < model->num_states; i++) {
        for (int j = 0; j < model->states[i].num_transitions; j++) {
            int target = model->states[i].transitions[j];
            fprintf(file, "  s%d -> s%d;\n", i, target);
        }
    }
    
    // Add a legend
    fprintf(file, "  subgraph cluster_legend {\n");
    fprintf(file, "    label=\"Legend\";\n");
    fprintf(file, "    node [shape=plaintext];\n");
    fprintf(file, "    legend [label=<\n");
    fprintf(file, "      <table border=\"0\" cellpadding=\"2\" cellspacing=\"0\" cellborder=\"0\">\n");
    fprintf(file, "        <tr><td align=\"left\" port=\"i1\">States where %s is true ([[%s]])</td></tr>\n", 
            model->prop_names[prop_p], model->prop_names[prop_p]);
    fprintf(file, "        <tr><td align=\"left\" port=\"i2\">States where %s is true ([[%s]])</td></tr>\n", 
            model->prop_names[prop_q], model->prop_names[prop_q]);
    fprintf(file, "        <tr><td align=\"left\" port=\"i3\">States where both are true</td></tr>\n");
    fprintf(file, "        <tr><td align=\"left\">All colored states = [[%s]] ∪ [[%s]]</td></tr>\n", 
            model->prop_names[prop_p], model->prop_names[prop_q]);
    fprintf(file, "      </table>\n");
    fprintf(file, "    >];\n");
    fprintf(file, "    node [shape=circle];\n");
    fprintf(file, "    n1 [style=filled, fillcolor=lightblue];\n");
    fprintf(file, "    n2 [style=filled, fillcolor=lightgreen];\n");
    fprintf(file, "    n3 [style=filled, fillcolor=purple];\n");
    fprintf(file, "    legend:i1 -> n1 [style=invis];\n");
    fprintf(file, "    legend:i2 -> n2 [style=invis];\n");
    fprintf(file, "    legend:i3 -> n3 [style=invis];\n");
    fprintf(file, "  }\n");
    
    // End the digraph
    fprintf(file, "}\n");
    
    fclose(file);
} 
