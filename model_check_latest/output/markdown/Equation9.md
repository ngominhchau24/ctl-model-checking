# Equation 9 Verification Results

## Equation:
 [[AF P]] = μZ.([[P]] ∪ τAX(Z)) where P = p1n:

   This equation defines the semantics of the always finally operator using a least fixpoint.    μZ.f(Z) denotes the least fixpoint of the operation f(Z). 

## Results:

| P | Q | Computed | Manually Computed | Holds |
|:---|:---|:---------|:-----------------|:------|
| p1n |  | `{0, 1, 2, 3, 4, 5, 6, 7, 8}` | `{0, 1, 2, 3, 4, 5, 6, 7, 8}` | YES |
| p1t |  | `{1, 3, 6, 8}` | `{1, 3, 6, 8}` | YES |
| p1c |  | `{2, 4}` | `{2, 4}` | YES |
| p2n |  | `{0, 1, 2, 3, 4, 5, 6, 7, 8}` | `{0, 1, 2, 3, 4, 5, 6, 7, 8}` | YES |
| p2t |  | `{3, 4, 5, 6}` | `{3, 4, 5, 6}` | YES |
| p2c |  | `{3, 4, 5, 6, 7, 8}` | `{3, 4, 5, 6, 7, 8}` | YES |

---

For more details, see the [log file](../log/Equation9.log).
