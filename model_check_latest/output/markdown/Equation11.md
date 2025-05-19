# Equation 11 Verification Results

## Equation:
 [[EP UQ]] = μZ.([[Q]] ∪ ([[P]] ∩ τEX(Z))) where P = p1n, Q = p2n:

    This equation defines the semantics of the existential until operator using a least fixpoint.     μZ.f(Z) denotes the least fixpoint of the operation f(Z). 

## Results:

| P | Q | Computed | Manually Computed | Holds |
|:---|:---|:---------|:-----------------|:------|
| p1n | p2n | `{0, 1, 2, 5, 7}` | `{0, 1, 2, 5, 7}` | YES |
| p1n | p2t | `{0, 3, 4, 5, 6, 7}` | `{0, 3, 4, 5, 6, 7}` | YES |
| p1n | p2c | `{0, 5, 7, 8}` | `{0, 5, 7, 8}` | YES |
| p1t | p2n | `{0, 1, 2}` | `{0, 1, 2}` | YES |
| p1t | p2t | `{1, 3, 4, 5, 6}` | `{1, 3, 4, 5, 6}` | YES |
| p1t | p2c | `{1, 3, 6, 7, 8}` | `{1, 3, 6, 7, 8}` | YES |
| p1c | p2n | `{0, 1, 2}` | `{0, 1, 2}` | YES |
| p1c | p2t | `{2, 3, 4, 5, 6}` | `{2, 3, 4, 5, 6}` | YES |
| p1c | p2c | `{7, 8}` | `{7, 8}` | YES |

---

For more details, see the [log file](../log/Equation11.log).
