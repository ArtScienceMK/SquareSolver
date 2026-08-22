#ifndef QUADRATIC_SOLVER_H
#define QUADRATIC_SOLVER_H

const int INF_SOLUTIONS = 3;
const int QUADRATIC_SOLVER_ERROR = -1;


enum CNT_ROOTS {
    ZERO_ROOTS = 0,
    ONE_ROOT = 1,
    TWO_ROOTS = 2,
    INF_ROOTS = 3
};

struct equationCoefs {
    double a, b, c;
    int cntRoot;
    double eqRoot1, eqRoot2;
};

int QuadraticSolver(const equationCoefs* const coefs, double* eqRoot1, double* eqRoot2);

#endif /* QUADRATIC_SOLVER_H */
