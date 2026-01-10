#ifndef _DRAWING_H_
#define _DRAWING_H_ 


#include <SDL3/SDL.h>

#include "include.h"
#include "vectors.h"



void RenderLine(SDL_Renderer *renderer, SDL_FPoint P0, SDL_FPoint P1, int32_t radius);
void RenderArrow(SDL_Renderer *renderer, SDL_FPoint P0, SDL_FPoint P1, double base, SDL_FColor arrow_color);
void RenderCircle(SDL_Renderer *renderer, float x0, float y0, float radius);
void Render_RounderRect(SDL_Renderer *renderer, SDL_FRect rect, float radius);

#endif // _DRAWING_H_