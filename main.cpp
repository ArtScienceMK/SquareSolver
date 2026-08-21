
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


#include "QuadraticSolver.h"

const int INPUT_ERROR = 1;
const int BAD_INPUT = 1;
const int MAX_RESUME_LEN = 5; // yes\n\0

enum CHECK_STATUS {
    CHECK_SUCCESS = 1,
    CHECK_FAIL = 0,
    CHECK_ERROR = -1
};

enum CNT_ROOTS {
    ZERO_ROOTS = 0,
    ONE_ROOT = 1,
    TWO_ROOTS = 2,
    INF_ROOTS = 3
};

enum INPUT_STATUSES {
    INPUT_ERROR_FIRST = 0,
    INPUT_ERROR_SECOND = 1,
    INPUT_ERROR_THIRD = 2,
    INPUT_ERROR_AFTER_THIRD = 3,
    UNKNOWN_INPUT_ERROR = 4,
    INPUT_CORRECT = 5
};

enum RESUME_RESULTS {
    EXIT_RESULT = 0,
    RESUME_RESULT = 1,
    UNKNOWN_ERROR_RESULT = 2
};


const char* RESUME_STATEMENT = "yes";
const char* EXIT_STATEMENT = "no";

struct equationCoefs equationCoefsInit(void);

struct equationCoefs equationCoefsInit() {
    struct equationCoefs coefs;
    coefs.a = 0;
    coefs.b = 0;
    coefs.c = 0;

    coefs.cntRoot = 0;

    coefs.eq_root1 = 0;
    coefs.eq_root2 = 0;

    return coefs;
}

void equationCoefsInitPointers(equationCoefs* ptr_coefs);

void equationCoefsInitPointers(equationCoefs* ptr_coefs) {
    ptr_coefs->a = 0;
    ptr_coefs->b = 0;
    ptr_coefs->c = 0;
}

char RESUME_LINE[MAX_RESUME_LEN];

void greetings_attempts(void);
int inputAttempts(void);

int processEnd(void);
void startUserCycle(int attempts);
void greetings(void);
void printAttempts(int attempts);
INPUT_STATUSES getInput(equationCoefs* ptr_coefs);
bool goodEnd(void);
void readToEnd(void);

void printRoots(const int cntRoots, const double eq_root1, const double eq_root2);
int badInput(void);
void getResumeLine(void);
RESUME_RESULTS resume(void);
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

int inputAttempts() { // 2a
    int attempts = 0;
//gjgghbkjbn

    while (true) {
        greetings_attempts();
        if (!scanf("%i", &attempts)) {
            readToEnd();
        } else if (processEnd() && attempts >= 0) {
            return attempts;
        }
    }
}

int processEnd() {
    int is_good_end = goodEnd();
    if (!is_good_end) {
           readToEnd();
    }
    return is_good_end;
}

void printAttempts(int attempts) {
    assert(attempts >= 0);

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

    equationCoefs* ptr_coefs = &coefs;

    equationCoefsInitPointers(ptr_coefs);

    double eq_root1 = 0, eq_root2 = 0;

    while (attempts--) {
        printAttempts(attempts);
        bool stopImmediately = false;
        RESUME_RESULTS resumeResult = resume();
        switch (resumeResult) {
            case EXIT_RESULT:
                stopImmediately = true;
                break;

            case RESUME_RESULT:
                break;

            case UNKNOWN_ERROR_RESULT:
                printf("Got an unknown error during resume() function!\n");
                break;

            default:
                printf("Got an unknown error in StartUserCycle() function!\n");
                break;
        }
        if (stopImmediately) {
            break;
        }
        printf("Enter a, b, c:");
        INPUT_STATUSES inputStatus = getInput(ptr_coefs);
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

            case INPUT_CORRECT:
                printf("Input correct\n");
                break;

            case UNKNOWN_INPUT_ERROR:
                printf("Unknown input error!\n");
                break;

            default:
                printf("Unknown error during the input!\n");
                break;
        }
        printf("Try another time!\n");
        continue;
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

INPUT_STATUSES getInput(equationCoefs* ptr_coefs) {
    int inputStatus = scanf("%lf %lf %lf", &(ptr_coefs->a), &(ptr_coefs->b), &(ptr_coefs->c));
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
    while (scanf("%c", &c) && (c == ' ' || c == '\t')) {
        ;
    }
    printf("(goodEnd) last char:%c\n", c);
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
    char c = 'A';
    while ((c = (char)getchar()) && c != '\n') {
        ;
    }
    int res = c == '\n';
    printf("(readToEnd) c == backslashn:%i\n", res);
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

void getResumeLine() {
    int cnt_input_chars_before = 0, cnt_input_chars_after = 0;
    scanf("%n%3s%n", &cnt_input_chars_before, RESUME_LINE, &cnt_input_chars_after);
    if (cnt_input_chars_after - cnt_input_chars_before != 0) {
        cnt_input_chars_after = 0;
    }
    RESUME_LINE[cnt_input_chars_after] = '\0';
    printf("(getResumeLine) RESUME_LINE:%s", RESUME_LINE);
}

RESUME_RESULTS resume(void) {
    printf("Continue? %s / %s:", RESUME_STATEMENT, EXIT_STATEMENT);

    getResumeLine();
    if (strcmp(RESUME_LINE, RESUME_STATEMENT) == 0) {
        return RESUME_RESULT;
    } else if (strcmp(RESUME_LINE, EXIT_STATEMENT) == 0) {
        return EXIT_RESULT;
    } else {
        return UNKNOWN_ERROR_RESULT;
    }
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
