#ifndef QUADRATIC_SOLVER_H
#define QUADRATIC_SOLVER_H

const int INF_SOLUTIONS = 3;
const int QUADRATIC_SOLVER_ERROR = -1;
const double EPSILON = 1e-9;

struct equationCoefs {
    double a, b, c;
    int cntRoot;
    double eq_root1, eq_root2;
};

int QuadraticSolver(const equationCoefs coefs, double* eq_root1, double* eq_root2);

#endif /* QUADRATIC_SOLVER_H */
