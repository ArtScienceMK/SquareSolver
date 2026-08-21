#include <math.h>

#include "QuadraticSolver.h"
#include "DoubleCmp.h"

int cmp_double(const double op1, const double op2) {
    // printf("(cmp_double) op1:%lf op2:%lf\n", op1, op2);
    if (fabs(op1 - op2) < EPSILON) {
        // printf("(cmp_double) equal\n");
        return 0;
    }
    if (op1 - op2 > EPSILON) {
        // printf("(cmp_double) more\n");
        return 1;
    }
    // printf("(cmp_double) less\n");
    return -1;
}
