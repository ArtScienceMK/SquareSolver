
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <fstream>
#include "raylib.h"

#include "FunctionGraphic.h"
#include "QuadraticSolver.h"
#include "CmpDouble.h"

#include "lib.h"
#include "test.h"

int main() {
    greetings();
    INPUT_MODE_STATUS inputModeStatus = userChoosingMode();

    chooseMode(inputModeStatus);

    return 0;
}

//  Start of functions, declared in lib.h
//  Functions to work with structs
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

void equationCoefsInitPointers(equationCoefs* ptrCoefs) {
    ptrCoefs->a = 0;
    ptrCoefs->b = 0;
    ptrCoefs->c = 0;
}

void printEquationCoefsTest(equationCoefsTest test) {
    printf("Coefs: a: %lf, b: %lf, c: %lf\n"
           "refCntRoots: %i, refRoot1: %lf, refRoot2: %lf\n",
           test.coefs.a, test.coefs.b, test.coefs.c,
           test.refCntRoots, test.refRoot1, test.refRoot2);
} 

//  UI main menu functions

void greetings() {
    printf("-------------\n"
           "Equation solver\n"
           "Powered by AK\n"
           "-------------\n");
}

INPUT_MODE_STATUS userChoosingMode() {
    printf("Choose input mode:\n"
           "For solving type %s\n"
           "For testing type %s\n"
           "For graphical mode type %s\n"
           "Choose mode:", SOLVE_TEST_GRAPHICAL.statements[0],
            SOLVE_TEST_GRAPHICAL.statements[1], 
            SOLVE_TEST_GRAPHICAL.statements[2]);

    char modeLine[MAX_STATEMENT_LEN] = "";

    getStatementLine(modeLine, SOLVE_TEST_GRAPHICAL);

    if (strcmp(modeLine, SOLVE_TEST_GRAPHICAL.statements[0]) == 0) {

ON_DEBUG(printf("%s(userChoosingMode) SOLVING\n%s", MY_YELLOW, MY_RESET));

        return SOLVING;
    }
    if (strcmp(modeLine, SOLVE_TEST_GRAPHICAL.statements[1]) == 0) {

ON_DEBUG(printf("%s(userChoosingMode) TESTING\n%s", MY_YELLOW, MY_RESET));

        return TESTING;
    }

    if (strcmp(modeLine, SOLVE_TEST_GRAPHICAL.statements[2]) == 0) {

ON_DEBUG(printf("%s(userChoosingMode) GRAPHICAL\n%s", MY_YELLOW, MY_RESET));

        return FUNCTION_GRAPHIC;
    }

ON_DEBUG(printf("%s(userChoosingMode) USER_INPUT_MODE_ERROR\n%s", MY_YELLOW, MY_RESET));

    return USER_INPUT_MODE_ERROR;

}

void chooseMode(INPUT_MODE_STATUS inputModeStatus) {
    switch (inputModeStatus) {
        case SOLVING: {
            int attempts = getPositiveIntInputSafe();

            solveRealTime(attempts);

            break;
        }

        case TESTING: {
            TESTING_MODE_STATUSES testingModeStatus = userChoosingTestingMode();

            // chooseTestingMode(testingModeStatus);
            // TODO заменить if на switch ,вынести в ф-ю

            if (testingModeStatus != TESTING_MODE_INPUT_ERROR) {
                RUN_ALL_TESTS_STATUSES runAllTestsStatus = runAllTests(testingModeStatus);

                const char* const LineRunAllTestsStatus = getStringRunAllTestsStatus(runAllTestsStatus);
                
                if (runAllTestsStatus != RUN_ALL_TESTS_SUCCESS) {
                    printf("%s%s%s", MY_RED, LineRunAllTestsStatus, MY_RESET);
                } else {
                    printf("%s%s%s", MY_GREEN, LineRunAllTestsStatus, MY_RESET);
                }

            } else {
                printf("%sError: cannot process testing mode input\n%s", MY_RED, MY_RESET);
            }

            break;
        }

        case FUNCTION_GRAPHIC: {
            buildFunctionGraphic();

            break;
        }

        case USER_INPUT_MODE_ERROR: {
            printf("%sError: cannot process input mode! Bye...\n%s", MY_RED, MY_RESET);

            break;
        }


        default: {
            printf("%sError: unknown input mode error happened\n%s", MY_RED, MY_RESET);

            break;
        }
    }
}

TESTING_MODE_STATUSES userChoosingTestingMode() {
    printf("Choose testing mode:\n"
           "For manual testing type %s\n"
           "For testing from file type %s\n"
           "For auto testing on prepared tests type %s\n"
           "Choose testing mode:",
           MANUAL_FILE_AUTO.statements[0], MANUAL_FILE_AUTO.statements[1],
           MANUAL_FILE_AUTO.statements[2]);

    char testingModeLine[MAX_STATEMENT_LEN] = "";

    getStatementLine(testingModeLine, MANUAL_FILE_AUTO);

    if (strcmp(testingModeLine, MANUAL_FILE_AUTO.statements[0]) == 0) {
        return MANUAL_TESTING;
    }

    if (strcmp(testingModeLine, MANUAL_FILE_AUTO.statements[1]) == 0) {
        return FILE_TESTING;
    }

    if (strcmp(testingModeLine, MANUAL_FILE_AUTO.statements[2]) == 0) {
        return UNIT_TESTING;
    }

    return TESTING_MODE_INPUT_ERROR;

}

//  Functions to handle User text input
void getFormatLine(char* formatLine, size_t scanfLen) {
    snprintf(formatLine, MAX_NUMBER_LEN, " %%%zus", scanfLen);
}

void getStatementLine(char* statementLine, statementInfo info) {
    size_t scanfLen = 0;

    char tempStatementLine [MAX_STATEMENT_LEN] = "";

    for (int i = 0; i < MAX_STATEMENTS && info.statements[i]; i++) {
        snprintf(tempStatementLine, MAX_STATEMENT_LEN, "%s", info.statements[i]);

ON_DEBUG(printf("%s%s\n%s", MY_YELLOW, info.statements[i], MY_RESET));
ON_DEBUG(printf("%s(getStatementLine) info.statements[%i] have len%i\n%s",
        MY_YELLOW, i, (int)strlen(info.statements[i]), MY_RESET));

        scanfLen = max_size_t(scanfLen, strlen(tempStatementLine));
    }

ON_DEBUG(printf("%s(getStatementLine) scanfLen:%zu\n%s",
        MY_YELLOW, scanfLen, MY_RESET));

    char formatLine [MAX_FORMAT_LINE_LEN] = "";

    getFormatLine(formatLine, scanfLen);

ON_DEBUG(printf("%s(getStatementLine) formatLine:%s\n%s",
        MY_YELLOW, formatLine, MY_RESET));

    scanf(formatLine,  statementLine);

    if (!goodEnd()) {
        readToEnd();

ON_DEBUG(printf("%s(getStatementLine) read to end!\n%s", 
        MY_YELLOW, MY_RESET));

        char* ptrStatementLine = statementLine + (int)strlen(statementLine);
        *ptrStatementLine++ = TRASH_CHAR;
        *ptrStatementLine = '\0';
    }

ON_DEBUG(printf("%s(getStatementLine) statementLine:%s\n%s",
        MY_YELLOW, statementLine, MY_RESET));
ON_DEBUG(printf("%s(getStatementLine) len of statementLine:%i\n%s",
        MY_YELLOW, (int)strlen(statementLine), MY_RESET));
}

void getResumeLine(char * resumeLine) {
    int cntInputCharsBefore = 0, cntInputCharsAfter = 0;

    scanf("%n%3s%n", &cntInputCharsBefore, resumeLine, &cntInputCharsAfter); //TODO edit format line, delete %n

ON_DEBUG(printf("%s(getResumeLine) cntInputCharsBefore: %i, cntInputCharsAfter: %i\n%s",
        MY_YELLOW, cntInputCharsBefore, cntInputCharsAfter, MY_RESET);)

    int diff = cntInputCharsAfter - cntInputCharsBefore;

    if (diff != (int)strlen(YES_NO.statements[0]) &&
     diff != (int)strlen(YES_NO.statements[1])) {
        cntInputCharsAfter = 0;
    }

    resumeLine[cntInputCharsAfter] = '\0';

ON_DEBUG(printf("%s(getResumeLine) resumeLine:%s\n%s",
        MY_YELLOW, resumeLine, MY_RESET);)

}


#pragma GCC diagnostic ignored "-Wignored-qualifiers"

const char* const getStringCheckStatus(CHECK_STATUSES CheckStatus) {
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

#pragma GCC diagnostic warning "-Wignored-qualifiers"

//  Functions to process User input lines
int processEnd() {
    int isGoodEnd = goodEnd();
    if (!isGoodEnd) {
        readToEnd();
    }
    return isGoodEnd;
}

bool goodEnd() {
    char c = ' ';

    while (scanf("%c", &c) && (c == ' ' || c == '\t')) {
        ;
    }

ON_DEBUG(printf("%s(goodEnd) last char:%c\n%s",
        MY_YELLOW, c, MY_RESET));

    return c == '\n' || c == EOF;
}

void readToEnd() {
    char c = 'A';

    while ((c = (char)getchar()) && c != '\n') {
        ;
    }

    int res = c == '\n';

ON_DEBUG(printf("%s(readToEnd) c == backslashn:%i\n%s",
        MY_YELLOW, res, MY_RESET));
}

//  Functions to handle User attempts input
void greetingsIntInput() {
    printf("Enter the positive number:");
}

int getPositiveIntInputSafe() {
    int attempts = 0;

    while (true) {
        greetingsIntInput();
        if (!scanf("%i", &attempts)) {
            readToEnd();
        } else if (processEnd() && attempts >= 0) {
            return attempts;
        }
    }

    //tests, causing errrors in eraly versions:
    //gjgghbkjbn
    //2a
}

//  Functions, used in real time solving case
// 1.Interaction&Solve
void solveRealTime(int attempts) {
    equationCoefs coefs;

    equationCoefs* ptrCoefs = &coefs;

    equationCoefsInitPointers(ptrCoefs);

    double eqRoot1 = NAN, eqRoot2 = NAN;

    while (attempts--) {
        printAttempts(attempts);

        if (!resume()) {
            break;
        }

        INPUT_STATUSES inputStatus = fileCoefInput(ptrCoefs, stdin);

        if (inputStatus != INPUT_CORRECT) {
            printf("Try another time!\n");
            continue;
        }

ON_DEBUG(printf("%s%lf %lf %lf\n%s", MY_YELLOW, 
        ptrCoefs->a, ptrCoefs->b, ptrCoefs->c, MY_RESET));

        QUADRATIC_SOLVER_STATUSES cntRoots = QuadraticSolver(ptrCoefs, &eqRoot1, &eqRoot2);

        CHECK_STATUSES equationCheckStatus = checkRoots(coefs, cntRoots, eqRoot1, eqRoot2);


        const char* const equationCheckStatusLine = getStringCheckStatus(equationCheckStatus);
        
        if (equationCheckStatus != CHECK_SUCCESS) {
            printf("%s%s%s", MY_RED, equationCheckStatusLine, MY_RESET);
        } else {
            printf("%s%s%s", MY_GREEN, equationCheckStatusLine, MY_RESET);
        }

        printRoots(cntRoots, eqRoot1, eqRoot2);
    }
}

bool resume(void) {
    printf("Continue? %s / %s:", YES_NO.statements[0], YES_NO.statements[1]);

    char resumeLine[MAX_STATEMENT_LEN] = "";

    getResumeLine(resumeLine);

ON_DEBUG(printf("%s(resume) resumeLine: %c%c%c%c\n%s",
        MY_YELLOW, resumeLine[0], resumeLine[1], resumeLine[2], resumeLine[3], MY_RESET));

    if (strcmp(resumeLine, YES_NO.statements[0]) == 0) {
        return true;
    }
    if (strcmp(resumeLine, YES_NO.statements[1]) == 0) {
        return false;
    }
    printf("%sError: unknown error while resuming\n%s", MY_RED, MY_RESET);
    return false;
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

//  2.Check roots & print them
CHECK_STATUSES checkRoots(const equationCoefs coefs, QUADRATIC_SOLVER_STATUSES cntRoots, const double eqRoot1, const double eqRoot2) {
    switch (cntRoots) {
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

int checkSingleRoot(const equationCoefs coefs, const double x) {
    double a = coefs.a, b = coefs.b, c = coefs.c;

    double result = a * x * x + b * x + c;

    return CmpDouble(result, 0) == 0;
}

void printRoots(QUADRATIC_SOLVER_STATUSES cntRoots, const double eqRoot1, const double eqRoot2) {
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
            printf("%sHave error in quadratic solver\n%s", MY_RED, MY_RESET);
            break;

        default:
            printf("%sError: have %i roots\n%s", MY_RED, cntRoots, MY_RESET);
            break;
    }
}

//Real time solving case ended

// Functions, handling coefficients input
#pragma GCC diagnostic ignored "-Wignored-qualifiers"

const char* const getStringInputStatus(INPUT_STATUSES inputStatus) {
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

        case INPUT_ERROR_BAD_NUMBERS:
            return "Error: bad input coefs\n";
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

#pragma GCC diagnostic warning "-Wignored-qualifiers"

INPUT_STATUSES getCoefInput(equationCoefs* ptrCoefs, FILE* ptrFile) {
    int inputStatus = fscanf(ptrFile, "%lf %lf %lf", &(ptrCoefs->a), &(ptrCoefs->b), &(ptrCoefs->c));
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
            if (!isGoodEnd) {
                return INPUT_ERROR_AFTER_THIRD;
            }

            if (!isGoodCoefs(ptrCoefs)) {
                return INPUT_ERROR_BAD_NUMBERS;
            }

            return  INPUT_CORRECT;
            
            break;

        default:
            return UNKNOWN_INPUT_ERROR;
            break;
    }
}

//  Functions for testing
#pragma GCC diagnostic ignored "-Wignored-qualifiers"

const char* const getStringRunAllTestsStatus(RUN_ALL_TESTS_STATUSES RunAllTestsStatus) {
    switch (RunAllTestsStatus) {
        case RUN_ALL_TESTS_SUCCESS:
            return "All tests passed without testing errors!\n";
            break;

        case RUN_ALL_TESTS_ERROR:
            return "Error: error happened while running all tests!\n";
            break;

        default:
            return "Error: unknown error happened while getting RunAllTests status!\n";
            break;
    }
}

#pragma GCC diagnostic warning "-Wignored-qualifiers"

RUN_ALL_TESTS_STATUSES runAllTests(TESTING_MODE_STATUSES testingModeStatus) {
    switch (testingModeStatus) {
        case MANUAL_TESTING: {
            return userTesting(MANUAL_FILE_AUTO.statements[0]);

            break;
        }

        case FILE_TESTING: {
            return userTesting(MANUAL_FILE_AUTO.statements[1]);

            break;
        }

        case UNIT_TESTING: {
            return unitTesting();

            break;
        }

        case TESTING_MODE_INPUT_ERROR: {
            printf("%sError: cannot process testing mode input\n%s", MY_RED, MY_RESET);
            
            return RUN_ALL_TESTS_ERROR;

            break;
        }

        default: {
            return RUN_ALL_TESTS_ERROR;

            break;
        }
    }

    return RUN_ALL_TESTS_SUCCESS;
}

#pragma GCC diagnostic ignored "-Wignored-qualifiers"

const char* const getStringTestCheckStatus(TEST_CHECK_STATUSES TestCheckStatus) {
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

#pragma GCC diagnostic warning "-Wignored-qualifiers"

TEST_CHECK_STATUSES runSingleTest(equationCoefsTest test) {
ON_DEBUG(printf("%s(runSingleTest)%s", 
        MY_YELLOW, MY_RESET));

    printEquationCoefsTest(test);

    double eqRoot1 = NAN, eqRoot2 = NAN;

    QUADRATIC_SOLVER_STATUSES cntRoots = QuadraticSolver(&test.coefs, &eqRoot1, &eqRoot2);
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

//  Function for manual&file testing
RUN_ALL_TESTS_STATUSES userTesting(const char* const mode) {

    equationCoefsTest coefsTest;

    equationCoefs* ptrCoefs = &coefsTest.coefs;

    equationCoefsInitPointers(ptrCoefs);

    coefsTest.refCntRoots = ZERO_ROOTS;

    coefsTest.refRoot1 = NAN, coefsTest.refRoot2 = NAN;


    printf("Starting %s testing...\n", mode);

    FILE* ptrFile = stdin;

    if (mode == MANUAL_FILE_AUTO.statements[1]) {
        ptrFile = acessFile("Enter the name of the file with tests:", "r");

        if (!ptrFile) {
            return RUN_ALL_TESTS_ERROR;
        }
    }

    int testsLeft = 0;

    if (ptrFile != stdin && !fscanf(ptrFile, "%i", &testsLeft)) {
        printf("Cannot read test number!\n");
        return RUN_ALL_TESTS_ERROR;
    }
    if (ptrFile == stdin) {
        testsLeft = getPositiveIntInputSafe();
    }

    for (int i = 0; i < testsLeft; i++) {

        if (ptrFile == stdin && !resume()) {
            break;
        }

        INPUT_STATUSES inputCoefStatus = fileCoefInput(ptrCoefs, ptrFile);

        if (inputCoefStatus != INPUT_CORRECT) {
            printf("Try another time!\n");
            if (ptrFile != stdin) {
                printf("%sError: cannot process coefs in test number %i . Skipping...%s", MY_RED, i, MY_RESET);
            }
            continue;
        }

        if (ptrFile == stdin) {
            printf("Enter refCntRoots, refRoot1, refRoot2:");
        }

        INPUT_STATUSES inputRefStatus =  fileRefInput(&coefsTest, ptrFile);

        if (inputRefStatus != INPUT_CORRECT) {
            printf("Try another time!\n");
            if (ptrFile != stdin) {
                printf("%sError: cannot process reference values in test number %i . Skipping...%s", MY_RED, i, MY_RESET);
            }
            continue;
        }

        TEST_CHECK_STATUSES checkTestStatus = runSingleTest(coefsTest);

        const char* const CheckTestStatusLine = getStringTestCheckStatus(checkTestStatus);
        printf("%s", CheckTestStatusLine);

        bool stopTestingCycle = false; // TODO a flag, that specifies the behaviour when caught error while testing (break or continue)

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

//  Function for unit testing
RUN_ALL_TESTS_STATUSES unitTesting() {
    int TESTSize = sizeof(TESTS) / sizeof(TESTS[0]);

    for (int i = 0; i < TESTSize; i++) {
        TEST_CHECK_STATUSES checkTestStatus = runSingleTest(TESTS[i]);

        const char* const CheckTestStatusLine = getStringTestCheckStatus(checkTestStatus);
        printf("%s", CheckTestStatusLine);

        switch (checkTestStatus) {
            case TEST_CHECK_SUCCESS:
                break;

            case TEST_CHECK_FAIL:
                printf("Test number:%i has incorrect answer\n"
                       "Info about test:", i);

                printEquationCoefsTest(TESTS[i]);

                break;

            case TEST_CHECK_ERROR:
            default:
                return RUN_ALL_TESTS_ERROR;
                break;
        }
    }
    return RUN_ALL_TESTS_SUCCESS;
}

//  Function for graphic function case
void buildFunctionGraphic() {
    FILE* ptrFile = acessFile("Enter the filename to store logs:", "w");

    if (!ptrFile) {
        ptrFile = stdout;
        fprintf(ptrFile, "Warning: cannot open the debug output file! Redirecting to console...\n");
    }

    equationCoefs coefs;

    equationCoefs* ptrCoefs = &coefs;

    equationCoefsInitPointers(ptrCoefs);
    
    bool isBadInput = true;

    while (isBadInput) {
        printf("Enter a, b, c:");
    
        fscanf(stdin, "%lf %lf %lf", &(ptrCoefs->a), &(ptrCoefs->b), &(ptrCoefs->c));
        
        if (isfinite(ptrCoefs->a) && isfinite(ptrCoefs->b) && isfinite(ptrCoefs->c)) {
            isBadInput = false;
        } else {
            printf("Cannot process input! Try again!\n");
        }

        fprintf(ptrFile, "a:%lf, b:%lf, c:%lf\n", ptrCoefs->a, ptrCoefs->b, ptrCoefs->c);
    }

    SetTargetFPS(60);
    InitWindow(2000, 2000, "Sample");

    double zoomCoef = 1.0;
    double gridOffset = 20;
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        drawGrid(gridOffset, zoomCoef);
        drawFunctionGraphic(ptrCoefs, -100, 100, 1, zoomCoef, ptrFile);
        drawAxes(zoomCoef);
        drawScale(zoomCoef);
        printParabolaApex(ptrCoefs, zoomCoef, ptrFile);
        
        zoomCoef += GetMouseWheelMoveV().y;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            drawMouseLines();
            // printMouseCoords();

            // Vector2 mouseCoords = GetMousePosition();
            // DrawLine(XMIN, mouseCoords.y, XMAX, mouseCoords.y, RED);
        }
        EndDrawing();
    }
    CloseWindow();
}

// Working with files functions
FILE* acessFile(const char* const greetings, const char* const mode) {
    printf("%s", greetings);
    char filename [MAX_FILENAME_LEN] = "";

    scanf("%s", filename);

    FILE* ptrFile = fopen(filename, mode);

    if (ptrFile == NULL) {
        printf("%sError: cannot open file %s\n%s", MY_RED, filename, MY_RESET);
    } else {
        printf("%sFile opened sucessfully!\n%s", MY_GREEN, MY_RESET);
    }

    return ptrFile;
}

INPUT_STATUSES fileCoefInput(equationCoefs* ptrCoefs, FILE* ptrFile) {

    if (ptrFile == stdin) {
        printf("Enter a, b, c:");
    }

    INPUT_STATUSES inputStatus = getCoefInput(ptrCoefs, ptrFile);

ON_DEBUG(printf("%s%sa:%lf, b:%lf, c:%lf%s",
        MY_YELLOW, ptrCoefs->a, ptrCoefs->b, ptrCoefs->c, MY_RESET));
    if (inputStatus != INPUT_CORRECT) {
        const char* const inputStatusLine = getStringInputStatus(inputStatus);
        assert(inputStatusLine != NULL);
        printf("%s%s%s", MY_RED, inputStatusLine, MY_RESET);
    }
    return inputStatus;
}

INPUT_STATUSES fileRefInput(equationCoefsTest* ptrTest, FILE* ptrFile) {
    INPUT_STATUSES inputStatus = getRefInput(ptrTest, ptrFile);

    if (inputStatus != INPUT_CORRECT) {
        const char* const inputStatusLine = getStringInputStatus(inputStatus);
        assert(inputStatusLine != NULL);
        printf("%s", inputStatusLine);
    }

    return inputStatus;
}

INPUT_STATUSES getRefInput(equationCoefsTest* ptrTest, FILE* ptrFile) {
    int inputStatus = fscanf(ptrFile, "%i %lf %lf",
    &(ptrTest->refCntRoots), &(ptrTest->refRoot1), &(ptrTest->refRoot2));

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
            if (!isGoodEnd) {
                return INPUT_ERROR_AFTER_THIRD;
            }

            if (!isGoodRef(ptrTest)) {
                return INPUT_ERROR_BAD_NUMBERS;
            }

            return INPUT_CORRECT;
            break;

        default:
            return UNKNOWN_INPUT_ERROR;
            break;
    }
}

//  Functions for checking User numeric input
bool isGoodCoefs(equationCoefs* ptrCoefs) {
    return isfinite(ptrCoefs->a) && isfinite(ptrCoefs->b) && isfinite(ptrCoefs->c); // nan также учитывает
}

bool isGoodRef(equationCoefsTest* ptrTest) {
    QUADRATIC_SOLVER_STATUSES refCntRoots = ptrTest->refCntRoots;
    
    switch (refCntRoots) {
        case ZERO_ROOTS:
            break;

        case ONE_ROOT:
            break;

        case TWO_ROOTS:
            break;

        case INF_ROOTS:
            break;

        default:
            return false;
            break;
    }

    return !isinf(ptrTest->refRoot1) && !isinf(ptrTest->refRoot2);
}

// Math functions
size_t max_size_t(size_t op1, size_t op2) {
    return (op1 > op2) ? op1 : op2;
}

//  End of functions, declared in lib.h

//  Start of functions, declared in FunctionGraphic.h
//  Drawing functions
void drawGrid(double offset, double zoomCoef) {
    offset *= zoomCoef; //  note, offset is passed by copying!

    for (double x = XMIN; x < XMAX; x += offset) {
        DrawLine(x, YMIN, x, YMAX, LIGHTGRAY);
    }
    for (double y = YMIN; y < YMAX; y += offset) {
        DrawLine(XMIN, y, XMAX, y, LIGHTGRAY);
    }
}

void drawFunctionGraphic(equationCoefs* ptrCoefs, double lX, double rX, double step, double zoomCoef, FILE* ptrFile) {
    double a = ptrCoefs->a, b = ptrCoefs->b, c = ptrCoefs->c;   

    double x1 = ORIGIN.x, y1 = ORIGIN.y;
    for (double x2 = ORIGIN.x + lX + step; x2 <= ORIGIN.x + rX; x2 += step) {
        double y2 = getY(ptrCoefs, x2); // TODO вынести в отдельную функцию
        
        double invDifY = (double)1 / (y2 - y1);

        fprintf(ptrFile, "x:%i, y:%i, step:%lf, invDifY:%lf\n", x2, y2, step, invDifY);
        
        DrawCircle(zoom(x2, X, zoomCoef), zoom(y2, Y, zoomCoef), FUNCTION_GRAPHIC_THICKNESS, GREEN);

        step *= invDifY;
        step = max(step, MAX_STEP);
        step = min(step, MIN_STEP);

        x1 = x2, y1 = y2;
    }
}

void drawAxes(double zoomCoef) {
    DrawLine(ORIGIN.x, ORIGIN.y, XMAX, ORIGIN.y, BLUE);
    DrawText("X", XMAX - TEXT_X_RIGHT_OFFSET, ORIGIN.y - DRAW_AXES_Y_OX_OFFSET, FONT_SIZE, BLACK);

    DrawLine(ORIGIN.x, ORIGIN.y, ORIGIN.x, 0, RED);
    DrawText("Y", ORIGIN.x + TEXT_X_LEFT_OFFSET, DRAW_AXES_Y_OY_OFFSET, FONT_SIZE, BLACK);
}

void drawScale(double zoomCoef) {
    DrawText(AXES_SCALE_TEXT, ORIGIN.x + AXES_SCALE_X_OFFSET * zoomCoef, ORIGIN.y, FONT_SIZE, BLACK);
    DrawText(AXES_SCALE_TEXT, ORIGIN.x, ORIGIN.y - AXES_SCALE_Y_OFFSET * zoomCoef, FONT_SIZE, BLACK);
}

void drawMouseLines() {
    Vector2 mouseCoords = GetMousePosition();
    DrawLine(XMIN, mouseCoords.y, mouseCoords.x, mouseCoords.y, ORANGE);
    DrawLine(mouseCoords.x, YMAX, mouseCoords.x, mouseCoords.y, ORANGE);
}

void printParabolaApex(equationCoefs* ptrCoefs, double zoomCoef, FILE* ptrFile) { // TODO улучшить отображение
    double a = ptrCoefs->a, b = ptrCoefs->b, c = ptrCoefs->c;

    if (CmpDouble(a, 0) == 0) {
        fprintf(ptrFile, "(printParabolaApex) a is zero!\n");
        return;
    }
    
    double apexX = ORIGIN.x -b / (2 * a);
    double apexY = ORIGIN.y - getFunc(ptrCoefs, apexX);

    DrawCircle(zoom(apexX, X, zoomCoef), zoom(apexY, Y, zoomCoef), 5.0f, YELLOW);
    DrawText("Apex", apexX - 20, apexY + 20, 20, BLACK);
}

double zoom(double coord, COORD_TYPES coordType, double zoomCoef) {
    switch (coordType) {
        case X: {
            return 2 * (coord - ORIGIN.x) + ORIGIN.x;
            break;
        }
        
        case Y: {
            return 2 * (coord - ORIGIN.y) + ORIGIN.y;
            break;
        }

        default: {
            printf("%sError: error while zooming happened!%s", MY_RED, MY_RESET);
            return NAN;
            break;
        }
    }
}

//  Math functions
double getFunc(equationCoefs* ptrCoefs, const double x) {
    double a = ptrCoefs->a, b = ptrCoefs->b, c = ptrCoefs->c;

    return a * x * x + b * x + c;
}

double getY(equationCoefs* ptrCoefs, const double x) {
    return ORIGIN.y - getFunc(ptrCoefs, x - ORIGIN.x);
}

double min(double op1, double op2) {
    return (op1 < op2) ? op1 : op2;
}

double max(double op1, double op2) {
    return (op1 > op2) ? op1 : op2;
}

//  End of functions, declared in FunctionGraphic.h 
