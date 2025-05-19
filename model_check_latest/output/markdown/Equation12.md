# Equation 12 Verification Results

## Equation:
 [[AP UQ]] = μZ.([[Q]] ∪ ([[P]] ∩ τAX(Z))) where P = p1n, Q = p2n:

    This equation defines the semantics of the universal until operator using a least fixpoint.     μZ.f(Z) denotes the least fixpoint of the operation f(Z). 

## Results:

| P | Q | Computed | Manually Computed | Holds |
|:---|:---|:---------|:-----------------|:------|
| p1n | p2n | `{0, 5, 7}` | `{0, 1, 2, 7}` | NO |
| p1n | p2t | `{4, 5}` | `{3, 4, 5, 6}` | NO |
| p1n | p2c | `{5, 7, 8}` | `{7, 8}` | NO |
| p1t | p2n | `{1, 3, 6, 8}` | `{0, 1, 2}` | NO |
| p1t | p2t | `{3, 6}` | `{3, 4, 5, 6}` | NO |
| p1t | p2c | `{3, 6, 8}` | `{6, 7, 8}` | NO |
| p1c | p2n | `{2, 4}` | `{0, 1, 2}` | NO |
| p1c | p2t | `{4}` | `{3, 4, 5, 6}` | NO |
| p1c | p2c | `{4}` | `{7, 8}` | NO |

---

For more details, see the [log file](../log/Equation12.log).
