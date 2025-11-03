#ifndef __PATH_H__
#define __PATH_H__


#include <stdint.h>

#include <SDL3/SDL.h>

#include "include.h"
#include "points.h"
#include "drawing.h"
#include "label.h"





typedef struct PArray {
        Point points[MAX_POINTS];
        Point *selected_point;
        int32_t count;
} PArray;



void RenderPath(SDL_Renderer *renderer, SDL_Texture *point_texture, PArray *points, LABEL *point_label);
bool CheckMousePos(PArray *points, double x, double y, bool mouse_pressed, bool shift_pressed);




#endif // __PATH_H__