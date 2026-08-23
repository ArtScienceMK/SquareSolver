
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <fstream>


#include "QuadraticSolver.h"
#include "CmpDouble.h"

const int INPUT_ERROR = 1;
const int MAX_STATEMENT_LEN = 9; // >= 8 to avoid warning
const int MAX_STATEMENTS = 10;
const int MAX_FORMAT_LINE_LEN = 50;
const int MAX_NUMBER_LEN = 20;

const char TRASH_CHAR = 'a';

enum INPUT_MODE_STATUS {
    SOLVING = 0,
    TESTING = 1,
    USER_INPUT_MODE_ERROR = 2
};

enum TESTING_MODE_STATUSES {
    MANUAL_TESTING = 0,
    FILE_TESTING = 1, // input from file
    TESTING_MODE_INPUT_ERROR = 2
};

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

struct statementInfo {
    const char* statements[MAX_STATEMENTS];
};

const struct statementInfo YES_NO = {{"yes", "no"}};

const statementInfo SOLVE_TEST = {{"solve", "test"}};

const statementInfo MANUAL_FILE = {{"manual", "file"}};

struct equationCoefsTest {
    equationCoefs coefs;

    int refCntRoots;
    double refRoot1, refRoot2;
};

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

void printEquationCoefsTest(equationCoefsTest test);

void equationCoefsInitPointers(equationCoefs* ptrCoefs);

void equationCoefsInitPointers(equationCoefs* ptrCoefs) {
    ptrCoefs->a = 0;
    ptrCoefs->b = 0;
    ptrCoefs->c = 0;
}

INPUT_MODE_STATUS userChoosingMode(void);

void chooseMode(INPUT_MODE_STATUS inputModeStatus);

TESTING_MODE_STATUSES userChoosingTestingMode(void);

void getFormatLine(char* formatLine, int scanfLen);
void getStatementLine(char *statementLine, statementInfo info);
void getTestingModeLine(char *testingModeLine);

void greetingsIntInput(void);
int getPositiveIntInputSafe(void);

int processEnd(void);
void startUserCycle(int attempts);

INPUT_STATUSES handleCoefInput(equationCoefs* ptrCoefs);
// void getModeLine(char* modeLine);

void greetings(void);
void printAttempts(int attempts);

const char* getStringInputStatus(INPUT_STATUSES);
INPUT_STATUSES getCoefInput(equationCoefs* ptrCoefs);

bool goodEnd(void);
void readToEnd(void);

void printRoots(const int cntRoots, const double eqRoot1, const double eqRoot2);
void getResumeLine(char* resumeLine);

bool resume(void);

RUN_ALL_TESTS_STATUSES manualTesting(void);

const char* getStringCheckStatus(CHECK_STATUSES);
CHECK_STATUSES checkRoots(const equationCoefs coefs, const int cntRoot, const double eqRoot1, const double eqRoot2);

int checkSingleRoot(const equationCoefs coefs, const double x);

const char* getStringRunAllTestsStatus(RUN_ALL_TESTS_STATUSES);
RUN_ALL_TESTS_STATUSES runAllTests(TESTING_MODE_STATUSES testingModeStatus);

const char* getStringTestCheckStatus(TEST_CHECK_STATUSES);
TEST_CHECK_STATUSES runSingleTest(equationCoefsTest test);

INPUT_STATUSES getRefInput(int* refCntRoots, double* refRoot1, double* refRoot2);
INPUT_STATUSES handleRefInput(equationCoefsTest* ptrTest);

int max(int op1, int op2);

int main() {
    greetings();
    INPUT_MODE_STATUS inputModeStatus = userChoosingMode();

    chooseMode(inputModeStatus);

    return 0;
}

void chooseMode(INPUT_MODE_STATUS inputModeStatus) {
    switch (inputModeStatus) {
        case SOLVING: {
            int attempts = getPositiveIntInputSafe();

            startUserCycle(attempts);

            break;
        }

        case TESTING: {
            TESTING_MODE_STATUSES testingModeStatus = userChoosingTestingMode();

            // chooseTestingMode(testingModeStatus);
            // TODO заменить if на switch ,вынести в ф-ю

            if (testingModeStatus != TESTING_MODE_INPUT_ERROR) {
                RUN_ALL_TESTS_STATUSES runAllTestsStatus = runAllTests(testingModeStatus);

                const char* LineRunAllTestsStatus = getStringRunAllTestsStatus(runAllTestsStatus);

                printf("%s", LineRunAllTestsStatus);
            } else {
                printf("Error: cannot process testing mode input\n");
            }

            break;
        }

        case USER_INPUT_MODE_ERROR: {
            printf("Cannot process input mode! Aborting...\n");

            break;
        }

        default: {
            printf("Error: unknown input mode error happened\n");

            break;
        }
    }
}

INPUT_MODE_STATUS userChoosingMode() {
    printf("Choose input mode:\n"
           "For solving type %s\n"
           "For testing type %s\n"
           "Choose mode:", SOLVE_TEST.statements[0],
            SOLVE_TEST.statements[1]);

    char modeLine[MAX_STATEMENT_LEN] = "";

    getStatementLine(modeLine, SOLVE_TEST);

    if (strcmp(modeLine, SOLVE_TEST.statements[0]) == 0) {

printf("(userChoosingMode) SOLVING\n");

        return SOLVING;
    }
    if (strcmp(modeLine, SOLVE_TEST.statements[1]) == 0) {

printf("(userChoosingMode) TESTING\n");


        return TESTING;
    }

printf("(userChoosingMode) USER_INPUT_MODE_ERROR\n");

    return USER_INPUT_MODE_ERROR;

}

// void getModeLine(char* modeLine) {
//     int cntInputCharsBefore = 0, cntInputCharsAfter = 0;
//
//     scanf("%n%5s%n", &cntInputCharsBefore, modeLine, &cntInputCharsAfter);
//
//     printf("(getModeLine) cntInputCharsBefore: %i, cntInputCharsAfter: %i\n",
//      cntInputCharsBefore, cntInputCharsAfter);
//
//     int diff = cntInputCharsAfter - cntInputCharsBefore;
//
//     if (diff != (int)strlen(SOLVE_INPUT_STATEMENT) &&
//      diff != (int)strlen(TEST_INPUT_STATEMENT)) {
//         cntInputCharsAfter = 0;
//     }
//
//     modeLine[cntInputCharsAfter] = '\0';
//
//     printf("(getModeLine) modeLine:%s\n", modeLine);
// }

TESTING_MODE_STATUSES userChoosingTestingMode() {
    printf("Choose testing mode:\n"
           "For manual testing type %s\n"
           "For testing from file type %s\n"
           "Choose testing mode:",
           MANUAL_FILE.statements[0], MANUAL_FILE.statements[1]);

    char testingModeLine[MAX_STATEMENT_LEN] = "";

    getStatementLine(testingModeLine, MANUAL_FILE);

    if (strcmp(testingModeLine, MANUAL_FILE.statements[0]) == 0) {
        return MANUAL_TESTING;
    }
    if (strcmp(testingModeLine, MANUAL_FILE.statements[1]) == 0) {
        return FILE_TESTING;
    }
    return TESTING_MODE_INPUT_ERROR;

}

// void getTestingModeLine(char* testingModeLine) {
//     int cntInputCharsBefore = 0, cntInputCharsAfter = 0;
//
//     scanf("%n%6s%n", &cntInputCharsBefore, testingModeLine, &cntInputCharsAfter);
//     printf("(getTestingModeLine) testingModeLine:%s\n", testingModeLine);
//     printf("(getTestingModeLine) len of testingModeLine:%i\n",
//      (int)strlen(testingModeLine));
//
//     //TODO понять, почему здесь 7, а не 6
//
//     printf("(getTestingModeLine) cntInputCharsBefore: %i, cntInputCharsAfter: %i\n",
//      cntInputCharsBefore, cntInputCharsAfter);
//
//     int diff = cntInputCharsAfter - cntInputCharsBefore;
//
//     if (diff != (int)strlen(MANUAL_TESTING_STATEMENT) &&
//      diff != (int)strlen(FILE_TESTING_STATEMENT)) {
//         cntInputCharsAfter = 0;
//     }
//
//     testingModeLine[cntInputCharsAfter] = '\0';
//
//     printf("(getTestingModeLine) testingModeLine:%s\n", testingModeLine);
// }

void getStatementLine(char* statementLine, statementInfo info) {
    int scanfLen = 0;

    char tempStatementLine [MAX_STATEMENT_LEN] = "";

    for (int i = 0; i < MAX_STATEMENTS && info.statements[i]; i++) {
        sprintf(tempStatementLine, "%s", info.statements[i]);

// printf("%s\n", info.statements[i]);
printf("(getStatementLine) info.statements[%i] have len%i\n",
        i, (int)strlen(info.statements[i]));

        scanfLen = max(scanfLen, (int)strlen(tempStatementLine));
    }

    printf("(getStatementLine) scanfLen:%i\n", scanfLen);

    char formatLine [MAX_FORMAT_LINE_LEN] = " %";

    getFormatLine(formatLine, scanfLen);

printf("(getStatementLine) formatLine:%s\n", formatLine);

    scanf(formatLine,  statementLine);

    if (!goodEnd()) {
        readToEnd();

printf("(getStatementLine) read to end!\n");

        char* ptrStatementLine = statementLine + (int)strlen(statementLine);
        *ptrStatementLine++ = TRASH_CHAR;
        *ptrStatementLine = '\0';
    }

    printf("(getStatementLine) statementLine:%s\n", statementLine);
    printf("(getStatementLine) len of statementLine:%i\n",
           (int)strlen(statementLine));

    // TODO scanf without checking specifiers!!!!
}

void getFormatLine(char* formatLine, int scanfLen) {
    char scanfLenLine [MAX_NUMBER_LEN] = "";

    sprintf(scanfLenLine, "%i", scanfLen);

    char *ptrFormatLine = formatLine + 2; // because space and % are already in string

    strcpy(ptrFormatLine, scanfLenLine);
    ptrFormatLine += strlen(scanfLenLine);
    *ptrFormatLine++ = 's';
    // *ptrFormatLine++ = ' '; // space added to cut spaces symbols
    // *ptrFormatLine++ = '\n';
    *ptrFormatLine = '\0';
}

void printEquationCoefsTest(equationCoefsTest test) {
    printf("Coefs: a: %lf, b: %lf, c: %lf\n"
           "refCntRoots: %i, refRoot1: %lf, refRoot2: %lf\n",
           test.coefs.a, test.coefs.b, test.coefs.c,
           test.refCntRoots, test.refRoot1, test.refRoot2);
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
    printf("-------------\n"
           "Equation solver\n"
           "Powered by AK\n"
           "-------------\n");
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

void getResumeLine(char * resumeLine) {
    int cntInputCharsBefore = 0, cntInputCharsAfter = 0;

    scanf("%n%3s%n", &cntInputCharsBefore, resumeLine, &cntInputCharsAfter);

    printf("(getResumeLine) cntInputCharsBefore: %i, cntInputCharsAfter: %i\n",
     cntInputCharsBefore, cntInputCharsAfter);

    int diff = cntInputCharsAfter - cntInputCharsBefore;

    if (diff != (int)strlen(YES_NO.statements[0]) &&
     diff != (int)strlen(YES_NO.statements[1])) {
        cntInputCharsAfter = 0;
    }

    resumeLine[cntInputCharsAfter] = '\0';

    printf("(getResumeLine) resumeLine:%s\n", resumeLine);
}

bool resume(void) {
    printf("Continue? %s / %s:", YES_NO.statements[0], YES_NO.statements[1]);
    char resumeLine[MAX_STATEMENT_LEN] = "";
    getResumeLine(resumeLine);

    // printf("resumeLine: %c%c%c%c\n", resumeLine[0], resumeLine[1], resumeLine[2], resumeLine[3]);

    if (strcmp(resumeLine, YES_NO.statements[0]) == 0) {
        return true;
    }
    if (strcmp(resumeLine, YES_NO.statements[1]) == 0) {
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


RUN_ALL_TESTS_STATUSES runAllTests(TESTING_MODE_STATUSES testingModeStatus) {

    switch (testingModeStatus) {
        case MANUAL_TESTING: {
            manualTesting();

            break;
        }

        case FILE_TESTING: {
            break;
        }

        case TESTING_MODE_INPUT_ERROR: {
            printf("Error: cannot process testing mode input\n");

            break;
        }

        default: {
            return RUN_ALL_TESTS_ERROR;

            break;
        }
    }

    return RUN_ALL_TESTS_SUCCESS;
}

RUN_ALL_TESTS_STATUSES manualTesting() {
    printf("Starting manual testing...\n");

    int testsLeft = getPositiveIntInputSafe();

    equationCoefsTest coefsTest;

    equationCoefs* ptrCoefs = &coefsTest.coefs;

    equationCoefsInitPointers(ptrCoefs);

    coefsTest.refCntRoots = 0;

    coefsTest.refRoot1 = NAN, coefsTest.refRoot2 = NAN;

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

        INPUT_STATUSES inputRefStatus =  handleRefInput(&coefsTest);

        if (inputRefStatus != INPUT_CORRECT) {
            printf("Try another time!\n");
            continue;
        }
        // TODO можно избавиться от inputRefStatus,сделав handle CoefInput(ptrCoefs, int MODE, ptrTest)

        printf("Starting testing a sample!\n");

        TEST_CHECK_STATUSES checkTestStatus = runSingleTest(coefsTest);

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

TEST_CHECK_STATUSES runSingleTest(equationCoefsTest test) {
    printf("(runSingleTest) ");
    printEquationCoefsTest(test);

    int CmpDouble(const double, const double);

    double eqRoot1 = NAN, eqRoot2 = NAN;

    int cntRoots = QuadraticSolver(&test.coefs, &eqRoot1, &eqRoot2);
    bool correct_test = true;

    if (test.refCntRoots != cntRoots) {
        correct_test = false;

        printf("Number of roots differs!\n"
               "Expected: %i            \n"
               "Got:      %i            \n", test.refCntRoots, cntRoots);
    }
    if (CmpDouble(test.refRoot1, eqRoot1) != 0 && !(isnan(test.refRoot1) && isnan(eqRoot1))) {
        correct_test = false;

        printf("First root differs!\n"
               "Expected: %lf      \n"
               "Got:      %lf      \n", test.refRoot1, eqRoot1);
    }
    if (CmpDouble(test.refRoot2, eqRoot2)  != 0 && !(isnan(test.refRoot2) && isnan(eqRoot2))) {
        correct_test = false;

        printf("Second root differs!\n"
               "Expected: %lf\n"
               "Got:      %lf\n", test.refRoot2, eqRoot2);
    }

    if (correct_test) {
        return TEST_CHECK_SUCCESS;
    }
    printf("a: %lf, b: %lf, c: %lf, cntRoots: %i,"
            "eqRoot1: %lf, eqRoot2: %lf, refCntRoots: %i, refRoot1:"
            "%lf, refRoot2: %lf\n", test.coefs.a, test.coefs.b, test.coefs.c, cntRoots,
            eqRoot1, eqRoot2, test.refCntRoots, test.refRoot1, test.refRoot2);

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

INPUT_STATUSES handleRefInput(equationCoefsTest* ptrTest) {
    printf("Enter refCntRoots, refRoot1, refRoot2:");

    INPUT_STATUSES inputStatus = getRefInput(&(ptrTest->refCntRoots),
     &(ptrTest->refRoot1), &(ptrTest->refRoot2));

    if (inputStatus != INPUT_CORRECT) {
        const char* inputStatusLine = getStringInputStatus(inputStatus);
        assert(inputStatusLine != NULL);
        printf("%s", inputStatusLine);
    }

    return inputStatus;
}

int max(int op1, int op2) {
    return (op1 > op2) ? op1 : op2;
}
