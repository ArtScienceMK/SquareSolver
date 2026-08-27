#ifndef LIB_H
#define LIB_H

#define DEBUG

#ifdef DEBUG

#define ON_DEBUG(...) __VA_ARGS__

#else

#define ON_DEBUG(...)

#endif

const int INPUT_ERROR = 1;
const int MAX_STATEMENT_LEN = 9; // >= 8 to avoid warning
const int MAX_STATEMENTS = 10;

const int MAX_FORMAT_LINE_LEN = 50;
const int MAX_NUMBER_LEN = 20;
const int MAX_FILENAME_LEN = 20;

const char TRASH_CHAR = 'a';

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
    const char* const statements[MAX_STATEMENTS];
};

const struct statementInfo YES_NO = {{"yes", "no"}};

const statementInfo SOLVE_TEST_GRAPHICAL = {{"solve", "test", "graph"}};

const statementInfo MANUAL_FILE_AUTO = {{"manual", "file", "unit"}};

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

void getFormatLine(char* formatLine, size_t scanfLen);
void getStatementLine(char *statementLine, statementInfo info);
void getTestingModeLine(char *testingModeLine);

void greetingsIntInput(void);
int getPositiveIntInputSafe(void);

int processEnd(void);
void startUserCycle(int attempts);


void greetings(void);
void printAttempts(int attempts);

const char* const getStringInputStatus(INPUT_STATUSES);
INPUT_STATUSES getCoefInput(equationCoefs* ptrCoefs, FILE* ptrFile);

bool goodEnd(void);
void readToEnd(void);

void printRoots(const int cntRoots, const double eqRoot1, const double eqRoot2);
void getResumeLine(char* resumeLine);

bool resume(void);

const char* const getStringCheckStatus(CHECK_STATUSES);
CHECK_STATUSES checkRoots(const equationCoefs coefs,
 const int cntRoot, const double eqRoot1, const double eqRoot2);

int checkSingleRoot(const equationCoefs coefs, const double x);

const char* const getStringRunAllTestsStatus(RUN_ALL_TESTS_STATUSES);
RUN_ALL_TESTS_STATUSES runAllTests(TESTING_MODE_STATUSES testingModeStatus);

const char* const getStringTestCheckStatus(TEST_CHECK_STATUSES);
TEST_CHECK_STATUSES runSingleTest(equationCoefsTest test);

INPUT_STATUSES getRefInput(equationCoefsTest* ptrTest, FILE* ptrFile);

size_t max_size_t(size_t op1, size_t op2);

INPUT_STATUSES fileCoefInput(equationCoefs* ptrCoefs, FILE* ptrFile);

INPUT_STATUSES fileRefInput(equationCoefsTest* coefsTest, FILE* ptrFile);

RUN_ALL_TESTS_STATUSES userTesting(const char* const mode);

RUN_ALL_TESTS_STATUSES unitTesting(void);

void buildFunctionGraphic(void);

FILE* acessFile(const char* const greetings, const char* const);

#endif /*LIB_H*/
