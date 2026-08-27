
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

                const char* const LineRunAllTestsStatus = getStringRunAllTestsStatus(runAllTestsStatus);

                printf("%s", LineRunAllTestsStatus);
            } else {
                printf("Error: cannot process testing mode input\n");
            }

            break;
        }

        case FUNCTION_GRAPHIC: {
            buildFunctionGraphic();

            break;
        }

        case USER_INPUT_MODE_ERROR: {
            printf("Cannot process input mode! Bye...\n");

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
           "For graphical mode type %s\n"
           "Choose mode:", SOLVE_TEST_GRAPHICAL.statements[0],
            SOLVE_TEST_GRAPHICAL.statements[1], 
            SOLVE_TEST_GRAPHICAL.statements[2]);

    char modeLine[MAX_STATEMENT_LEN] = "";

    getStatementLine(modeLine, SOLVE_TEST_GRAPHICAL);

    if (strcmp(modeLine, SOLVE_TEST_GRAPHICAL.statements[0]) == 0) {

ON_DEBUG(printf("(userChoosingMode) SOLVING\n"));

        return SOLVING;
    }
    if (strcmp(modeLine, SOLVE_TEST_GRAPHICAL.statements[1]) == 0) {

ON_DEBUG(printf("(userChoosingMode) TESTING\n"));

        return TESTING;
    }

    if (strcmp(modeLine, SOLVE_TEST_GRAPHICAL.statements[2]) == 0) {

ON_DEBUG(printf("(userChoosingMode) GRAPHICAL\n"));

        return FUNCTION_GRAPHIC;
    }

ON_DEBUG(printf("(userChoosingMode) USER_INPUT_MODE_ERROR\n"));

    return USER_INPUT_MODE_ERROR;

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

void getStatementLine(char* statementLine, statementInfo info) {
    size_t scanfLen = 0;

    char tempStatementLine [MAX_STATEMENT_LEN] = "";

    for (int i = 0; i < MAX_STATEMENTS && info.statements[i]; i++) {
        snprintf(tempStatementLine, MAX_STATEMENT_LEN, "%s", info.statements[i]);
// printf("%s\n", info.statements[i]);
ON_DEBUG(printf("(getStatementLine) info.statements[%i] have len%i\n",
        i, (int)strlen(info.statements[i])));

        scanfLen = max_size_t(scanfLen, strlen(tempStatementLine));
    }

ON_DEBUG(printf("(getStatementLine) scanfLen:%zu\n", scanfLen));

    char formatLine [MAX_FORMAT_LINE_LEN] = "";

    getFormatLine(formatLine, scanfLen);

ON_DEBUG(printf("(getStatementLine) formatLine:%s\n", formatLine));

    scanf(formatLine,  statementLine);

    if (!goodEnd()) {
        readToEnd();

ON_DEBUG(printf("(getStatementLine) read to end!\n"));

        char* ptrStatementLine = statementLine + (int)strlen(statementLine);
        *ptrStatementLine++ = TRASH_CHAR;
        *ptrStatementLine = '\0';
    }

ON_DEBUG(printf("(getStatementLine) statementLine:%s\n", statementLine));
ON_DEBUG(printf("(getStatementLine) len of statementLine:%i\n",
           (int)strlen(statementLine)));
}

void getFormatLine(char* formatLine, size_t scanfLen) {
    snprintf(formatLine, MAX_NUMBER_LEN, " %%%zus", scanfLen);
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

        INPUT_STATUSES inputStatus = fileCoefInput(ptrCoefs, stdin);

        if (inputStatus != INPUT_CORRECT) {
            printf("Try another time!\n");
            continue;
        }

ON_DEBUG(printf("%lf %lf %lf\n", ptrCoefs->a, ptrCoefs->b, ptrCoefs->c));

        int cntRoots = QuadraticSolver(ptrCoefs, &eqRoot1, &eqRoot2);

        CHECK_STATUSES equationCheckStatus = checkRoots(coefs, cntRoots, eqRoot1, eqRoot2);


        const char* const equationCheckStatusLine = getStringCheckStatus(equationCheckStatus);

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
            return isGoodEnd ? INPUT_CORRECT : INPUT_ERROR_AFTER_THIRD;
            break;

        default:
            return UNKNOWN_INPUT_ERROR;
            break;
    }
}


bool goodEnd() {
    char c = ' ';

    while (scanf("%c", &c) && (c == ' ' || c == '\t')) {
        ;
    }

ON_DEBUG(printf("(goodEnd) last char:%c\n", c));

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

ON_DEBUG(printf("(readToEnd) c == backslashn:%i\n", res));
}

void getResumeLine(char * resumeLine) {
    int cntInputCharsBefore = 0, cntInputCharsAfter = 0;

    scanf("%n%3s%n", &cntInputCharsBefore, resumeLine, &cntInputCharsAfter); //TODO edit format line, delete %n

ON_DEBUG(printf("(getResumeLine) cntInputCharsBefore: %i, cntInputCharsAfter: %i\n",
        cntInputCharsBefore, cntInputCharsAfter);)

    int diff = cntInputCharsAfter - cntInputCharsBefore;

    if (diff != (int)strlen(YES_NO.statements[0]) &&
     diff != (int)strlen(YES_NO.statements[1])) {
        cntInputCharsAfter = 0;
    }

    resumeLine[cntInputCharsAfter] = '\0';

ON_DEBUG(printf("(getResumeLine) resumeLine:%s\n", resumeLine);)

}

bool resume(void) {
    printf("Continue? %s / %s:", YES_NO.statements[0], YES_NO.statements[1]);

    char resumeLine[MAX_STATEMENT_LEN] = "";

    getResumeLine(resumeLine);

ON_DEBUG(printf("(resume) resumeLine: %c%c%c%c\n", resumeLine[0], resumeLine[1], resumeLine[2], resumeLine[3]));

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


int checkSingleRoot(const equationCoefs coefs, const double x) {
    double a = coefs.a, b = coefs.b, c = coefs.c;

    double result = a * x * x + b * x + c;

    return CmpDouble(result, 0) == 0;
}

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
            printf("Error: cannot process testing mode input\n");
            
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

RUN_ALL_TESTS_STATUSES userTesting(const char* const mode) {

    equationCoefsTest coefsTest;

    equationCoefs* ptrCoefs = &coefsTest.coefs;

    equationCoefsInitPointers(ptrCoefs);

    coefsTest.refCntRoots = 0;

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
                printf("Error: cannot process coefs in test number %i . Skipping...", i);
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
                printf("Error: cannot process reference values in test number %i . Skipping...", i);
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

TEST_CHECK_STATUSES runSingleTest(equationCoefsTest test) {
ON_DEBUG(printf("(runSingleTest) "));

    printEquationCoefsTest(test);

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
            return isGoodEnd ? INPUT_CORRECT : INPUT_ERROR_AFTER_THIRD;
            break;

        default:
            return UNKNOWN_INPUT_ERROR;
            break;
    }
}

size_t max_size_t(size_t op1, size_t op2) {
    return (op1 > op2) ? op1 : op2;
}

INPUT_STATUSES fileCoefInput(equationCoefs* ptrCoefs, FILE* ptrFile) {

    if (ptrFile == stdin) {
        printf("Enter a, b, c:");
    }

    INPUT_STATUSES inputStatus = getCoefInput(ptrCoefs, ptrFile);

ON_DEBUG(printf("a:%lf, b:%lf, c:%lf", ptrCoefs->a, ptrCoefs->b, ptrCoefs->c);)

    if (inputStatus != INPUT_CORRECT) {
        const char* const inputStatusLine = getStringInputStatus(inputStatus);
        assert(inputStatusLine != NULL);
        printf("%s", inputStatusLine);
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

void buildFunctionGraphic() {
    FILE* ptrFile = acessFile("Enter the filename to store logs:", "w"); // TODO разобраться с filename их много
    if (!ptrFile) {
        ptrFile = stdout;
        fprintf(ptrFile, "Warning: cannot open the debug output file! Redirecting to console...\n");
    }

    double a = NAN, b = NAN, c = NAN;
    
    bool isBadInput = true;

    while (isBadInput) {
        printf("Enter a, b, c:");
    
        fscanf(stdin, "%lf %lf %lf", &a, &b, &c);
        
        if (!(isnan(a) || isnan(b) || isnan(c))) {
            isBadInput = false;
        } else {
            printf("Cannot process input! Try again!\n");
        }

        fprintf(ptrFile, "a:%lf, b:%lf, c:%lf\n", a, b, c);
    }

    SetTargetFPS(60);

    InitWindow(2000, 2000, "Sample");
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        double gridOffset = 20;

        drawGrid(20);
        drawFunctionGraphic(a, b, c, -100, 100, 1, ptrFile);
        drawAxes();
        drawScale();
        printParabolaApex(a, b, c, ptrFile);


        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            drawMouseLines();
            // printMouseCoords();

            // Vector2 mouseCoords = GetMousePosition();
            // DrawLine(XMIN, mouseCoords.y, XMAX, mouseCoords.y, RED);
        }
        //getmouseposition()
        // DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }
    CloseWindow();
}

void drawGrid(double offset) {
    for (double x = 0; x < XMAX; x += offset) {
        DrawLine(x, YMIN, x, YMAX, LIGHTGRAY); // TODO поменять цвет на светло-серый
    }
    for (double y = 0; y < YMAX; y += offset) {
        DrawLine(XMIN, y, XMAX, y, LIGHTGRAY);
    }
}

double getFunc(const double a, const double b, const double c, const double x) {
    return a * x * x + b * x + c; // TODO check overflow
}

void drawFunctionGraphic(const double a, const double b, const double c, double lX, double rX, double step, FILE* ptrFile) {
    // double x1 = ORIGIN.x + lX, x2 = ORIGIN.x + lX + step;
    // double y1 = ORIGIN.y - getFunc(x1 - ORIGIN.x), y2 = ORIGIN.y - getFunc(x2 - ORIGIN.x);    
    for (double x3 = ORIGIN.x + lX + 2 * step; x3 <= ORIGIN.x + rX; x3 += step) {
        double y3 = getY(a, b, c, x3); // TODO вынести в отдельную функцию
        double x2 = x3 - step;
        double y2 = getY(a, b, c, x2);
        // double invDifDifY = (double)1 / (y3 - 2 * y2 + y1);
        double invDifY = (double)1 / (y3 - y2);
        // printf("x1:%i, y1:%i, x2:%i, y2:%i, x3:%i, y3:%i\n", x1, y1, x2, y2, x3, y3);
        // printf("x:%i, y:%i, chisl:%i, znam:%i, res:%lf\n", x3, y3, 1, (y3 - 2 * y2 + y1),  invDifDifY);
        // printf("x:%i, y:%i, chisl:%i, znam:%i, res:%lf\n", x3, y3, 1, (y3 - y2),  invDifDifY);
        fprintf(ptrFile, "x:%i, y:%i, step:%lf, invDifY:%lf\n", x3, y3, step, invDifY);
        DrawCircle(x3, y3, 2.0f, GREEN);
        step *= (double)1 / (y3 - y2);
        step = max(step, 100);
        step = min(step, 0.01);
        // DrawLine(x, y, nextX, nextY, GREEN);
        // x1 = x2, x2 = x3;
        // y1 = y2, y2 = y3;
    }
    // DrawSplineBezierQuadratic(POINTS, 3, 5.0f, GREEN);
}

FILE* acessFile(const char* const greetings, const char* const mode) {
    printf("%s", greetings);
    char filename [MAX_FILENAME_LEN] = "";

    scanf("%s", filename);

    FILE* ptrFile = fopen(filename, mode);

    if (ptrFile == NULL) {
        printf("Error: cannot open file %s\n", filename);
    } else {
        printf("File opened sucessfully!\n");
    }

    return ptrFile;
}

void drawAxes() {
    DrawLine(ORIGIN.x, ORIGIN.y, XMAX, ORIGIN.y, BLUE);
    DrawText("X", XMAX - TEXT_X_RIGHT_OFFSET, ORIGIN.y - 20, 20, BLACK);

    DrawLine(ORIGIN.x, ORIGIN.y, ORIGIN.x, 0, RED);
    DrawText("Y", ORIGIN.x + TEXT_X_LEFT_OFFSET, 20, 20, BLACK);
}

void drawScale() {
    DrawText("10", ORIGIN.x + 10, ORIGIN.y - 10, 20, BLACK);
    DrawText("10", ORIGIN.x - 10, ORIGIN.y - 10, 20, BLACK);
}

void printParabolaApex(const double a, const double b, const double c, FILE* ptrFile) {    
    if (CmpDouble(a, 0) == 0) {
        fprintf(ptrFile, "(printParabolaApex) a is zero!\n");
        return;
    }
    
    double apexX = ORIGIN.x -b / (2 * a);
    double apexY = ORIGIN.y - getFunc(a, b, c, apexX);

    DrawCircle(apexX, apexY, 5.0f, YELLOW);
    DrawText("Apex", apexX - 20, apexY + 20, 20, BLACK);
}

double getY(const double a, const double b, const double c, const double x) {
    return ORIGIN.y - getFunc(a, b, c, x - ORIGIN.x);
}

double min(double op1, double op2) {
    return (op1 < op2) ? op1 : op2;
}

double max(double op1, double op2) {
    return (op1 > op2) ? op1 : op2;
}

void drawMouseLines() {
    Vector2 mouseCoords = GetMousePosition();
    DrawLine(XMIN, mouseCoords.y, mouseCoords.x, mouseCoords.y, ORANGE);
    DrawLine(mouseCoords.x, YMAX, mouseCoords.x, mouseCoords.y, ORANGE);
}

