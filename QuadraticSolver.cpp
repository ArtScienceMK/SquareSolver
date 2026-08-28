QUADRATIC_SOLVER_STATUSES QuadraticSolver(const equationCoefs * const coefs, double* eqRoot1, double* eqRoot2) {
    double a = coefs->a, b = coefs->b, c = coefs->c;

    assert(isfinite(a) && isfinite(b) && isfinite(c));
    assert(eqRoot1 != NULL && eqRoot2 != NULL);

    if (CmpDouble(a, 0) == 0) {
        // printf("HAVE ZERO A\n");
        if (CmpDouble(b, 0) == 0) {
            if (CmpDouble(c, 0) == 0) {
                return INF_ROOTS;
            }

            return ZERO_ROOTS; //  have NO solutions

        } else {
            *eqRoot1 = -c / b;
            return ONE_ROOT;
        }

    } else {
        double d = b * b - 4 * a * c;

        if (CmpDouble(d, 0) == -1) {
            return ZERO_ROOTS;
        }

        if (CmpDouble(d, 0) == 0) {
            *eqRoot1 = -b / (2 * a);
            return ONE_ROOT;
        }

        // printf("discriminant: %lf\n", d);
        double sqrt_d = sqrt(d);

        *eqRoot1 = (-b - sqrt_d) / (2 * a);
        *eqRoot2 = (-b + sqrt_d) / (2 * a);

        return TWO_ROOTS;
    }
    return QUADRATIC_SOLVER_ERROR;
}
