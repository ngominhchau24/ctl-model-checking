# Equation 3 Verification Results

## Equation:
 [[P ∧ Q]] = [[P]] ∩ [[Q]] where P = p1n, Q = p2n:

   This equation defines the semantics of conjunction in CTL.   Computed [[P ∧ Q]] = P ∧ Q = {0} 

## Results:

| P | Q | Computed | Manually Computed | Holds |
|:---|:---|:---------|:-----------------|:------|
| p1n | p2n | `{0}` | `{0}` | YES |
| p1n | p2t | `{5}` | `{5}` | YES |
| p1n | p2c | `{7}` | `{7}` | YES |
| p1t | p2n | `{1}` | `{1}` | YES |
| p1t | p2t | `{3, 6}` | `{3, 6}` | YES |
| p1t | p2c | `{8}` | `{8}` | YES |
| p1c | p2n | `{2}` | `{2}` | YES |
| p1c | p2t | `{4}` | `{4}` | YES |
| p1c | p2c | `{}` | `{}` | YES |

---

For more details, see the [log file](../log/Equation3.log).
