# BigInteger – Usage

This document describes how to use the BigInteger module: creating big integers, doing arithmetic, converting to strings, freeing memory, and using helper operations.

## 1. Creating Big Integers

### **Zero / One / -One**

```c
GtBigint a = gontiBigintZero();
GtBigint b = gontiBigintOne();
GtBigint c = gontiBigintNegOne();
```

### **From C integer types**

```c
GtBigint bi1 = gontiBigintFromInt(-12345);
GtBigint bi2 = gontiBigintFromUnsignedInt(4000000000U);
GtBigint bi3 = gontiBigintFromLL(-9223372036854775807LL);
GtBigint bi4 = gontiBigintFromUnsignedLL(18446744073709551615ULL);
```

### **From string**

```c
GtBigint x = gontiBigintFromStr("-123456789012345678901234567890");
GtBigint y = gontiBigintFromStr("123456789012345678901234567890");
```

The string can contain an optional leading minus. Leading zeros are skipped.

---

## 2. Converting BigInt → String

### **Return by value**

```c
char* s = gontiBigintToString(x);
printf("x = %s\n", s);
gontiBigintFreeString(s);
```

### **Pointer version**

```c
char* s = gontiBigintPtrToString(&x);
// identical usage
```

---

## 3. Arithmetic

### **Addition**

```c
GtBigint sum = gontiBigintAdd(a, b);
```

Works correctly with signs.

### **Subtraction**

```c
GtBigint diff = gontiBigintSubtract(bi1, bi2);
```

Implements sign-aware subtraction.

### **Multiplication**

```c
GtBigint mul = gontiBigintMultiply(bi1, bi2);
```

Automatically selects between long multiplication and Karatsuba depending on operand sizes.

### **Direct primitive multipliers**

```c
GtBigint mu1 = gontiBigintMultiplyUnsignedInt(40000, 40000);
GtBigint mu2 = gontiBigintMultiplyUnsignedLL(1ULL<<32, 1234567ULL);
```

---

## 4. Compare

```c
char cmp = gontiBigintCompare(bi1, bi2);
if (cmp > 0) printf("bi1 > bi2");
else if (cmp < 0) printf("bi1 < bi2");
else printf("equal");
```

---

## 5. Debugging

```c
gontiBigintDebugPrint("myBigInt", &x);
```

Prints both the base-array representation and the normal string form.

---

## 6. Base Shifts

These shift by **base digits**, not decimal digits.

### Right shift (divide by BASE^d)

```c
GtB32 newSize;
bi.digits = gontiBigintRightShiftArr(bi.digits, bi.noDigits, d, &newSize);
bi.noDigits = newSize;
```

### Left shift (multiply by BASE^d)

```c
GtB32 newSize;
bi.digits = gontiBigintLeftShiftArr(bi.digits, bi.noDigits, d, &newSize);
bi.noDigits = newSize;
```

---

## 7. Freeing big integers

```c
gontiBigintFree(&bi);  // frees digit array if allocated
```

### Freeing strings returned from ToString

```c
gontiBigintFreeString(str);
```
