#include "points.h"


SDL_FPoint Point_Sum(SDL_FPoint P1, SDL_FPoint P2) {
        return (SDL_FPoint){ 
                P1.x + P2.x, 
                P1.y + P2.y 
        };
}

SDL_FPoint Point_Sub(SDL_FPoint P1, SDL_FPoint P2) {
        return (SDL_FPoint){ 
                P1.x - P2.x, 
                P1.y - P2.y 
        };
}

double Point_SclMult(SDL_FPoint P1, SDL_FPoint P2) {
        return P1.x * P2.x + P1.y * P2.y;
}

SDL_FPoint Point_Mult(SDL_FPoint P1, double k) {
        return (SDL_FPoint){
                P1.x * k, 
                P1.y * k 
        };
}

SDL_FPoint Point_Div(SDL_FPoint P1, double k) {
        return (SDL_FPoint){ 
                P1.x / k, 
                P1.y / k 
        };
}

double Point_Abs(SDL_FPoint P) {
        return sqrt( P.x * P.x + P.y * P.y );
}

SDL_FPoint Point_Norm(SDL_FPoint P) {
        double l = Point_Abs(P);
        return (SDL_FPoint){ 
                P.x / l, 
                P.y / l 
        };
}

double Point_Cos(SDL_FPoint P1, SDL_FPoint P2) {
        return Point_SclMult(P1, P2) / sqrt( (P1.x * P1.x + P1.y * P1.y) * (P2.x * P2.x + P2.y * P2.y) );
}


SDL_FPoint Point_Rotate(SDL_FPoint P, double rad) {
        double _Cos = cos(rad);
        double _Sin = sin(rad);
        return (SDL_FPoint){
             _Cos * P.x - _Sin * P.y,
             _Sin * P.x + _Cos * P.y   
        };
}

SDL_FPoint Point_Rotate180(SDL_FPoint P) {
        return (SDL_FPoint) {
                -P.x,
                -P.y
        };
}

SDL_FPoint Point_Rotate90(SDL_FPoint P) {
        return (SDL_FPoint) {
                -P.y,
                P.x
        };
}

SDL_FPoint Point_Rotate90m(SDL_FPoint P) {
        return (SDL_FPoint) {
                P.y,
                -P.x
        };
}

SDL_FPoint Point_Invert(SDL_FPoint P) {
        return (SDL_FPoint) {
                -P.x,
                -P.y
        };
}






double Matrix_Determinant(Matrix_2x2 M) {
        return M.A00 * M.A11 + M.A01 * M.A10;
} 

Matrix_2x2 Matrix_Mult(Matrix_2x2 M1, Matrix_2x2 M2) {
        return (Matrix_2x2) {
                M1.A00 * M2.A00 + M1.A01 * M2.A10,
                M1.A00 * M2.A01 + M1.A01 * M2.A11,
                M1.A10 * M2.A00 + M1.A11 * M2.A10,
                M1.A10 * M2.A01 + M1.A11 * M2.A11
        };
}