#ifndef __POINTS_H__
#define __POINTS_H__

#include <SDL3/SDL.h>

#define _USE_MATH_DEFINES
#include <math.h>


SDL_FPoint Vector_Sum(SDL_FPoint P1, SDL_FPoint P2);
SDL_FPoint Vector_Sub(SDL_FPoint P1, SDL_FPoint P2);

double Vector_SclMult(SDL_FPoint P1, SDL_FPoint P2);

SDL_FPoint Vector_Mult(SDL_FPoint P1, double k);
SDL_FPoint Vector_Div(SDL_FPoint P1, double k);

double Vector_SqAbs(SDL_FPoint P);
double Vector_Abs(SDL_FPoint P);
SDL_FPoint Vector_Norm(SDL_FPoint P);

double Vector_Cos(SDL_FPoint P1, SDL_FPoint P2);

SDL_FPoint Vector_Rotate(SDL_FPoint P, double rad);
SDL_FPoint Vector_Rotate180(SDL_FPoint P);
SDL_FPoint Vector_Rotate90(SDL_FPoint P);
SDL_FPoint Vector_Rotate90m(SDL_FPoint P);

SDL_FPoint Vector_Invert(SDL_FPoint P);


#endif // __POINTS_H__