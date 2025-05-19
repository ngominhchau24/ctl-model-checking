# Equation 4 Verification Results

## Equation:
 [[P ∨ Q]] = [[P]] ∪ [[Q]] where P = p1n, Q = p2n:

   This equation defines the semantics of disjunction in CTL.   Computed [[P ∨ Q]] = P ∨ Q = {0, 1, 2, 5, 7} 

## Results:

| P | Q | Computed | Manually Computed | Holds |
|:---|:---|:---------|:-----------------|:------|
| p1n | p2n | `{0, 1, 2, 5, 7}` | `{0, 1, 2, 5, 7}` | YES |
| p1n | p2t | `{0, 3, 4, 5, 6, 7}` | `{0, 3, 4, 5, 6, 7}` | YES |
| p1n | p2c | `{0, 5, 7, 8}` | `{0, 5, 7, 8}` | YES |
| p1t | p2n | `{0, 1, 2, 3, 6, 8}` | `{0, 1, 2, 3, 6, 8}` | YES |
| p1t | p2t | `{1, 3, 4, 5, 6, 8}` | `{1, 3, 4, 5, 6, 8}` | YES |
| p1t | p2c | `{1, 3, 6, 7, 8}` | `{1, 3, 6, 7, 8}` | YES |
| p1c | p2n | `{0, 1, 2, 4}` | `{0, 1, 2, 4}` | YES |
| p1c | p2t | `{2, 3, 4, 5, 6}` | `{2, 3, 4, 5, 6}` | YES |
| p1c | p2c | `{2, 4, 7, 8}` | `{2, 4, 7, 8}` | YES |

---

For more details, see the [log file](../log/Equation4.log).
