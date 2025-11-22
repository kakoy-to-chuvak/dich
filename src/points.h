#ifndef __POINTS_H__
#define __POINTS_H__

#include <SDL3/SDL.h>

#define _USE_MATH_DEFINES
#include <math.h>



SDL_FPoint Point_Sum(SDL_FPoint P1, SDL_FPoint P2);
SDL_FPoint Point_Sub(SDL_FPoint P1, SDL_FPoint P2);

double Point_SclMult(SDL_FPoint P1, SDL_FPoint P2);

SDL_FPoint Point_Mult(SDL_FPoint P1, double k);
SDL_FPoint Point_Div(SDL_FPoint P1, double k);

double Point_SqAbs(SDL_FPoint P);
double Point_Abs(SDL_FPoint P);
SDL_FPoint Point_Norm(SDL_FPoint P);

double Point_Cos(SDL_FPoint P1, SDL_FPoint P2);

SDL_FPoint Point_Rotate(SDL_FPoint P, double rad);
SDL_FPoint Point_Rotate180(SDL_FPoint P);
SDL_FPoint Point_Rotate90(SDL_FPoint P);
SDL_FPoint Point_Rotate90m(SDL_FPoint P);

SDL_FPoint Point_Invert(SDL_FPoint P);


#endif // __POINTS_H__