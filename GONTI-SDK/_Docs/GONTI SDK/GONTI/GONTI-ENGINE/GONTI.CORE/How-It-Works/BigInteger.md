# BigInteger — How It Works

---

## 1. Representation (internal)

**Summary:** little-endian base 10^9 with explicit memory ownership.

### Fields (exact meaning)

* `GtB8 sign` — `GtTrue` means non-negative, `GtFalse` means negative. Note: zero is represented with `sign = GtTrue` in static `BIGINT_ZERO`.
* `GtU32 capacity` — number of `GtB32` slots allocated (size of `digits` buffer).
* `GtU32 noDigits` — number of meaningful slots. `0` may indicate "not initialized" in some constructors; canonical zero should have `noDigits == 1` and `digits[0] == 0`.
* `GtB32* digits` — pointer to contiguous integer chunks (`GtB32`), each in `[0, BIGINT_BASE-1]`.
* `GtB8 heapAllocated` — if `GtTrue`, `gontiBigintFree` will `gt_free(digits)`; if `GtFalse`, `digits` points to static data and must not be freed.

### Layout invariants

* **Canonical GtBigint**: `noDigits >= 1` (except transient states in allocation), `digits[noDigits - 1] != 0` unless representing zero.
* **Little-endian**: index 0 is least-significant chunk.

### Example internal states

* `0` ⇒ `noDigits=1`, `digits=[0]`, `sign=GtTrue`, `heapAllocated=GtTrue/GtFalse` depending on source.
* `1234567890123456789` ⇒ store in base 1e9: chunks `[234567889, 345678901, 1]` (LSB → MSB) — *verify by dividing repeatedly by 1e9*.

---

## 2. Allocation and static templates

### Static templates

* `DIG0`, `DIG1` are single-element static arrays used by `BIGINT_ZERO`, `BIGINT_ONE`, `BIGINT_NEG_ONE`.
* `BIGINT_*` objects have `heapAllocated = GtFalse` and `capacity = 1`, `noDigits = 1`.
* **Caller MUST NOT** call `gontiBigintFree` on the `BIGINT_*` static objects.

### `gontiBigintAllocate(GtU32 capacity)` — exact steps

1. Create a `GtBigint ret` on the stack.
2. Set `ret.capacity = capacity`.
3. Set `ret.noDigits = 0` (important: uninitialized state; callers are expected to set `noDigits` afterwards).
4. Set `ret.sign = GtTrue`.
5. Set `ret.heapAllocated = GtTrue`.
6. `ret.digits = gt_allocate(capacity * sizeof(GtB32), GT_MEM_TAG_BIGINT);`
7. Return `ret` by value.

**Notes & pitfalls**

* The function does **not** zero-initialize `digits`.
* Many functions depend on `noDigits` > 0 — ensure constructors set it immediately after allocation.

---

## 3. Primitive constructors (detailed flow)

### `gontiBigintFromUnsignedInt(GtU32 ui)`

1. `noDigits = gontiMathNumDigits(ui, BIGINT_BASE)` — returns number of base-chunks required; expected to return 1 for `ui==0`.
2. `ret = gontiBigintAllocate(noDigits)`.
3. `ret.noDigits = noDigits`.
4. Loop `for idx in 0..noDigits-1`:
   * `gontiMathDivMod(ui, BIGINT_BASE, &ui, &digit);` // divides ui by BASE, returns remainder
   * `ret.digits[idx] = (GtB32)digit`.
5. Return `ret`.

### `gontiBigintFromUnsignedLL(GtU64 ull)`

* Same as above but using `gontiMathNumDigitsLL` and `gontiMathDivModLL` to handle 64-bit.

### Signed variants (`FromInt`, `FromLL`)

* Determine sign; if negative multiply by -1 (note: using `i *= -1` could overflow for `INT_MIN`/`LLONG_MIN` in extreme cases — the code does not guard against this).
* Call unsigned constructor and set `ret.sign = sign`.

**Edge cases**

* If input is minimal negative value (two's complement min), negation overflows — undefined behavior; the code does not protect against this.

---

## 4. From string: `gontiBigintFromStr(char* str)` (line-by-line behavior)

1. `len = gontiStringLength(str)`.
2. `effectiveLen = len`; `firstDigit = 0`.
3. Sign parsing: if `str[firstDigit] == '-'` then `sign=GtFalse`, `effectiveLen--`, `firstDigit++`.
4. Skip leading zeros: while `str[firstDigit] == '0'` → `effectiveLen--`, `firstDigit++`.
5. If `effectiveLen == 0` → `return gontiBigintZero()` (heap-allocated zero via wrapper).
6. Compute chunks:
   * `gontiMathDivMod(effectiveLen, BIGINT_NO_BASE_DIGITS, &numDigits, &extra)`.
   * If `extra` non-zero `numDigits++`.
7. `ret = gontiBigintAllocate(numDigits); ret.noDigits = numDigits; ret.sign = sign;`
8. Parse characters left-to-right starting at `firstDigit`:
   * Validate each char is `'0'..'9'` else free and return zero.
   * Accumulate `currentDigit = currentDigit * 10 + (c - '0')`.
   * Decrement `currentDigitCharIdx`; when zero write `ret.digits[digitArrIdx--] = currentDigit`, reset accumulator and `currentDigitCharIdx` to 9.

**Important details**

* `digitArrIdx` starts at `numDigits - 1` and decrements — so writing order is MSB → ... → LSB.
* At the end the digits are stored little-endian if reading was correct: confirm by indexing order.

**Failure mode**

* On invalid char, `gontiBigintFree(&ret)` then `return gontiBigintZero()` — ensures no memory leak when allocation succeeded.

---

## 5. Convert to string: `gontiBigintPtrToString`

Stepwise specifics:

1. If `bi->noDigits == 0` → returns the literal string `"0"` (unsafe; should return allocated string). **See bug note**.
2. `noChars = (bi->noDigits - 1) * BIGINT_NO_BASE_DIGITS`.
3. `numDigitsLast = gontiMathNumDigits(bi->digits[bi->noDigits - 1], 10)`.
4. `noChars += numDigitsLast`.
5. If negative `noChars++` for `'-'`.
6. `ret = gt_allocate((noChars+1) * sizeof(char), GT_MEM_TAG_STRING)`.
7. Fill buffer: starting with `'-'` if needed, then for each chunk from MSB to LSB:
   * For MSB: write exactly `numDigitsLast` digits (no leading zeros).
   * For others: repeated `gontiMathDivMod(digit,10,&digit,&strDigit)` to extract decimal digits into the correct positions (the code writes `ret[strIdx + offset] = '0' + strDigit` while decreasing offset).
8. Null-terminate and return pointer.

**Bug note**

* Returning a string literal for zero: callers often `gt_free` the returned string (e.g. debug print), which is undefined behavior. The API should always return an allocated string and let caller free it with `gontiBigintFreeString`.

---

## 6. Comparison: `gontiBigintCompare`

Detailed algorithm:

1. If `bi1.sign && !bi2.sign` → return `1`.
2. If `!bi1.sign && bi2.sign` → return `-1`.
3. If both negative:
   * temporarily set `bi1.sign = bi2.sign = GtTrue` and call `gontiBigintCompare(bi1, bi2)` recursively,
   * invert the result (return `-comparison`).
4. If `bi1.noDigits > bi2.noDigits` → return `1`.
5. If `bi1.noDigits < bi2.noDigits` → return `-1`.
6. For `idx = bi1.noDigits - 1` down to `0` compare `digits[idx]`:
   * If `bi1.digits[idx] > bi2.digits[idx]` → `1`.
   * If `<` → `-1`.
7. Otherwise return `0`.

**Notes**

* The function uses value-copies when flipping sign; safe and cheap for small struct.

---

## 7. Addition: `gontiBigintAdd`

Exact steps implemented:

1. If `bi1.sign && !bi2.sign` → set `bi2.sign = GtTrue` and call `gontiBigintSubtract(bi1, bi2)`.
2. Else if `!bi1.sign && bi2.sign` → set `bi1.sign = GtTrue` and call `gontiBigintSubtract(bi2, bi1)`.
3. Otherwise both signs equal. `noDigits = max(bi1.noDigits, bi2.noDigits) + 1`.
4. `ret = gontiBigintAllocate(noDigits); ret.noDigits = noDigits;`
5. `carry = GtFalse`.
6. For `idx` in `0..ret.noDigits-1`:
   * `res = carry ? 1 : 0`.
   * If `idx < bi1.noDigits` → `res += bi1.digits[idx]`.
   * If `idx < bi2.noDigits` → `res += bi2.digits[idx]`.
   * If `res >= BASE` → `carry = GtTrue; res -= BASE` else `carry = GtFalse`.
   * `ret.digits[idx] = res`.
7. `gontiBigintTrim(&ret)`.
8. If both original operands negative `ret.sign = GtFalse`.
9. Return `ret`.

Complexity: O(n) where n = max(noDigits1, noDigits2).

Edge-case: final carry stored because of `+1` allocation; trimming removes unused leading zero.

---

## 8. Subtraction: `gontiBigintSubtract`

Complete control flow:

1. `comparison = gontiBigintCompare(bi1, bi2);` if `comparison == 0` return `BIGINT_ZERO` (static).
2. If signs differ:
   * `bi2.sign = GtTrue` and call `gontiBigintAdd(bi1, bi2)` when `bi1.sign && !bi2.sign`.
   * `bi2.sign = GtFalse` and call `gontiBigintAdd(bi1, bi2)` when `!bi1.sign && bi2.sign`.
3. If both negative: flip signs, call subtract in reversed order: `return gontiBigintSubtract(bi2, bi1)`.
4. If `comparison < 0` (|b1| < |b2|): compute `res = gontiBigintSubtract(bi2, bi1); res.sign = !res.sign; return res;`.
5. Otherwise (|b1| >= |b2|):
   * `noDigits = bi1.noDigits`.
   * `ret = gontiBigintAllocate(noDigits); ret.noDigits = noDigits`.
   * `carry = GtFalse`.
   * For `idx` in `0..ret.noDigits-1`:
     * `res = carry ? -1 : 0`;
     * If `idx < bi1.noDigits` → `res += bi1.digits[idx]`;
     * If `idx < bi2.noDigits` → `res -= bi2.digits[idx]`;
     * If `res < 0` → `carry = GtTrue; res += BASE` else `carry = GtFalse`;
     * `ret.digits[idx] = res`.
   * `gontiBigintTrim(&ret)`.
   * return `ret`.

Complexity: O(n).

**API inconsistency note:** When result is zero the function returns static `BIGINT_ZERO` which must not be freed — callers that always free results may crash. Prefer returning a heap-allocated zero consistently.

---

## 9. Multiplication (overview)

High-level selection logic:

* **Zero check:** If either operand is zero → return `BIGINT_ZERO` (static). The check uses: `if (!(gontiBigintCompare(i1, BIGINT_ZERO) && gontiBigintCompare(i2, BIGINT_ZERO)))`. This is **BUGGY** — `gontiBigintCompare` returns `0` for equality, which evaluates to GtFalse, so the negation `!` causes incorrect behavior. This bug means zero multiplication may not work correctly.
* **±1 optimization:** If either operand equals ±1 → return copy with appropriate sign adjustment via `gontiBigintCopyIntArr`.
* **Algorithm selection:** Choose between long multiply and Karatsuba:
  * `if (maxNoDigits < GT_KARATSUBA_THRESHOLD || minNoDigits <= sqrt(maxNoDigits))` use long multiply;
  * else use Karatsuba.

Steps after multiplication array returned:

1. `retArr` is an `GtB32*` with `noDigits` returned by the karatsuba helper.
2. `ret = gontiBigintCopyIntArr(retArr, noDigits, sign)`.
3. `gontiBigintTrim(&ret)` to canonicalize.
4. Free `retArr` with `gt_free(retArr)` (consistent with allocation).

Performance:

* Long multiply: O(n*m) where n,m are operand lengths.
* Karatsuba: asymptotically O(n^{log2(3)}), faster for large balanced operands.

---

## 10. Copying raw arrays: `gontiBigintCopyIntArr`

Exact behavior:

1. `ret = gontiBigintAllocate(n)`.
2. `ret.noDigits = n`.
3. `ret.sign = sign`.
4. `gt_copyMemory(ret.digits, arr, n * sizeof(GtB32))`.
5. Return `ret`.

Memory ownership: the returned GtBigint owns its `digits` (heapAllocated=GtTrue) and caller must free it with `gontiBigintFree`.

---

## 11. Raw array base-shift helpers (full detail)

These two functions transform raw `GtB32*` arrays that represent digits in base 1e9. They reallocate and return new memory pointers, and set `*newSize` to the resulting length.

### `gontiBigintRightShiftArr(GtB32* i, GtB32 n, GtB32 d, GtB32* newSize)`

Purpose: implement an array-level division by `BASE^d`. Input array length is `n`. Return value is new pointer or `NULL` when result is zero.

Step-by-step behavior:

1. If `d < 0`, forward to `gontiBigintLeftShiftArr(i, n, -d, newSize)`.
2. If `d == 0`, set `*newSize = n` and return input pointer unchanged.
3. If `d >= n`, the result is zero:
   * call `gt_free(i);`
   * set `*newSize = 0` and return `NULL`.
4. Count trailing zeros at the high end (most significant zeros) by scanning from `idx = n-1` downwards. The code uses `while (idx >= 0 && !i[idx--]) noLeadingZeros++;` — the post-decrement pattern works but requires careful reading: it checks `i[idx]` then decrements, so loop terminates when `idx` becomes -1.
5. If `noLeadingZeros == n` (array was all zeros), free and return `NULL`.
6. Compute `*newSize = n - noLeadingZeros - d`.
7. Allocate `ret` using `gt_allocate(*newSize * sizeof(GtB32), GT_MEM_TAG_ARRAY)`.
8. Copy memory from `i + d` (skipping `d` least significant chunks) into `ret` with size `(n - noLeadingZeros - d) * sizeof(GtB32)`.
9. Free the old array using `gt_free(i)` and return `ret`.

Edge conditions and pitfalls:

* If `d` is larger than the number of significant chunks (after trimming zeros), the function frees and returns `NULL`.
* Consistent allocator usage: now uses `gt_free` throughout (previously had `free` mismatch).
* The function does not set any explicit values for freed or returned pointers beyond returning `NULL` when appropriate.

### `gontiBigintLeftShiftArr(GtB32* i, GtB32 n, GtB32 d, GtB32* newSize)`

Purpose: implement an array-level multiplication by `BASE^d`. Returns new pointer or `NULL` when result is zero.

Step-by-step behavior:

1. If `d < 0`, forward to `gontiBigintRightShiftArr(i, n, -d, newSize)`.
2. If `d == 0`, set `*newSize = n` and return original pointer.
3. Count trailing zeros at the high end (most significant zeros) as in right shift.
4. If `noLeadingZeros == n` (all zeros), free and return `NULL`.
5. Compute `*newSize = n - noLeadingZeros + d`.
6. Allocate `ret` with `gt_allocate(*newSize * sizeof(GtB32), GT_MEM_TAG_ARRAY)`.
7. **Zero the first `d` elements** using `gt_copyMemory(ret + d, i, (n - noLeadingZeros) * sizeof(GtB32))` (copies significant digits to offset position).
8. **Zero the low `d` positions** using `gt_setMemory(ret, 0, d * sizeof(GtB32))`.
9. Free `i` with `gt_free(i)` and return `ret`.

Edge conditions:

* Allocator now consistent with `gt_free`.
* The function preserves the logical numeric order: adding `d` zeros at the low end (i.e. shifting up) corresponds to multiplication by `BASE^d`.

---

## 12. Bigint-level base shifts

These small wrappers use the raw-array functions and return the modified GtBigint by value (not by pointer). They assign the new `digits` pointer and new `noDigits`.

### `gontiBigintBaseDivide(GtBigint bi, GtB32 d)`

* Calls `gontiBigintRightShiftArr(bi.digits, bi.noDigits, d, &newSize)`.
* Assigns returned pointer to `bi.digits` and `bi.noDigits = (GtU32)newSize`.
* Returns `bi` by value.

Notes:

* If new pointer is `NULL`, `bi.digits` becomes `NULL` and `bi.noDigits` becomes 0. The caller must ensure subsequent calls do not assume `digits` is non-null.
* The function does not update `capacity` nor `heapAllocated`.

### `gontiBigintBaseMult(GtBigint bi, GtB32 d)`

* Same behavior using `gontiBigintLeftShiftArr`.

---

## 13. Trim: `gontiBigintTrim(GtBigint* bi)` (full, line-level details)

`gontiBigintTrim` makes the GtBigint canonical by removing high-order zero chunks and reallocating a smaller `digits` buffer.

Exact steps:

1. While the most significant digit `bi->digits[bi->noDigits - 1]` equals 0, decrement `bi->noDigits`. This loop **assumes** `bi->noDigits > 0` initially; if `noDigits` is 0 this causes undefined behavior.
2. Save `oldDigits = bi->digits`.
3. Allocate new buffer `bi->digits = gt_allocate(bi->noDigits * sizeof(GtB32), GT_MEM_TAG_BIGINT)`.
4. Copy `bi->noDigits * sizeof(GtB32)` from `oldDigits` to new buffer using `gt_copyMemory`.
5. `gt_free(oldDigits)`.

Important invariants:

* After trim, `noDigits` equals allocated length and top digit must be non-zero. If the number is zero, assumed representation is `noDigits=1` and `digits[0]=0`.
* The function always reallocates; if `noDigits` equals previous capacity it still allocates new memory then frees the old one.

Potential problems:

* If `bi->noDigits` becomes 0 due to trimming an already-zero representation, indexing `digits[bi->noDigits - 1]` is invalid — code logic relies on calling contexts that avoid this situation.
* Trim always uses `gt_allocate`/`gt_free` which matches other `k_` operations.

---

## 14. Freeing bigints and strings

### `gontiBigintFree(GtBigint* bi)`

* If `!bi->heapAllocated` → return immediately.
* `gt_free(bi->digits)` — frees the allocated `digits` buffer.
* Reset `capacity = 0`, `noDigits = 0`, `sign = 1`, `heapAllocated = GtFalse`.

Concise notes:

* The function does not set `digits = NULL`. After free the pointer field still holds the previous address, but `heapAllocated == GtFalse` prevents double-free. This leaves a dangling pointer visible in the struct and can cause bugs if callers accidentally use it.

### `gontiBigintFreeString(char* str)`

* If `str == NULL` → return.
* `gt_free(str)`.

Caveat:

* `gontiBigintPtrToString` returns a literal for zero in one branch; freeing such literal with `gontiBigintFreeString` is undefined.

---

## 15. Debug printing: `gontiBigintDebugPrint(const char* name, GtBigint* bi)`

Purpose: human readable dump of internal state for debugging.

Behavior details:

1. Prints metadata `name: sign=%d, noDigits=%u, capacity=%u`.
2. Prints digit array in base `BIGINT_BASE` with indexes.
3. Checks each digit:
   * if `digit < 0` → log `GTWARN("Negative digit at index %u: %d\n"...)` and set `hasNegative=GtTrue`.
   * if `digit >= BIGINT_BASE` → log `GTWARN("Digit >= BASE at index %u: %d\n"...)`.
4. If no negative digits found, obtains a decimal string via `gontiBigintToString(*bi)` and prints it; then `gt_free(str)`.

Important details:

* `gontiBigintToString` may return a literal for zero; the code `gt_free(str)` on that literal is undefined behavior. This occurs in the canonical zero case when `noDigits==0` in `gontiBigintPtrToString`.
* The function assumes `bi->digits` is accessible; if `bi` was partially freed or became `NULL` via shifting helpers, printing may crash.

---

## 16. Memory and allocator consistency

Current status:

1. **Consistent allocator usage:** The code now uses `gt_allocate` / `gt_free` throughout for bigints and arrays. Previously `free()` (standard C) was used in `gontiBigintRightShiftArr` and `gontiBigintLeftShiftArr`, causing allocator mismatch. This has been **corrected** — all array shift functions now use `gt_free`.

2. **Multiplication cleanup:** `gontiBigintMultiply` now consistently uses `gt_free(retArr)` after copying the result, matching the `gt_allocate` used in Karatsuba helpers.

**Current state:** Allocator usage is now standardized on `gt_allocate`/`gt_free` throughout the GtBigint module.

---

## 17. API inconsistencies and safety bugs (catalog)

This section lists all implementation-level issues *inside BigInteger.c/h* that affect correctness or safety.

1. **Returning static zero vs heap zero:**
   * `gontiBigintSubtract` returns `BIGINT_ZERO` (static) on equal operands. Other constructors return heap-allocated zero. Callers that `gontiBigintFree` every result will double-free or crash.
   * `gontiBigintFromStr` returns `gontiBigintZero()` (heap) for zero. Inconsistent contract.

2. **gontiBigintPtrToString returns literal on noDigits==0:**
   * The function returns `"0"` (literal) if `noDigits==0`. Then callers free result — UB.

3. **Incorrect multiplication zero-check:**
   * `if (!(gontiBigintCompare(i1, BIGINT_ZERO) && gontiBigintCompare(i2, BIGINT_ZERO))) return BIGINT_ZERO;` is logically wrong. The comparison functions return `0` for equality; the code treats `0` as GtFalse in boolean context and negates it incorrectly. This means if either operand is zero, `gontiBigintCompare` returns `0`, which is GtFalse, and `!(GtFalse && ...)` becomes `!(GtFalse)` = `GtTrue`, incorrectly returning `BIGINT_ZERO`.

4. **Potential integer overflow when negating minimal signed values in `gontiBigintFromInt` / `gontiBigintFromLL`:**
   * Using `i *= -1` or `ll *= -1LL` can overflow for `INT_MIN` / `LLONG_MIN`. The code has no guard.

5. **Trim assumes `noDigits > 0`:**
   * If a GtBigint has `noDigits == 0` (possible after base-shift wrappers returning NULL), `gontiBigintTrim` will dereference `digits[-1]` — UB.

6. **gontiBigintBaseDivide/Mult do not update capacity or heapAllocated flags:**
   * After replacing `digits` pointer, `capacity` and `heapAllocated` are stale and may indicate a different allocator state.

---

## 18. Concrete suggested fixes (GtBigint-local only)

All fixes below are restricted to BigInteger.c/h and avoid touching external headers.

1. **Always return heap-allocated zero:**
   * Replace `return BIGINT_ZERO;` with `return gontiBigintZero();` in `gontiBigintSubtract` and any other function that returns `BIGINT_ZERO`.

2. **Make `gontiBigintPtrToString` always return allocated buffer:**
   * At the top, if `bi->noDigits == 0` allocate a small buffer with "0" and return it. Remove returning string literal.

3. **Fix multiplication zero-check:**
   * Replace the incorrect boolean test with explicit comparisons:
     ```c
     if (gontiBigintCompare(i1, BIGINT_ZERO) == 0 || gontiBigintCompare(i2, BIGINT_ZERO) == 0)
         return gontiBigintZero();
     ```

4. **Guard against `noDigits == 0`:**
   * Add checks in `gontiBigintTrim`, `gontiBigintPtrToString`, and other functions that assume at least one digit.
   * If `noDigits == 0`, treat as zero and set `noDigits = 1` and `digits[0]=0` (allocate if needed).

5. **Negation safety for signed constructors:**
   * Instead of `i *= -1`, compute absolute via unsigned conversion; e.g. for GtB32 x:
     ```c
     GtB8 sign = x < 0;
     GtU32 ux = sign ? (GtU32)(-(long long)x) : (GtU32)x;
     ```
   * Or handle INT_MIN specially by converting to GtU64.

6. **Update capacity/heapAllocated after base shifts:**
   * When replacing `bi.digits` in `gontiBigintBaseDivide` / `BaseMult`, set `bi.capacity = newSize; bi.heapAllocated = GtTrue;` or copy `heapAllocated` status returned by shift helpers if they return that metadata.

7. **Set `digits=NULL` after free:**
   * In `gontiBigintFree`, after `gt_free(bi->digits);` set `bi->digits = NULL;` to avoid dangling pointers.

Each fix is local to BigInteger.* and does not require changing external header semantics.

---

## 19. Appendix — recommended API invariants

Enforce these invariants at the public API boundary:

* `noDigits >= 1` and `digits != NULL` for any non-NULL GtBigint returned by constructors.
* `heapAllocated` accurately reflects ownership of `digits`.
* All functions that return a `GtBigint` must return a heap-allocated structure unless documentation explicitly states otherwise.
* `gontiBigintPtrToString` must always return heap-allocated pointer which caller frees via `gontiBigintFreeString`.