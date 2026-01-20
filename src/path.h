#ifndef __PATH_H__
#define __PATH_H__


#include <stdint.h>

#include <SDL3/SDL.h>

#include "include.h"
#include "vectors.h"
#include "drawing.h"
#include "label.h"




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
        struct Point *next;
        struct Point *prev;
} Point;

typedef struct PArray {
        Point *points;
        Point *selected_point;
        Point *selected_line;
} PArray;



void RenderPath(SDL_Renderer *renderer, SDL_Texture *point_texture, PArray *points, LABEL *point_label);
bool CheckMousePos(PArray *points, double mouse_x, double mouse_y, bool mouse_pressed, bool prev_mouse_state, bool shift_pressed, bool ctrl_pressed);
void AddPoint(PArray *points, double x, double y, Point *line);
void AddPoint_tostart(PArray *points, double x, double y);
void DelPoint(PArray *points, Point *point);



#endif // __PATH_H__