#ifndef FUNCTION_GRAPH_H
#define FUNCTION_GRAPH_H

#include "CmpDouble.h"
#include "QuadraticSolver.h"

const char SCREENSHOT_KEY = 'S';
const char* const SCREENSHOT_FILE = "Sketch.bmp";

const Vector2 ORIGIN = {.x = 1000, .y = 800};

const double XMIN = 0; 
const double XMAX = 2000;
const double YMIN = 0;
const double YMAX = 1500;

const double CAMERA_TARGET_STEP_X = 10;
const double CAMERA_TARGET_STEP_Y = 10;

/// @brief Offset for X coord to place text for naming OX
const double DRAW_AXES_X_RIGHT_OFFSET = 100;

/// @brief Offset for Y coord to place text for naming OX
const double DRAW_AXES_Y_ORIGIN_OFFSET = 20;

/// @brief Offset for X coord to place text for naming OY
const double DRAW_AXES_X_ORIGIN_OFFSET = 100;

/// @brief Offset for Y coord to place text for naming OY
const double DRAW_AXES_Y_UP_OFFSET = 20;

const double AXES_THICKNESS = 10;

/// @brief Coef to slow down the mouse speed, to make zoom smoother 
const double ZOOM_SMOOTH_COEF = 100;

const char* const AXES_SCALE_TEXT = "10";
const double AXES_SCALE_X_ORIGIN_OFFSET = 10;
const double AXES_SCALE_Y_ORIGIN_OFFSET = 10;

/// @brief This size is used in all functions, placing text
const double FONT_SIZE = 20;

// Bounds for sketching graph 
const double MAX_STEP = 100;
const double MIN_STEP = 0.01;


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

struct equationCoefsGraph {
    equationCoefs coefs;
    Color color;
};

//  Drawing Functions

/// @brief Iteratively drawing vertical and horiznotal lines from the origin
/// @param offset Space between two adjacent lines
/// @param zoomCoef Ofsset multiplier, to show scaling
void drawGrid(double offset, double zoomCoef);

/// @brief Sketching function graphic segment [lX; rX]. The less inverted first derivative, the tighter points (circles) place to each other.     
/// @param ptrCoefs A pointer to structure, storaging equation coeficients
/// @param lX A value x to start sketching
/// @param rX A value x to finish sketching
/// @param step An initial step to move to the next point
/// @param zoomCoef 
/// @param ptrFile An input source, can be pointer to File, can be stdin
void drawFunctionGraphic(equationCoefs* ptrCoefs, double lX, double rX, double step, double zoomCoef, FILE* ptrFile);

void drawAxes(double zoomCoef);

void drawScale(double zoomCoef);

/// @brief Draws vertical ray below the mouse and horizontal ray left to it
void drawMouseLines(void);

/// @brief Calculates the apex of parabola (if exists), then drawing cycle on it 
void printParabolaApex(equationCoefs* ptrCoefs, double zoomCoef, FILE* ptrFile);

/// @brief Use to zoom picture, takes pixel position return zoomed
/// @param coord position value
/// @param coordType X = horizontal, Y = vertical 
/// @param zoomCoef zoom value
/// @return Return zoomed value of pixel position 
double zoom(double coord, COORD_TYPES coordType, double zoomCoef);



//  Math functions

/// @brief calculates value of polynomial with power < 3: a*x*x + b*x + c
double getFunc(equationCoefs* ptrCoefs, const double x);

/// @brief Calculates Y value of pixel based on X value 
/// @param ptrCoefs pointer to structure with coefficients
/// @param x X value
/// @return Y value
double getY(equationCoefs* ptrCoefs, const double x);

double min(double op1, double op2);
double max(double op1, double op2);

#endif /*FUNCTION_GRAPH_H*/