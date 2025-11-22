#ifndef __PATH_H__
#define __PATH_H__


#include <stdint.h>

#include <SDL3/SDL.h>

#include "include.h"
#include "points.h"
#include "drawing.h"
#include "label.h"



#define LINE_RADIUS 5

typedef enum PState {
        UNDEFINED = -1,
        NONE_STATE = 0,
        UNDER_MOUSE = 1,
        SELECTED = 2,
} PState;

typedef struct Point {
        SDL_FPoint cords;
        PState state;
        PState line_state;
        // bool changed;
} Point;

typedef struct PArray {
        Point points[MAX_POINTS];
        Point *selected_point;
        Point *selected_line;
        int32_t count;
} PArray;



void RenderPath(SDL_Renderer *renderer, SDL_Texture *point_texture, PArray *points, LABEL *point_label);
bool CheckMousePos(PArray *points, double mouse_x, double mouse_y, bool mouse_pressed, bool prev_mouse_state, bool shift_pressed);




#endif // __PATH_H__