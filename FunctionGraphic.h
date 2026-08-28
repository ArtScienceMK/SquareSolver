#ifndef FUNCTION_GRAPH_H
#define FUNCTION_GRAPH_H

#include "CmpDouble.h"
#include "QuadraticSolver.h"

const Vector2 ORIGIN = {.x = 1000, .y = 800};

const double XMIN = 0; 
const double XMAX = 2000;

const double TEXT_X_RIGHT_OFFSET = 100;
const double TEXT_X_LEFT_OFFSET = 100;

const double YMIN = 0;
const double YMAX = 1500;

const char* const AXES_SCALE_TEXT = "10";
const double AXES_SCALE_X_OFFSET = 10;
const double AXES_SCALE_Y_OFFSET = 10;
const double FONT_SIZE = 20;

const double MAX_STEP = 100;
const double MIN_STEP = 0.01;

const double DRAW_AXES_Y_OX_OFFSET = 20;
const double DRAW_AXES_Y_OY_OFFSET = 20;

const double FUNCTION_GRAPHIC_THICKNESS = 3.0;

const double TARGET_FPS = 60;

const double WINDOW_WIDTH = 2000;
const double WINDOW_HEIGHT = 2000;

const double DRAW_APEX_RADIUS = 5.0; 
const double DRAW_APEX_X_OFFSET = 20;
const double DRAW_APEX_Y_OFFSET = 20;

enum COORD_TYPES {
    X = 0,
    Y = 1
};

//  Drawing Functions
void drawGrid(double offset, double zoomCoef);
void drawFunctionGraphic(equationCoefs* ptrCoefs, double lX, double rX, double step, double zoomCoef, FILE* ptrFile);
void drawAxes(double zoomCoef);

void drawScale(double zoomCoef);
void drawMouseLines(void);
void printParabolaApex(equationCoefs* ptrCoefs, double zoomCoef, FILE* ptrFile);
double zoom(double coord, COORD_TYPES coordType, double zoomCoef);



//  Math functions
double getFunc(equationCoefs* ptrCoefs, const double x);
double getY(equationCoefs* ptrCoefs, const double x);
double min(double op1, double op2);
double max(double op1, double op2);

#endif /*FUNCTION_GRAPH_H*/