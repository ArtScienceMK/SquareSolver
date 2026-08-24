#include <math.h>

#include "CmpDouble.h"

int CmpDouble(const double op1, const double op2) {
    // printf("(CmpDouble) op1:%lf op2:%lf\n", op1, op2);
    if (fabs(op1 - op2) < EPSILON) {
        // printf("(CmpDouble) equal\n");
        return 0;
    }
    if (op1 - op2 > EPSILON) {
        // printf("(CmpDouble) more\n");
        return 1;
    }
    // printf("(CmpDouble) less\n");
    return -1;
}
