#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "QuadraticSolver.h"
#include "DoubleCmp.h"

int QuadraticSolver(const double a, const double b, const double c, double* x1, double* x2) {
    assert(isfinite(a) && isfinite(b) && isfinite(c));
    assert(x1 != NULL && x2 != NULL);

    if (cmp_double(a, 0) == 0) {
        // printf("HAVE ZERO A\n");
        if (cmp_double(b, 0) == 0) {
            if (cmp_double(c, 0) == 0) {
                return INF_SOLUTIONS;
            }
            return 0; //  have NO solutions
        } else {
            *x1 = -c / b;
            return 1;
        }
    } else {
        double d = b * b - 4 * a * c;
        if (cmp_double(d, 0) == -1) {
            return 0;
        }
        if (cmp_double(d, 0) == 0) {
            *x1 = -b / (2 * a);
            return 1;
        }
        // printf("discriminant: %lf\n", d);
        double sqrt_d = sqrt(d);
        *x1 = (-b - sqrt_d) / (2 * a);
        *x2 = (-b + sqrt_d) / (2 * a);
        return 2;
    }
    return QUADRATIC_SOLVER_ERROR ;
}

