#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

#define INF_SOLUTIONS 3
#define QUADRATIC_SOLVER_ERROR -1
#define cntCoef 3
#define INPUT_ERROR 1

void greetings(void);
int QuadraticSolver(const double a, const double b, const double c, double* x1, double* x2);
void printRoots(const int cntRoots, const double x1, const double x2);

int main() {
    greetings();

    double a = 0, b = 0, c = 0;
    double x1 = 0, x2 = 0;

    int cntInput = scanf("%lf %lf %lf", &a, &b, &c);
    if (cntInput != cntCoef) {
        printf("Input error!\n");
        return INPUT_ERROR;
    }
    // printf("%lf %lf %lf\n", a, b, c);

    int cntRoots = QuadraticSolver(a, b, c, &x1, &x2);
    printRoots(cntRoots, x1, x2);

    return 0;
}

void greetings() {
    printf("Equation solver\n"
           "Powered by AK\n"
           "Insert a, b, c:");
}

int QuadraticSolver(const double a, const double b, const double c, double* x1, double* x2) {
    assert(isfinite(a) && isfinite(b) && isfinite(c));
    assert(x1 != NULL && x2 != NULL);

    if (a == (double)0) {
        if (b == (double)0) {
            if (c == (double)0) {
                return INF_SOLUTIONS;
            }
            return 0; //  have NO solutions
        } else {
            *x1 = -c / b;
            return 1;
        }
    } else {
        double d = b * b - 4 * a * c;
        if (d < (double)0) {
            return 0;
        }
        if (d == (double)0) {
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

void printRoots(const int cntRoots, const double x1, const double x2) {
    switch (cntRoots) {
        case 0:
            printf("No solutions\n");
            break;
        case 1:
            printf("Have one root: %lf\n", x1);
            break;
        case 2:
            printf("Have two roots: %lf, %lf\n", x1, x2);
            break;
        case INF_SOLUTIONS:
            printf("Have infinite number of solutions\n");
            break;
        case QUADRATIC_SOLVER_ERROR :
            printf("Have error in quadratic solver\n");
            break;
        default:
            printf("Error: have %i roots\n", cntRoots);
            break;
    }
}

// TODO:
// 1. Сделать именованные константы HAVE_INF_ROOTS, ERROR +
// 2. Форматирование логических блоков +
// 3. Проверка в solver для чисел с плав. точкой
