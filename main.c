
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

void greetings(void);
void printRoots(const int cntRoots, const double x1, const double x2);
int badInput(void);
int resume(void);

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

// TODO:
// 1. Сделать именованные константы HAVE_INF_ROOTS, ERROR +
// 2. Форматирование логических блоков +
// 3. Проверка в solver для чисел с плав. точкой
