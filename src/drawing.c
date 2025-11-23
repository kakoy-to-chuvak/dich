#include "drawing.h"


#define FIX_CORD(x, app_x) ( x < 0 ? 0 : ( x > app_x ? app_x : x) )


void RenderLine(SDL_Renderer *renderer, SDL_FPoint P0, SDL_FPoint P1, int32_t radius) {
        int32_t x = radius;
        int32_t y = 0; 
        int32_t err = 0;

        int32_t x0 = FIX_CORD(P0.x, APP_WIDTH);
        int32_t y0 = FIX_CORD(P0.y, APP_HEIGHT);
        int32_t x1 = FIX_CORD(P1.x, APP_WIDTH);
        int32_t y1 = FIX_CORD(P1.y, APP_HEIGHT);

        while ( x >= y ) {
                SDL_RenderLine(renderer, x0 + x, y0 + y, x1 + x, y1 + y);
                SDL_RenderLine(renderer, x0 + x, y0 - y, x1 + x, y1 - y);
                SDL_RenderLine(renderer, x0 - x, y0 + y, x1 - x, y1 + y);
                SDL_RenderLine(renderer, x0 - x, y0 - y, x1 - x, y1 - y);
                SDL_RenderLine(renderer, x0 + y, y0 + x, x1 + y, y1 + x);
                SDL_RenderLine(renderer, x0 + y, y0 - x, x1 + y, y1 - x);
                SDL_RenderLine(renderer, x0 - y, y0 + x, x1 - y, y1 + x);
                SDL_RenderLine(renderer, x0 - y, y0 - x, x1 - y, y1 - x);

                if (err <= 0) {
        	    y += 1;
        	    err += 2*y + 1;
        	} else {
        	    x -= 1;
        	    err -= 2*x + 1;
        	}
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


void RenderCircle(SDL_Renderer *renderer, float x0, float y0, float radius) {
        float x = 0;
        float y = radius;
        float p = 1 - radius;

        SDL_RenderPoint(renderer, x0 + x, y0 + y);
        SDL_RenderPoint(renderer, x0 + y, y0 + x);
        SDL_RenderPoint(renderer, x0 - y, y0 + x);
        SDL_RenderPoint(renderer, x0 - x, y0 + y);
        SDL_RenderPoint(renderer, x0 - x, y0 - y);
        SDL_RenderPoint(renderer, x0 - y, y0 - x);
        SDL_RenderPoint(renderer, x0 + y, y0 - x);
        SDL_RenderPoint(renderer, x0 + x, y0 - y);

        while (x < y)
        {       
                
                if (p < 0){
                        p += 2 * x + 1;
                        x++;
                }else{
                        p += 2 * (x - y) + 1;
                        y--;
                        x++;
                }

                SDL_RenderPoint(renderer, x0 + x, y0 + y);
        	SDL_RenderPoint(renderer, x0 + y, y0 + x);
        	SDL_RenderPoint(renderer, x0 - y, y0 + x);
        	SDL_RenderPoint(renderer, x0 - x, y0 + y);
        	SDL_RenderPoint(renderer, x0 - x, y0 - y);
        	SDL_RenderPoint(renderer, x0 - y, y0 - x);
        	SDL_RenderPoint(renderer, x0 + y, y0 - x);
        	SDL_RenderPoint(renderer, x0 + x, y0 - y);

                
        }
}


void Render_RounderRect(SDL_Renderer *renderer, SDL_FRect rect, float radius) {

        SDL_FRect now_rect = {
                rect.x + radius,
                rect.y,
                rect.w - 2 * radius,
                rect.h
        };

        SDL_RenderFillRect(renderer, &now_rect);

        now_rect = (SDL_FRect){
                rect.x,
                rect.y + radius,
                radius,
                rect.h - 2 * radius
        };

        SDL_RenderFillRect(renderer, &now_rect);

        now_rect = (SDL_FRect){
                rect.x + rect.w - radius,
                rect.y + radius,
                radius,
                rect.h - 2 * radius
        };

        SDL_RenderFillRect(renderer, &now_rect);

        float x_left = rect.x + radius;
        float y_up = rect.y + radius;
        float x_right = rect.x + rect.w - radius - 1;
        float y_bottom = rect.y + rect.h - radius - 1;

        float x = 0;
        float y = radius;
        float p = 1 - radius;
        
        SDL_RenderLine(renderer, x_left - x, y_up - y, x_right + x, y_up - y);
        SDL_RenderLine(renderer, x_left - x, y_bottom + y, x_right + x, y_bottom + y);
        SDL_RenderLine(renderer, x_left - y, y_up - x, x_right + y, y_up - x);
        SDL_RenderLine(renderer, x_left - y, y_bottom + x, x_right + y, y_bottom + x);
                
        while (x < y)
        {
        	if (p < 0){
                        p += 2 * x + 1;
                        x++;
                }else{
                        p += 2 * (x - y) + 1;
                        y--;
                        x++;
                }

                SDL_RenderLine(renderer, x_left - x, y_up - y, x_right + x, y_up - y);
                SDL_RenderLine(renderer, x_left - x, y_bottom + y, x_right + x, y_bottom + y);
                SDL_RenderLine(renderer, x_left - y, y_up - x, x_right + y, y_up - x);
                SDL_RenderLine(renderer, x_left - y, y_bottom + x, x_right + y, y_bottom + x);
        }
}
