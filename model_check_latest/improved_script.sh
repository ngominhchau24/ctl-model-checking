#!/bin/bash
# Usage: ./split_equations_2.sh your_log_file.log output_dir

logfile="$1"
outdir="$2"

if [ -z "$logfile" ] || [ -z "$outdir" ]; then
    echo "Usage: $0 <logfile> <output_dir>"
    exit 1
fi

logdir="$outdir/log"
csvdir="$outdir/csv"
mddir="$outdir/markdown"

mkdir -p "$logdir" "$csvdir" "$mddir"
# Remove old files
rm -f "$logdir"/Equation*.log "$csvdir"/Equation*.csv "$csvdir"/Equation*_clean.csv "$mddir"/Equation*.md

# Step 1: Split the log into temp files at each division line
awk -v prefix="$logdir/chunk_" '
/^----------------------------------------/ { 
    if (out) close(out); 
    chunknum++; 
    out=sprintf("%s%04d.tmp", prefix, chunknum); 
    next 
}
{ if (out) print > out }
' "$logfile"

# Step 2: For each chunk, find the equation label and append to the corresponding big file
for f in "$logdir"/chunk_*.tmp; do
    label=$(grep -m1 -o 'Equation [0-9]\+' "$f" | tr -d ' ')
    if [ -n "$label" ]; then
        cat "$f" >> "$logdir/${label}.log"
        echo >> "$logdir/${label}.log"  # Add a blank line between chunks
    fi
    rm "$f"
done

mainlog="$logfile"

for n in {1..12}; do
    eqfile="$logdir/Equation${n}.log"
    cleanfile="$csvdir/Equation${n}_clean.csv"
    
    if [ -f "$eqfile" ]; then
        # Still generate CSV files for backward compatibility
        echo "P,Q,computed,manually computed,holds" > "$cleanfile"
        awk '
        /^Equation [0-9]+\. Verifying/ {
            if (match($0, /P = ([^,]+), Q = ([^:]+):/, pq)) {
                P = pq[1]
                Q = pq[2]
            } else if (match($0, /P = ([^:]+):/, p)) {
                P = p[1]
                Q = ""
            } else {
                P = ""; Q = ""
            }
            next
        }
        /^  Computed/ {
            match($0, /\{[^}]*\}/, c)
            computed = c[0]
            next
        }
        /^  Manually computed/ {
            match($0, /\{[^}]*\}/, m)
            manual = m[0]
            next
        }
        /^  Equation holds:/ {
            n = split($0, arr, /[ ]+/)
            holds = arr[n]
            print P "," Q "," computed "," manual "," holds
        }
        ' "$eqfile" >> "$cleanfile"

        # IMPROVED: Generate markdown directly from the log file, not from CSV
        mdfile="$mddir/Equation${n}.md"
        
        # Extract the equation description and explanation
        equation_desc=$(grep -m1 "Equation ${n}\. Verifying" "$eqfile" | sed 's/^Equation [0-9]\+\. Verifying//')
        equation_explanation=$(grep -A1 "This equation defines" "$eqfile" | head -2 | tr '\n' ' ')
        
        # Start creating the markdown file
        {
            echo "# Equation ${n} Verification Results"
            echo
            echo "## Equation:"
            echo "${equation_desc}"
            echo
            echo "${equation_explanation}"
            echo
            echo "## Results:"
            echo
            echo "| P | Q | Computed | Manually Computed | Holds |"
            echo "|:---|:---|:---------|:-----------------|:------|"
        } > "$mdfile"
        
        # Process the file and extract verification results
        awk -v n="$n" '
        BEGIN { in_chunk = 0; equation_count = 0 }
        
        # Start of a new verification chunk
        /^Equation '"$n"'\. Verifying/ {
            in_chunk = 1
            equation_count++
            
            # Extract P and Q values
            if (match($0, /P = ([^,]+), Q = ([^:]+):/, pq)) {
                P = pq[1]
                Q = pq[2]
            } else if (match($0, /P = ([^:]+):/, p)) {
                P = p[1]
                Q = ""
            } else {
                P = ""; Q = ""
            }
            
            computed = ""
            manual = ""
            holds = ""
            next
        }
        
        # Extract computed result
        in_chunk && /^  Computed/ {
            match($0, /= \{[^}]*\}/, c)
            computed = substr(c[0], 3)  # Remove the "= " part
            next
        }
        
        # Extract manually computed result
        in_chunk && /^  Manually computed/ {
            match($0, /= \{[^}]*\}/, m)
            manual = substr(m[0], 3)  # Remove the "= " part
            next
        }
        
        # Extract whether equation holds
        in_chunk && /^  Equation holds:/ {
            n = split($0, arr, /[ ]+/)
            holds = arr[n]
            
            # Output the row for this verification
            printf("| %s | %s | `%s` | `%s` | %s |\n", P, Q, computed, manual, holds)
            
            in_chunk = 0
        }
        ' "$eqfile" >> "$mdfile"
        
        # Add footer
        {
            echo
            echo "---"
            echo
            echo "For more details, see the [log file](../log/Equation${n}.log)."
        } >> "$mdfile"
        
        echo "Created $mdfile"

    fi
done

cat $(ls -v $mddir/Equation*.md) > $mddir/summary.md

echo "Processing complete. Check the markdown directory for results."