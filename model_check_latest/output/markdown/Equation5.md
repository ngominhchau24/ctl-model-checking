# Equation 5 Verification Results

## Equation:
 [[EX P]] = τEX([[P]]) where P = p1n:

   This equation defines the semantics of the existential next operator in CTL.    τEX(Z) = {s ∈ W : t ∈ Z for some state t with s y t} 

## Results:

| P | Q | Computed | Manually Computed | Holds |
|:---|:---|:---------|:-----------------|:------|
| p1n |  | `{0, 2, 4, 5, 7, 8}` | `{0, 2, 4, 5, 7, 8}` | YES |
| p1t |  | `{0, 1, 3, 5, 6}` | `{0, 1, 3, 5, 6}` | YES |
| p1c |  | `{1, 2, 3}` | `{1, 2, 3}` | YES |
| p2n |  | `{0, 1, 2, 7}` | `{0, 1, 2, 7}` | YES |
| p2t |  | `{0, 1, 2, 3, 4, 5}` | `{0, 1, 2, 3, 4, 5}` | YES |
| p2c |  | `{5, 6, 8}` | `{5, 6, 8}` | YES |

---

For more details, see the [log file](../log/Equation5.log).
