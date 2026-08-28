#ifndef QUADRATIC_SOLVER_H
#define QUADRATIC_SOLVER_H


enum QUADRATIC_SOLVER_STATUSES {
    QUADRATIC_SOLVER_ERROR = -1,
    ZERO_ROOTS = 0,
    ONE_ROOT = 1,
    TWO_ROOTS = 2,
    INF_ROOTS = 8
};

struct equationCoefs {
    double a, b, c;
    int cntRoot;
    double eqRoot1, eqRoot2;
};

QUADRATIC_SOLVER_STATUSES QuadraticSolver(const equationCoefs* const coefs, double* eqRoot1, double* eqRoot2);

#endif /* QUADRATIC_SOLVER_H */
