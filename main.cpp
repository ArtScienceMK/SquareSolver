
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

enum INPUT_STATUSES {
    INPUT_ERROR_FIRST = 0,
    INPUT_ERROR_SECOND = 1,
    INPUT_ERROR_THIRD = 2,
    INPUT_ERROR_AFTER_THIRD = 3,
    UNKNOWN_INPUT_ERROR = 4,
    INPUT_CORRECT = 5};

struct equationCoefs equationCoefsInit(void);

struct equationCoefs equationCoefsInit() {
    struct equationCoefs coefs;
    coefs.a = 0;
    coefs.b = 0;
    coefs.c = 0;
    return coefs;
}

void equationCoefsInitPointers(equationCoefs* pcoefs);

void equationCoefsInitPointers(equationCoefs* pcoefs) {
    pcoefs->a = 0;
    pcoefs->b = 0;
    pcoefs->c = 0;
}

void greetings_attempts(void);
int inputAttempts(void);

int processEnd(void);
void startUserCycle(int attempts);
void greetings(void);
void printAttempts(int attempts);
int getInput(equationCoefs* pcoefs);
bool goodEnd(void);
void readToEnd(void);

void printRoots(const int cntRoots, const double eq_root1, const double eq_root2);
int badInput(void);
int resume(void);
int checkRoots(const equationCoefs coefs, const int cntRoot, const double eq_root1, const double eq_root2);
int checkRoot(const equationCoefs coefs, const double x);


int main() {
    greetings();

    int attempts = inputAttempts();

    startUserCycle(attempts);

    return 0;
}

void greetings_attempts() {
    printf("Enter the positive number of attempts:");
}

int inputAttempts() { // 2;ad;sflkjasdf
    greetings_attempts();

    int attempts = 0;
    int processed_end = 0;

    while (!scanf("%i", &attempts) || !(processed_end = processEnd()) || attempts <= 0) {
        printf("Cannot process number of attempts!\n"
               "Try again!\n");
        greetings_attempts();
        if (!processed_end) {
            processEnd();
        }
    }
    return attempts;
}

int processEnd() {
    int is_good_end = goodEnd();
    if (!is_good_end) {
           readToEnd();
    }
    return is_good_end;
}

void printAttempts(int attempts) {
    switch (attempts) {
        case 0:
            printf("This is last attempt!\n");
            break;

        case 1:
            printf("%i attempt left!\n", attempts);
            break;

        default:
            printf("%i attempts left!\n", attempts);
            break;
    }
}

void startUserCycle(int attempts) {
    equationCoefs coefs;

    equationCoefs* pcoefs = &coefs;

    equationCoefsInitPointers(pcoefs);

    double eq_root1 = 0, eq_root2 = 0;

    while (attempts--) {
        printAttempts(attempts);
        resume();
        printf("Enter a, b, c:");
        int inputStatus = getInput(pcoefs);
        if (inputStatus != INPUT_CORRECT) {
            switch (inputStatus) {
                case INPUT_ERROR_FIRST:
                    printf("Error: cannot process first coef\n");
                    break;

                case INPUT_ERROR_SECOND:
                    printf("Error: cannot process second coef\n");
                    break;

                case INPUT_ERROR_THIRD:
                    printf("Error: cannot process third coef\n");
                    break;

                case INPUT_ERROR_AFTER_THIRD:
                    printf("Error: bad input after the third coef\n");
                    break;

                default:
                    printf("Unknown error during the input!\n");
                    break;
            }
            printf("Try another time!\n");
            continue;
        }
        // printf("%lf %lf %lf\n", a, b, c);

        int cntRoots = QuadraticSolver(coefs, &eq_root1, &eq_root2);
        int equationCheckStatus = checkRoots(coefs, cntRoots, eq_root1, eq_root2);

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
}

void greetings() {
    printf("Equation solver\n"
           "Powered by AK\n");
}

int getInput(equationCoefs* pcoefs) {
    int inputStatus = scanf("%lf %lf %lf", &(pcoefs->a), &(pcoefs->b), &(pcoefs->c));
    int is_good_end = processEnd();
    switch (inputStatus) {
        case 0:
            return INPUT_ERROR_FIRST;
            break;

        case 1:
            return INPUT_ERROR_SECOND;
            break;

        case 2:
            return INPUT_ERROR_THIRD;
            break;

        case 3:
            return is_good_end ? INPUT_CORRECT : INPUT_ERROR_AFTER_THIRD;
            break;

        default:
            return UNKNOWN_INPUT_ERROR;
            break;
    }
    return INPUT_CORRECT;
}

bool goodEnd() {
    char c = ' ';
    while ((c = (char)getchar()) == ' ' || c == '\t') {
        ;
    }
    return c == '\n' || c == EOF;
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

void readToEnd() {
    char c = ' ';
    while ((c = (char)getchar()) != '\n' && c != EOF) {
        ;
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

int checkRoots(const equationCoefs coefs, const int cntRoot, const double eq_root1, const double eq_root2) {
    switch (cntRoot) {
        case ZERO_ROOTS:
            return CHECK_SUCCESS;
            break;

        case ONE_ROOT:
            return (checkRoot(coefs, eq_root1)) ? CHECK_SUCCESS : CHECK_FAIL;
            break;

        case TWO_ROOTS:
            return (checkRoot(coefs, eq_root1) && checkRoot(coefs, eq_root2)) ? CHECK_SUCCESS : CHECK_FAIL;
            break;

        case INF_ROOTS:
            return (checkRoot(coefs, 0.0) && checkRoot(coefs, 1.0) && checkRoot(coefs, 2.0)) ? CHECK_SUCCESS : CHECK_FAIL;
            break;

        default:
            return CHECK_ERROR;
            break;
    }
}

int checkRoot(const equationCoefs coefs, const double x) {
    int cmp_double(const double, const double);

    double a = coefs.a, b = coefs.b, c = coefs.c;

    double result = a * x * x + b * x + c;

    return cmp_double(result, 0) == 0;
}
