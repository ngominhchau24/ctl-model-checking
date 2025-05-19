# Equation 1 Verification Results

## Equation:
 [[p]] = [s ∈ W : v(s)(p) = true] where P =p1n:

   This equation defines the semantics of atomic propositions in CTL.   Computed [[p]] = p = {0, 5, 7} 

## Results:

| P | Q | Computed | Manually Computed | Holds |
|:---|:---|:---------|:-----------------|:------|
|  |  | `{0, 5, 7}` | `{0, 5, 7}` | YES |
|  |  | `{1, 3, 6, 8}` | `{1, 3, 6, 8}` | YES |
|  |  | `{2, 4}` | `{2, 4}` | YES |
|  |  | `{0, 1, 2}` | `{0, 1, 2}` | YES |
|  |  | `{3, 4, 5, 6}` | `{3, 4, 5, 6}` | YES |
|  |  | `{7, 8}` | `{7, 8}` | YES |

---

For more details, see the [log file](../log/Equation1.log).
