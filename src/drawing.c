#include "drawing.h"


#define FIX_CORD(x, app_x, r) ( x < r ? r : ( x + r > app_x ? app_x - r : x) )


void RenderLine(SDL_Renderer *renderer, SDL_FPoint P0, SDL_FPoint P1, int32_t radius) {
        int32_t x = 0; 

        int32_t x0 = FIX_CORD(P0.x, APP_WIDTH, radius);
        int32_t y0 = FIX_CORD(P0.y, APP_HEIGHT, radius);
        int32_t x1 = FIX_CORD(P1.x, APP_WIDTH, radius);
        int32_t y1 = FIX_CORD(P1.y, APP_HEIGHT, radius);

        while ( x <= radius ) {
                SDL_RenderLine(renderer, x0 + x, y0 + radius, x1 + x, y1 + radius);
                SDL_RenderLine(renderer, x0 + x, y0 - radius, x1 + x, y1 - radius);
                SDL_RenderLine(renderer, x0 - x, y0 + radius, x1 - x, y1 + radius);
                SDL_RenderLine(renderer, x0 - x, y0 - radius, x1 - x, y1 - radius);
                SDL_RenderLine(renderer, x0 + radius, y0 + x, x1 + radius, y1 + x);
                SDL_RenderLine(renderer, x0 + radius, y0 - x, x1 + radius, y1 - x);
                SDL_RenderLine(renderer, x0 - radius, y0 + x, x1 - radius, y1 + x);
                SDL_RenderLine(renderer, x0 - radius, y0 - x, x1 - radius, y1 - x);

                x++;
        }
}
                        

void RenderArrow(SDL_Renderer *renderer, SDL_FPoint P0, SDL_FPoint P1, double base, SDL_FColor arrow_color) {
        SDL_FPoint Pa = Point_Norm( Point_Sub(P1, P0) );

        P1 = Point_Sub(P1, Point_Mult(Pa, POINT_RADIUS + base));

        SDL_Vertex vertex[3];

        vertex[0].color = arrow_color;
        vertex[1].color = arrow_color;
        vertex[2].color = arrow_color;

        Pa = Point_Mult(Pa, base);
        vertex[0].position = Point_Sum(P1, Pa);

        Pa = Point_Rotate90(Pa);
        vertex[1].position = Point_Sum(P1, Pa);

        Pa = Point_Invert(Pa);
        vertex[2].position = Point_Sum(P1, Pa);


        SDL_RenderGeometry(renderer, NULL, vertex, 3, NULL, 0);
}