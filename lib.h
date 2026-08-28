#ifndef LIB_H
#define LIB_H

#define DEBUG

#ifdef DEBUG

#define ON_DEBUG(...) __VA_ARGS__

#else

#define ON_DEBUG(...)

#endif


//  Colors for terminal
const char* const MY_RED = "\033[31m";
const char* const MY_YELLOW = "\033[33m";
const char* const MY_GREEN = "\033[32m";
const char* const MY_RESET = "\033[0m";

//  Sizes of static arrays
const int MAX_STATEMENT_LEN = 9; // >= 8 to avoid warning
const int MAX_STATEMENTS = 10;

const int MAX_FORMAT_LINE_LEN = 50;
const int MAX_NUMBER_LEN = 20;
const int MAX_FILENAME_LEN = 20;

//  Trash char
const char TRASH_CHAR = 'a';

//  Statuses
enum INPUT_MODE_STATUS {
    SOLVING = 0,
    TESTING = 1,
    FUNCTION_GRAPHIC = 2,
    USER_INPUT_MODE_ERROR = 3
};

enum TESTING_MODE_STATUSES {
    MANUAL_TESTING = 0,
    FILE_TESTING = 1, // input from file
    UNIT_TESTING = 2, // testing on set of cases
    TESTING_MODE_INPUT_ERROR = 3
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
    INPUT_ERROR_BAD_NUMBERS = 4,
    UNKNOWN_INPUT_ERROR = 5,
    INPUT_CORRECT = 6
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

//  Statements - phrases shown to user
struct statementInfo {
    const char* const statements[MAX_STATEMENTS];
};

const struct statementInfo YES_NO = {{"yes", "no"}};

const statementInfo SOLVE_TEST_GRAPHICAL = {{"solve", "test", "graph"}};

const statementInfo MANUAL_FILE_AUTO = {{"manual", "file", "unit"}};

//  Structs to store info about tests
struct equationCoefsTest {
    equationCoefs coefs; //defined in QuadraticSolver.h

    QUADRATIC_SOLVER_STATUSES refCntRoots;
    double refRoot1, refRoot2;
};

//  Functions to work with structs
struct equationCoefs equationCoefsInit(void);

void printEquationCoefsTest(equationCoefsTest test);

void equationCoefsInitPointers(equationCoefs* ptrCoefs);

//  UI main menu functions
void greetings(void);

INPUT_MODE_STATUS userChoosingMode(void);

void chooseMode(INPUT_MODE_STATUS inputModeStatus);

TESTING_MODE_STATUSES userChoosingTestingMode(void);

//  Functions to handle User text input
void getFormatLine(char* formatLine, size_t scanfLen);
void getStatementLine(char *statementLine, statementInfo info);
void getResumeLine(char* resumeLine);
const char* const getStringCheckStatus(CHECK_STATUSES);


// Functions to process User input lines
int processEnd(void);
bool goodEnd(void);
void readToEnd(void);



//  Functions to handle User attempts input
void greetingsIntInput(void);
int getPositiveIntInputSafe(void);

//  Functions, used in real time solving case
// 1.Interaction&Solve
void solveRealTime(int attempts);
bool resume(void);
void printAttempts(int attempts);

//  2.Check roots & print them
CHECK_STATUSES checkRoots(const equationCoefs coefs,
 QUADRATIC_SOLVER_STATUSES cntRoot, const double eqRoot1, const double eqRoot2);
int checkSingleRoot(const equationCoefs coefs, const double x);
void printRoots(QUADRATIC_SOLVER_STATUSES cntRoots, const double eqRoot1, const double eqRoot2);

// Real time solving case ended

 
// Functions, handling coefficients input
const char* const getStringInputStatus(INPUT_STATUSES);
INPUT_STATUSES getCoefInput(equationCoefs* ptrCoefs, FILE* ptrFile);

// Functions for testing
const char* const getStringRunAllTestsStatus(RUN_ALL_TESTS_STATUSES);
RUN_ALL_TESTS_STATUSES runAllTests(TESTING_MODE_STATUSES testingModeStatus);

const char* const getStringTestCheckStatus(TEST_CHECK_STATUSES);
TEST_CHECK_STATUSES runSingleTest(equationCoefsTest test);

//  Function for manual & file testing
RUN_ALL_TESTS_STATUSES userTesting(const char* const mode);

//  Function for unit testing
RUN_ALL_TESTS_STATUSES unitTesting(void);

// Function, covering case graphic function
void buildFunctionGraphic(void);
// End of case graphic function



// Working with files&stdin functions
FILE* acessFile(const char* const greetings, const char* const);
INPUT_STATUSES fileCoefInput(equationCoefs* ptrCoefs, FILE* ptrFile); // note: this function is used in real time case!
INPUT_STATUSES fileRefInput(equationCoefsTest* coefsTest, FILE* ptrFile);
INPUT_STATUSES getRefInput(equationCoefsTest* ptrTest, FILE* ptrFile);

//  Functions for checking User numeric input
bool isGoodCoefs(equationCoefs* ptrCoefs);
bool isGoodRef(equationCoefsTest* ptrTest);

// math functions
size_t max_size_t(size_t op1, size_t op2);


#endif /*LIB_H*/
