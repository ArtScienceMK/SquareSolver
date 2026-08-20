#ifndef QUADRATIC_SOLVER_H
#define QUADRATIC_SOLVER_H

const int INF_SOLUTIONS = 3;
const int QUADRATIC_SOLVER_ERROR = -1;
const double EPSILON = 1e-9;

int QuadraticSolver(const double a, const double b, const double c, double* x1, double* x2);

#endif /* QUADRATIC_SOLVER_H */
