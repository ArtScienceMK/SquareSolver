#ifndef QUADRATIC_SOLVER_H
#define QUADRATIC_SOLVER_H

/// @brief INF_ROOTS are 8, becuase it is rotated 90 degrees:)
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

/// @brief solves quadratic equation, edge cases e.g. linear equation are handled
/// @param coefs 
/// @param eqRoot1  
/// @param eqRoot2 
/// @return first root <= than second, if there is one root, *eqRoot2 = NAN; no roots - both are NAN
QUADRATIC_SOLVER_STATUSES QuadraticSolver(const equationCoefs* const ptrCoefs, double* eqRoot1, double* eqRoot2);

#endif /* QUADRATIC_SOLVER_H */
