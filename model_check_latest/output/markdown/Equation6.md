# Equation 6 Verification Results

## Equation:
 [[AX P]] = τAX([[P]]) where P = p1n:

   This equation defines the semantics of the universal next operator in CTL.    τAX(Z) = {s ∈ W : t ∈ Z for all states t with s y t} 

## Results:

| P | Q | Computed | Manually Computed | Holds |
|:---|:---|:---------|:-----------------|:------|
| p1n |  | `{4, 7, 8}` | `{4, 7, 8}` | YES |
| p1t |  | `{6}` | `{6}` | YES |
| p1c |  | `{}` | `{}` | YES |
| p2n |  | `{7}` | `{7}` | YES |
| p2t |  | `{3, 4}` | `{3, 4}` | YES |
| p2c |  | `{6, 8}` | `{6, 8}` | YES |

---

For more details, see the [log file](../log/Equation6.log).
