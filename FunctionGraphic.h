#ifndef FUNCTION_GRAPH_H
#define FUNCTION_GRAPH_H

#include "CmpDouble.h"
#include "QuadraticSolver.h"

const Vector2 ORIGIN = {.x = 1000, .y = 800};

// const double MONITOR_ID = GetCurrentMonitor();
const double XMIN = 0; 
const double XMAX = 2000;

const double TEXT_X_RIGHT_OFFSET = 100;
const double TEXT_X_LEFT_OFFSET = 100;

// const double XMAX = GetMonitorWidth(MONITOR_ID);
// const double YMAX = GetMonitorHeight(MONITOR_ID);
const double YMIN = 0;
const double YMAX = 1500;

// const Vector2 ORIGIN = {.x = (double)GetScreenWidth() / 2, .y = (double)GetScreenHeight() / 2};

const Vector2 POINTS[] = {{.x = ORIGIN.x - 10, .y = ORIGIN.y - 100}, ORIGIN, {.x = ORIGIN.x + 10, .y = ORIGIN.y - 100}};

void drawGrid(double offset);

double getFunc(equationCoefs* ptrCoefs, const double x);
double getY(equationCoefs* ptrCoefs, const double x);

void drawFunctionGraphic(equationCoefs* ptrCoefs, double lX, double rX, double step, FILE* ptrFile);
void drawAxes(void);
void drawScale(void);

void drawMouseLines(void);

void printParabolaApex(equationCoefs* ptrCoefs, FILE* ptrFile);

double min(double op1, double op2);
double max(double op1, double op2);

#endif /*FUNCTION_GRAPH_H*/