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

double Point_SqAbs(SDL_FPoint P) {
        return P.x * P.x + P.y * P.y;
}

SDL_FPoint Point_Norm(SDL_FPoint P) {
        return Point_Div(P, Point_Abs(P));
}

double Point_Cos(SDL_FPoint P1, SDL_FPoint P2) {
        return Point_SclMult(P1, P2) / ( Point_Abs(P1) * Point_Abs(P2) );
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
