
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


#include "QuadraticSolver.h"
#include "CmpDouble.h"

const int INPUT_ERROR = 1;
const int MAX_RESUME_LEN = 9; // yes\n\0

enum CHECK_STATUSES {
    CHECK_SUCCESS = 1,
    CHECK_FAIL = 0,
    CHECK_ERROR = -1
};

enum INPUT_STATUSES {
    INPUT_ERROR_FIRST = 0,
    INPUT_ERROR_SECOND = 1,
    INPUT_ERROR_THIRD = 2,
    INPUT_ERROR_AFTER_THIRD = 3,
    UNKNOWN_INPUT_ERROR = 4,
    INPUT_CORRECT = 5
};

enum TEST_CHECK_STATUSES {
    TEST_CHECK_SUCCESS = 0,
    TEST_CHECK_FAIL = 1,
    TEST_CHECK_ERROR = 2
};

enum RUN_ALL_TESTS_STATUSES {
    RUN_ALL_TESTS_SUCCESS = 0,
    RUN_ALL_TESTS_ERROR = 1
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

    coefs.eqRoot1 = NAN;
    coefs.eqRoot2 = NAN;

    return coefs;
}

void equationCoefsInitPointers(equationCoefs* ptrCoefs);

void equationCoefsInitPointers(equationCoefs* ptrCoefs) {
    ptrCoefs->a = 0;
    ptrCoefs->b = 0;
    ptrCoefs->c = 0;
}

void greetingsIntInput(void);
int getPositiveIntInputSafe(void);

int processEnd(void);
void startUserCycle(int attempts);
INPUT_STATUSES handleCoefInput(equationCoefs* ptrCoefs);
void greetings(void);
void printAttempts(int attempts);

const char* getStringInputStatus(INPUT_STATUSES);
INPUT_STATUSES getCoefInput(equationCoefs* ptrCoefs);

bool goodEnd(void);
void readToEnd(void);

void printRoots(const int cntRoots, const double eqRoot1, const double eqRoot2);
void getresumeLine(char* resumeLine);

bool resume(void);

const char* getStringCheckStatus(CHECK_STATUSES);
CHECK_STATUSES checkRoots(const equationCoefs coefs, const int cntRoot, const double eqRoot1, const double eqRoot2);

int checkSingleRoot(const equationCoefs coefs, const double x);

const char* getStringRunAllTestsStatus(RUN_ALL_TESTS_STATUSES);
RUN_ALL_TESTS_STATUSES runAllTests();

const char* getStringTestCheckStatus(TEST_CHECK_STATUSES);
TEST_CHECK_STATUSES runSingleTest(equationCoefs* ptrCoefs, const int refCntRoots, const double refRoot1, const double refRoot2);

INPUT_STATUSES getRefInput(int* refCntRoots, double* refRoot1, double* refRoot2);
INPUT_STATUSES handleRefInput(int* refCntRoots, double* refRoot1, double* refRoot2);


int main() {
    greetings();

    RUN_ALL_TESTS_STATUSES runAllTestsStatus = runAllTests();

    // const char* LineRunAllTestsStatus = getStringRunAllTestsStatus(runAllTestsStatus);

    // printf("%s", LineRunAllTestsStatus);
    //int attempts = getPositiveIntInputSafe();

    //startUserCycle(attempts);

    return 0;
}

void greetingsIntInput() {
    printf("Enter the positive number:");
}

int getPositiveIntInputSafe() { // 2a
    int attempts = 0;
//gjgghbkjbn

    while (true) {
        greetingsIntInput();
        if (!scanf("%i", &attempts)) {
            readToEnd();
        } else if (processEnd() && attempts >= 0) {
            return attempts;
        }
    }
}

int processEnd() {
    int isGoodEnd = goodEnd();
    if (!isGoodEnd) {
           readToEnd();
    }
    return isGoodEnd;
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

    equationCoefs* ptrCoefs = &coefs;

    equationCoefsInitPointers(ptrCoefs);

    double eqRoot1 = NAN, eqRoot2 = NAN;

    while (attempts--) {
        printAttempts(attempts);

        if (!resume()) {
            break;
        }

        INPUT_STATUSES inputStatus = handleCoefInput(ptrCoefs);

        if (inputStatus != INPUT_CORRECT) {
            printf("Try another time!\n");
            continue;
        }
        // printf("%lf %lf %lf\n", a, b, c);
        int cntRoots = QuadraticSolver(ptrCoefs, &eqRoot1, &eqRoot2);

        CHECK_STATUSES equationCheckStatus = checkRoots(coefs, cntRoots, eqRoot1, eqRoot2);

        const char* equationCheckStatusLine = getStringCheckStatus(equationCheckStatus);

        printf("%s", equationCheckStatusLine);

        printRoots(cntRoots, eqRoot1, eqRoot2);
    }
}

void greetings() {
    printf("Equation solver\n"
           "Powered by AK\n");
}

const char* getStringInputStatus(INPUT_STATUSES inputStatus) {
    switch (inputStatus) {
        case INPUT_ERROR_FIRST:
            return "Error: cannot process first coef\n";
            break;

        case INPUT_ERROR_SECOND:
            return "Error: cannot process second coef\n";
            break;

        case INPUT_ERROR_THIRD:
            return "Error: cannot process third coef\n";
            break;

        case INPUT_ERROR_AFTER_THIRD:
            return "Error: bad input after third coef\n";
            break;

        case UNKNOWN_INPUT_ERROR:
            return "Error: unknown input error happened\n";
            break;

        case INPUT_CORRECT:
            return "Input correct\n";
            break;

        default:
            return "Error: unknown error happened while getting input status\n";
            break;
    }
    return NULL;
}

INPUT_STATUSES getCoefInput(equationCoefs* ptrCoefs) {
    int inputStatus = scanf("%lf %lf %lf", &(ptrCoefs->a), &(ptrCoefs->b), &(ptrCoefs->c));
    int isGoodEnd = processEnd();
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
            return isGoodEnd ? INPUT_CORRECT : INPUT_ERROR_AFTER_THIRD;
            break;

        default:
            return UNKNOWN_INPUT_ERROR;
            break;
    }
}

INPUT_STATUSES handleCoefInput(equationCoefs* ptrCoefs) {
    printf("Enter a, b, c:");

    INPUT_STATUSES inputStatus = getCoefInput(ptrCoefs);

    if (inputStatus != INPUT_CORRECT) {
        const char* inputStatusLine = getStringInputStatus(inputStatus);
        assert(inputStatusLine != NULL);
        printf("%s", inputStatusLine);
    }

    return inputStatus;
    // TODO
}

bool goodEnd() {
    char c = ' ';

    while (scanf("%c", &c) && (c == ' ' || c == '\t')) {
        ;
    }

    printf("(goodEnd) last char:%c\n", c);

    return c == '\n' || c == EOF;
}

void printRoots(const int cntRoots, const double eqRoot1, const double eqRoot2) {
    switch (cntRoots) {
        case ZERO_ROOTS:
            printf("No solutions\n");
            break;

        case ONE_ROOT:
            printf("Have one root: %lf\n", eqRoot1);
            break;

        case TWO_ROOTS:
            printf("Have two roots: %lf, %lf\n", eqRoot1, eqRoot2);
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

void getresumeLine(char * resumeLine) {
    int cntInputCharsBefore = 0, cntInputCharsAfter = 0;

    scanf("%n%3s%n", &cntInputCharsBefore, resumeLine, &cntInputCharsAfter);

    printf("(getresumeLine) cntInputCharsBefore: %i, cntInputCharsAfter: %i\n",
     cntInputCharsBefore, cntInputCharsAfter);

    int diff = cntInputCharsAfter - cntInputCharsBefore;

    if (diff != (int)strlen(RESUME_STATEMENT) && diff != (int)strlen(EXIT_STATEMENT)) {
        cntInputCharsAfter = 0;
    }

    resumeLine[cntInputCharsAfter] = '\0';

    printf("(getresumeLine) resumeLine:%s\n", resumeLine);
}

bool resume(void) {
    printf("Continue? %s / %s:", RESUME_STATEMENT, EXIT_STATEMENT);
    char resumeLine[MAX_RESUME_LEN] = {};
    getresumeLine(resumeLine);

    // printf("resumeLine: %c%c%c%c\n", resumeLine[0], resumeLine[1], resumeLine[2], resumeLine[3]);

    if (strcmp(resumeLine, RESUME_STATEMENT) == 0) {
        return true;
    }
    if (strcmp(resumeLine, EXIT_STATEMENT) == 0) {
        return false;
    }
    printf("Error: unknown error while resuming\n");
    return false;
}

CHECK_STATUSES checkRoots(const equationCoefs coefs, const int cntRoot, const double eqRoot1, const double eqRoot2) {
    switch (cntRoot) {
        case ZERO_ROOTS:
            return CHECK_SUCCESS;
            break;

        case ONE_ROOT:
            if (checkSingleRoot(coefs, eqRoot1)) {
                return CHECK_SUCCESS;
            } else {
                return CHECK_FAIL;
            }
            break;

        case TWO_ROOTS:
            return (checkSingleRoot(coefs, eqRoot1) && checkSingleRoot(coefs, eqRoot2)) ? CHECK_SUCCESS : CHECK_FAIL;
            break;

        case INF_ROOTS:
            return (checkSingleRoot(coefs, 0.0) && checkSingleRoot(coefs, 1.0) && checkSingleRoot(coefs, 2.0)) ? CHECK_SUCCESS : CHECK_FAIL;
            break;

        default:
            return CHECK_ERROR;
            break;
    }
}

const char* getStringCheckStatus(CHECK_STATUSES CheckStatus) {
    switch (CheckStatus) {
        case CHECK_SUCCESS:
            return "Both roots are correct\n";
            break;

        case CHECK_FAIL:
            return "Roots are incorrect\n";
            break;

        case CHECK_ERROR:
            return "Error: error happened when checking coefs\n";
            break;

        default:
            return "Error: unknown error happened while getting check status\n";
            break;
    }
}

int checkSingleRoot(const equationCoefs coefs, const double x) {
    int CmpDouble(const double, const double);

    double a = coefs.a, b = coefs.b, c = coefs.c;

    double result = a * x * x + b * x + c;

    return CmpDouble(result, 0) == 0;
}

RUN_ALL_TESTS_STATUSES runAllTests() {
    printf("Starting manual testing...\n");

    int testsLeft = getPositiveIntInputSafe();

    equationCoefs coefs;

    equationCoefs* ptrCoefs = &coefs;

    equationCoefsInitPointers(ptrCoefs);

    int refCntRoots = 0;

    double refRoot1 = NAN, refRoot2 = NAN;

    while (testsLeft--) { // for (int i = 0; i < testLeft; i++)
        printf("%i test left\n", testsLeft);

        if (!resume()) {
            break;
        }

        INPUT_STATUSES inputCoefStatus = handleCoefInput(ptrCoefs);

        if (inputCoefStatus != INPUT_CORRECT) {
            printf("Try another time!\n");
            continue;
        }

        INPUT_STATUSES inputRefStatus =  handleRefInput(&refCntRoots, &refRoot1, &refRoot2);

        if (inputRefStatus != INPUT_CORRECT) {
            printf("Try another time!\n");
            continue;
        }

        if (!processEnd()) {
            printf("Try another time!\n");
            continue;
        }

        TEST_CHECK_STATUSES checkTestStatus = runSingleTest(ptrCoefs, refCntRoots, refRoot1, refRoot2);

        const char* CheckTestStatusLine = getStringTestCheckStatus(checkTestStatus);
        printf("%s", CheckTestStatusLine);

        bool stopTestingCycle = false;

        switch (checkTestStatus) {
            case TEST_CHECK_SUCCESS:
                break;

            case TEST_CHECK_FAIL:
                stopTestingCycle = true;
                break;

            case TEST_CHECK_ERROR:
            default:
                return RUN_ALL_TESTS_ERROR;
                break;
        }
        if (stopTestingCycle) {
            break;
        }
    }
    return RUN_ALL_TESTS_SUCCESS;
}

TEST_CHECK_STATUSES runSingleTest(equationCoefs* ptrCoefs, const int refCntRoots,
                                  const double refRoot1, const double refRoot2) {
    int CmpDouble(const double, const double);

    double eqRoot1 = NAN, eqRoot2 = NAN;

    int cntRoots = QuadraticSolver(ptrCoefs, &eqRoot1, &eqRoot2);
    bool correct_test = true;

    if (refCntRoots != cntRoots) {
        correct_test = false;

        printf("Number of roots differs!\n"
               "Expected: %i            \n"
               "Got:      %i            \n", refCntRoots, cntRoots);
    }
    if (CmpDouble(refRoot1, eqRoot1) != 0) {
        correct_test = false;

        printf("First root differs!\n"
               "Expected: %lf      \n"
               "Got:      %lf      \n", refRoot1, eqRoot1);
    }
    if (CmpDouble(refRoot2, eqRoot2) != 0) {
        correct_test = false;

        printf("Second root differs!\n"
               "Expected: %lf\n"
               "Got:      %lf\n", refRoot2, eqRoot2);
    }

    if (correct_test) {
        return TEST_CHECK_SUCCESS;
    }
    printf("a: %lf, b: %lf, c: %lf, cntRoots: %i,"
            "eqRoot1: %lf, eqRoot2: %lf, refCntRoots: %i, refRoot1:"
            "%lf, refRoot2: %lf\n", ptrCoefs->a, ptrCoefs->b, ptrCoefs->c, cntRoots,
            eqRoot1, eqRoot2, refCntRoots, refRoot1, refRoot2);

    return TEST_CHECK_FAIL;
}

const char* getStringTestCheckStatus(TEST_CHECK_STATUSES TestCheckStatus) {
    switch (TestCheckStatus) {
        case TEST_CHECK_SUCCESS:
            return "Test passed successfully!\n";
            break;

        case TEST_CHECK_FAIL:
            return "Test gives incorrect answer!\n";
            break;

        case TEST_CHECK_ERROR:
            return "Error: error happened while testing!\n";
            break;

        default:
            return "Error: unknown error happened while getting test check status!\n";
            break;
    }
}

const char* getStringRunAllTestsStatus(RUN_ALL_TESTS_STATUSES RunAllTestsStatus) {
    switch (RunAllTestsStatus) {
        case RUN_ALL_TESTS_SUCCESS:
            return "All tests passed successfully!\n";
            break;

        case RUN_ALL_TESTS_ERROR:
            return "Error: error happened while running all tests!\n";
            break;

        default:
            return "Error: unknown error happened while getting RunAllTests status!\n";
            break;
    }
}

INPUT_STATUSES getRefInput(int* refCntRoots, double* refRoot1, double* refRoot2) {
    int inputStatus = scanf("%i %lf %lf", refCntRoots, refRoot1, refRoot2);
    int isGoodEnd = processEnd();
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
            return isGoodEnd ? INPUT_CORRECT : INPUT_ERROR_AFTER_THIRD;
            break;

        default:
            return UNKNOWN_INPUT_ERROR;
            break;
    }
}

INPUT_STATUSES handleRefInput(int* refCntRoots, double* refRoot1, double* refRoot2) {
    printf("Enter refCntRoots, refRoot1, refRoot2:");

    INPUT_STATUSES inputStatus = getRefInput(refCntRoots, refRoot1, refRoot2);

    if (inputStatus != INPUT_CORRECT) {
        const char* inputStatusLine = getStringInputStatus(inputStatus);
        assert(inputStatusLine != NULL);
        printf("%s", inputStatusLine);
    }

    return inputStatus;
}
