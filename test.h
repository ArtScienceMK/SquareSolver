#ifndef TEST_H
#define TEST_H

 equationCoefsTest TESTS [] = {
{.coefs{.a = 0, .b = 0, .c = 0}, .refCntRoots = INF_ROOTS, .refRoot1 = NAN, .refRoot2 = NAN}, // INF ROOTS
{.coefs{.a = 1, .b = 0, .c = -1}, .refCntRoots = TWO_ROOTS, .refRoot1 = -1, .refRoot2 = 1},
{.coefs{.a = 1, .b = 0, .c = -4}, .refCntRoots = TWO_ROOTS, .refRoot1 = -2, .refRoot2 = 2},
{.coefs{.a = 1, .b = 0, .c = -9}, .refCntRoots = TWO_ROOTS, .refRoot1 = -3, .refRoot2 = 3}, // TWO ROOTS
{.coefs{.a = 1, .b = 0, .c = 0}, .refCntRoots = ONE_ROOT, .refRoot1 = 0, .refRoot2 = NAN}, // ONE ROOT
{.coefs{.a = 0, .b = 100, .c = -50}, .refCntRoots = ONE_ROOT, .refRoot1 = 0.5, .refRoot2 = NAN}, // ONE ROOT, LINEAR
{.coefs{.a = 0, .b = 0, .c = 1}, .refCntRoots = ZERO_ROOTS, .refRoot1 = NAN, .refRoot2 = NAN}, // NO ROOTS
{.coefs{.a = 0, .b = 0, .c = -1}, .refCntRoots = ZERO_ROOTS, .refRoot1 = NAN, .refRoot2 = NAN}
};

#endif /*TEST_H*/
