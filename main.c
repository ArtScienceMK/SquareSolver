
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>


#include "QuadraticSolver.h"

const int cntCoef = 3;
const int INPUT_ERROR = 1;

void greetings(void);
void printRoots(const int cntRoots, const double x1, const double x2);
int badInputEnd(char c);

int main() {
    greetings();

    double a = 0, b = 0, c = 0;
    double x1 = 0, x2 = 0;
    char trash = EOF;

    int cntInput = scanf("%lf %lf %lf%c", &a, &b, &c, &trash);
    if (cntInput != cntCoef + 1 || badInputEnd(trash)) {
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

int badInputEnd(char c) {
    printf("c:%c\n", c);
    return !(isspace(c) || c == '\n');
}

// TODO:
// 1. Сделать именованные константы HAVE_INF_ROOTS, ERROR +
// 2. Форматирование логических блоков +
// 3. Проверка в solver для чисел с плав. точкой
