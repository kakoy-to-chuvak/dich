#ifndef __POINTS_H__
#define __POINTS_H__

#include <SDL3/SDL.h>

#define _USE_MATH_DEFINES
#include <math.h>




typedef enum PState {
        selected,
        under,
        none
} PState;

typedef struct Point {
        SDL_FPoint cords;
        PState state;
        // bool changed;
} Point;

typedef struct Matrix_2x2 {
        double A00, A01;
        double A10, A11;
} Matrix_2x2;


SDL_FPoint Point_Sum(SDL_FPoint P1, SDL_FPoint P2);
SDL_FPoint Point_Sub(SDL_FPoint P1, SDL_FPoint P2);

double Point_SclMult(SDL_FPoint P1, SDL_FPoint P2);

SDL_FPoint Point_Mult(SDL_FPoint P1, double k);
SDL_FPoint Point_Div(SDL_FPoint P1, double k);

double Point_Abs(SDL_FPoint P);
SDL_FPoint Point_Norm(SDL_FPoint P);

SDL_FPoint Point_Rotate(SDL_FPoint P, double rad);
SDL_FPoint Point_Rotate180(SDL_FPoint P);
SDL_FPoint Point_Rotate90(SDL_FPoint P);
SDL_FPoint Point_Rotate90m(SDL_FPoint P);

SDL_FPoint Point_Invert(SDL_FPoint P);




double Matrix_Determinant(Matrix_2x2 M);
Matrix_2x2 Matrix_Mult(Matrix_2x2 M1, Matrix_2x2 M2);


#endif // __POINTS_H__