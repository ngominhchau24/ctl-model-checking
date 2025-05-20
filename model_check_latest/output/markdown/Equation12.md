# Equation 12 Verification Results

## Equation:
 [[AP UQ]] = μZ.([[Q]] ∪ ([[P]] ∩ τAX(Z))) where P = p1n, Q = p2n:

    This equation defines the semantics of the universal until operator using a least fixpoint.     μZ.f(Z) denotes the least fixpoint of the operation f(Z). 

## Results:

| P | Q | Computed | Manually Computed | Holds |
|:---|:---|:---------|:-----------------|:------|
| p1n | p2n | `{0, 1, 2, 7}` | `{0, 1, 2, 7}` | YES |
| p1n | p2t | `{3, 4, 5, 6}` | `{3, 4, 5, 6}` | YES |
| p1n | p2c | `{7, 8}` | `{7, 8}` | YES |
| p1t | p2n | `{0, 1, 2}` | `{0, 1, 2}` | YES |
| p1t | p2t | `{3, 4, 5, 6}` | `{3, 4, 5, 6}` | YES |
| p1t | p2c | `{6, 7, 8}` | `{6, 7, 8}` | YES |
| p1c | p2n | `{0, 1, 2}` | `{0, 1, 2}` | YES |
| p1c | p2t | `{3, 4, 5, 6}` | `{3, 4, 5, 6}` | YES |
| p1c | p2c | `{7, 8}` | `{7, 8}` | YES |

---

For more details, see the [log file](../log/Equation12.log).
