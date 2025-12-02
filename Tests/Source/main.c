#include <stdio.h>
#include <GONTI/GONTI-ENGINE/GONTI.RENDER.VK/Source/Platform/VulkanPlatform.h>
#include <GONTI/GONTI-ENGINE/GONTI.RUNTIME/Source/Application/Vulkan/VulkanApplication.h>
#include "UserCode/events.h"
#include "UserCode/game.h"
#include "UserSetupEngine/entry.h"

#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Math/Algorithms/Arithmetic/Karatsuba/Karatsuba.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Math/Algebra/BigInteger/BigInteger.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/String/String.h>

/* =========================
       TESTY KONSTRUKTORÓW
========================= */
void testConstructors() {
       printf("\n=== TEST CONSTRUCTORS ===\n");

       printf("\n--- TEST 1: NewBigint(123456) ---\n");
       printf("Creating bigint a...\n");
       bigint a = gontiBigintFromInt(123456);
       printf("a created: sign=%d, noDigits=%u, capacity=%u, heapAllocated=%d, digits=%p\n",
              a.sign, a.noDigits, a.capacity, a.heapAllocated, (void *)a.digits);

       printf("Converting to string...\n");
       char *strA = gontiBigintToString(a);
       printf("NewBigint(123456) = %s | Expected: 123456\n", strA);

       printf("Freeing string...\n");
       gontiBigintFreeString(strA);
       printf("String freed successfully\n\n");

       printf("--- TEST 2: NewBigint(-987654) ---\n");
       printf("Creating bigint b...\n");
       bigint b = gontiBigintFromInt(-987654);
       printf("b created: sign=%d, noDigits=%u, capacity=%u, heapAllocated=%d, digits=%p\n",
              b.sign, b.noDigits, b.capacity, b.heapAllocated, (void *)b.digits);

       printf("Converting to string...\n");
       char *strB = gontiBigintToString(b);
       printf("NewBigint(-987654) = %s | Expected: -987654\n", strB);

       printf("Freeing string...\n");
       gontiBigintFreeString(strB);
       printf("String freed successfully\n\n");

       printf("--- TEST 3: NewBigintLL(1234567890123) ---\n");
       printf("Creating bigint c...\n");
       bigint c = gontiBigintFromLL(1234567890123LL);
       printf("c created: sign=%d, noDigits=%u, capacity=%u, heapAllocated=%d, digits=%p\n",
              c.sign, c.noDigits, c.capacity, c.heapAllocated, (void *)c.digits);

       printf("Converting to string...\n");
       char *strC = gontiBigintToString(c);
       printf("NewBigintLL(1234567890123) = %s | Expected: 1234567890123\n", strC);

       printf("Freeing string...\n");
       gontiBigintFreeString(strC);
       printf("String freed successfully\n\n");

       printf("--- TEST 4: NewPositiveBigint(999999999) ---\n");
       printf("Creating bigint d...\n");
       bigint d = gontiBigintFromUnsignedInt(999999999);
       printf("d created: sign=%d, noDigits=%u, capacity=%u, heapAllocated=%d, digits=%p\n",
              d.sign, d.noDigits, d.capacity, d.heapAllocated, (void *)d.digits);

       printf("Converting to string...\n");
       char *strD = gontiBigintToString(d);
       printf("NewPositiveBigint(999999999) = %s | Expected: 999999999\n", strD);

       printf("Freeing string...\n");
       gontiBigintFreeString(strD);
       printf("String freed successfully\n\n");

       printf("--- TEST 5: NewPositiveBigintLL(123000000000) ---\n");
       printf("Creating bigint e...\n");
       bigint e = gontiBigintFromUnsignedLL(123000000000ULL);
       printf("e created: sign=%d, noDigits=%u, capacity=%u, heapAllocated=%d, digits=%p\n",
              e.sign, e.noDigits, e.capacity, e.heapAllocated, (void *)e.digits);

       printf("Converting to string...\n");
       char *strE = gontiBigintToString(e);
       printf("NewPositiveBigintLL(123000000000) = %s | Expected: 123000000000\n", strE);

       printf("Freeing string...\n");
       gontiBigintFreeString(strE);
       printf("String freed successfully\n\n");

       printf("--- TEST 6: FromStr(\"00000012345600\") ---\n");
       printf("Creating bigint s1...\n");
       bigint s1 = gontiBigintFromStr("00000012345600");
       printf("s1 created: sign=%d, noDigits=%u, capacity=%u, heapAllocated=%d, digits=%p\n",
              s1.sign, s1.noDigits, s1.capacity, s1.heapAllocated, (void *)s1.digits);

       printf("Converting to string...\n");
       char *strS1 = gontiBigintToString(s1);
       printf("FromStr(\"00000012345600\") = %s | Expected: 12345600\n", strS1);

       printf("Freeing string...\n");
       gontiBigintFreeString(strS1);
       printf("String freed successfully\n\n");

       printf("--- TEST 7: FromStr(\"-0000012345000\") ---\n");
       printf("Creating bigint s2...\n");
       bigint s2 = gontiBigintFromStr("-0000012345000");
       printf("s2 created: sign=%d, noDigits=%u, capacity=%u, heapAllocated=%d, digits=%p\n",
              s2.sign, s2.noDigits, s2.capacity, s2.heapAllocated, (void *)s2.digits);

       printf("Converting to string...\n");
       char *strS2 = gontiBigintToString(s2);
       printf("FromStr(\"-0000012345000\") = %s | Expected: -12345000\n", strS2);

       printf("Freeing string...\n");
       gontiBigintFreeString(strS2);
       printf("String freed successfully\n\n");

       printf("--- TEST 8: FromStr(\"0\") ---\n");
       printf("Creating bigint s3...\n");
       bigint s3 = gontiBigintFromStr("0");
       printf("s3 created: sign=%d, noDigits=%u, capacity=%u, heapAllocated=%d, digits=%p\n",
              s3.sign, s3.noDigits, s3.capacity, s3.heapAllocated, (void *)s3.digits);

       printf("Converting to string...\n");
       char *strS3 = gontiBigintToString(s3);
       printf("FromStr(\"0\") = %s | Expected: 0\n", strS3);

       printf("Freeing string...\n");
       gontiBigintFreeString(strS3);
       printf("String freed successfully\n\n");

       printf("=== STARTING FREE OPERATIONS ===\n");

       printf("Freeing a (123456)...\n");
       printf("  Before: sign=%d, noDigits=%u, capacity=%u, heapAllocated=%d, digits=%p\n",
              a.sign, a.noDigits, a.capacity, a.heapAllocated, (void *)a.digits);
       gontiBigintFree(&a);
       printf("  After:  sign=%d, noDigits=%u, capacity=%u, heapAllocated=%d, digits=%p\n",
              a.sign, a.noDigits, a.capacity, a.heapAllocated, (void *)a.digits);
       printf("✓ a freed successfully\n\n");

       printf("Freeing b (-987654)...\n");
       printf("  Before: sign=%d, noDigits=%u, capacity=%u, heapAllocated=%d, digits=%p\n",
              b.sign, b.noDigits, b.capacity, b.heapAllocated, (void *)b.digits);
       gontiBigintFree(&b);
       printf("  After:  sign=%d, noDigits=%u, capacity=%u, heapAllocated=%d, digits=%p\n",
              b.sign, b.noDigits, b.capacity, b.heapAllocated, (void *)b.digits);
       printf("✓ b freed successfully\n\n");

       printf("Freeing c (1234567890123)...\n");
       printf("  Before: sign=%d, noDigits=%u, capacity=%u, heapAllocated=%d, digits=%p\n",
              c.sign, c.noDigits, c.capacity, c.heapAllocated, (void *)c.digits);
       gontiBigintFree(&c);
       printf("  After:  sign=%d, noDigits=%u, capacity=%u, heapAllocated=%d, digits=%p\n",
              c.sign, c.noDigits, c.capacity, c.heapAllocated, (void *)c.digits);
       printf("✓ c freed successfully\n\n");

       printf("Freeing d (999999999)...\n");
       printf("  Before: sign=%d, noDigits=%u, capacity=%u, heapAllocated=%d, digits=%p\n",
              d.sign, d.noDigits, d.capacity, d.heapAllocated, (void *)d.digits);
       gontiBigintFree(&d);
       printf("  After:  sign=%d, noDigits=%u, capacity=%u, heapAllocated=%d, digits=%p\n",
              d.sign, d.noDigits, d.capacity, d.heapAllocated, (void *)d.digits);
       printf("✓ d freed successfully\n\n");

       printf("Freeing e (123000000000)...\n");
       printf("  Before: sign=%d, noDigits=%u, capacity=%u, heapAllocated=%d, digits=%p\n",
              e.sign, e.noDigits, e.capacity, e.heapAllocated, (void *)e.digits);
       gontiBigintFree(&e);
       printf("  After:  sign=%d, noDigits=%u, capacity=%u, heapAllocated=%d, digits=%p\n",
              e.sign, e.noDigits, e.capacity, e.heapAllocated, (void *)e.digits);
       printf("✓ e freed successfully\n\n");

       printf("Freeing s1 (12345600)...\n");
       printf("  Before: sign=%d, noDigits=%u, capacity=%u, heapAllocated=%d, digits=%p\n",
              s1.sign, s1.noDigits, s1.capacity, s1.heapAllocated, (void *)s1.digits);
       gontiBigintFree(&s1);
       printf("  After:  sign=%d, noDigits=%u, capacity=%u, heapAllocated=%d, digits=%p\n",
              s1.sign, s1.noDigits, s1.capacity, s1.heapAllocated, (void *)s1.digits);
       printf("✓ s1 freed successfully\n\n");

       printf("Freeing s2 (-12345000)...\n");
       printf("  Before: sign=%d, noDigits=%u, capacity=%u, heapAllocated=%d, digits=%p\n",
              s2.sign, s2.noDigits, s2.capacity, s2.heapAllocated, (void *)s2.digits);
       gontiBigintFree(&s2);
       printf("  After:  sign=%d, noDigits=%u, capacity=%u, heapAllocated=%d, digits=%p\n",
              s2.sign, s2.noDigits, s2.capacity, s2.heapAllocated, (void *)s2.digits);
       printf("✓ s2 freed successfully\n\n");

       printf("Freeing s3 (0)...\n");
       printf("  Before: sign=%d, noDigits=%u, capacity=%u, heapAllocated=%d, digits=%p\n",
              s3.sign, s3.noDigits, s3.capacity, s3.heapAllocated, (void *)s3.digits);
       gontiBigintFree(&s3);
       printf("  After:  sign=%d, noDigits=%u, capacity=%u, heapAllocated=%d, digits=%p\n",
              s3.sign, s3.noDigits, s3.capacity, s3.heapAllocated, (void *)s3.digits);
       printf("✓ s3 freed successfully\n\n");

       printf("=== ALL FREES COMPLETED ===\n");
}

/* =========================
       TESTY COMPARE
========================= */
void testCompare() {
       printf("\n=== TEST COMPARE ===\n");

       bigint a = gontiBigintFromInt(100);
       bigint b = gontiBigintFromInt(50);
       bigint c = gontiBigintFromInt(100);
       bigint d = gontiBigintFromInt(-100);

       printf("Compare 100 vs 50 = %d | Expected: 1\n", gontiBigintCompare(a, b));
       printf("Compare 50 vs 100 = %d | Expected: -1\n", gontiBigintCompare(b, a));
       printf("Compare 100 vs 100 = %d | Expected: 0\n", gontiBigintCompare(a, c));
       printf("Compare 100 vs -100 = %d | Expected: 1\n", gontiBigintCompare(a, d));
       printf("Compare -100 vs 100 = %d | Expected: -1\n", gontiBigintCompare(d, a));

       gontiBigintFree(&a);
       gontiBigintFree(&b);
       gontiBigintFree(&c);
       gontiBigintFree(&d);
}

/* =========================
       TESTY ADD
========================= */
void testAdd() {
       printf("\n=== TEST ADD ===\n");

       bigint a = gontiBigintFromStr("999999999");
       bigint b = gontiBigintFromStr("1");
       bigint r1 = gontiBigintAdd(a, b);
       char *strR1 = gontiBigintToString(r1);
       printf("999999999 + 1 = %s | Expected: 1000000000\n", strR1);
       gontiBigintFreeString(strR1);

       bigint c = gontiBigintFromStr("5000000000");
       bigint d = gontiBigintFromStr("5000000000");
       bigint r2 = gontiBigintAdd(c, d);
       char *strR2 = gontiBigintToString(r2);
       printf("5000000000 + 5000000000 = %s | Expected: 10000000000\n", strR2);
       gontiBigintFreeString(strR2);

       bigint e = gontiBigintFromStr("-12345");
       bigint f = gontiBigintFromStr("12345");
       bigint r3 = gontiBigintAdd(e, f);
       char *strR3 = gontiBigintToString(r3);
       printf("-12345 + 12345 = %s | Expected: 0\n", strR3);
       gontiBigintFreeString(strR3);

       gontiBigintFree(&a);
       gontiBigintFree(&b);
       gontiBigintFree(&r1);
       gontiBigintFree(&c);
       gontiBigintFree(&d);
       gontiBigintFree(&r2);
       gontiBigintFree(&e);
       gontiBigintFree(&f);
       gontiBigintFree(&r3);
}

/* =========================
   TESTY SUBTRACT
   ========================= */
void testSubtract() {
       printf("\n=== TEST SUBTRACT ===\n");

       bigint a = gontiBigintFromStr("1000000000");
       bigint b = gontiBigintFromStr("1");
       bigint r1 = gontiBigintSubtract(a, b);
       char *strR1 = gontiBigintToString(r1);
       printf("1000000000 - 1 = %s | Expected: 999999999\n", strR1);
       gontiBigintFreeString(strR1);

       bigint c = gontiBigintFromStr("12345");
       bigint d = gontiBigintFromStr("12345");
       bigint r2 = gontiBigintSubtract(c, d);
       char *strR2 = gontiBigintToString(r2);
       printf("12345 - 12345 = %s | Expected: 0\n", strR2);
       gontiBigintFreeString(strR2);

       bigint e = gontiBigintFromStr("0");
       bigint f = gontiBigintFromStr("12345");
       bigint r3 = gontiBigintSubtract(e, f);
       char *strR3 = gontiBigintToString(r3);
       printf("0 - 12345 = %s | Expected: -12345\n", strR3);
       gontiBigintFreeString(strR3);

       gontiBigintFree(&a);
       gontiBigintFree(&b);
       gontiBigintFree(&r1);
       gontiBigintFree(&c);
       gontiBigintFree(&d);
       gontiBigintFree(&r2);
       gontiBigintFree(&e);
       gontiBigintFree(&f);
       gontiBigintFree(&r3);
}

/* =========================
       TESTY MULTIPLY INT / LL
========================= */
void testMultiplySimple() {
       printf("\n=== TEST MULTIPLY INT/LL ===\n");

       bigint r1 = gontiBigintMultiplyUnsignedInt(12345, 10);
       char *strR1 = gontiBigintToString(r1);
       printf("12345 * 10 = %s | Expected: 123450\n", strR1);
       gontiBigintFreeString(strR1);

       bigint r2 = gontiBigintMultiplyUnsignedLL(123456789ULL, 123456789ULL);
       char *strR2 = gontiBigintToString(r2);
       printf("123456789 * 123456789 = %s | Expected: 15241578750190521\n", strR2);
       gontiBigintFreeString(strR2);

       gontiBigintFree(&r1);
       gontiBigintFree(&r2);
}

/* =========================
       TESTY COPY + TRIM
========================= */
void testCopyAndTrim() {
       printf("\n=== TEST COPY + TRIM ===\n");

       int arr[] = {1, 0, 0, 0, 0};
       bigint bi = gontiBigintCopyIntArr(arr, 5, true);

       printf("Before Trim:\n");
       gontiBigintDebugPrint("copy", &bi);
       printf("Expected noDigits: 5, digits: 1,0,0,0,0\n");

       gontiBigintTrim(&bi);
       printf("After Trim:\n");
       gontiBigintDebugPrint("trimmed", &bi);
       printf("Expected noDigits: 1, digits: 1\n");

       gontiBigintFree(&bi);
}

/* =========================
       TEST BASIC MULTIPLICATION
========================= */
void testBasicMultiplication() {
       printf("=== TEST BASIC MULTIPLICATION ===\n");

       bigint a = gontiBigintFromInt(999999999);
       bigint b = gontiBigintFromInt(2);
       bigint result1 = gontiBigintLongMultiply(a, b);
       char *strR1 = gontiBigintToString(result1);
       printf("999999999 * 2 = %s | Expected: 1999999998\n", strR1);
       gontiBigintFreeString(strR1);

       bigint x = gontiBigintFromStr("1000000000");
       bigint y = gontiBigintFromStr("1000000000");
       bigint result2 = gontiBigintLongMultiply(x, y);
       char *strR2 = gontiBigintToString(result2);
       printf("1000000000 * 1000000000 = %s | Expected: 1000000000000000000\n", strR2);
       gontiBigintFreeString(strR2);

       bigint x2 = gontiBigintFromStr("293187123947883712964819237");
       bigint y2 = gontiBigintFromStr("123987040123875132908741");
       bigint result3 = gontiBigintLongMultiply(x2, y2);
       char *strR3 = gontiBigintToString(result3);
       printf("293187123947883712964819237 * 123987040123875132908741 = %s\n", strR3);
       printf("Expected: 36351403700729809780893177598379367542877082250617\n");
       gontiBigintFreeString(strR3);

       gontiBigintFree(&a);
       gontiBigintFree(&b);
       gontiBigintFree(&result1);
       gontiBigintFree(&x);
       gontiBigintFree(&y);
       gontiBigintFree(&result2);
       gontiBigintFree(&x2);
       gontiBigintFree(&y2);
       gontiBigintFree(&result3);
}

/* =========================
       FULL TEST
========================= */
void fullTest() {
       testConstructors();

       testCompare();
       testAdd();
       testSubtract();
       testMultiplySimple();
       testCopyAndTrim();
       testBasicMultiplication();

       printf("\n=== TEST KARATSUBA VS LONG ===\n");
       bigint x = gontiBigintFromStr("293187123947883712964819237");
       bigint y = gontiBigintFromStr("123987040123875132908741");

       bigint resultLong = gontiBigintLongMultiply(x, y);
       bigint resultKaratsuba = gontiBigintKaratsubaMultiply(x, y);

       char *strLong = gontiBigintToString(resultLong);
       char *strKaratsuba = gontiBigintToString(resultKaratsuba);
       printf("Long:      %s\n", strLong);
       printf("Karatsuba: %s\n", strKaratsuba);
       printf("Expected:  36351403700729809780893177598379367542877082250617\n");
       printf("Comparison: %d (0 = equal)\n", gontiBigintCompare(resultLong, resultKaratsuba));

       gontiBigintFreeString(strLong);
       gontiBigintFreeString(strKaratsuba);

       gontiBigintFree(&x);
       gontiBigintFree(&y);
       gontiBigintFree(&resultLong);
       gontiBigintFree(&resultKaratsuba);
}

int main(void) {
       gontiInitializeMemory();

       GontiVulkanEntry entry;
       gontiVkEntry(&entry);
       gontiVkApplicationCreate(&entry);
       gontiVkApplicationRun(30, true);

       gontiShutdownMemory();
       return 0;
}