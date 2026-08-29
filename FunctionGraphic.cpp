void drawGrid(double offset, double zoomCoef) {
    offset *= zoomCoef; //  note, offset is passed by copying!

    for (double x = ORIGIN.x; x > XMIN; x -= offset) {
        DrawLine(x, YMIN, x, YMAX, LIGHTGRAY);
    }

    for (double x = ORIGIN.x; x < XMAX; x += offset) {
        DrawLine(x, YMIN, x, YMAX, LIGHTGRAY);
    }

    for (double y = ORIGIN.y; y > YMIN; y -= offset) {
        DrawLine(XMIN, y, XMAX, y, LIGHTGRAY);
    }

    for (double y = ORIGIN.y; y < YMAX; y += offset) {
        DrawLine(XMIN, y, XMAX, y, LIGHTGRAY);
    }
}
 
void drawFunctionGraphic(equationCoefs* ptrCoefs, double lX, double rX, double step, double zoomCoef, FILE* ptrFile) {
    double a = ptrCoefs->a, b = ptrCoefs->b, c = ptrCoefs->c;   

    double x1 = ORIGIN.x, y1 = ORIGIN.y;
    for (double x2 = ORIGIN.x + lX + step; x2 <= ORIGIN.x + rX; x2 += step) {
        double y2 = getY(ptrCoefs, x2);
        
        double invDifY = (double)1 / (y2 - y1);

        // fprintf(ptrFile, "x:%i, y:%i, step:%lf, invDifY:%lf\n", x2, y2, step, invDifY);
        
        DrawCircle(zoom(x2, X, zoomCoef), zoom(y2, Y, zoomCoef), FUNCTION_GRAPHIC_THICKNESS, GREEN);

        step *= invDifY;
        step = max(step, MAX_STEP);
        step = min(step, MIN_STEP);

        x1 = x2, y1 = y2;
    }
}
 
void drawAxes(double zoomCoef) {
    DrawLine(ORIGIN.x, ORIGIN.y, XMAX, ORIGIN.y, BLUE);
    DrawText("X", XMAX - DRAW_AXES_X_RIGHT_OFFSET, ORIGIN.y - DRAW_AXES_Y_ORIGIN_OFFSET, FONT_SIZE, BLACK);

    DrawLine(ORIGIN.x, ORIGIN.y, ORIGIN.x, YMIN, RED);
    DrawText("Y", ORIGIN.x + DRAW_AXES_X_ORIGIN_OFFSET, DRAW_AXES_Y_UP_OFFSET, FONT_SIZE, BLACK);
}

void drawScale(double zoomCoef) {
    DrawText(AXES_SCALE_TEXT, ORIGIN.x + AXES_SCALE_X_ORIGIN_OFFSET * zoomCoef, ORIGIN.y, FONT_SIZE, BLACK);
    DrawText(AXES_SCALE_TEXT, ORIGIN.x, ORIGIN.y - AXES_SCALE_Y_ORIGIN_OFFSET * zoomCoef, FONT_SIZE, BLACK);
}

void drawMouseLines() {
    Vector2 mouseCoords = GetMousePosition();
    DrawLine(XMIN, mouseCoords.y, mouseCoords.x, mouseCoords.y, ORANGE);
    DrawLine(mouseCoords.x, YMAX, mouseCoords.x, mouseCoords.y, ORANGE);
}

void printParabolaApex(equationCoefs* ptrCoefs, double zoomCoef, FILE* ptrFile) {
    double a = ptrCoefs->a, b = ptrCoefs->b, c = ptrCoefs->c;

    if (CmpDouble(a, 0) == 0) {
        fprintf(ptrFile, "(printParabolaApex) a is zero!\n");
        return;
    }
    
    double addX = -b / (2 * a); 
    double apexX = ORIGIN.x + addX;
    double apexY = ORIGIN.y - getFunc(ptrCoefs, addX);

    DrawCircle(zoom(apexX, X, zoomCoef), zoom(apexY, Y, zoomCoef), DRAW_APEX_RADIUS, YELLOW);
    DrawText("Apex", apexX - DRAW_APEX_X_OFFSET, apexY + DRAW_APEX_Y_OFFSET, FONT_SIZE, BLACK);
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
            myPlaySound(INCORRECT_SOUND);
            
            return NAN;
            break;
        }
    }
}

// Math functions
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