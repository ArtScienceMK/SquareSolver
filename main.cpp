
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>


#include "QuadraticSolver.h"

const int cntCoef = 3;
const int INPUT_ERROR = 1;
const int BAD_INPUT = 1;
const char EXIT_CHAR = 'q';
const int EXIT = 0;
const int RESUME = 1;

enum CHECK_STATUS {CHECK_SUCCESS = 1,
                   CHECK_FAIL = 0,
                   CHECK_ERROR = -1};
enum CNT_ROOTS {
    ZERO_ROOTS = 0,
    ONE_ROOT = 1,
    TWO_ROOTS = 2,
    INF_ROOTS = 3};


void greetings(void);
void printRoots(const int cntRoots, const double eq_root1, const double eq_root2);
int badInput(void);
int resume(void);
int checkRoots(const double a, const double b, const double c, const int cntRoot, const double eq_root1, const double eq_root2);
int checkRoot(const double a, const double b, const double c, const double x);

int main() {
    greetings();

    double a = 0, b = 0, c = 0;
    double eq_root1 = 0, eq_root2 = 0;
    while (resume()) {
        printf("Enter a, b, c:");
        int cntInput = scanf("%lf %lf %lf", &a, &b, &c);
        if (cntInput != cntCoef || badInput() == BAD_INPUT) {
            printf("Input error!\n"
                   "Try another time!\n");
            continue;
        }
        // printf("%lf %lf %lf\n", a, b, c);

        int cntRoots = QuadraticSolver(a, b, c, &eq_root1, &eq_root2);
        int equationCheckStatus = checkRoots(a, b, c, cntRoots, eq_root1, eq_root2);
        switch (equationCheckStatus) {
            case CHECK_SUCCESS:
                printf("Roots are valid!\n");
                break;
            case CHECK_FAIL:
                printf("Roots are invalid!\n");
                break;
            case CHECK_ERROR:
                printf("Error during testing!\n");
                break;
            default:
                printf("Unknown error during testing!\n");
                break;
        }
        printRoots(cntRoots, eq_root1, eq_root2);
    }
    return 0;
}

void greetings() {
    printf("Equation solver\n"
           "Powered by AK\n");
}



void printRoots(const int cntRoots, const double eq_root1, const double eq_root2) {
    switch (cntRoots) {
        case ZERO_ROOTS:
            printf("No solutions\n");
            break;
        case ONE_ROOT:
            printf("Have one root: %lf\n", eq_root1);
            break;
        case TWO_ROOTS:
            printf("Have two roots: %lf, %lf\n", eq_root1, eq_root2);
            break;
        case INF_ROOTS:
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

int badInput() {
    char c;
    while ((c = (char)getchar()) != '\n') {
        if (!isspace(c)) {
            return BAD_INPUT;
        }
    }
    return 0;
}

int resume(void) {
    printf("Enter q to exit, any other button to continue\n");

    char c = ' ';

    scanf("%c", &c);

    if (c == EXIT_CHAR) {
        return EXIT;
    }

    return RESUME;
}

int checkRoots(const double a, const double b, const double c, const int cntRoot, const double eq_root1, const double eq_root2) {
    switch (cntRoot) {
        case ZERO_ROOTS:
            return CHECK_SUCCESS;
            break;
        case ONE_ROOT:
            return (checkRoot(a, b, c, eq_root1)) ? CHECK_SUCCESS : CHECK_FAIL;
            break;
        case TWO_ROOTS:
            return (checkRoot(a, b, c, eq_root1) && checkRoot(a, b, c, eq_root2)) ? CHECK_SUCCESS : CHECK_FAIL;
            break;
        case INF_ROOTS:
            return (checkRoot(a, b, c, 0.0) && checkRoot(a, b, c, 1.0) && checkRoot(a, b, c, 2.0)) ? CHECK_SUCCESS : CHECK_FAIL;
            break;
        default:
            return CHECK_ERROR;
            break;
    }
}

int checkRoot(const double a, const double b, const double c, const double x) {
    int cmp_double(const double, const double);
    double result = a * x * x + b * x + c;
    return cmp_double(result, 0) == 0;
}
