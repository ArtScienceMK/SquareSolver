
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
const int CHECK_SUCCESS = 1;
const int CHECK_FAIL = 0;
const int CHECK_ERROR = -1;

void greetings(void);
void printRoots(const int cntRoots, const double x1, const double x2);
int badInput(void);
int resume(void);
int checkRoots(const double a, const double b, const double c, const int cntRoot, const double x1, const double x2);
int checkRoot(const double a, const double b, const double c, const double x);

int main() {
    greetings();

    double a = 0, b = 0, c = 0;
    double x1 = 0, x2 = 0;
    while (resume()) {
        printf("Enter a, b, c:");
        int cntInput = scanf("%lf %lf %lf", &a, &b, &c);
        if (cntInput != cntCoef || badInput() == BAD_INPUT) {
            printf("Input error!\n"
                   "Try another time!\n");
            continue;
        }
        // printf("%lf %lf %lf\n", a, b, c);

        int cntRoots = QuadraticSolver(a, b, c, &x1, &x2);
        int checkStatus = checkRoots(a, b, c, cntRoots, x1, x2);
        switch (checkStatus) {
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
        printRoots(cntRoots, x1, x2);
    }
    return 0;
}

void greetings() {
    printf("Equation solver\n"
           "Powered by AK\n");
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

int checkRoots(const double a, const double b, const double c, const int cntRoot, const double x1, const double x2) {
    switch (cntRoot) {
        case 0:
            return CHECK_SUCCESS;
            break;
        case 1:
            return (checkRoot(a, b, c, x1)) ? CHECK_SUCCESS : CHECK_FAIL;
            break;
        case 2:
            return (checkRoot(a, b, c, x1) && checkRoot(a, b, c, x2)) ? CHECK_SUCCESS : CHECK_FAIL;
            break;
        case INF_SOLUTIONS:
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

// TODO:
// 1. Сделать именованные константы HAVE_INF_ROOTS, ERROR +
// 2. Форматирование логических блоков +
// 3. Проверка в solver для чисел с плав. точкой
