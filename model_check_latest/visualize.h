#!/bin/bash

# Check if Graphviz is installed
if ! command -v dot &> /dev/null; then
    echo "Graphviz (dot) is not installed. Please install it first."
    echo "For Ubuntu/Debian: sudo apt-get install graphviz"
    echo "For macOS with Homebrew: brew install graphviz"
    exit 1
fi

# Build the project
echo "Building CTL verifier and visualizer..."
make

# Run the visualizer
echo "Running the CTL visualizer..."
./ctl_visualizer

# Generate PNG files from DOT files
echo "Generating PNG images from DOT files..."
dot -Tpng mutex_model.dot -o mutex_model.png
dot -Tpng equation1.dot -o equation1.png
dot -Tpng equation2.dot -o equation2.png
dot -Tpng equation3.dot -o equation3.png
dot -Tpng equation4.dot -o equation4.png

echo "Visualization complete. Generated PNG files:"
echo "- mutex_model.png: The mutual exclusion model"
echo "- equation1.png: Visualization of equation 1 (atomic propositions)"
echo "- equation2.png: Visualization of equation 2 (negation)"
echo "- equation3.png: Visualization of equation 3 (conjunction)"
echo "- equation4.png: Visualization of equation 4 (disjunction)"

# Run the CTL verifier to see the verification results
echo ""
echo "Would you like to run the CTL verifier to see the equation verification results? (y/n)"
read -r response
if [[ "$response" =~ ^([yY][eE][sS]|[yY])$ ]]; then
    echo "Running CTL verifier..."
    ./ctl_verifier
fi
