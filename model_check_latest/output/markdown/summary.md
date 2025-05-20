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
# Equation 2 Verification Results

## Equation:
 [[¬P]] = W \ [[P]] where P = p1n:

   This equation defines the semantics of negation in CTL.   Computed [[¬P]] = not P = {1, 2, 3, 4, 6, 8} 

## Results:

| P | Q | Computed | Manually Computed | Holds |
|:---|:---|:---------|:-----------------|:------|
| p1n |  | `{1, 2, 3, 4, 6, 8}` | `{1, 2, 3, 4, 6, 8}` | YES |
| p1t |  | `{0, 2, 4, 5, 7}` | `{0, 2, 4, 5, 7}` | YES |
| p1c |  | `{0, 1, 3, 5, 6, 7, 8}` | `{0, 1, 3, 5, 6, 7, 8}` | YES |
| p2n |  | `{3, 4, 5, 6, 7, 8}` | `{3, 4, 5, 6, 7, 8}` | YES |
| p2t |  | `{0, 1, 2, 7, 8}` | `{0, 1, 2, 7, 8}` | YES |
| p2c |  | `{0, 1, 2, 3, 4, 5, 6}` | `{0, 1, 2, 3, 4, 5, 6}` | YES |

---

For more details, see the [log file](../log/Equation2.log).
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
# Equation 7 Verification Results

## Equation:
 [[EF P]] = μZ.([[P]] ∪ τEX(Z)) where P = p1n:

   This equation defines the semantics of the existential finally operator using a least fixpoint.    μZ.f(Z) denotes the least fixpoint of the operation f(Z). 

## Results:

| P | Q | Computed | Manually Computed | Holds |
|:---|:---|:---------|:-----------------|:------|
| p1n |  | `{0, 1, 2, 3, 4, 5, 6, 7, 8}` | `{0, 1, 2, 3, 4, 5, 6, 7, 8}` | YES |
| p1t |  | `{0, 1, 2, 3, 4, 5, 6, 7, 8}` | `{0, 1, 2, 3, 4, 5, 6, 7, 8}` | YES |
| p1c |  | `{0, 1, 2, 3, 4, 5, 6, 7, 8}` | `{0, 1, 2, 3, 4, 5, 6, 7, 8}` | YES |
| p2n |  | `{0, 1, 2, 3, 4, 5, 6, 7, 8}` | `{0, 1, 2, 3, 4, 5, 6, 7, 8}` | YES |
| p2t |  | `{0, 1, 2, 3, 4, 5, 6, 7, 8}` | `{0, 1, 2, 3, 4, 5, 6, 7, 8}` | YES |
| p2c |  | `{0, 1, 2, 3, 4, 5, 6, 7, 8}` | `{0, 1, 2, 3, 4, 5, 6, 7, 8}` | YES |

---

For more details, see the [log file](../log/Equation7.log).
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
# Equation 10 Verification Results

## Equation:
 [[AG P]] = νZ.([[P]] ∩ τAX(Z)) where P = p1n:

    This equation defines the semantics of the always globally operator using a greatest fixpoint.     νZ.f(Z) denotes the greatest fixpoint of the operation f(Z). 

## Results:

| P | Q | Computed | Manually Computed | Holds |
|:---|:---|:---------|:-----------------|:------|
| p1n |  | `{}` | `{}` | YES |
| p1t |  | `{}` | `{}` | YES |
| p1c |  | `{}` | `{}` | YES |
| p2n |  | `{}` | `{}` | YES |
| p2t |  | `{}` | `{}` | YES |
| p2c |  | `{}` | `{}` | YES |

---

For more details, see the [log file](../log/Equation10.log).
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
