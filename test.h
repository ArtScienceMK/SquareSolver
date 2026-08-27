#ifndef TEST_H
#define TEST_H

 equationCoefsTest TESTS [] = {
{.coefs{.a = 0, .b = 0, .c = 0}, .refCntRoots = 8, .refRoot1 = NAN, .refRoot2 = NAN}, // INF ROOTS
{.coefs{.a = 1, .b = 0, .c = -1}, .refCntRoots = 2, .refRoot1 = -1, .refRoot2 = 1},
{.coefs{.a = 1, .b = 0, .c = -4}, .refCntRoots = 2, .refRoot1 = -2, .refRoot2 = 2},
{.coefs{.a = 1, .b = 0, .c = -9}, .refCntRoots = 2, .refRoot1 = -3, .refRoot2 = 3}, // TWO ROOTS
{.coefs{.a = 1, .b = 0, .c = 0}, .refCntRoots = 1, .refRoot1 = 0, .refRoot2 = NAN}, // ONE ROOT
{.coefs{.a = 0, .b = 100, .c = -50}, .refCntRoots = 1, .refRoot1 = 0.5, .refRoot2 = NAN}, // ONE ROOT, LINEAR
{.coefs{.a = 0, .b = 0, .c = 1}, .refCntRoots = 0, .refRoot1 = NAN, .refRoot2 = NAN}, // NO ROOTS
{.coefs{.a = 0, .b = 0, .c = -1}, .refCntRoots = 0, .refRoot1 = NAN, .refRoot2 = NAN}
};

#endif /*TEST_H*/
