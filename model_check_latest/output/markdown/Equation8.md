# Equation 8 Verification Results

## Equation:
 [[EG P]] = νZ.([[P]] ∩ τEX(Z)) where P = p1n:

   This equation defines the semantics of the existential globally operator using a greatest fixpoint.    νZ.f(Z) denotes the greatest fixpoint of the operation f(Z). 

## Results:

| P | Q | Computed | Manually Computed | Holds |
|:---|:---|:---------|:-----------------|:------|
| p1n |  | `{0, 5, 7}` | `{0, 5, 7}` | YES |
| p1t |  | `{}` | `{}` | YES |
| p1c |  | `{}` | `{}` | YES |
| p2n |  | `{0, 1, 2}` | `{0, 1, 2}` | YES |
| p2t |  | `{}` | `{}` | YES |
| p2c |  | `{}` | `{}` | YES |

---

For more details, see the [log file](../log/Equation8.log).
