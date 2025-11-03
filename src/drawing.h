#ifndef _DRAWING_H_
#define _DRAWING_H_ 


#include <SDL3/SDL.h>

#include "include.h"
#include "points.h"



void RenderLine(SDL_Renderer *renderer, SDL_FPoint P0, SDL_FPoint P1, int32_t radius);
void RenderArrow(SDL_Renderer *renderer, SDL_FPoint P0, SDL_FPoint P1, double base, SDL_FColor arrow_color);


#endif // _DRAWING_H_